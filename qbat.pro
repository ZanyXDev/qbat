TEMPLATE = app
TARGET = qbat

QT += widgets x11extras

CONFIG += qtsingleapplication

CONFIG   += link_pkgconfig
PKGCONFIG = flyintegration


CODECFORSRC = UTF-8
CODECFORTR = ISO-8859-1

OBJECTS_DIR = build/
MOC_DIR = build/
UI_DIR = ui/

#alex:
#LIBS += -lflycore -lflyui -lflyuiextra -lSM
LIBS += -lflycore

TRANSLATIONS = qbat_ru.ts

SOURCES += main.cpp \
    batteryicon.cpp \
    powermanager.cpp \
    common.cpp \
    settings.cpp \
    qtimermessagebox.cpp

HEADERS += constants.h \
    batteryicon.h \
    common.h \
    powermanager.h \
    settings.h \
    qtimermessagebox.h

FORMS += settingsdialog.ui

DESTDIR = .

target.path = /usr/bin
iconstarget.path = /usr/share/qbat/icons
iconstarget.files = res/*.png res/*.svg
langtarget.path = /usr/share/qbat/lang
langtarget.files = qbat_*.qm
shortcuttarget.path = /usr/share/applications
shortcuttarget.files = qbat.desktop

INSTALLS += target iconstarget langtarget shortcuttarget
