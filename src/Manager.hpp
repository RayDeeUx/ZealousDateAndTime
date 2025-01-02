#pragma once

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	bool calledAlready = false;

	std::array<std::string, 13> monthsFallback = {
		"Unknown",
		"January", "February", "March", "April",
		"May", "June", "July", "August",
		"September", "October", "November", "December"
	};

	std::array<std::string, 7> daysOfWeekFallback = {
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
	};

	std::unordered_map<std::string, std::array<std::string, 7>> daysOfWeekMap = {
		{"English", {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}},
		{"Spanish", {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"}},
		{"French", {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"}},
		{"Italian", {"Domenica", "Lunedi", "Martedi", "Mercoledi", "Giovedi", "Venerdi", "Sabato"}},
		{"German", {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"}},
		{"Portuguese", {"Domingo", "Segunda-feira", "Terca-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sabado"}},
		{"Finnish", {"Sunnuntai", "Maanantai", "Tiistai", "Keskiviikko", "Torstai", "Perjantai", "Lauantai"}},
		{"Czech", {"Nedele", "Pondeli", "Utery", "Streda", "Ctvrtek", "Patek", "Sobota"}},
		{"Slovak", {"Nedel'a", "Pondelok", "Utorok", "Streda", "Stvrtok", "Piatok", "Sobota"}},
		{"Croatian", {"Nedjelja", "Ponedjeljak", "Utorak", "Srijeda", "Cetvrtak", "Petak", "Subota"}},
		{"Romanian", {"Duminica", "Luni", "Marti", "Miercuri", "Joi", "Vineri", "Sambata"}},
		{"Dutch", {"Zondag", "Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag"}},
		{"Welsh", {"Sul", "Llun", "Mawrth", "Mercher", "Iau", "Gwener", "Sadwrn"}},
		{"Scots Gaelic", {"Didomhnaich", "Diluain", "Dimairt", "Diciadain", "Diardaoin", "Dihaoine", "Disathairne"}},
		{"Indonesian", {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"}},
		{"Japanese (Romaji)", {"Nichi", "Getsu", "Ka", "Sui", "Moku", "Kin", "Do"}},
		{"Chinese Mandarin (Pinyin)", {"zhou1 ri4", "zhou1 yi1", "zhou1 er4", "zhou1 san1", "zhou1 si4", "zhou1 wu3", "zhou1 liu4"}},
		{"Korean (Revised Romanization)", {"il", "wol", "hwa", "su", "mog", "geum", "to"}}
	};

	std::unordered_map<std::string, std::array<std::string, 13>> monthsMap = {
		{"English", {"Unknown", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}},
		{"Spanish", {"Desconocido", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"}},
		{"French", {"Inconnu", "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"}},
		{"Italian", {"Sconosciuto", "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"}},
		{"German", {"Unbekannt", "Januar", "Februar", "Marz", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"}},
		{"Portuguese", {"Desconhecido", "Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"}},
		{"Finnish", {"Tuntematon", "Tammikuu", "Helmikuu", "Maaliskuu", "Huhtikuu", "Toukokuu", "Kesakuu", "Heinakuu", "Elokuu", "Syyskuu", "Lokakuu", "Marraskuu", "Joulukuu"}},
		{"Czech", {"Neznamy", "Leden", "Unor", "Brezen", "Duben", "Kveten", "Cerven", "Cervenec", "Srpen", "Zari", "rijen", "Listopad", "Prosinec"}},
		{"Slovak", {"Nezname", "Januar", "Februar", "Marec", "Aprila", "Maj", "Jun", "Jul", "Augusta", "Septembra", "Oktober", "Novembra", "Decembra"}},
		{"Croatian", {"Nepoznato", "Sijecanj", "Veljaca", "Ozujak", "Travanj", "Svibanj", "Lipanj", "Srpanj", "Kolovoz", "Rujan", "Listopad", "Studeni", "Prosinac"}},
		{"Romanian", {"Necunoscut", "Ianuarie", "Februarie", "Martie", "Aprilie", "Mai", "Iunie", "Iulie", "August", "Septembrie", "Octombrie", "Noiembrie", "Decembrie"}},
		{"Dutch", {"Onbekend", "Januari", "Februari", "Maart", "April", "Mei", "Juni", "Juli", "Augustus", "September", "Oktober", "November", "December"}},
		{"Welsh", {"Anhysbys", "Ionawr", "Chwefror", "Mawrth", "Ebrill", "Mai", "Mehefin", "Gorffennaf", "Awst", "Medi", "Hydref", "Tachwedd", "Rhagfyr"}},
		{"Scots Gaelic", {"Neo-aithnichte", "Faoilleach", "Gearran", "Mart", "Giblean", "Ceitean", "Ogmhios", "Iuchar", "Lunastal", "Sultain", "Damhair", "Samhain", "Dubhlachd"}},
		{"Indonesian", {"Tidak diketahui", "Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"}},
		{"Japanese (Romaji)", {"Fumei", "1tsuki", "2tsuki", "3tsuki", "4tsuki", "5tsuki", "6tsuki", "7tsuki", "8tsuki", "9tsuki", "10tsuki", "11tsuki", "12tsuki"}},
		{"Chinese Mandarin (Pinyin)", {"wei4 zhi1", "yi1 yue4", "er4 yue4", "san3 yue4", "si4 yue4", "wu3 yue4", "liu4 yue4", "qi1 yue4", "ba1 yue4", "jiu3 yue4", "shi2 yue4", "shi2yi1 yue4", "shi2er4 yue4"}},
		{"Korean (Revised Romanization)", {"al su eobs-eum", "1wol", "2wol", "3wol", "4wol", "5wol", "6wol", "7wol", "8wol", "9wol", "10wol", "11wol", "12wol"}}
	};

	std::array<std::string, 7> daysOfWeek;
	std::array<std::string, 13> months;

	std::time_t originalTimestamp;

	static Manager* getSharedInstance() {
		if (!instance) {
			instance = new Manager();
		}
		return instance;
	}

};