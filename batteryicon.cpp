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
	
	CBatteryIcon::CBatteryIcon(QString batteryName, QMenu * contextMenu, QObject * parent) :
		QSystemTrayIcon(parent),
		m_BatteryName(batteryName),
		m_Icon(32, 32)
	{
		m_Icon.fill(Qt::transparent);
		setContextMenu(contextMenu);
		updateData();
		show();
	}
	
	CBatteryIcon::~CBatteryIcon() {
	}
	
	void CBatteryIcon::updateData() {
		QDir workDir(UI_PATH_SYSFS_DIR + ('/' + m_BatteryName));
		int chargeFull       = readIntSysFile(workDir.filePath("charge_full").toAscii().constData());
		int chargeFullDesign = readIntSysFile(workDir.filePath("charge_full_design").toAscii().constData());
		int chargeNow        = readIntSysFile(workDir.filePath("charge_now").toAscii().constData());
		int currentNow       = readIntSysFile(workDir.filePath("current_now").toAscii().constData());
		string status        = readStringSysFile(workDir.filePath("status").toAscii().constData());
		
		QString newToolTip = tr("QBat - %1: %2%").arg(m_BatteryName) +'\n';
		if (chargeFull)
			newToolTip = newToolTip.arg((int)(100.0 * chargeNow / chargeFull));
		else
			newToolTip = newToolTip.arg('-');
		
		if (status == "Discharging") {
			newToolTip += tr("status: %1").arg(tr("dischaging"));
			if (currentNow) {
				newToolTip += '\n';
				qreal remainingTime  = (qreal)chargeNow / (qreal)currentNow;
				int remainingHours   = (int)remainingTime;
				int remainungMinutes = (int)(remainingTime * 60) % 60;
				newToolTip += tr("remaining time: %1:%2").arg(remainingHours, 2, QChar('0')).arg(remainungMinutes, 2, QChar('0'));
			}
		}
		else if (status == "Charging")
			newToolTip += tr("status: %1").arg(tr("charging"));
		else if (status == "Full")
			newToolTip += tr("status: %1").arg(tr("full"));
		else
			newToolTip += tr("status: %1").arg(tr("unknown"));
		
		newToolTip += '\n';
		
		if (status != "Full")
			newToolTip += tr("current rate: %1A").arg(qRound(currentNow / 100000.0) / 10.0) + '\n';
		
		newToolTip += tr("current capacity: %2mAh").arg(chargeNow / 1000) + '\n';
		
		newToolTip += tr("last full capacity: %3mAh").arg(chargeFull / 1000) + '\n';
		newToolTip += tr("design capacity: %4mAh").arg(chargeFullDesign / 1000);
		
		setToolTip(newToolTip);
		m_Icon.fill(Qt::transparent);
		
		QPainter painter(&m_Icon);
		
		if (chargeNow != chargeFull) {
			painter.setPen(Qt::black);
			painter.setBrush(Qt::white);
			painter.drawRect(0, 5, 31, 26);
			
			painter.setPen(Qt::NoPen);
			painter.setBrush(Qt::green);
			painter.drawRect(1, 6 + 25 - (int)(25.0 * chargeNow / chargeFull), 30, (int)(25.0 * chargeNow / chargeFull));
			
			painter.setPen(Qt::black);
			painter.setBrush(Qt::yellow);
		}
		else {
			painter.setPen(Qt::black);
			painter.setBrush(Qt::green);
			painter.drawRect(0, 5, 31, 26);
			
			painter.setBrush(Qt::blue);
		}
		painter.drawRect(9, 0, 13, 5);
		
		painter.setBrush(Qt::NoBrush);
		painter.font().setPixelSize(16);
		painter.drawText(1, 12, 30, 16, Qt::AlignHCenter, QString::number((int)(100.0 * chargeNow / chargeFull)));
		
		setIcon(m_Icon);
	}
}
