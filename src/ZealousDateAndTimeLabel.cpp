#include "ZealousDateAndTimeLabel.hpp"
#include "Manager.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

ZealousDateAndTimeLabel* ZealousDateAndTimeLabel::create(const char* content, const char* fontFile) {
	auto* ret = new ZealousDateAndTimeLabel();
	if (ret && ret->initWithString(content, fontFile)) {
		ret->iFuckingHateHavingToDoThis();
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

void iFuckingHateHavingToDoThis() {
	if (this->m_pComponentContainer) return;
	this->m_pComponentContainer = new CCComponentContainer(this);
}

void ZealousDateAndTimeLabel::update(float dt) {
	ZealousDateAndTimeLabel* zdatl = Utils::getZDATL();
	if (!zdatl) return;
	if (!Utils::modEnabled()) return zdatl->setVisible(false);
	Utils::handleZDATL();
}