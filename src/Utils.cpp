#include "ZealousDateAndTimeLabel.hpp"
#include "Manager.hpp"
#include "Utils.hpp"
#include <ctime>

using namespace geode::cocos;

namespace Utils {
	template<class T> T getSetting(std::string setting) { return Mod::get()->getSettingValue<T>(setting); }

	bool getBool(std::string setting) { return getSetting<bool>(setting); }
	
	int64_t getInt(std::string setting) { return getSetting<int64_t>(setting); }
	
	double getDouble(std::string setting) { return getSetting<double>(setting); }

	std::string getString(std::string setting) { return getSetting<std::string>(setting); }

	ccColor3B getColor(std::string setting) { return getSetting<ccColor3B>(setting); }

	ccColor4B getColorAlpha(std::string setting) { return getSetting<ccColor4B>(setting); }

	bool modEnabled() { return getBool("enabledZDATL"); }
	
	bool isModLoaded(std::string modID) { return Loader::get()->isModLoaded(modID); }

	Mod* getMod(std::string modID) { return Loader::get()->getLoadedMod(modID); }

	std::string getModVersion(Mod* mod) { return mod->getVersion().toNonVString(); }

	std::string getCurrentTime() {
		if (!Utils::modEnabled()) return "";
		Manager* manager = Manager::getSharedInstance();
		std::time_t tinnyTim = std::time(nullptr);
		std::tm* now = std::localtime(&tinnyTim);
		std::string month = manager->months[now->tm_mon + 1];
		int hour = now->tm_hour;
		std::string ampm = "";
		if (Utils::getBool("twelveHour")) {
			if (hour > 12) {
				hour = hour % 12;
				ampm = " PM";
			} else {
				if (hour == 0) hour = 12;
				ampm = " AM";
			}
		}
		if (Utils::getBool("shortMonth") && month != "May") {
			month = fmt::format("{}", month.substr(0, 3));
		}
		std::string dow = Utils::getBool("dayOfWeek") ? manager->daysOfWeek[now->tm_wday] : ""; // dow = day of week
		std::string dayOfWeek = !Utils::getBool("dayOfWeek") ? "" : fmt::format("{}, ", !Utils::getBool("shortDayOfWeek") ? dow : dow.substr(0, 3));
		std::string dateMonth = Utils::getBool("dayFirst") ?
			fmt::format("{} {}", now->tm_mday, month) : fmt::format("{} {}", month, now->tm_mday);
		std::string seconds = Utils::getBool("includeSeconds") ? fmt::format(":{:02}", now->tm_sec % 60) : "";
		std::string separator = Utils::getBool("splitDateAndTime") ? "\n" : " ";
		#ifndef GEODE_IS_WINDOWS
		std::string timeZone = Utils::getBool("useUTC") ? Utils::getUTCOffset() : now->tm_zone;
		#else
		/*
		original approach: display UTC offset
		didn't work for cvolton apparently because of 0 offset
		*/

		/*
		std::tm* gmt = std::gmtime(&tinnyTim);
		std::string timeZone = fmt::format("UTC{:.2f}", static_cast<double>(difftime(mktime(now), mktime(gmt))) / 60 / 60);
		*/

		// "i love bill gates!", said no one ever, after discovering that timezone abbreviations were paywalled behind arbitrary bullshit

		/*
		second approach: force out an abbreviation of any sort by <ctime>'s string formatting
		apparently still didn't work for cvolton??? (he says it's currently CET for him and not CEST)
		[the "S" is for "'S'ummer", not "'S'tandard"]
		*/

		/*
		char buffer[80];
		strftime(buffer, 80, "%EZ", now);

		std::string timeZone = buffer;
		if (timeZone == "Coordinated Universal Time") timeZone = "UTC";
		std::regex capitalsOnly = std::regex("[^A-Z]");
		timeZone = std::regex_replace(timeZone, capitalsOnly, "");
		*/

		/*
		last resort: cvolton assist + manual UTC calculation
		(why can't MSVC just include a timezone abbv member variable like any other competent C implementation????)
		*/

		std::string timeZone = Utils::getUTCOffset();
		#endif
		return fmt::format("{}{}, {}{}{:02}:{:02}{}{} {}",
			dayOfWeek, dateMonth, now->tm_year + 1900, separator,
			hour, now->tm_min, seconds, ampm, timeZone
		);
	}

	std::string getUTCOffset() {
		// code adapted from cvolton with heavily implied permission
		// proof: https://discord.com/channels/911701438269386882/911702535373475870/1322321142819586099
		std::tm timeInfo;
		std::time_t epoch = 0;
		#ifdef GEODE_IS_WINDOWS
		localtime_s(&timeInfo, &epoch);
		#else
		localtime_r(&epoch, &timeInfo);
		#endif
		if (timeInfo.tm_hour == 0 && timeInfo.tm_min == 0) return "UTC";
		char sign = timeInfo.tm_hour >= 12 ? '-' : timeInfo.tm_hour > 0 ? '+' : ' ';
		int hour = timeInfo.tm_hour >= 12 ? 24 - timeInfo.tm_hour : timeInfo.tm_hour;
		std::string minutes = timeInfo.tm_min != 0 ? fmt::format(":{:02}", timeInfo.tm_min) : "";
		return fmt::format("UTC{}{}{}", sign, hour, minutes);
	}

	cocos2d::CCNode* getZDATL(cocos2d::CCScene* scene) {
		return scene->getChildByID("zealous-date-and-time-container"_spr);
	}

