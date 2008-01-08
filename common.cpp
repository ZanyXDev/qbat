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
}
