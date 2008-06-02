
TEMPLATE = app
CONFIG += qt

CODECFORSRC = UTF-8
CODECFORTR = ISO-8859-1

OBJECTS_DIR = build/
MOC_DIR = build/
UI_DIR = ui/


#FORMS =
#TRANSLATIONS = qbat_de.ts

#HEADERS += 
SOURCES += main.cpp \
 batteryicon.cpp \
 powermanager.cpp \
 common.cpp \
 settings.cpp
DESTDIR = .

target.path = /usr/bin
iconstarget.path = /usr/share/qbat/icons
iconstarget.files = res/*.png res/qbat.svg
langtarget.path = /usr/share/qbat/lang
langtarget.files = qbat_*.qm
shortcuttarget.path = /usr/share/applications
shortcuttarget.files = qbat.desktop

INSTALLS += target iconstarget langtarget shortcuttarget

HEADERS += constants.h \
batteryicon.h \
common.h \
powermanager.h \
 settings.h
TARGET = qbat

FORMS += settingsdialog.ui

