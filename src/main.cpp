#include <Geode/loader/SettingV3.hpp>

#include "Manager.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	listenForSettingChanges<bool>("enabled", [](bool isEnabled) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!isEnabled && zdatl) return Utils::removeZDATL();
		if (isEnabled && !zdatl) return Utils::addZDATL();
	});
	listenForSettingChanges<double>("scale", [](double scale) {
		cocos2d::CCNode* zdatl = Utils::getZDATLLabel();
		if (!zdatl) return;
		return zdatl->setScale(static_cast<float>(scale));
	});
	listenForSettingChanges<double>("rotation", [](double rotation) {
		cocos2d::CCNode* zdatl = Utils::getZDATLLabel();
		if (!zdatl) return;
		return zdatl->setRotation(rotation);
	});
	listenForSettingChanges<int64_t>("zOrder", [](int64_t zOrder) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!zdatl) return;
		return zdatl->setZOrder(static_cast<int>(zOrder));
	});
	listenForSettingChanges<int64_t>("font", [](int64_t fontID) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		zdatl->setFntFile(Utils::chooseFontFile(fontID).c_str());
		if (Utils::getBool("blending") && fontID == -2) return zdatl->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
	});
	listenForSettingChanges<double>("xPosition", [](double xPos) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!zdatl) return;
		return zdatl->setPositionX(CCDirector::get()->getWinSize().width * (static_cast<float>(xPos) / 100.f));
	});
	listenForSettingChanges<double>("yPosition", [](double yPos) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!zdatl) return;
		return zdatl->setPositionY(CCDirector::get()->getWinSize().height * (static_cast<float>(yPos) / 100.f));
	});
	listenForSettingChanges<bool>("blending", [](bool blending) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		if (blending && Utils::getInt("font") == -2) return zdatl->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		return zdatl->setBlendFunc({});
	});
	listenForSettingChanges<cocos2d::ccColor4B>("color", [](cocos2d::ccColor4B color) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		if (color == ccColor4B{0, 0, 0, 0}) {
			Utils::addChroma(zdatl);
		} else {
			zdatl->stopAllActions();
			zdatl->setColor({color.r, color.g, color.b});
			zdatl->setOpacity(color.a);
		}
	});
	listenForSettingChanges<std::string>("textAlign", [](std::string alignment) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		if (alignment == "Left") return zdatl->setAlignment(kCCTextAlignmentLeft);
		if (alignment == "Center") return zdatl->setAlignment(kCCTextAlignmentCenter);
		if (alignment == "Right") return zdatl->setAlignment(kCCTextAlignmentRight);
	});
	listenForSettingChanges<std::string>("language", [](std::string lang) {
		Manager* manager = Manager::getSharedInstance();
		manager->daysOfWeek = manager->daysOfWeekMap.contains(lang) ?
			manager->daysOfWeekMap.at(lang) : manager->daysOfWeekFallback;
		manager->months = manager->monthsMap.contains(lang) ?
			manager->monthsMap.at(lang) : manager->monthsFallback;
	});
}