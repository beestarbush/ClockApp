#include "Configuration.h"
#include <QDebug>
#include <QJsonObject>
#include <QSettings>
using namespace Clock;

const QString PROPERTY_HOUR_COLOR_KEY = QStringLiteral("hour-color");
const QColor PROPERTY_HOUR_COLOR_DEFAULT = QColor("#995000");
const QString PROPERTY_MINUTE_COLOR_KEY = QStringLiteral("minute-color");
const QColor PROPERTY_MINUTE_COLOR_DEFAULT = QColor("#005099");
const QString PROPERTY_SECOND_COLOR_KEY = QStringLiteral("second-color");
const QColor PROPERTY_SECOND_COLOR_DEFAULT = QColor("#009950");
const QString PROPERTY_PENDULUM_BOB_COLOR_KEY = QStringLiteral("pendulum-bob-color");
const QColor PROPERTY_PENDULUM_BOB_COLOR_DEFAULT = QColor("#009950");
const QString PROPERTY_PENDULUM_ROD_COLOR_KEY = QStringLiteral("pendulum-rod-color");
const QColor PROPERTY_PENDULUM_ROD_COLOR_DEFAULT = QColor("#333333");

Configuration::Configuration(QString name, QObject* parent)
    : Common::Configuration(name, parent),
      m_hourColor(PROPERTY_HOUR_COLOR_DEFAULT),
      m_minuteColor(PROPERTY_MINUTE_COLOR_DEFAULT),
      m_secondColor(PROPERTY_SECOND_COLOR_DEFAULT),
      m_pendulumBobColor(PROPERTY_PENDULUM_BOB_COLOR_DEFAULT),
      m_pendulumRodColor(PROPERTY_PENDULUM_ROD_COLOR_DEFAULT)
{
}

void Configuration::load()
{
    Common::Configuration::load();

    static QSettings settings;
    settings.beginGroup(m_name);
    m_hourColor = settings.value(PROPERTY_HOUR_COLOR_KEY, PROPERTY_HOUR_COLOR_DEFAULT).value<QColor>();
    m_minuteColor = settings.value(PROPERTY_MINUTE_COLOR_KEY, PROPERTY_MINUTE_COLOR_DEFAULT).value<QColor>();
    m_secondColor = settings.value(PROPERTY_SECOND_COLOR_KEY, PROPERTY_SECOND_COLOR_DEFAULT).value<QColor>();
    m_pendulumBobColor = settings.value(PROPERTY_PENDULUM_BOB_COLOR_KEY, PROPERTY_PENDULUM_BOB_COLOR_DEFAULT).value<QColor>();
    m_pendulumRodColor = settings.value(PROPERTY_PENDULUM_ROD_COLOR_KEY, PROPERTY_PENDULUM_ROD_COLOR_DEFAULT).value<QColor>();
    settings.endGroup();
}

QJsonObject Configuration::toJson() const
{
    auto json = Common::Configuration::toJson();
    json[PROPERTY_HOUR_COLOR_KEY] = m_hourColor.name();
    json[PROPERTY_MINUTE_COLOR_KEY] = m_minuteColor.name();
    json[PROPERTY_SECOND_COLOR_KEY] = m_secondColor.name();
    json[PROPERTY_PENDULUM_BOB_COLOR_KEY] = m_pendulumBobColor.name();
    json[PROPERTY_PENDULUM_ROD_COLOR_KEY] = m_pendulumRodColor.name();

    return json;
}

void Configuration::fromJson(const QJsonObject& json)
{
    Common::Configuration::fromJson(json);

    if (json.contains(PROPERTY_HOUR_COLOR_KEY)) {
        setHourColor(QColor(json[PROPERTY_HOUR_COLOR_KEY].toString()));
    }
    if (json.contains(PROPERTY_MINUTE_COLOR_KEY)) {
        setMinuteColor(QColor(json[PROPERTY_MINUTE_COLOR_KEY].toString()));
    }
    if (json.contains(PROPERTY_SECOND_COLOR_KEY)) {
        setSecondColor(QColor(json[PROPERTY_SECOND_COLOR_KEY].toString()));
    }
    if (json.contains(PROPERTY_PENDULUM_BOB_COLOR_KEY)) {
        setPendulumBobColor(QColor(json[PROPERTY_PENDULUM_BOB_COLOR_KEY].toString()));
    }
    if (json.contains(PROPERTY_PENDULUM_ROD_COLOR_KEY)) {
        setPendulumRodColor(QColor(json[PROPERTY_PENDULUM_ROD_COLOR_KEY].toString()));
    }
}

QColor Configuration::hourColor() const
{
    return m_hourColor;
}

void Configuration::setHourColor(const QColor& hourColor)
{
    if (m_hourColor == hourColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(PROPERTY_HOUR_COLOR_KEY, hourColor);
    settings.endGroup();

    m_hourColor = hourColor;
    emit hourColorChanged();
}

QColor Configuration::minuteColor() const
{
    return m_minuteColor;
}

void Configuration::setMinuteColor(const QColor& minuteColor)
{
    if (m_minuteColor == minuteColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(PROPERTY_MINUTE_COLOR_KEY, minuteColor);
    settings.endGroup();

    m_minuteColor = minuteColor;
    emit minuteColorChanged();
}

QColor Configuration::secondColor() const
{
    return m_secondColor;
}

void Configuration::setSecondColor(const QColor& secondColor)
{
    if (m_secondColor == secondColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(PROPERTY_SECOND_COLOR_KEY, secondColor);
    settings.endGroup();

    m_secondColor = secondColor;
    emit secondColorChanged();
}

QColor Configuration::pendulumBobColor() const
{
    return m_pendulumBobColor;
}

void Configuration::setPendulumBobColor(const QColor& pendulumBobColor)
{
    if (m_pendulumBobColor == pendulumBobColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(PROPERTY_PENDULUM_BOB_COLOR_KEY, pendulumBobColor);
    settings.endGroup();

    m_pendulumBobColor = pendulumBobColor;
    emit pendulumBobColorChanged();
}

QColor Configuration::pendulumRodColor() const
{
    return m_pendulumRodColor;
}

void Configuration::setPendulumRodColor(const QColor& pendulumRodColor)
{
    if (m_pendulumRodColor == pendulumRodColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(PROPERTY_PENDULUM_ROD_COLOR_KEY, pendulumRodColor);
    settings.endGroup();

    m_pendulumRodColor = pendulumRodColor;
    emit pendulumRodColorChanged();
}

Configuration& Configuration::operator=(const Configuration& other)
{
    if (this != &other) {
        // Copy member variables
        Common::Configuration::operator=(other);

        setHourColor(other.m_hourColor);
        setMinuteColor(other.m_minuteColor);
        setSecondColor(other.m_secondColor);
        setPendulumBobColor(other.m_pendulumBobColor);
        setPendulumRodColor(other.m_pendulumRodColor);
    }
    return *this;
}

namespace Clock
{
QDebug operator<<(QDebug debug, const Configuration& config)
{
    QDebugStateSaver saver(debug);
    // Print base class properties first
    debug.nospace() << static_cast<const Common::Configuration&>(config);
    debug.nospace() << "Clock: (\n"
                    << " - hourColor=" << config.hourColor() << "\n"
                    << " - minuteColor=" << config.minuteColor() << "\n"
                    << " - secondColor=" << config.secondColor() << "\n"
                    << " - pendulumBobColor=" << config.pendulumBobColor() << "\n"
                    << " - pendulumRodColor=" << config.pendulumRodColor() << ")";
    return debug;
}
} // namespace Clock
