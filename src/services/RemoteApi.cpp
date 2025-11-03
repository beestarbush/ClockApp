#include "RemoteApi.h"
#include "remoteapi/SerializableObject.h"

#include "hal/Network.h"

#include <QDebug>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QSslSocket>
#include <QTimer>

constexpr int NETWORK_TIMEOUT_MS = 30 * 1000; // 30 seconds
const QString PROPERTIES_GROUP_NAME = QStringLiteral("remote-api");
const QString PROPERTY_ENABLED_KEY = QStringLiteral("enabled");
const bool PROPERTY_ENABLED_DEFAULT = false;
const QString PROPERTY_SERVER_URL_KEY = QStringLiteral("url");
const QString PROPERTY_SERVER_URL_DEFAULT = QStringLiteral("https://bijsterbosch.org/clock");
const QString PROPERTY_DEVICE_ID_KEY = QStringLiteral("device-id");
const QString PROPERTY_DEVICE_ID_DEFAULT = QStringLiteral("SN-XXXX");

RemoteApi::RemoteApi(Network& network, QObject* parent)
    : QObject(parent),
      m_network(network),
      m_networkManager(this),
      m_enabled(PROPERTY_ENABLED_DEFAULT),
      m_serverUrl(PROPERTY_SERVER_URL_DEFAULT),
      m_deviceId(PROPERTY_DEVICE_ID_DEFAULT),
      m_connected(false),
      m_pendingRequests()
{
    if (QSslSocket::supportsSsl()) {
        qInfo() << "SSL build version:" << QSslSocket::sslLibraryBuildVersionString();
        qInfo() << "CA certs available:" << QSslConfiguration::defaultConfiguration().caCertificates().count();
    }
    else {
        qWarning() << "SSL is not supported on this system. Remote API connections will fail.";
    }

    loadProperties();

    // Test connection if enabled and configured
    if (m_network.connected() && m_enabled && !m_serverUrl.isEmpty()) {
        QTimer::singleShot(1000, this, [this]() {
            testConnection();
        });
    }

    // When network connectivity changes, retest connection if needed.
    connect(&m_network, &Network::connectedChanged, this, [this]() {
        if (m_network.connected() && m_enabled && !m_serverUrl.isEmpty()) {
            testConnection();
        }
        else {
            setConnected(false);
        }
    });
}

bool RemoteApi::enabled() const
{
    return m_enabled;
}

QString RemoteApi::serverUrl() const
{
    return m_serverUrl;
}

QString RemoteApi::deviceId() const
{
    return m_deviceId;
}

bool RemoteApi::connected() const
{
    return m_connected;
}

void RemoteApi::setEnabled(bool enabled)
{
    if (m_enabled == enabled) {
        return;
    }

    saveProperty(PROPERTY_ENABLED_KEY, enabled);
    m_enabled = enabled;
    emit enabledChanged();
}

void RemoteApi::setServerUrl(const QString& url)
{
    if (m_serverUrl == url) {
        return;
    }

    saveProperty(PROPERTY_SERVER_URL_KEY, url);
    m_serverUrl = url;
    emit serverUrlChanged();
    setConnected(false);
}

void RemoteApi::setDeviceId(const QString& id)
{
    if (m_deviceId == id) {
        return;
    }

    saveProperty(PROPERTY_DEVICE_ID_KEY, id);
    m_deviceId = id;
    emit deviceIdChanged();
}

