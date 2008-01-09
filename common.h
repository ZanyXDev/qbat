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
#include <QColor>
#include "constants.h"

namespace qbat {
	struct Settings {
		quint16 pollingRate;
		bool mergeBatterys;
		bool handleCritical;
		quint8 criticalCapacity;
		bool executeCommand;
		QString criticalCommand;
		QRgb colors[UI_COUNT_COLORS];
/*		QRgb textColor;
		QRgb mainEmptyColor;
		QRgb mainChargedColor;
		QRgb poleColor;
		QRgb textFullColor;
		QRgb mainFullColor;
		QRgb poleFullColor;*/
	};
	
	std::string readStringSysFile(const char * fileName);
	int readIntSysFile(const char * fileName);
	int toStatusInt(std::string status);
}

#endif
