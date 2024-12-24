#include "ZealousDateAndTimeLabel.hpp"
#include "Utils.hpp"
#include <ctime>

#include "Manager.hpp"

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
		std::time_t tinnyTim = std::time(nullptr);
		std::tm* now = std::localtime(&tinnyTim);
		std::string month = Manager::getSharedInstance()->months[now->tm_mon + 1];
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
			month = fmt::format("{}.", month.substr(0, 3));
		}
		std::string dateMonth = Utils::getBool("dayFirst") ?
			fmt::format("{} {}", now->tm_mday, month) : fmt::format("{} {}", month, now->tm_mday);
		std::string seconds = Utils::getBool("includeSeconds") ? fmt::format(":{:02}", now->tm_sec % 60) : "";
		std::string separator = Utils::getBool("splitDateAndTime") ? "\n" : " ";
		#ifndef GEODE_IS_WINDOWS
		std::string timeZone = fmt::format(" {}", now->tm_zone);
		#else
		std::string timeZone = "";
		#endif
		return fmt::format("{}, {}{}{:02}:{:02}{}{}{}",
			dateMonth, now->tm_year + 1900, separator,
			hour, now->tm_min, seconds, ampm, timeZone.c_str()
		);
	}

	cocos2d::CCNode* getZDATL(cocos2d::CCScene* scene) {
		return scene->getChildByID("zealous-date-and-time-container"_spr);
	}

	cocos2d::CCNode* getZDATLLabel() {
		return getZDATL()->getChildByID("zealous-date-and-time-label"_spr);
	}
	
	void handleZDATL(cocos2d::CCScene* scene) {
		if (!Utils::modEnabled()) return removeZDATL();
		const auto gjbgl = GJBaseGameLayer::get();
		const auto lel = LevelEditorLayer::get();
		const auto pl = PlayLayer::get();
		cocos2d::CCNode* zdatl = getZDATL();
		if (zdatl && getBool("hideEverywhereElseZDATL") && !pl && !lel) zdatl->setVisible(false);
		if (zdatl && getBool("hideInLevelEditorLayerZDATL") && lel) return removeZDATL();
		if (!zdatl && getBool("visibilityInPlayLayerZDATL") && !lel) {
			addZDATL();
			zdatl = getZDATL();
		}
		if (gjbgl) {
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
		if (zdatl) static_cast<CCLabelBMFont*>(zdatl->getChildByID("zealous-date-and-time-label"_spr))->setString(getCurrentTime().c_str());
	}

	void addZDATL() {
		if (!Utils::modEnabled()) return log::info("mod disabled, addition aborted");
		auto zdatl = getZDATL();
		if (zdatl) return zdatl->setVisible(true);
		auto newLabel = ZealousDateAndTimeLabel::create();
		if (!newLabel) return log::info("ZDATL addition operation failed, node was not found");
		setupZDATL(newLabel);
		CCScene::get()->addChild(newLabel);
		SceneManager::get()->keepAcrossScenes(newLabel);
		newLabel->setVisible(true);
		if (Utils::getBool("loggingZDATL")) log::info("ZDATL added");
	}

	void removeZDATL() {
		if (!Utils::modEnabled()) return log::info("mod disabled, removal aborted");
		auto zdatl = getZDATL();
		if (!zdatl) return log::info("ZDATL removal operation failed, node was not found");
		CCScene::get()->removeChildByID("zealous-date-and-time-container"_spr);
		if (Utils::getBool("loggingZDATL")) log::info("ZDATL removed");
	}

	void setupZDATL(CCNode* zdatl) {
		auto win = CCDirector::get()->getWinSize();
		zdatl->setID("zealous-date-and-time-container"_spr);
		zdatl->setScale(1.0f);
		auto label = CCLabelBMFont::create(Utils::getCurrentTime().c_str(), Utils::chooseFontFile(Utils::getInt("font")).c_str());
		label->setID("zealous-date-and-time-label"_spr);
		label->setScale(Utils::getDouble("scaleZDATL"));
		label->setRotation(Utils::getDouble("rotationZDATL"));
		auto color = Utils::getColorAlpha("color");
		label->setColor({color.r, color.g, color.b});
		label->setOpacity(color.a);
		if (Utils::getInt("font") == -2 && Utils::getBool("blendingZDATL"))
			label->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		zdatl->addChild(label);
		zdatl->setPosition({
			win.width * static_cast<float>(Utils::getDouble("xPositionZDATL") / 100.f),
			win.height * static_cast<float>(Utils::getDouble("yPositionZDATL") / 100.f)
		});
		zdatl->setZOrder(Utils::getInt("zOrderZDATL"));
		std::string alignment = Utils::getString("textAlignZDATL");
		if (alignment == "Left") return label->setAlignment(kCCTextAlignmentLeft);
		if (alignment == "Center") return label->setAlignment(kCCTextAlignmentCenter);
		if (alignment == "Right") return label->setAlignment(kCCTextAlignmentRight);
		zdatl->setAnchorPoint({.5f, .5f});
	}

	std::string chooseFontFile(int64_t font) {
		if (font == -1) return "goldFont.fnt";
		if (font == -2) return "chatFont.fnt";
		if (font != 0) return fmt::format("gjFont{:02d}.fnt", font);
		return "bigFont.fnt";
	}
}