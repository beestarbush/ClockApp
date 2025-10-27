#include "MediaList.h"
#include "MediaInfo.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDebug>

MediaList::MediaList()
{
}

QJsonObject MediaList::toJson() const
{
    QJsonObject json;
    
    if (!deviceId.isEmpty()) {
        json["device_id"] = deviceId;
    }
    
    QJsonArray mediaArray;
    for (const QString &mediaId : mediaIds) {
        mediaArray.append(mediaId);
    }
    json["media"] = mediaArray;
    
    return json;
}

MediaList MediaList::fromJson(const QJsonObject &json)
{
    MediaList list;
    
    list.deviceId = json["device_id"].toString();
    
    QJsonArray mediaArray = json["media"].toArray();
    for (const QJsonValue &value : mediaArray) {
        QString mediaId = value.toString();
        if (!mediaId.isEmpty()) {
            list.mediaIds.append(mediaId);
        }
    }
    
    return list;
}

int MediaList::count() const
{
    return mediaIds.count();
}

bool MediaList::isEmpty() const
{
    return mediaIds.isEmpty();
}

bool MediaList::isValid() const
{
    return !deviceId.isEmpty();
}

QString MediaList::getEndpoint() const
{
    if (deviceId.isEmpty()) {
        return "/api/media";
    }
    return QString("/api/media/%1").arg(deviceId);
}

QString MediaList::getFetchEndpoint() const
{
    if (deviceId.isEmpty()) {
        return "/api/media";
    }
    return QString("/api/media/%1/list").arg(deviceId);
}

void MediaList::saveToFile(const QString &directory) const
{
    QString filePath = getMediaListFilePath(directory);
    
    QJsonObject obj;
    obj["device_id"] = deviceId;
    
    QJsonArray mediaArray;
    for (const QString &mediaId : mediaIds) {
        mediaArray.append(mediaId);
    }
    obj["media"] = mediaArray;
    
    QJsonDocument doc(obj);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to save media list to" << filePath;
        return;
    }
    
    file.write(doc.toJson());
    file.close();
    
    qDebug() << "Saved" << mediaIds.count() << "media IDs to" << filePath;
}

MediaList MediaList::loadFromFile(const QString &directory)
{
    QString filePath = getMediaListFilePath(directory);
    QFile file(filePath);
    
    if (!file.exists()) {
        qDebug() << "Local media.json does not exist yet";
        return MediaList();
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open local media.json for reading";
        return MediaList();
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qWarning() << "Invalid local media.json format";
        return MediaList();
    }
    
    MediaList list = MediaList::fromJson(doc.object());
    
    qDebug() << "Loaded" << list.count() << "media IDs from local media.json";
    return list;
}

QString MediaList::getMediaListFilePath(const QString &directory)
{
    return QDir(directory).absoluteFilePath("media.json");
}
