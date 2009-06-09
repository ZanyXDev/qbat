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
#include <QDir>

namespace qbat {
	struct Settings;
	
	class CBatteryIcon : public QSystemTrayIcon {
		Q_OBJECT
	private:
		QString m_BatteryName;
		QPixmap m_Icon;
		Settings * m_Settings;
		
		qint8 m_RelativeCharge;
		
		int m_FullCapacity;
		int m_DesignCapacity;
		int m_CurrentCapacity;
		int m_Rate;
		int m_Voltage;
		int m_Status;
		
		bool m_EnergyUnits;
		
// 		void updateIcon();
		void updateToolTip();
	public:
		static QDir sysfsDir;
		
		CBatteryIcon(QString batteryName, Settings * settings, QMenu * contextMenu, QObject * parent = 0);
		~CBatteryIcon();
		
		QString batteryName() const { return m_BatteryName; }
		qint8 relativeCharge() const { return m_RelativeCharge; }
		
// 		void updateMergedData();
		void updateData();
		void updateIcon();
	};
}

#endif
