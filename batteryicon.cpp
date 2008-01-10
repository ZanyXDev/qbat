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
	
	void CBatteryIcon::updateData(int chargeFull, int chargeFullDesign, int chargeNow, int currentNow, int status) {
		QString newToolTip = tr("QBat - %1: %2%").arg(m_BatteryName) +'\n';
		
		if (chargeFull)
			newToolTip = newToolTip.arg((int)(100.0 * chargeNow / chargeFull));
		else
			newToolTip = newToolTip.arg('-');
		
		switch (status) {
		case UI_BATTERY_DISCHARGING:
			newToolTip += tr("status: %1").arg(tr("dischaging"));
			if (currentNow) {
				newToolTip += '\n';
				qreal remainingTime  = (qreal)chargeNow / (qreal)currentNow;
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
		
		if (status != UI_BATTERY_FULL)
			newToolTip += tr("current rate: %1A").arg(qRound(currentNow / 100000.0) / 10.0) + '\n';
		
		newToolTip += tr("current capacity: %2mAh").arg(chargeNow / 1000) + '\n';
		
		if (chargeFull)
			newToolTip += tr("last full capacity: %3mAh").arg(chargeFull / 1000) + '\n';
		
		if (chargeFullDesign)
			newToolTip += tr("design capacity: %4mAh").arg(chargeFullDesign / 1000);
		
		setToolTip(newToolTip);
		
		m_Icon.fill(Qt::transparent);
		QPainter painter(&m_Icon);
		
		if (chargeNow != chargeFull) {
			painter.setPen(QColor(m_Settings->colors[UI_COLOR_PEN]));
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_EMPTY]));
			painter.drawRect(0, 4, 27, 23);
			
			painter.setPen(Qt::NoPen);
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_CHARGED]));
			
			int chargedPixels = (int)(21 * (float)chargeNow / (float)chargeFull);
			
			painter.drawRect(1, 1 + 26 - chargedPixels, 26, chargedPixels);
			
			painter.setPen(QColor(m_Settings->colors[UI_COLOR_PEN]));
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
		
		((QFont&)painter.font()).setPixelSize(15);
		painter.drawText(1, 10, 26, 15, Qt::AlignHCenter, QString::number((int)(100.0 * chargeNow / chargeFull)));
		
		setIcon(m_Icon);
	}
}
