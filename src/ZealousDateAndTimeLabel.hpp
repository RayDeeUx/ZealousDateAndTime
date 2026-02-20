#pragma once

class ZealousDateAndTimeLabel : public cocos2d::CCNode {
protected:
	bool init(const char*, const char*);
	cocos2d::CCLabelBMFont* m_actualLabel;
public:
	static ZealousDateAndTimeLabel* create(const char*, const char*);
	void update(float dt) override;
	void updateWrapper(float dt) {
		ZealousDateAndTimeLabel::update(dt);
	}
};