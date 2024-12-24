#pragma once

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	bool calledAlready = false;

	std::array<std::string, 13> months = {
		"Unknown",
		"January", "February", "March", "April",
		"May", "June", "July", "August",
		"September", "October", "November", "December"
	};

	std::array<std::string, 7> daysOfWeek = {
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
	};

	static Manager* getSharedInstance() {
		if (!instance) {
			instance = new Manager();
		}
		return instance;
	}

};