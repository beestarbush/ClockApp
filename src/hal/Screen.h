#ifndef HAL_SCREEN_H
#define HAL_SCREEN_H

#include <QObject>

class Screen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint8 brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)

  public:
    Screen(QObject* parent = nullptr);

    qint8 brightness() const;
    void setBrightness(const qint8 value);

  signals:
    void brightnessChanged(qint8 brightness);

  private:
    qint8 m_brightness;
    qint8 readBrightnessFromFile(const QString& filePath) const;
    void writeBrightnessToFile(const QString& filePath, qint8 value) const;
    void loadProperties();
    void saveProperty(const QString& key, const QVariant& value);
};

#endif // HAL_SCREEN_H