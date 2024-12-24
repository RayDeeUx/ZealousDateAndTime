#include "ZealousDateAndTimeLabel.hpp"

using namespace geode::prelude;

ZealousDateAndTimeLabel* ZealousDateAndTimeLabel::create() {
	auto *ret = new ZealousDateAndTimeLabel();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool ZealousDateAndTimeLabel::init() {
	if (!CCNode::init()) return false;
	return true;
}