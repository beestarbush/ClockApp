#ifndef HAL_NETWORK_DRIVER_H
#define HAL_NETWORK_DRIVER_H

#include <QObject>
#include <QTimer>

namespace Network
{
class Driver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString interfaceName READ interfaceName NOTIFY interfaceNameChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(QString ipAddress READ ipAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(QString subnetMask READ subnetMask NOTIFY subnetMaskChanged)

  public:
    Driver(QObject* parent = nullptr);

    QString interfaceName() const;
    bool connected() const;
    bool running() const;
    QString ipAddress() const;
    QString subnetMask() const;

  signals:
    void interfaceNameChanged();
    void connectedChanged();
    void runningChanged();
    void ipAddressChanged();
    void subnetMaskChanged();

  private:
    void update();

    QTimer m_refreshTimer;
    QString m_interfaceName;
    bool m_connected;
    bool m_running;
    QString m_ipAddress;
    QString m_subnetMask;
};
} // namespace Network

#endif // HAL_NETWORK_DRIVER_H
