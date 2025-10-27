#include "BirthdayTimer.h"
#include <QDebug>
#include <QSettings>

// BirthdayTimer-specific constants
const QString PROPERTY_BAR_COLOR_KEY = QStringLiteral("bar-color");
const QColor PROPERTY_BAR_COLOR_DEFAULT = QColor("#02996c");
const QString PROPERTY_TEXT_COLOR_KEY = QStringLiteral("text-color");
const QColor PROPERTY_TEXT_COLOR_DEFAULT = QColor("#BBBBBB");

BirthdayTimer::BirthdayTimer(const QString& name, MediaManager& mediaManager, QObject* parent)
    : TimeElapsedTimer(name, mediaManager, parent),
      m_barColor(PROPERTY_BAR_COLOR_DEFAULT),
      m_textColor(PROPERTY_TEXT_COLOR_DEFAULT)
{
    loadAdditionalProperties();
}

void BirthdayTimer::loadAdditionalProperties()
{
    static QSettings settings;
    settings.beginGroup(m_name);
    m_barColor = settings.value(PROPERTY_BAR_COLOR_KEY, PROPERTY_BAR_COLOR_DEFAULT).value<QColor>();
    m_textColor = settings.value(PROPERTY_TEXT_COLOR_KEY, PROPERTY_TEXT_COLOR_DEFAULT).value<QColor>();
    settings.endGroup();
}

QColor BirthdayTimer::barColor() const
{
    return m_barColor;
}

void BirthdayTimer::setBarColor(const QColor& barColor)
{
    if (m_barColor == barColor) {
        return;
    }

    saveProperty(PROPERTY_BAR_COLOR_KEY, barColor);
    m_barColor = barColor;
    emit barColorChanged();
}

QColor BirthdayTimer::textColor() const
{
    return m_textColor;
}

void BirthdayTimer::setTextColor(const QColor& textColor)
{
    if (m_textColor == textColor) {
        return;
    }

    saveProperty(PROPERTY_TEXT_COLOR_KEY, textColor);
    m_textColor = textColor;
    emit textColorChanged();
}
