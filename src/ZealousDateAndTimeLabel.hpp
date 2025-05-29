#pragma once

#include "Utils.hpp"

class ZealousDateAndTimeLabel : public cocos2d::CCLabelBMFont {
protected:
	bool init(const char*, const char*);

public:
	static ZealousDateAndTimeLabel* create(const char*, const char*);
	void update(float dt) {
		ZealousDateAndTimeLabel* zdatl = Utils::getZDATL();
		if (!zdatl) return;
		if (!Utils::modEnabled()) return zdatl->setVisible(false);
		Utils::handleZDATL();
	}
};