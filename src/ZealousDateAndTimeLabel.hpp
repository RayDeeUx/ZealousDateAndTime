#pragma once

class ZealousDateAndTimeLabel : public cocos2d::CCLabelBMFont, public cocos2d::CCNode* {
protected:
	bool init(const char*, const char*);

public:
	static ZealousDateAndTimeLabel* create(const char*, const char*);
	void update(float);
};