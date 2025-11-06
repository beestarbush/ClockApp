#ifndef CLOCKCONFIG_H
#define CLOCKCONFIG_H

#include "applications/common/Configuration.h"
#include <QColor>
#include <QDebug>

namespace Applications::Clock
{
class Configuration : public Common::Configuration
{
    Q_OBJECT
    Q_PROPERTY(QColor hourColor READ hourColor WRITE setHourColor NOTIFY hourColorChanged)
    Q_PROPERTY(QColor minuteColor READ minuteColor WRITE setMinuteColor NOTIFY minuteColorChanged)
    Q_PROPERTY(QColor secondColor READ secondColor WRITE setSecondColor NOTIFY secondColorChanged)
    Q_PROPERTY(QColor pendulumBobColor READ pendulumBobColor WRITE setPendulumBobColor NOTIFY pendulumBobColorChanged)
    Q_PROPERTY(QColor pendulumRodColor READ pendulumRodColor WRITE setPendulumRodColor NOTIFY pendulumRodColorChanged)

  public:
    Configuration(QString name, QObject* parent = nullptr);

    void load() override;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);

    QColor hourColor() const;
    void setHourColor(const QColor& hourColor);

    QColor minuteColor() const;
    void setMinuteColor(const QColor& minuteColor);

    QColor secondColor() const;
    void setSecondColor(const QColor& secondColor);

    QColor pendulumBobColor() const;
    void setPendulumBobColor(const QColor& pendulumBobColor);

    QColor pendulumRodColor() const;
    void setPendulumRodColor(const QColor& pendulumRodColor);

    Configuration& operator=(const Configuration& other);
    friend QDebug operator<<(QDebug debug, const Configuration& config);

  signals:
    void hourColorChanged();
    void minuteColorChanged();
    void secondColorChanged();
    void pendulumBobColorChanged();
    void pendulumRodColorChanged();

  private:
    QColor m_hourColor;
    QColor m_minuteColor;
    QColor m_secondColor;
    QColor m_pendulumBobColor;
    QColor m_pendulumRodColor;
};
} // namespace Applications::Clock

#endif // CLOCKCONFIG_H
