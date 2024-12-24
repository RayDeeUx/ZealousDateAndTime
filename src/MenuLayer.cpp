#include <Geode/modify/MenuLayer.hpp>
#include "Manager.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		if (Manager::getSharedInstance()->calledAlready) return true;

		Manager::getSharedInstance()->calledAlready = true;

		if (!Utils::modEnabled()) return true;
		if (Utils::getZDATL()) return true;

		Utils::addZDATL();

		return true;
	}
};