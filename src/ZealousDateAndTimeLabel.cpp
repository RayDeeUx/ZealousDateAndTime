#include "ZealousDateAndTimeLabel.hpp"

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
	return true;
}