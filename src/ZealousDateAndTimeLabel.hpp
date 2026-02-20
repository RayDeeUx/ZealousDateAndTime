#pragma once

class ZealousDateAndTimeLabel : public cocos2d::CCLabelBMFont {
public:
	static ZealousDateAndTimeLabel* create(const char*, const char*);
	void update(float dt);
	void updateWrapper(float dt) {
		ZealousDateAndTimeLabel::update(dt);
	}
};