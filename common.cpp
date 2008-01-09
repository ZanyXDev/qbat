//
// C++ Implementation: common
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#include <fstream>
#include "common.h"

namespace qbat {
	using namespace std;
	
	string readStringSysFile(const char * fileName) {
		ifstream inFile;
		string buffer;
		
		inFile.open(fileName);
		inFile >> buffer;
		inFile.close();
		
		return buffer;
	}
	
	int readIntSysFile(const char * fileName) {
		ifstream inFile;
		int buffer;
		
		inFile.open(fileName);
		inFile >> buffer;
		inFile.close();
		
		return buffer;
	}
	
	int toStatusInt(std::string status) {
		if (status == "Discharging")
			return UI_BATTERY_DISCHARGING;
		else if (status == "Charging")
			return UI_BATTERY_CHARGING;
		else if (status == "Full")
			return UI_BATTERY_FULL;
		else
			return 0;
	}
}
