#ifndef APPS_CLOCK_H
#define APPS_CLOCK_H

#include <QColor>
#include <QObject>

class MediaManager;

class Clock : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(qreal backgroundOpacity READ backgroundOpacity WRITE setBackgroundOpacity NOTIFY backgroundOpacityChanged)
    Q_PROPERTY(QString background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(QColor hourColor READ hourColor WRITE setHourColor NOTIFY hourColorChanged)
    Q_PROPERTY(QColor minuteColor READ minuteColor WRITE setMinuteColor NOTIFY minuteColorChanged)
    Q_PROPERTY(QColor secondColor READ secondColor WRITE setSecondColor NOTIFY secondColorChanged)
    Q_PROPERTY(QColor pendulumBobColor READ pendulumBobColor WRITE setPendulumBobColor NOTIFY pendulumBobColorChanged)
    Q_PROPERTY(QColor pendulumRodColor READ pendulumRodColor WRITE setPendulumRodColor NOTIFY pendulumRodColorChanged)

  public:
    Clock(MediaManager& mediaManager, QObject* parent = nullptr);

    bool enabled() const;
    void setEnabled(const bool& enabled);

    qreal backgroundOpacity() const;
    void setBackgroundOpacity(const qreal& backgroundOpacity);

    QString background() const;
    void setBackground(const QString& background);

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

  signals:
    void enabledChanged();
    void backgroundOpacityChanged();
    void backgroundChanged();
    void hourColorChanged();
    void minuteColorChanged();
    void secondColorChanged();
    void pendulumBobColorChanged();
    void pendulumRodColorChanged();

  private:
    void loadProperties();

    // Properties
    bool m_enabled;
    qreal m_backgroundOpacity;
    QString m_background;
    QColor m_hourColor;
    QColor m_minuteColor;
    QColor m_secondColor;
    QColor m_pendulumBobColor;
    QColor m_pendulumRodColor;

    // Dependencies
    MediaManager& m_mediaManager;
};

#endif // APPS_CLOCK_H