	cocos2d::CCNode* getZDATLLabel() {
		return getZDATL()->getChildByID("zealous-date-and-time-label"_spr);
	}
	
	void handleZDATL() {
		if (!Utils::modEnabled()) return removeZDATL();
		const auto gjbgl = GJBaseGameLayer::get();
		const auto lel = LevelEditorLayer::get();
		const auto pl = PlayLayer::get();
		cocos2d::CCNode* zdatl = getZDATL();
		if (zdatl && getBool("hideEverywhereElseZDATL") && !pl && !lel) zdatl->setVisible(false);
		if (zdatl && getBool("hideInLevelEditorLayerZDATL") && lel) return removeZDATL();
		if (!zdatl && getBool("hideInLevelEditorLayerZDATL") && !lel) {
			addZDATL();
			zdatl = getZDATL();
		}
		if (gjbgl && zdatl) {
			if (pl) {
				std::string playLayerVisibility = getString("visibilityInPlayLayerZDATL");
				if (playLayerVisibility.starts_with("Always ")) {
					if (playLayerVisibility == "Always Visible") zdatl->setVisible(true);
					else if (playLayerVisibility == "Always Hidden") zdatl->setVisible(false);
				} else if (playLayerVisibility.starts_with("Only ") && playLayerVisibility.ends_with(" When Dead")) {
					if (auto player = pl->m_player1) {
						bool onlyShowWhenDead = playLayerVisibility == "Only Show When Dead";
						bool onlyHideWhenDead = playLayerVisibility == "Only Hide When Dead";
						if (player->m_isDead) {
							if (onlyShowWhenDead) zdatl->setVisible(true);
							else if (onlyHideWhenDead) zdatl->setVisible(false);
						} else {
							if (onlyShowWhenDead) zdatl->setVisible(false);
							else if (onlyHideWhenDead) zdatl->setVisible(true);
						}
					}
				}
			} else if (lel) {
				if (getBool("hideInLevelEditorLayerZDATL")) removeZDATL();
				else addZDATL();
			}
		} else if (zdatl) {
			zdatl->setVisible(!getBool("hideEverywhereElseZDATL"));
		}
		if (auto label = getZDATLLabel()) static_cast<CCLabelBMFont*>(label)->setString(getCurrentTime().c_str());
	}

	void addZDATL() {
		auto zdatl = getZDATL();
		if (zdatl) return zdatl->setVisible(true);
		auto newLabel = ZealousDateAndTimeLabel::create();
		if (!newLabel) return log::info("ZDATL addition operation failed, node was not created properly");
		setupZDATL(newLabel);
		CCScene::get()->addChild(newLabel);
		SceneManager::get()->keepAcrossScenes(newLabel);
		newLabel->setVisible(true);
		if (Utils::getBool("loggingZDATL")) log::info("ZDATL added");
	}

	void removeZDATL() {
		auto zdatl = getZDATL();
		if (!zdatl) return;
		CCScene::get()->removeChildByID("zealous-date-and-time-container"_spr);
		if (Utils::getBool("loggingZDATL")) log::info("ZDATL removed");
	}

	void setupZDATL(CCNode* zdatl, CCSize win) {
		auto label = CCLabelBMFont::create(Utils::getCurrentTime().c_str(), Utils::chooseFontFile(Utils::getInt("font")).c_str());
		label->setID("zealous-date-and-time-label"_spr);
		label->setScale(Utils::getDouble("scaleZDATL"));
		label->setRotation(Utils::getDouble("rotationZDATL"));
		auto color = Utils::getColorAlpha("color");
		if (color == ccColor4B{0, 0, 0, 0}) {
			Utils::addChroma(label);
		} else {
			label->setColor({color.r, color.g, color.b});
			label->setOpacity(color.a);
		}
		if (Utils::getInt("font") == -2 && Utils::getBool("blendingZDATL"))
			label->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		std::string alignment = Utils::getString("textAlignZDATL");
		if (alignment == "Left") label->setAlignment(kCCTextAlignmentLeft);
		else if (alignment == "Center") label->setAlignment(kCCTextAlignmentCenter);
		else if (alignment == "Right") label->setAlignment(kCCTextAlignmentRight);
		zdatl->setPosition({
			win.width * static_cast<float>(Utils::getDouble("xPositionZDATL") / 100.f),
			win.height * static_cast<float>(Utils::getDouble("yPositionZDATL") / 100.f)
		});
		zdatl->setID("zealous-date-and-time-container"_spr);
		zdatl->setZOrder(Utils::getInt("zOrderZDATL"));
		zdatl->setAnchorPoint({.5f, .5f});
		zdatl->setScale(1.0f);
		zdatl->addChild(label);
	}

	void addChroma(cocos2d::CCLabelBMFont* label) {
		label->setColor({255, 255, 255});
		label->setOpacity(255);
		CCActionInterval* sequence = CCSequence::create(
			CCTintTo::create(1.f, 255, 128, 128), CCTintTo::create(1.f, 255, 255, 128),
			CCTintTo::create(1.f, 128, 255, 128), CCTintTo::create(1.f, 128, 255, 255),
			CCTintTo::create(1.f, 128, 128, 255), CCTintTo::create(1.f, 255, 128, 255), nullptr
		);
		CCAction* repeat = CCRepeatForever::create(sequence);
		label->runAction(repeat);
	}

	std::string chooseFontFile(int64_t font) {
		if (font == -1) return "goldFont.fnt";
		if (font == -2) return "chatFont.fnt";
		if (font != 0) return fmt::format("gjFont{:02d}.fnt", font);
		return "bigFont.fnt";
	}
}