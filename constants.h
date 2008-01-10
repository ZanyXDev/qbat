#ifndef QBAT_CONSTANTS_H
#define QBAT_CONSTANTS_H

#define UI_VERSION                      "0.1.3"
#define UI_NAME                         tr("QBat - Qt Battery Monitor")

#define UI_DIR_WORK                     ".qbat/"

#define UI_PATH_SYSFS_DIR               "/sys/class/power_supply"
#define UI_PATH_TRANSLATIONS            "/usr/share/qbat/lang/"
#define UI_PATH_ICONS                   "/usr/share/qbat/icons/"
#define UI_PATH_WORK                    (QDir::toNativeSeparators(QDir::homePath()) + "/" UI_DIR_WORK)

#define UI_FILE_CONFIG                  (UI_PATH_WORK + "qbat.conf")

#define UI_ICON_QBAT                    UI_PATH_ICONS "qbat.svg"

#define UI_BATTERY_CHARGING             1
#define UI_BATTERY_DISCHARGING          2
#define UI_BATTERY_FULL                 3

#define UI_COUNT_COLORS                 7

#define UI_COLOR_PEN                    0
#define UI_COLOR_PEN_FULL               1
#define UI_COLOR_BRUSH_CHARGED          2
#define UI_COLOR_BRUSH_EMPTY            3
#define UI_COLOR_BRUSH_FULL             4
#define UI_COLOR_BRUSH_POLE             5
#define UI_COLOR_BRUSH_POLE_FULL        6

#endif