void RemoteApi::get(const QString& endpoint, ResponseCallback callback)
{
    if (!m_enabled || m_serverUrl.isEmpty()) {
        if (callback) {
            callback(false, QJsonObject(), "Server connection not configured");
        }
        return;
    }

    QNetworkRequest request = createRequest(endpoint);
    QNetworkReply* reply = m_networkManager.get(request);

    PendingRequest pending;
    pending.reply = reply;
    pending.responseCallback = callback;
    pending.isBinaryDownload = false;
    m_pendingRequests[reply] = pending;

    // Log SSL errors
    connect(reply, QOverload<const QList<QSslError>&>::of(&QNetworkReply::sslErrors), [](const QList<QSslError>& errors) {
        for (const auto& err : errors) {
            qWarning() << "SSL Error:" << err.errorString();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleResponse(reply);
    });

    qDebug() << "GET:" << request.url().toString();
}

void RemoteApi::post(const QString& endpoint, const QJsonObject& payload, ResponseCallback callback)
{
    if (!m_enabled || m_serverUrl.isEmpty()) {
        if (callback) {
            callback(false, QJsonObject(), "Server connection not configured");
        }
        return;
    }

    QNetworkRequest request = createRequest(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    QNetworkReply* reply = m_networkManager.post(request, data);

    PendingRequest pending;
    pending.reply = reply;
    pending.responseCallback = callback;
    pending.isBinaryDownload = false;
    m_pendingRequests[reply] = pending;

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleResponse(reply);
    });

    qDebug() << "POST:" << request.url().toString();
}

void RemoteApi::put(const QString& endpoint, const QJsonObject& payload, ResponseCallback callback)
{
    if (!m_enabled || m_serverUrl.isEmpty()) {
        if (callback) {
            callback(false, QJsonObject(), "Server connection not configured");
        }
        return;
    }

    QNetworkRequest request = createRequest(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    QNetworkReply* reply = m_networkManager.put(request, data);

    PendingRequest pending;
    pending.reply = reply;
    pending.responseCallback = callback;
    pending.isBinaryDownload = false;
    m_pendingRequests[reply] = pending;

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleResponse(reply);
    });

    qDebug() << "PUT:" << request.url().toString();
}

void RemoteApi::deleteRequest(const QString& endpoint, ResponseCallback callback)
{
    if (!m_enabled || m_serverUrl.isEmpty()) {
        if (callback) {
            callback(false, QJsonObject(), "Server connection not configured");
        }
        return;
    }

    QNetworkRequest request = createRequest(endpoint);
    QNetworkReply* reply = m_networkManager.deleteResource(request);

    PendingRequest pending;
    pending.reply = reply;
    pending.responseCallback = callback;
    pending.isBinaryDownload = false;
    m_pendingRequests[reply] = pending;

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleResponse(reply);
    });

    qDebug() << "DELETE:" << request.url().toString();
}

void RemoteApi::download(const QString& endpoint, DataCallback callback)
{
    if (!m_enabled || m_serverUrl.isEmpty()) {
        if (callback) {
            callback(false, QByteArray(), "Server connection not configured");
        }
        return;
    }

    QNetworkRequest request = createRequest(endpoint);
    QNetworkReply* reply = m_networkManager.get(request);

    PendingRequest pending;
    pending.reply = reply;
    pending.dataCallback = callback;
    pending.isBinaryDownload = true;
    m_pendingRequests[reply] = pending;

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleResponse(reply);
    });

    qDebug() << "DOWNLOAD:" << request.url().toString();
}

void RemoteApi::createObject(const SerializableObject& object, SuccessCallback callback)
{
    if (!object.isValid()) {
        if (callback) {
            callback(false, "Invalid object: " + object.typeName());
        }
        qWarning() << "Attempted to create invalid object:" << object.typeName();
        return;
    }

    QString endpoint = object.getCreateEndpoint();
    QJsonObject payload = object.toJson();

    post(endpoint, payload, [callback, endpoint](bool success, const QJsonObject& response, const QString& error) {
        if (callback) {
            callback(success, error);
        }

        if (success) {
            qDebug() << "Object created successfully at:" << endpoint;
        }
        else {
            qWarning() << "Failed to create object at" << endpoint << ":" << error;
        }
    });
}

void RemoteApi::updateObject(const SerializableObject& object, SuccessCallback callback)
{
    if (!object.isValid()) {
        if (callback) {
            callback(false, "Invalid object: " + object.typeName());
        }
        qWarning() << "Attempted to update invalid object:" << object.typeName();
        return;
    }

    QString endpoint = object.getUpdateEndpoint();
    QJsonObject payload = object.toJson();

    put(endpoint, payload, [callback, endpoint](bool success, const QJsonObject& response, const QString& error) {
        if (callback) {
            callback(success, error);
        }

        if (success) {
            qDebug() << "Object updated successfully at:" << endpoint;
        }
        else {
            qWarning() << "Failed to update object at" << endpoint << ":" << error;
        }
    });
}

