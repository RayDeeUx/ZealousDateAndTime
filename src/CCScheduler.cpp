#include <Geode/modify/CCScheduler.hpp>
#include "Utils.hpp"

#define PREFERRED_HOOK_PRIO -2123456789

using namespace geode::prelude;

class $modify(MyCCScheduler, CCScheduler) {
	static void onModify(auto& self) {
		(void) self.setHookPriority("CCScheduler::update", PREFERRED_HOOK_PRIO);
	}
	void update(float dt) {
		CCScheduler::update(dt);
		auto zdatl = Utils::getZDATL();
		if (!zdatl) return;
		if (!Utils::modEnabled()) {
			zdatl->setVisible(false);
			return;
		}
		Utils::handleZDATL();
	}
};