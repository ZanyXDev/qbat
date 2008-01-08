//
// C++ Interface: cpowermanager
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#ifndef QBAT_POWERMANAGER_H
#define QBAT_POWERMANAGER_H

#include <QHash>
#include <QDir>
#include <QMenu>
#include "batteryicon.h"

namespace qbat {
	class CPowerManager : public QObject {
		Q_OBJECT
	private:
		void timerEvent(QTimerEvent * event);
		int m_Timer;
		QDir m_SysfsDir;
		QMenu m_ContextMenu;
		QHash<QString, CBatteryIcon *> m_BatteryIcons;
		QSystemTrayIcon m_DefaultTrayIcon;
	public:
		CPowerManager(QObject * parent = 0);
		~CPowerManager();
	};
}

#endif
