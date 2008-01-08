//
// C++ Implementation: cpowermanager
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#include <QtGui>
#include "powermanager.h"
#include "common.h"

namespace qbat {
	using namespace std;
	
	CPowerManager::CPowerManager(QObject * parent) :
		QObject(parent),
		m_SysfsDir(UI_PATH_SYSFS_DIR),
		m_DefaultTrayIcon(QIcon(UI_ICON_QBAT), this)
	{
		m_ContextMenu.addAction(tr("&Settings"))->setEnabled(false);
		m_ContextMenu.addSeparator();
		m_ContextMenu.addAction(tr("&Quit"), qApp, SLOT(quit()));
		
		m_DefaultTrayIcon.setContextMenu(&m_ContextMenu);
		
		timerEvent(NULL);
		if (m_SysfsDir.exists()) {
			m_Timer = startTimer(3000);
		}
	}
	
	CPowerManager::~CPowerManager() {
		killTimer(m_Timer);
	}
	
	void CPowerManager::timerEvent(QTimerEvent *) {
		if (m_SysfsDir.exists()) {
			bool acPlug = false;
			QStringList powerSupplies = m_SysfsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
			
			QList<CBatteryIcon *> newBatteryIcons;
			CBatteryIcon * currentBatteryIcon;
			
			foreach(QString i, powerSupplies) {
				string buffer = readStringSysFile(m_SysfsDir.filePath(i + "/type").toAscii().constData());
				
				if (buffer == "Mains") {
					if (1 == readIntSysFile(m_SysfsDir.filePath(i + "/online").toAscii().constData()))
						acPlug = true;
				}
				else if (buffer == "Battery") {
					if (!m_BatteryIcons.contains(i))
						currentBatteryIcon = new CBatteryIcon(i, &m_ContextMenu, this);
					else {
						currentBatteryIcon = m_BatteryIcons.take(i);
						currentBatteryIcon->updateData();
					}
					newBatteryIcons << currentBatteryIcon;
				}
			}
			
			foreach(CBatteryIcon * i, m_BatteryIcons) {
				delete i;
			}
			
			m_BatteryIcons.clear();
			
			foreach(CBatteryIcon * i, newBatteryIcons)
				m_BatteryIcons.insert(i->batteryName(), i);
			
			if (acPlug)
				m_DefaultTrayIcon.setToolTip("QBat - " + tr("AC adapter plugged in"));
			else
				m_DefaultTrayIcon.setToolTip("QBat - " + tr("AC adapter unplugged"));
		}
		else
			m_DefaultTrayIcon.setToolTip("QBat - " + tr("no information available"));
		
		m_DefaultTrayIcon.setVisible(m_BatteryIcons.isEmpty());
	}
}
