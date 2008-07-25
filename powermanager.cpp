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
#include "settings.h"
#include "batteryicon.h"

namespace qbat {
	using namespace std;
	
	CPowerManager::CPowerManager(QObject * parent) :
		QObject(parent),
		m_CriticalHandled(false),
		m_SysfsDir(UI_PATH_SYSFS_DIR),
		m_SettingsFile(UI_FILE_CONFIG, QSettings::IniFormat, this),
		m_DefaultTrayIcon(QIcon(UI_ICON_QBAT), this)
	{
		readSettings();
		m_ContextMenu.addAction(tr("&Settings"), this, SLOT(showSettings()))->setEnabled(m_SysfsDir.exists());
		m_ContextMenu.addAction(tr("&About"), this, SLOT(showAbout()));
		m_ContextMenu.addSeparator();
		m_ContextMenu.addAction(tr("&Quit"), qApp, SLOT(quit()));
		
		m_DefaultTrayIcon.setContextMenu(&m_ContextMenu);
		
		timerEvent(NULL);
		if (m_SysfsDir.exists()) {
			m_Timer = startTimer(m_Settings.pollingRate);
		}
	}
	
	CPowerManager::~CPowerManager() {
		killTimer(m_Timer);
		writeSettings();
	}
	
	inline void CPowerManager::readSettings() {
		m_SettingsFile.beginGroup("Main");
		m_Settings.pollingRate = m_SettingsFile.value("pollingRate", 3000).toUInt();
		m_Settings.showBalloon = m_SettingsFile.value("showBalloon", false).toBool();
		m_SettingsFile.endGroup();
		
		m_SettingsFile.beginGroup("TrayIcons");
		m_Settings.mergeBatterys = m_SettingsFile.value("mergeBatterys", true).toBool();
		m_Settings.colors[UI_COLOR_PEN] = m_SettingsFile.value("textColor", 0).toUInt();
		m_Settings.colors[UI_COLOR_PEN_FULL] = m_SettingsFile.value("textFullColor", 0).toUInt();
		m_Settings.colors[UI_COLOR_BRUSH_CHARGED] = m_SettingsFile.value("mainChargedColor", qRgb( 0, 255, 0)).toUInt();
		m_Settings.colors[UI_COLOR_BRUSH_EMPTY] = m_SettingsFile.value("mainEmptyColor", qRgb(255, 255, 255)).toUInt();
		m_Settings.colors[UI_COLOR_BRUSH_FULL] = m_SettingsFile.value("mainFullColor", m_Settings.colors[UI_COLOR_BRUSH_CHARGED]).toUInt();
		m_Settings.colors[UI_COLOR_BRUSH_POLE] = m_SettingsFile.value("poleColor", qRgb(255, 255, 0)).toUInt();
		m_Settings.colors[UI_COLOR_BRUSH_POLE_FULL] = m_SettingsFile.value("poleFullColor", m_Settings.colors[UI_COLOR_BRUSH_POLE]).toUInt();
		m_SettingsFile.endGroup();
		
		m_SettingsFile.beginGroup("CriticalEvent");
		m_Settings.handleCritical = m_SettingsFile.value("handle", false).toBool();
		m_Settings.criticalCapacity = m_SettingsFile.value("criticalCapacity", 7).toUInt();
		m_Settings.executeCommand = m_SettingsFile.value("executeCommand", false).toBool();
		m_Settings.criticalCommand = m_SettingsFile.value("command", "").toString();
		m_SettingsFile.endGroup();
	}
	
