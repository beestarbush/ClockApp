#ifndef SERVICES_REMOTE_API_H
#define SERVICES_REMOTE_API_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <functional>

class Network;
class SerializableObject;

class RemoteApi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    Q_PROPERTY(QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

  public:
    explicit RemoteApi(Network& network, QObject* parent = nullptr);

    // Properties
    bool enabled() const;
    QString serverUrl() const;
    QString deviceId() const;
    bool connected() const;

    void setEnabled(bool enabled);
    void setServerUrl(const QString& url);
    void setDeviceId(const QString& id);

    // Test connection to server
    void testConnection(std::function<void(bool success, const QString& message)> callback = nullptr);

    // Generic callback types
    using SuccessCallback = std::function<void(bool success, const QString& error)>;
    using ResponseCallback = std::function<void(bool success, const QJsonObject& response, const QString& error)>;
    using DataCallback = std::function<void(bool success, const QByteArray& data, const QString& error)>;

    // Generic domain object methods (automatically uses object's endpoints)
    void createObject(const SerializableObject& object, SuccessCallback callback);
    void updateObject(const SerializableObject& object, SuccessCallback callback);
    void deleteObject(const SerializableObject& object, SuccessCallback callback);

    template <typename T, typename Callback>
    void fetchObject(const T& objectWithId, Callback callback)
    {
        QString endpoint = objectWithId.getFetchEndpoint();

        get(endpoint, [callback, objectWithId, this](bool success, const QJsonObject& response, const QString& error) {
            if (!success) {
                callback(false, T(), error);
                return;
            }

            T object = T::fromJson(response);
            object.deviceId = objectWithId.deviceId; // Preserve deviceId

            // Check if this object needs binary data
            if (object.hasBinaryData()) {
                QString binaryEndpoint = object.getBinaryDataEndpoint();

                download(binaryEndpoint, [callback, object](bool binarySuccess, const QByteArray& data, const QString& binaryError) mutable {
                    if (!binarySuccess) {
                        callback(false, T(), binaryError);
                        return;
                    }

                    object.setBinaryData(data);
                    callback(true, object, QString());
                });
            }
            else {
                callback(true, object, QString());
            }
        });
    }

  signals:
    void enabledChanged();
    void serverUrlChanged();
    void deviceIdChanged();
    void connectedChanged();

  private:
    struct PendingRequest
    {
        QNetworkReply* reply;
        ResponseCallback responseCallback;
        DataCallback dataCallback;
        bool isBinaryDownload;
    };

    // Low-level HTTP methods (used internally by generic methods)
    void get(const QString& endpoint, ResponseCallback callback);
    void post(const QString& endpoint, const QJsonObject& payload, ResponseCallback callback);
    void put(const QString& endpoint, const QJsonObject& payload, ResponseCallback callback);
    void deleteRequest(const QString& endpoint, ResponseCallback callback);
    void download(const QString& endpoint, DataCallback callback);

    void loadProperties();
    void saveProperty(const QString& key, const QVariant& value);
    QNetworkRequest createRequest(const QString& endpoint);
    void handleResponse(QNetworkReply* reply);
    void setConnected(bool connected);

    Network& m_network;
    QNetworkAccessManager m_networkManager;
    bool m_enabled;
    QString m_serverUrl;
    QString m_deviceId;
    bool m_connected;
    QMap<QNetworkReply*, PendingRequest> m_pendingRequests;
};

#endif // SERVICES_REMOTE_API_H
