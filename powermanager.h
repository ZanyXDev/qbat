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
#include <QSettings>
#include <QSystemTrayIcon>
#include "common.h"

namespace qbat {
	class CBatteryIcon;
	
	class CPowerManager : public QObject {
		Q_OBJECT
	private:
		enum messageType {MSGT_INFORMATION, MSGT_WARNING};
		
		int m_Timer;
		bool m_CriticalHandled;
		QDir m_SysfsDir;
		
		Settings m_Settings;
		QSettings m_SettingsFile;
		
		QMenu m_ContextMenu;
		
		QHash<QString, CBatteryIcon *> m_BatteryIcons;
		QSystemTrayIcon m_DefaultTrayIcon;
		
		inline void readSettings();
		inline void writeSettings();
		
		void timerEvent(QTimerEvent * event);
	public:
		CPowerManager(QObject * parent = 0);
		~CPowerManager();
	private slots:
		void updateData();
		void restartTimer();
	public slots:
		void showSettings();
		void showAbout();
	};
}

#endif
