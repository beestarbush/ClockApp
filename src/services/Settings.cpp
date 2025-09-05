#include "Settings.h"
#include <QSettings>

#ifdef PLATFORM_IS_TARGET
const QString SETTINGS_PATH = QStringLiteral("/usr/share/");
#else
const QString SETTINGS_PATH = QStringLiteral("/workdir/build");
#endif

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    QSettings::setDefaultFormat(QSettings::NativeFormat);
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, SETTINGS_PATH);
}
