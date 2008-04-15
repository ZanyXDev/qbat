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
	class Settings;
	
	class CBatteryIcon : public QSystemTrayIcon {
		Q_OBJECT
	private:
		QString m_BatteryName;
		QPixmap m_Icon;
		Settings * m_Settings;
		
		qint8 m_RelativeCharge;
		
		int m_ChargeFull;
		int m_ChargeFullDesign;
		int m_ChargeNow;
		int m_CurrentNow;
		int m_Status;
		
		void updateIcon();
		void updateToolTip();
	public:
		CBatteryIcon(QString batteryName, Settings * settings, QMenu * contextMenu, QObject * parent = 0);
		~CBatteryIcon();
		
		QString batteryName() const { return m_BatteryName; }
		
		void updateData(int chargeFull, int chargeFullDesign, int chargeNow, int currentNow, int status);
	};
}

#endif
