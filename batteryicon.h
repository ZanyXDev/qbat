//
// C++ Interface: batteryicon
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#ifndef QBAT_BATTERYICON_H
#define QBAT_BATTERYICON_H

#include <QSystemTrayIcon>

namespace qbat {
	class CBatteryIcon : public QSystemTrayIcon {
		Q_OBJECT
	private:
		QString m_BatteryName;
		QPixmap m_Icon;
	public:
		CBatteryIcon(QString batteryName, QMenu * contextMenu, QObject * parent = 0);
		~CBatteryIcon();
		
		QString batteryName() const { return m_BatteryName; }
		
		void updateData();
	};
}

#endif
