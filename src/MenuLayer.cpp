#include <Geode/modify/MenuLayer.hpp>
#include "ZealousDateAndTimeLabel.hpp"
#include "Manager.hpp"
#include "Utils.hpp"
#include <ctime>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		Manager* manager = Manager::getSharedInstance();
		if (manager->calledAlready) return true;
		manager->calledAlready = true;
		manager->originalTimestamp = std::time(nullptr);
		Utils::setupMonthsAndDay();

		if (!Utils::modEnabled()) return true;
		if (Utils::getZDATL()) return true;

		Utils::addZDATL();

		return true;
	}
};