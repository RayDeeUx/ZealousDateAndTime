#include "ZealousDateAndTimeLabel.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

ZealousDateAndTimeLabel* ZealousDateAndTimeLabel::create(const char* content, const char* fontFile) {
	auto *ret = new ZealousDateAndTimeLabel();
	if (ret && ret->init(content, fontFile)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool ZealousDateAndTimeLabel::init(const char* content, const char* fontFile) {
	if (!CCLabelBMFont::initWithString(content, fontFile)) return false;
	this->scheduleUpdate();
	return true;
}

void ZealousDateAndTimeLabel::update(float dt) {
	ZealousDateAndTimeLabel* zdatl = Utils::getZDATL();
	if (!zdatl) return;
	if (!Utils::modEnabled()) return zdatl->setVisible(false);
	Utils::handleZDATL();
}