#include "ZealousDateAndTimeLabel.hpp"
#include "Manager.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

ZealousDateAndTimeLabel* ZealousDateAndTimeLabel::create(const char* content, const char* fontFile) {
	auto* ret = new ZealousDateAndTimeLabel();
	if (ret && ret->init(content, fontFile)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool init(const char* content, const char* fontFile) {
	if (!CCNode::init()) return false;
	this->setContentSize({0, 0});
	this->setPosition(CCDirector::get()->getWinSize() / -2.f);
	this->m_actualLabel = CCLabelBMFont::create(content, fontFile);
	this->addChild(this->m_actualLabel);
	return true;
}

void ZealousDateAndTimeLabel::update(float dt) {
	CCLabelBMFont* zdatl = Utils::getZDATL();
	if (!zdatl || !zdatl->getParent()) return;
	if (!Utils::modEnabled()) return zdatl->getParent()->setVisible(false);
	Utils::handleZDATL();
}