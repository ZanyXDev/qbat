//
// C++ Implementation: batteryicon
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#include "batteryicon.h"
#include "common.h"
#include <string>
#include <QDir>
#include <QPainter>

namespace qbat {
	using namespace std;
	
	CBatteryIcon::CBatteryIcon(QString batteryName, Settings * settings, QMenu * contextMenu, QObject * parent) :
		QSystemTrayIcon(parent),
		m_BatteryName(batteryName),
		m_Icon(28, 28),
		m_Settings(settings)
	{
		m_Icon.fill(Qt::transparent);
		setIcon(m_Icon);
		setContextMenu(contextMenu);
		//updateData();
		show();
	}
	
	CBatteryIcon::~CBatteryIcon() {
	}
	
	void CBatteryIcon::updateIcon() {
		m_Icon.fill(Qt::transparent);
		QPainter painter(&m_Icon);
		
		if (m_ChargeNow != m_ChargeFull) {
			painter.setPen(QColor(m_Settings->colors[UI_COLOR_PEN]));
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_EMPTY]));
			painter.drawRect(0, 4, 27, 23);
			
			int chargedPixels = (int)(21 * m_RelativeCharge / 100.0);
			
			painter.fillRect(1, 1 + 26 - chargedPixels, 26, chargedPixels, QColor(m_Settings->colors[UI_COLOR_BRUSH_CHARGED]));
			
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_POLE]));
		}
		else {
			painter.setPen(QColor(m_Settings->colors[UI_COLOR_PEN_FULL]));
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_FULL]));
			painter.drawRect(0, 4, 27, 23);
			
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_POLE_FULL]));
		}
		painter.drawRect(8, 0, 11, 4);
		
		painter.setBrush(Qt::NoBrush);
		
		((QFont&)painter.font()).setPixelSize(14);
		((QFont&)painter.font()).setBold(true);
		painter.drawText(1, 10, 26, 15, Qt::AlignHCenter, QString::number(m_RelativeCharge));
		
		setIcon(m_Icon);
	}
	
	void CBatteryIcon::updateToolTip() {
		QString newToolTip = tr("QBat - %1: %2%").arg(m_BatteryName) +'\n';
		
		if (m_RelativeCharge == -1)
			newToolTip = newToolTip.arg('-');
		else
			newToolTip = newToolTip.arg(m_RelativeCharge);
		
		switch (m_Status) {
		case UI_BATTERY_DISCHARGING:
			newToolTip += tr("status: %1").arg(tr("dischaging"));
			if (m_CurrentNow) {
				newToolTip += '\n';
				qreal remainingTime  = (qreal)m_ChargeNow / m_CurrentNow;
				int remainingHours   = (int)remainingTime;
				int remainungMinutes = (int)(remainingTime * 60) % 60;
				newToolTip += tr("remaining time: %1:%2").arg(remainingHours, 2, 10, QChar('0')).arg(remainungMinutes, 2, 10, QChar('0'));
			}
			break;
		case UI_BATTERY_CHARGING:
			newToolTip += tr("status: %1").arg(tr("charging"));
			break;
		case UI_BATTERY_FULL:
			newToolTip += tr("status: %1").arg(tr("full"));
			break;
		default:
			newToolTip += tr("status: %1").arg(tr("unknown"));
			break;
		}
		newToolTip += '\n';
		
		if (m_Status != UI_BATTERY_FULL)
			newToolTip += tr("current rate: %1A").arg(qRound(m_CurrentNow / 100000.0) / 10.0) + '\n';
		
		newToolTip += tr("current capacity: %2mAh").arg(m_ChargeNow / 1000) + '\n';
		
		if (m_ChargeFull)
			newToolTip += tr("last full capacity: %3mAh").arg(m_ChargeFull / 1000) + '\n';
		
		if (m_ChargeFullDesign)
			newToolTip += tr("design capacity: %4mAh").arg(m_ChargeFullDesign / 1000);
		
		setToolTip(newToolTip);
	}
	
	void CBatteryIcon::updateData(int chargeFull, int chargeFullDesign, int chargeNow, int currentNow, int status) {
		bool update = false;
		if (chargeNow == m_ChargeNow)
			update = true;
		else
			m_ChargeNow = chargeNow;
		
		if (currentNow == m_CurrentNow)
			update = true;
		else
			m_CurrentNow = currentNow;
		
		if (status == m_Status)
			update = true;
		else
			m_Status = status;
		
		if (chargeFull == m_ChargeFullDesign)
			update = true;
		else
			m_ChargeFullDesign = chargeFull;
		
		if (chargeFullDesign == m_ChargeFullDesign)
			update = true;
		else
			m_ChargeFullDesign = chargeFullDesign;
		
		if (!update)
			return;
		
		qint8 newRelativeCharge = 0;
		
		if (m_ChargeFull)
			newRelativeCharge = (qint8)(100.0 * m_ChargeNow / m_ChargeFull);
		else
			newRelativeCharge = -1;
		
		if (newRelativeCharge != m_RelativeCharge) {
			m_RelativeCharge = newRelativeCharge;
			updateIcon();
		}
		
		updateToolTip();
	}
}
