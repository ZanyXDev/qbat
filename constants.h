#ifndef QBAT_CONSTANTS_H
#define QBAT_CONSTANTS_H

#define UI_VERSION                      "0.0.1"
#define UI_NAME                         tr("QBat - Qt Battery Monitor")

#define UI_DIR_WORK                     ".qbat/"

#define UI_PATH_SYSFS_DIR               "/sys/class/power_supply"
#define UI_PATH_TRANSLATIONS            "/usr/share/qbat/lang/"
#define UI_PATH_ICONS                   "/usr/share/qbat/icons/"
#define UI_PATH_WORK                    (QDir::toNativeSeparators(QDir::homePath()) + "/" UI_DIR_WORK)

#define UI_ICON_QBAT                    UI_PATH_ICONS "qbat.svg"

#endif