	inline void CPowerManager::writeSettings() {
		m_SettingsFile.beginGroup("Main");
		m_SettingsFile.setValue("pollingRate", m_Settings.pollingRate);
		m_SettingsFile.setValue("showBalloon", m_Settings.showBalloon);
		m_SettingsFile.endGroup();
		
		m_SettingsFile.beginGroup("TrayIcons");
		m_SettingsFile.setValue("mergeBatterys", m_Settings.mergeBatterys);
		m_SettingsFile.setValue("textColor", m_Settings.colors[UI_COLOR_PEN]);
		m_SettingsFile.setValue("textFullColor", m_Settings.colors[UI_COLOR_PEN_FULL]);
		m_SettingsFile.setValue("mainChargedColor", m_Settings.colors[UI_COLOR_BRUSH_CHARGED]);
		m_SettingsFile.setValue("mainEmptyColor", m_Settings.colors[UI_COLOR_BRUSH_EMPTY]);
		m_SettingsFile.setValue("mainFullColor", m_Settings.colors[UI_COLOR_BRUSH_FULL]);
		m_SettingsFile.setValue("poleColor", m_Settings.colors[UI_COLOR_BRUSH_POLE]);
		m_SettingsFile.setValue("poleFullColor", m_Settings.colors[UI_COLOR_BRUSH_POLE_FULL]);
		m_SettingsFile.endGroup();
		
		m_SettingsFile.beginGroup("CriticalEvent");
		m_SettingsFile.setValue("handle", m_Settings.handleCritical);
		m_SettingsFile.setValue("criticalCapacity", m_Settings.criticalCapacity);
		m_SettingsFile.setValue("executeCommand", m_Settings.executeCommand);
		m_SettingsFile.setValue("command", m_Settings.criticalCommand);
		m_SettingsFile.endGroup();
	}
	
	void CPowerManager::timerEvent(QTimerEvent *) {
		updateData();
	}
	
