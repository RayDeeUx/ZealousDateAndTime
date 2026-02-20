#pragma once

class ZealousDateAndTimeLabel : public cocos2d::CCLabelBMFont {
public:
	static ZealousDateAndTimeLabel* create(const char*, const char*);
	bool init(const char*, const char*) override;
	void update(float dt);
	void updateWrapper(float dt) {
		ZealousDateAndTimeLabel::update(dt);
	}

	cocos2d::CCLabelBMFont* m_actualLabel;
};