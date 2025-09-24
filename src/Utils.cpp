#include "Utils.hpp"
#include <fmt/chrono.h>

#define SECONDS_PER_DAY 86400
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60

using namespace geode::cocos;

namespace Utils {
	template<class T> T getSetting(std::string setting) { return Mod::get()->getSettingValue<T>(setting); }

	bool getBool(std::string setting) { return getSetting<bool>(setting); }
	
	int64_t getInt(std::string setting) { return getSetting<int64_t>(setting); }
	
	double getDouble(std::string setting) { return getSetting<double>(setting); }

	std::string getString(std::string setting) { return getSetting<std::string>(setting); }

	ccColor3B getColor(std::string setting) { return getSetting<ccColor3B>(setting); }

	ccColor4B getColorAlpha(std::string setting) { return getSetting<ccColor4B>(setting); }

	bool modEnabled() { return getBool("enabled"); }
	
	bool isModLoaded(std::string modID) { return Loader::get()->isModLoaded(modID); }

	Mod* getMod(std::string modID) { return Loader::get()->getLoadedMod(modID); }

	std::string getModVersion(Mod* mod) { return mod->getVersion().toNonVString(); }

	std::string getCurrentTime() {
		if (!Utils::modEnabled()) return "";
		Manager* manager = Manager::getSharedInstance();
		auto now = std::chrono::system_clock::now();
        auto timeNow = std::chrono::system_clock::to_time_t(now);
        std::tm tm_local = fmt::localtime(timeNow);
		std::string month = manager->months[tm_local.tm_mon + 1];
		int hour = tm_local.tm_hour;
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
		if (Utils::getBool("shortMonth") && month.length() > Utils::getInt("monthTruncation"))
			month = fmt::format("{}", month.substr(0, Utils::getInt("monthTruncation")));
		std::string dayOfWeek = Utils::getBool("dayOfWeek") ? manager->daysOfWeek[tm_local.tm_wday] : "";
		if (Utils::getBool("shortDayOfWeek") && dayOfWeek.length() > Utils::getInt("dOWTruncation"))
			dayOfWeek = fmt::format("{}", dayOfWeek.substr(0, Utils::getInt("dOWTruncation")));
		std::string dateMonth = Utils::getBool("dayFirst") ?
			fmt::format("{} {}", tm_local.tm_mday, month) : fmt::format("{} {}", month, tm_local.tm_mday);
		std::string seconds = Utils::getBool("includeSeconds") ? fmt::format(":{:02}", tm_local.tm_sec % 60) : "";
		std::string separator = Utils::getBool("splitDateAndTime") ? "\n" : " ";
		#ifndef GEODE_IS_WINDOWS
		std::string timeZone = Utils::getBool("useUTC") ? Utils::getUTCOffset(tm_local) : tm_local.tm_zone;
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

		std::string timeZone = Utils::getUTCOffset(tm_local);
		#endif
		std::string uptime = Utils::getBool("uptime") ? fmt::format("\n{}: {}", Utils::getString("uptimePrefix"), Utils::getUptime(timeNow)) : "";
		return fmt::format("{}, {}, {}{}{:02}:{:02}{}{} {}{}",
			dayOfWeek, dateMonth, tm_local.tm_year + 1900, separator,
			hour, tm_local.tm_min, seconds, ampm, timeZone, uptime
		);
	}