	void CPowerManager::updateData() {
		if (m_SysfsDir.exists()) {
			bool acPlug = false;
			QStringList powerSupplies = m_SysfsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
			
			int chargeFull = 0;
			int chargeFullDesign = 0;
			int chargeNow = 0;
			int currentNow = 0;
			int status = 0;
			
			int relativeCapacity = 100;
			
			if (m_Settings.mergeBatterys) {
				int batteryCount = 0;
				foreach(QString i, powerSupplies) {
					string buffer = readStringSysFile(m_SysfsDir.filePath(i + "/type").toAscii().constData());
					
					if ((!acPlug) && (buffer == "Mains")) {
						if (1 == readIntSysFile(m_SysfsDir.filePath(i + "/online").toAscii().constData()))
							acPlug = true;
					}
					else if (buffer == "Battery") {
						chargeFull       += readIntSysFile(m_SysfsDir.filePath(i + "/energy_full").toAscii().constData());
						chargeFullDesign += readIntSysFile(m_SysfsDir.filePath(i + "/energy_full_design").toAscii().constData());
						chargeNow        += readIntSysFile(m_SysfsDir.filePath(i + "/energy_now").toAscii().constData());
						currentNow       += readIntSysFile(m_SysfsDir.filePath(i + "/current_now").toAscii().constData());
						
						int statusBuffer = toStatusInt(readStringSysFile(m_SysfsDir.filePath(i + "/status").toAscii().constData()));
						
						if ((!status) || (statusBuffer != UI_BATTERY_FULL))
							status = statusBuffer;
						
						batteryCount++;
					}
				}
				
				if (batteryCount) {
					if (m_Settings.handleCritical)
						relativeCapacity = (int)(100.0 * chargeNow / chargeFull);
					
					chargeFull       /= batteryCount;
					chargeFullDesign /= batteryCount;
					chargeNow        /= batteryCount;
					currentNow       /= batteryCount;
					
					if (!m_BatteryIcons.contains("merged")) {
						if (!m_BatteryIcons.isEmpty()) {
							foreach(CBatteryIcon * i, m_BatteryIcons) {
								delete i;
							}
							
							m_BatteryIcons.clear();
						}
						
						CBatteryIcon * currentBatteryIcon = new CBatteryIcon("average", &m_Settings, &m_ContextMenu, this);
						currentBatteryIcon->updateData(chargeFull, chargeFullDesign, chargeNow, currentNow, status);
						m_BatteryIcons.insert("merged", currentBatteryIcon);
					}
					else
						m_BatteryIcons["merged"]->updateData(chargeFull, chargeFullDesign, chargeNow, currentNow, status);
				}
				else {
					delete m_BatteryIcons.take("merged");
				}
			}
			else {
				QList<CBatteryIcon *> newBatteryIcons;
				CBatteryIcon * currentBatteryIcon;
				
				int chargeNowMid = 0;
				int chargeFullMid = 0;
				
				foreach(QString i, powerSupplies) {
					string buffer = readStringSysFile(m_SysfsDir.filePath(i + "/type").toAscii().constData());
					
					if (buffer == "Mains") {
						if (1 == readIntSysFile(m_SysfsDir.filePath(i + "/online").toAscii().constData()))
							acPlug = true;
					}
					else if (buffer == "Battery") {
						chargeFull       = readIntSysFile(m_SysfsDir.filePath(i + "/charge_full").toAscii().constData());
						chargeFullDesign = readIntSysFile(m_SysfsDir.filePath(i + "/charge_full_design").toAscii().constData());
						chargeNow        = readIntSysFile(m_SysfsDir.filePath(i + "/charge_now").toAscii().constData());
						currentNow       = readIntSysFile(m_SysfsDir.filePath(i + "/current_now").toAscii().constData());
						status           = toStatusInt(readStringSysFile(m_SysfsDir.filePath(i + "/status").toAscii().constData()));
						
						chargeFullMid += chargeFull;
						chargeNowMid += chargeNow;
						
						if (!m_BatteryIcons.contains(i)) {
							currentBatteryIcon = new CBatteryIcon(i, &m_Settings, &m_ContextMenu, this);
							currentBatteryIcon->updateData(chargeFull, chargeFullDesign, chargeNow, currentNow, status);
						}
						else {
							currentBatteryIcon = m_BatteryIcons.take(i);
							currentBatteryIcon->updateData(chargeFull, chargeFullDesign, chargeNow, currentNow, status);
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
				
				if (!m_BatteryIcons.isEmpty())
					relativeCapacity = (int)(100.0 * chargeNowMid / chargeFullMid);
			}
			
			
			if (acPlug)
				m_DefaultTrayIcon.setToolTip("QBat - " + tr("AC adapter plugged in"));
			else {
				if ((m_Settings.handleCritical) && (relativeCapacity <= m_Settings.criticalCapacity) && (!m_CriticalHandled)) {
					if (m_Settings.executeCommand)
						QProcess::startDetached(m_Settings.criticalCommand);
					else {
						if (m_Settings.showBalloon) {
							m_BatteryIcons.begin().value()->showMessage(tr("QBat - critical battery capacity"),
								tr("WARNING: The attached battery(s) reached the critical mark.\n Please make sure to save and shut down soon or provide another source of power."),
								QSystemTrayIcon::Warning, 7000);
						}
						else
							QMessageBox::warning(NULL, tr("QBat - critical battery capacity"),
								tr("WARNING: The attached battery(s) reached the critical mark.\n Please make sure to save and shut down soon or provide another source of power."));
					}
					m_CriticalHandled = true;
				}
				m_DefaultTrayIcon.setToolTip("QBat - " + tr("AC adapter unplugged"));
			}
		}
		else
			m_DefaultTrayIcon.setToolTip("QBat - " + tr("no information available"));
		
		m_DefaultTrayIcon.setVisible(m_BatteryIcons.isEmpty());
	}
	
	void CPowerManager::restartTimer() {
		killTimer(m_Timer);
		m_Timer = startTimer(m_Settings.pollingRate);
	}
	
	void CPowerManager::showSettings() {
		CSettings dialog;
		connect(&dialog, SIGNAL(settingsChanged()), this, SLOT(updateData()));
		connect(&dialog, SIGNAL(settingsChanged()), this, SLOT(restartTimer()));
		
		dialog.execute(&m_Settings);
	}
	
	void CPowerManager::showAbout() {
		if (m_Settings.showBalloon) {
			if (m_DefaultTrayIcon.isVisible())
				m_DefaultTrayIcon.showMessage(tr("About QBat"), UI_NAME + "\nv" + QString(UI_VERSION),
					QSystemTrayIcon::Information, 4000);
			else
				m_BatteryIcons.begin().value()->showMessage(tr("About QBat"), UI_NAME + "\nv" + QString(UI_VERSION),
					QSystemTrayIcon::Information, 4000);
		}
		else {
			QMessageBox aboutBox;
			
			aboutBox.setWindowIcon(QIcon(UI_ICON_QBAT));
			aboutBox.setIconPixmap(QPixmap(UI_ICON_QBAT));
			aboutBox.setWindowTitle(tr("About QBat"));
			aboutBox.setText(UI_NAME + "\nv" + QString(UI_VERSION));
			aboutBox.setStandardButtons(QMessageBox::Ok);
			
			aboutBox.exec();
		}
	}
}
