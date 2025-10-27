#include "Screen.h"
#include <QDebug>
#include <QFile>
#include <QSettings>

#ifdef PLATFORM_IS_TARGET
const QString BRIGHTNESS_FILE_PATH = QStringLiteral("/sys/class/backlight/11-0045/brightness");
#else
const QString BRIGHTNESS_FILE_PATH = QStringLiteral("/tmp/brightness");
#endif
const QString PROPERTY_GROUP_NAME = QStringLiteral("screen");
const QString PROPERTY_BRIGHTNESS_KEY = QStringLiteral("brightness");
constexpr quint8 PROPERTY_BRIGHTNESS_DEFAULT = 100;

Screen::Screen(QObject* parent)
    : QObject(parent),
      m_brightness(0)
{
    loadProperties();
    writeBrightnessToFile(BRIGHTNESS_FILE_PATH, m_brightness);
}

qint8 Screen::brightness() const
{
    return m_brightness;
}

void Screen::setBrightness(const qint8 value)
{
    if (m_brightness != value) {
        m_brightness = value;
        saveProperty(PROPERTY_BRIGHTNESS_KEY, m_brightness);
        writeBrightnessToFile(BRIGHTNESS_FILE_PATH, m_brightness);
        emit brightnessChanged(m_brightness);
    }
}

qint8 Screen::readBrightnessFromFile(const QString& filePath) const
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool ok;
        qint8 value = in.readLine().toInt(&ok);
        file.close();
        if (!ok) {
            qWarning() << "Failed to read brightness value from file:" << filePath;
            return 0;
        }

        // The value read from the file is a range from 0 to 31, so we scale it to 0-100.
        value = (value * 100) / 31; // Assuming the file contains a value between 0 and 31
        return value;
    }
    return 0; // Default value if reading fails
}

void Screen::writeBrightnessToFile(const QString& filePath, qint8 value) const
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // Scale the value back to the range of 0-31 for writing to the file.
        int scaledValue = (value * 31) / 100; // Assuming we want to write a value between 0 and 31
        out << scaledValue;
        file.flush();
        file.close();
    }
    else {
        qWarning() << "Failed to open file for writing:" << filePath;
    }
}

void Screen::loadProperties()
{
    static QSettings settings;
    settings.beginGroup(PROPERTY_GROUP_NAME);

    m_brightness = settings.value(PROPERTY_BRIGHTNESS_KEY, PROPERTY_BRIGHTNESS_DEFAULT).toUInt();

    settings.endGroup();
}

void Screen::saveProperty(const QString& key, const QVariant& value)
{
    static QSettings settings;
    settings.beginGroup(PROPERTY_GROUP_NAME);
    settings.setValue(key, value);
    settings.endGroup();
}