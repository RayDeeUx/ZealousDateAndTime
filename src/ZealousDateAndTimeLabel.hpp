#pragma once

class ZealousDateAndTimeLabel : public cocos2d::CCLabelBMFont {
protected:
	bool init(const char*, const char*);

public:
	static ZealousDateAndTimeLabel* create(const char*, const char*);
	void update(float dt);
	void updateWrapper(float dt) const {
		ZealousDateAndTimeLabel::update(dt);
	}
};