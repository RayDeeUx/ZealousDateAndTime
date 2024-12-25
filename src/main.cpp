#include <Geode/loader/SettingV3.hpp>
#include "Utils.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	listenForSettingChanges<bool>("enabledZDATL", [](bool isEnabled) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!isEnabled && zdatl) return Utils::removeZDATL();
		if (isEnabled && !zdatl) return Utils::addZDATL();
	});
	listenForSettingChanges<double>("scaleZDATL", [](double scale) {
		cocos2d::CCNode* zdatl = Utils::getZDATLLabel();
		if (!zdatl) return;
		return zdatl->setScale(static_cast<float>(scale));
	});
	listenForSettingChanges<double>("rotationZDATL", [](double rotation) {
		cocos2d::CCNode* zdatl = Utils::getZDATLLabel();
		if (!zdatl) return;
		return zdatl->setRotation(rotation);
	});
	listenForSettingChanges<int64_t>("zOrderZDATL", [](int64_t zOrder) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!zdatl) return;
		return zdatl->setZOrder(static_cast<int>(zOrder));
	});
	listenForSettingChanges<int64_t>("font", [](int64_t fontID) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		zdatl->setFntFile(Utils::chooseFontFile(fontID).c_str());
		if (Utils::getBool("blendingZDATL") && fontID == -2) return zdatl->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
	});
	listenForSettingChanges<double>("xPositionZDATL", [](double xPos) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!zdatl) return;
		return zdatl->setPositionX(CCDirector::get()->getWinSize().width * (static_cast<float>(xPos) / 100.f));
	});
	listenForSettingChanges<double>("yPositionZDATL", [](double yPos) {
		cocos2d::CCNode* zdatl = Utils::getZDATL();
		if (!zdatl) return;
		return zdatl->setPositionY(CCDirector::get()->getWinSize().height * (static_cast<float>(yPos) / 100.f));
	});
	listenForSettingChanges<bool>("blendingZDATL", [](bool blending) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		if (blending && Utils::getInt("font") == -2) return zdatl->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		return zdatl->setBlendFunc({});
	});
	listenForSettingChanges<cocos2d::ccColor4B>("color", [](cocos2d::ccColor4B color) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		if (color == ccColor4B{0,0,0,0}) {
			zdatl->setOpacity(255);
			CCFiniteTimeAction* tintOne = CCTintTo::create(1.f, 255, 128, 128);
			CCFiniteTimeAction* tintTwo = CCTintTo::create(1.f, 255, 255, 128);
			CCFiniteTimeAction* tintThree = CCTintTo::create(1.f, 128, 255, 128);
			CCFiniteTimeAction* tintFour = CCTintTo::create(1.f, 128, 255, 255);
			CCFiniteTimeAction* tintFive = CCTintTo::create(1.f, 128, 128, 255);
			CCFiniteTimeAction* tintSix = CCTintTo::create(1.f, 255, 128, 255);
			CCActionInterval* sequence = CCSequence::create(tintOne, tintTwo, tintThree, tintFour, tintFive, tintSix, nullptr);
			CCAction* repeat = CCRepeatForever::create(sequence);
			zdatl->runAction(repeat);
			return;
		}
		zdatl->stopAllActions();
		zdatl->setColor({color.r, color.g, color.b});
		zdatl->setOpacity(color.a);
	});
	listenForSettingChanges<std::string>("textAlignZDATL", [](std::string alignment) {
		cocos2d::CCLabelBMFont* zdatl = static_cast<CCLabelBMFont*>(Utils::getZDATLLabel());
		if (!zdatl) return;
		if (alignment == "Left") return zdatl->setAlignment(kCCTextAlignmentLeft);
		if (alignment == "Center") return zdatl->setAlignment(kCCTextAlignmentCenter);
		if (alignment == "Right") return zdatl->setAlignment(kCCTextAlignmentRight);
	});
}