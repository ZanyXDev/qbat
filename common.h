//
// C++ Interface: common
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#ifndef QBAT_COMMON_H
#define QBAT_COMMON_H

#include <string>
#include "constants.h"

namespace qbat {
	std::string readStringSysFile(const char * fileName);
	int readIntSysFile(const char * fileName);
}

#endif
