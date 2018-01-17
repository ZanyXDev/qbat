//
// C++ Implementation: settings
//
// Author: Oliver Groß <z.o.gross@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
#include "settings.h"
#include <QColorDialog>
#include <QPushButton>

#ifndef ASTRA_VER
#include <fly/flyhelp.h>
#include <fly/flydeapi.h> //<fly/flyfileutils.h>
#endif

namespace qbat {
	CSettings::CSettings(QWidget * parent) : QDialog(parent) {
		ui.setupUi(this);

		QIcon ico = QIcon::fromTheme("qbat");
		if (ico.isNull()) ico = QIcon(UI_ICON_QBAT_SMALL);
		setWindowIcon(ico);
		
		colorSelectButtons.setExclusive(false);
		colorSelectButtons.addButton(ui.mainFullColorButton, 1 + UI_COLOR_BRUSH_FULL);
		colorSelectButtons.addButton(ui.mainChargedColorButton, 1 + UI_COLOR_BRUSH_CHARGED);
		colorSelectButtons.addButton(ui.mainEmptyColorButton, 1 + UI_COLOR_BRUSH_EMPTY);
		colorSelectButtons.addButton(ui.textChargingColorButton, 1 + UI_COLOR_PEN_CHARGE);
		colorSelectButtons.addButton(ui.textDischargingColorButton, 1 + UI_COLOR_PEN_DISCHARGE);
		colorSelectButtons.addButton(ui.textFullColorButton, 1 + UI_COLOR_PEN_FULL);
		colorSelectButtons.addButton(ui.poleChargingColorButton, 1 + UI_COLOR_BRUSH_POLE_CHARGE);
		colorSelectButtons.addButton(ui.poleDischargingColorButton, 1 + UI_COLOR_BRUSH_POLE_DISCHARGE);
		colorSelectButtons.addButton(ui.poleFullColorButton, 1 + UI_COLOR_BRUSH_POLE_FULL);
		connect(&colorSelectButtons, SIGNAL(buttonClicked(int)), this, SLOT(editColor(int)));
	
		QPushButton *hbtn = ui.buttonBox->button(QDialogButtonBox::Help); //alex
		if (hbtn)  //alex
		{
		connect(hbtn,SIGNAL(clicked()),this,SLOT(helpSlot()));
		hbtn->setShortcut(QKeySequence::HelpContents);
		}
	}
	
	CSettings::~CSettings() {}
	
    void CSettings::helpSlot() {
#ifndef ASTRA_VER
        flyHelpShow();
#endif
    } //alex
	
	void CSettings::applySettings() {
		m_Settings->handleCritical = ui.criticalGroup->isChecked();
		m_Settings->criticalCapacity = ui.criticalCapacitySpin->value();
		
		m_Settings->executeCommand = ui.criticalCommandRadio->isChecked();
		m_Settings->criticalCommand = ui.criticalCommandEdit->text();
		
		m_Settings->executeShutdown = ui.criticalShutdownRadio->isChecked(); //alex

		m_Settings->confirmCommand = ui.confirmCommandBox->isChecked();
		m_Settings->confirmWithTimeout = ui.timeoutCheck->isChecked();
		m_Settings->timeoutValue = ui.timeoutSpin->value();
		
		for (int i = 0; i < UI_COUNT_COLORS; i++)
			m_Settings->colors[i] = colors[i];
		
		m_Settings->mergeBatteries = ui.mergeBatteriesCheck->isChecked();
		
		m_Settings->showBalloon = ui.showBalloonCheck->isChecked();
#ifndef ASTRA_VER
        m_Settings->autostart = ui.autostartCheck->isChecked(); //alex
        flyEnableAutostart("qbat",m_Settings->autostart); //installInAutostart("qbat",m_Settings->autostart);//alex
        m_Settings->standardIcons = ui.standardIconsCheck->isChecked(); //alex
#endif

		m_Settings->pollingRate = ui.pollingRateSpin->value();
	}
	
	bool CSettings::execute(Settings * settings) {
		ui.criticalGroup->setChecked(settings->handleCritical);
		ui.criticalCapacitySpin->setValue(settings->criticalCapacity);
		
		if (settings->executeCommand)
			ui.criticalCommandRadio->setChecked(true);
		if (settings->executeShutdown) //alex
			ui.criticalShutdownRadio->setChecked(true);
		else
			ui.criticalWarningRadio->setChecked(true);
		
		ui.criticalCommandEdit->setText(settings->criticalCommand);
		ui.confirmCommandBox->setChecked(settings->confirmCommand);
		ui.timeoutCheck->setChecked(settings->confirmWithTimeout);
		ui.timeoutSpin->setValue(settings->timeoutValue);
		
		{
			QPixmap colorIcon(24, 24);
			
			for (int i = 0; i < UI_COUNT_COLORS; i++) {
				colorIcon.fill(settings->colors[i]);
				colorSelectButtons.button(i+1)->setIcon(colorIcon);
				colors[i] = settings->colors[i];
			}
		}
		ui.mergeBatteriesCheck->setChecked(settings->mergeBatteries);
		
		ui.pollingRateSpin->setValue(settings->pollingRate);
		ui.showBalloonCheck->setChecked(settings->showBalloon);
		ui.autostartCheck->setChecked(settings->autostart); //alex
		ui.standardIconsCheck->setChecked(settings->standardIcons); //alex

		m_Settings = settings;
		
		if (exec()) {
			applySettings();
			return true;
		}
		else
			return false;
	}
	
	void CSettings::editColor(int id) {
		QColor color = QColorDialog::getColor(QColor(colors[id-1]), this);
		if (color.isValid()) {
			QPixmap colorIcon(24, 24);
			colorIcon.fill(color);
			
			colorSelectButtons.button(id)->setIcon(QIcon(colorIcon));
			colors[id-1] = color.rgb();
		}
	}
}