	std::string getUTCOffset(std::tm timeInfo) {
		if (!Utils::modEnabled()) return "";
		// code adapted from cvolton with heavily implied permission
		// proof: https://discord.com/channels/911701438269386882/911702535373475870/1322321142819586099
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

	std::string getUptime(std::time_t now) {
		if (!Utils::modEnabled()) return "";
		long elapsed = difftime(now, Manager::getSharedInstance()->originalTimestamp);
		long days = elapsed / SECONDS_PER_DAY;
		elapsed -= days * SECONDS_PER_DAY;
		int hours = static_cast<int>(elapsed / SECONDS_PER_HOUR);
		elapsed -= hours * SECONDS_PER_HOUR;
		int minutes = static_cast<int>(elapsed / SECONDS_PER_MINUTE);
		elapsed -= minutes * SECONDS_PER_MINUTE;
		int seconds = static_cast<int>(elapsed);
		std::string daysString = days > 0 ? fmt::format("{}:", days) : "";
		// std::string daysString = fmt::format("{}:", days); // for debugging in case day count is way off
		std::string hoursString = hours > 0 ? fmt::format("{:02}:", hours) : "";
		// std::string hoursString = fmt::format("{:02}:", hours); // for debugging in case hour count is way off
		std::string minutesString = hours > 0 ? fmt::format("{:02}:", minutes) : fmt::format("{}:", minutes);
		std::string secondsString = fmt::format("{:02}", seconds);
		return fmt::format("{}{}{}{}", daysString, hoursString, minutesString, secondsString);
	}

	ZealousDateAndTimeLabel* getZDATL(cocos2d::CCScene* scene) {
		return static_cast<ZealousDateAndTimeLabel*>(scene->getChildByID("zealous-date-and-time-label"_spr));
	}
	
	void handleZDATL() {
		if (!Utils::modEnabled()) return removeZDATL();
		const auto gjbgl = GJBaseGameLayer::get();
		const auto lel = LevelEditorLayer::get();
		const auto pl = PlayLayer::get();
		ZealousDateAndTimeLabel* zdatl = getZDATL();
		if (zdatl && getBool("hideEverywhereElse") && !pl && !lel) zdatl->setVisible(false);
		if (zdatl && getBool("hideInLevelEditorLayer") && lel) return removeZDATL();
		if (!zdatl && getBool("hideInLevelEditorLayer") && !lel) {
			addZDATL();
			zdatl = getZDATL();
		}
		if (gjbgl && zdatl) {
			if (pl) {
				std::string playLayerVisibility = getString("visibilityInPlayLayer");
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
				if (getBool("hideInLevelEditorLayer")) removeZDATL();
				else addZDATL();
			}
		} else if (zdatl) zdatl->setVisible(!getBool("hideEverywhereElse"));
		if (zdatl) {
			if (!isModLoaded("ziegenhainy.dyslexia-simulator")) zdatl->setString(getCurrentTime().c_str());
			else zdatl->setString(getCurrentTime().c_str(), false);
		}
	}

	void addZDATL() {
		auto zdatl = getZDATL();
		if (zdatl) return zdatl->setVisible(true);
		auto newLabel = ZealousDateAndTimeLabel::create(Utils::getCurrentTime().c_str(), Utils::chooseFontFile(Utils::getInt("font")).c_str());
		if (!newLabel) return log::info("ZDATL addition operation failed, node was not created properly");
		setupZDATL(newLabel);
		CCScene::get()->addChild(newLabel);
		SceneManager::get()->keepAcrossScenes(newLabel);
		newLabel->setVisible(true);
		if (Utils::getBool("logging")) log::info("ZDATL added");
	}

	void removeZDATL() {
		auto zdatl = getZDATL();
		if (!zdatl) return;
		CCScene::get()->removeChildByID("zealous-date-and-time-label"_spr);
		if (Utils::getBool("logging")) log::info("ZDATL removed");
	}

	void setupZDATL(ZealousDateAndTimeLabel* label, CCSize win) {
		Utils::setupMonthsAndDay();
		label->setID("zealous-date-and-time-label"_spr);
		label->setScale(Utils::getDouble("scale"));
		label->setRotation(Utils::getDouble("rotation"));
		label->setZOrder(Utils::getInt("zOrder"));
		label->setAnchorPoint({.5f, .5f});
		if (Utils::getInt("font") == -2 && Utils::getBool("blending"))
			label->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		std::string alignment = Utils::getString("textAlign");
		if (alignment == "Left") label->setAlignment(kCCTextAlignmentLeft);
		else if (alignment == "Center") label->setAlignment(kCCTextAlignmentCenter);
		else if (alignment == "Right") label->setAlignment(kCCTextAlignmentRight);
		label->setPosition({
			win.width * static_cast<float>(Utils::getDouble("xPosition") / 100.f),
			win.height * static_cast<float>(Utils::getDouble("yPosition") / 100.f)
		});
		auto color = Utils::getColorAlpha("color");
		if (color == ccColor4B{0, 0, 0, 0}) return Utils::addChroma(label);
		label->setColor({color.r, color.g, color.b});
		label->setOpacity(color.a);
	}

	void setupMonthsAndDay(Manager* manager, std::string lang) {
		manager->daysOfWeek = manager->daysOfWeekMap.contains(lang) ?
			manager->daysOfWeekMap.at(lang) : manager->daysOfWeekFallback;
		manager->months = manager->monthsMap.contains(lang) ?
			manager->monthsMap.at(lang) : manager->monthsFallback;
	}

	void addChroma(ZealousDateAndTimeLabel* label) {
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