void RemoteApi::deleteObject(const SerializableObject& object, SuccessCallback callback)
{
    if (!object.isValid()) {
        if (callback) {
            callback(false, "Invalid object: " + object.typeName());
        }
        qWarning() << "Attempted to delete invalid object:" << object.typeName();
        return;
    }

    QString endpoint = object.getDeleteEndpoint();

    deleteRequest(endpoint, [callback, endpoint](bool success, const QJsonObject& response, const QString& error) {
        if (callback) {
            callback(success, error);
        }

        if (success) {
            qDebug() << "Object deleted successfully at:" << endpoint;
        }
        else {
            qWarning() << "Failed to delete object at" << endpoint << ":" << error;
        }
    });
}

void RemoteApi::testConnection(std::function<void(bool, const QString&)> callback)
{
    if (!m_enabled || m_serverUrl.isEmpty()) {
        QString error = "Server not configured";
        if (callback) {
            callback(false, error);
        }
        setConnected(false);
        qWarning() << "Test connection failed:" << error;
        return;
    }

    get("/api/test", [this, callback](bool success, const QJsonObject& response, const QString& error) {
        if (success) {
            QString message = response["message"].toString();
            if (message.isEmpty()) {
                message = "Connection successful";
            }

            setConnected(true);
            qDebug() << "Connection test successful:" << message;

            if (callback) {
                callback(true, message);
            }
        }
        else {
            setConnected(false);
            qWarning() << "Connection test failed:" << error;

            if (callback) {
                callback(false, error);
            }
        }
    });
}

void RemoteApi::loadProperties()
{
    QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    m_enabled = settings.value(PROPERTY_ENABLED_KEY, PROPERTY_ENABLED_DEFAULT).toBool();
    m_serverUrl = settings.value(PROPERTY_SERVER_URL_KEY, PROPERTY_SERVER_URL_DEFAULT).toString();
    m_deviceId = settings.value(PROPERTY_DEVICE_ID_KEY, PROPERTY_DEVICE_ID_DEFAULT).toString();
    settings.endGroup();
}

void RemoteApi::saveProperty(const QString& key, const QVariant& value)
{
    QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(key, value);
    settings.endGroup();
}

QNetworkRequest RemoteApi::createRequest(const QString& endpoint)
{
    QUrl url(m_serverUrl + endpoint);
    QNetworkRequest request(url);
    request.setTransferTimeout(NETWORK_TIMEOUT_MS);

    return request;
}

void RemoteApi::handleResponse(QNetworkReply* reply)
{
    reply->deleteLater();

    if (!m_pendingRequests.contains(reply)) {
        qWarning() << "Received response for unknown request";
        return;
    }

    PendingRequest pending = m_pendingRequests.take(reply);

    if (reply->error() != QNetworkReply::NoError) {
        QString error = reply->errorString();
        qWarning() << "Network error:" << error;

        if (pending.isBinaryDownload && pending.dataCallback) {
            pending.dataCallback(false, QByteArray(), error);
        }
        else if (pending.responseCallback) {
            pending.responseCallback(false, QJsonObject(), error);
        }

        setConnected(false);
        return;
    }

    QByteArray responseData = reply->readAll();

    if (pending.isBinaryDownload) {
        // Binary download (e.g., media file)
        if (pending.dataCallback) {
            pending.dataCallback(true, responseData, QString());
        }
        setConnected(true);
    }
    else {
        // JSON response
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isObject()) {
            QString error = "Invalid JSON response from server";
            qWarning() << error;
            if (pending.responseCallback) {
                pending.responseCallback(false, QJsonObject(), error);
            }
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (pending.responseCallback) {
            pending.responseCallback(true, jsonObj, QString());
        }

        setConnected(true);
    }
}

void RemoteApi::setConnected(bool connected)
{
    if (m_connected != connected) {
        m_connected = connected;
        emit connectedChanged();
    }
}
