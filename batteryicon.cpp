//
// C++ Implementation: batteryicon
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#include "batteryicon.h"
#include "common.h"
#include <QPainter>

namespace qbat {
	QDir CBatteryIcon::sysfsDir(UI_PATH_SYSFS_DIR);
	
	CBatteryIcon::CBatteryIcon(QString batteryName, Settings * settings, QMenu * contextMenu, QObject * parent) :
		QSystemTrayIcon(parent),
		m_BatteryName(batteryName),
		m_Icon(28, 28),
		m_Settings(settings),
		
		m_RelativeCharge(-1),
		
		m_FullCapacity(0),
		m_DesignCapacity(0),
		m_CurrentCapacity(0),
		m_Rate(0),
		m_Voltage(0),
		m_Status(0),
		
		m_EnergyUnits(false)
	{
		m_Icon.fill(Qt::transparent);
		setIcon(m_Icon);
		setContextMenu(contextMenu);
		show();
	}
	
	CBatteryIcon::~CBatteryIcon() {
	}
	
	void CBatteryIcon::updateIcon() {
		m_Icon.fill(Qt::transparent);
		QPainter painter(&m_Icon);
		
		if (m_CurrentCapacity != m_FullCapacity) {
			painter.setPen(QColor(m_Settings->colors[UI_COLOR_PEN]));
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_EMPTY]));
			painter.drawRect(0, 4, 27, 23);
			
			int chargedPixels = (int)(22 * m_RelativeCharge / 100.0);
			
			painter.fillRect(1, 5 + 22 - chargedPixels, 26, chargedPixels, QColor(m_Settings->colors[UI_COLOR_BRUSH_CHARGED]));
			
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_POLE]));
		}
		else {
			painter.setPen(QColor(m_Settings->colors[UI_COLOR_PEN_FULL]));
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_FULL]));
			painter.drawRect(0, 4, 27, 23);
			
			painter.setBrush(QColor(m_Settings->colors[UI_COLOR_BRUSH_POLE_FULL]));
		}
		painter.drawRect(7, 0, 13, 4);
		
		painter.setBrush(Qt::NoBrush);
		
		if (m_RelativeCharge < 100)
			((QFont&)painter.font()).setPixelSize(15);
		else
			((QFont&)painter.font()).setPixelSize(12);
		
		painter.setRenderHint(QPainter::TextAntialiasing);
		((QFont&)painter.font()).setBold(true);
		if (m_RelativeCharge == -1)
			painter.drawText(1, 9, 26, 16, Qt::AlignHCenter, QString('?'));
		else
			painter.drawText(1, 9, 26, 16, Qt::AlignHCenter, QString::number(m_RelativeCharge));
		
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
			if (m_Rate) {
				newToolTip += '\n';
				qreal remainingTime  = (qreal)m_CurrentCapacity / m_Rate;
				int remainingHours   = (int)remainingTime;
				int remainungMinutes = (int)(remainingTime * 60) % 60;
				newToolTip += tr("remaining time: %1:%2").arg(remainingHours, 2, 10, QChar('0')).arg(remainungMinutes, 2, 10, QChar('0'));
			}
			break;
		case UI_BATTERY_CHARGING:
			newToolTip += tr("status: %1").arg(tr("charging"));
			if (m_Rate && m_FullCapacity) {
				newToolTip += '\n';
				qreal remainingTime  = (qreal)(m_FullCapacity - m_CurrentCapacity) / m_Rate;
				int remainingHours   = (int)remainingTime;
				int remainungMinutes = (int)(remainingTime * 60) % 60;
				newToolTip += tr("remaining time: %1:%2").arg(remainingHours, 2, 10, QChar('0')).arg(remainungMinutes, 2, 10, QChar('0'));
			}
			break;
		case UI_BATTERY_FULL:
			newToolTip += tr("status: %1").arg(tr("full"));
			break;
		default:
			newToolTip += tr("status: %1").arg(tr("unknown"));
			break;
		}
		newToolTip += '\n';
		
		if (m_EnergyUnits) {
			if ((m_Rate) && (m_Status != UI_BATTERY_FULL)) {
				double rateW = qRound(m_Rate / 100000.0) / 10.0;
				double rateA = qRound((m_Rate / m_Voltage) / 1000.0) / 10.0;
				newToolTip += tr("current rate: %1W / %2A").arg(rateW).arg(rateA) + '\n';
			}
			
			newToolTip += tr("current capacity: %1mWh").arg(m_CurrentCapacity / 1000);
			
			if (m_FullCapacity)
				newToolTip += '\n' + tr("last full capacity: %1mWh").arg(m_FullCapacity / 1000);
			
			if (m_DesignCapacity)
				newToolTip += '\n' + tr("design capacity: %1mWh").arg(m_DesignCapacity / 1000);
		}
		else
		{
			if ((m_Rate) && (m_Status != UI_BATTERY_FULL)) {
				double rateA = m_Rate / 100000.0;
				double rateW = qRound(rateA * m_Voltage / 100.0) / 10.0;
				newToolTip += tr("current rate: %1W / %2A").arg(rateW).arg(qRound(rateA) / 10.0) + '\n';
			}
			
			newToolTip += tr("current capacity: %1mAh").arg(m_CurrentCapacity / 1000);
			
			if (m_FullCapacity)
				newToolTip += '\n' + tr("last full capacity: %1mAh").arg(m_FullCapacity / 1000);
			
			if (m_DesignCapacity)
				newToolTip += '\n' + tr("design capacity: %1mAh").arg(m_DesignCapacity / 1000);
		}
		setToolTip(newToolTip);
	}
	
	void CBatteryIcon::updateData() {
		int intBuffer;
		bool noupdate = true;
		
		m_EnergyUnits = sysfsDir.exists(m_BatteryName + UI_CAPTION_NOW(UI_CAPTION_ENERGY));
		
		intBuffer = readIntSysFile(sysfsDir.filePath(m_BatteryName + "/current_now").toAscii().constData());
		if (intBuffer != m_Rate) {
			noupdate = false;
			m_Rate = intBuffer;
		}
		
		intBuffer = readIntSysFile(sysfsDir.filePath(m_BatteryName + UI_CAPTION_NOW(UI_CAPTION_VOLTAGE)).toAscii().constData()) / 10000;
		if (intBuffer != m_Voltage) {
			noupdate = false;
			m_Voltage = intBuffer;
		}
		
		{
			char buffer[BUF_SIZE];
			readStringFromFile(buffer, sysfsDir.filePath(m_BatteryName + "/status").toAscii().constData());
			intBuffer = toStatusInt(buffer);
			
			if (intBuffer != m_Status) {
				noupdate = false;
				m_Status = intBuffer;
			}
		}
		
		{
			int intBuffer[3];
			if (m_EnergyUnits) {
				intBuffer[0] = readIntSysFile(sysfsDir.filePath(m_BatteryName + UI_CAPTION_FULL(UI_CAPTION_ENERGY)).toAscii().constData());
				intBuffer[1] = readIntSysFile(sysfsDir.filePath(m_BatteryName + UI_CAPTION_DESIGN(UI_CAPTION_ENERGY)).toAscii().constData());
				intBuffer[2] = readIntSysFile(sysfsDir.filePath(m_BatteryName + UI_CAPTION_NOW(UI_CAPTION_ENERGY)).toAscii().constData());
			}
			else {
				intBuffer[0] = readIntSysFile(sysfsDir.filePath(m_BatteryName + UI_CAPTION_FULL(UI_CAPTION_CHARGE)).toAscii().constData());
				intBuffer[1] = readIntSysFile(sysfsDir.filePath(m_BatteryName + UI_CAPTION_DESIGN(UI_CAPTION_CHARGE)).toAscii().constData());
				intBuffer[2] = readIntSysFile(sysfsDir.filePath(m_BatteryName + UI_CAPTION_NOW(UI_CAPTION_CHARGE)).toAscii().constData());
			}
			
			if (intBuffer[0] != m_FullCapacity) {
				noupdate = false;
				m_FullCapacity = intBuffer[0];
			}
			
			if (intBuffer[1] != m_DesignCapacity) {
				noupdate = false;
				m_DesignCapacity = intBuffer[1];
			}
			
			if (intBuffer[2] != m_CurrentCapacity) {
				noupdate = false;
				m_CurrentCapacity = intBuffer[2];
			}
		}
		
		if (noupdate)
			return;
		
		qint8 newRelativeCharge;
		
		if (m_FullCapacity)
			newRelativeCharge = (qint8)(100.0 * m_CurrentCapacity / m_FullCapacity);
		else
			newRelativeCharge = -1;
		
		
		if (newRelativeCharge != m_RelativeCharge) {
			m_RelativeCharge = newRelativeCharge;
			
			if (isVisible())
				updateIcon();
		}
		
		updateToolTip();
	}
}
