#pragma once
#include "ZealousDateAndTimeLabel.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

namespace Utils {
	template<class T> T getSetting(std::string setting, T type);
	bool getBool(std::string setting);
	int64_t getInt(std::string setting);
	double getDouble(std::string setting);
	std::string getString(std::string setting);
	ccColor3B getColor(std::string setting);
	ccColor4B getColorAlpha(std::string setting);
	bool modEnabled();
	
	bool isModLoaded(std::string modID);
	Mod* getMod(std::string modID);
	std::string getModVersion(Mod* mod);

	std::string getCurrentTime();
	std::string getUTCOffset();
	std::string getUptime(std::time_t now);

	ZealousDateAndTimeLabel* getZDATL(cocos2d::CCScene* scene = CCDirector::get()->getRunningScene());
	void handleZDATL();
	void addZDATL();
	void removeZDATL();
	void setupZDATL(ZealousDateAndTimeLabel* zdatl, cocos2d::CCSize win = CCDirector::get()->getWinSize());
	void addChroma(ZealousDateAndTimeLabel* label = Utils::getZDATL());
	void setupMonthsAndDay(Manager* manager = Manager::getSharedInstance(), std::string lang = Utils::getString("language"));
	std::string chooseFontFile(int64_t font);

	void showIncompat();
}