#include "Screen.h"
#include <QFile>
#include <QDebug>

#ifdef PLATFORM_IS_TARGET
const QString BRIGHTNESS_FILE_PATH = QStringLiteral("/sys/devices/platform/axi/1000120000.pcie/1f00088000.i2c/i2c-10/10-0045/backlight/10-0045/brightness");
#else
const QString BRIGHTNESS_FILE_PATH = QStringLiteral("/tmp/brightness");
#endif


Screen::Screen(QObject *parent) :
    QObject(parent),
    m_brightness(0)
{
#ifdef PLATFORM_IS_TARGET
    m_brightness = readBrightnessFromFile(BRIGHTNESS_FILE_PATH);
#else
    m_brightness = 100; // Default value for non-target platforms, always write 100%.
    writeBrightnessToFile(BRIGHTNESS_FILE_PATH, m_brightness);
#endif
}

qint8 Screen::brightness() const
{
    return m_brightness;
}

void Screen::setBrightness(const qint8 value)
{
    if (m_brightness != value)
    {
        m_brightness = value;
        writeBrightnessToFile(BRIGHTNESS_FILE_PATH, m_brightness);
        emit brightnessChanged(m_brightness);
    }
}

qint8 Screen::readBrightnessFromFile(const QString &filePath) const
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        bool ok;
        qint8 value = in.readLine().toInt(&ok);
        file.close();
        if (!ok)
        {
            qWarning() << "Failed to read brightness value from file:" << filePath;
            return 0;
        }

        // The value read from the file is a range from 0 to 32, so we scale it to 0-100.
        value = (value * 100) / 32; // Assuming the file contains a value between 0 and 32
        return value;
    }
    return 0; // Default value if reading fails
}

void Screen::writeBrightnessToFile(const QString &filePath, qint8 value) const
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        // Scale the value back to the range of 0-32 for writing to the file.
        int scaledValue = (value * 32) / 100; // Assuming we want to write a value between 0 and 32
        out << scaledValue;
        file.flush();
        file.close();
    }
    else
    {
        qWarning() << "Failed to open file for writing:" << filePath;
    }
}