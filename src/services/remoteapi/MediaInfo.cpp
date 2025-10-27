#include "MediaInfo.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDebug>

MediaInfo::MediaInfo()
    : size(0)
{
}

QJsonObject MediaInfo::toJson() const
{
    QJsonObject json;
    
    if (!id.isEmpty()) {
        json["id"] = id;
    }
    if (!filename.isEmpty()) {
        json["filename"] = filename;
    }
    
    json["size"] = size;
    
    if (uploadedAt.isValid()) {
        json["uploaded_at"] = uploadedAt.toString(Qt::ISODate);
    }
    if (!mimeType.isEmpty()) {
        json["mime_type"] = mimeType;
    }
    
    return json;
}

MediaInfo MediaInfo::fromJson(const QJsonObject &json)
{
    MediaInfo info;
    
    // Check if this is the full response with "media" wrapper
    if (json.contains("media") && json["media"].isObject()) {
        QJsonObject mediaObj = json["media"].toObject();
        info.id = mediaObj["id"].toString();
        info.filename = mediaObj["filename"].toString();
        info.size = mediaObj["size"].toInteger();
        info.mimeType = mediaObj["mime_type"].toString();
        
        QString uploadedAtStr = mediaObj["uploaded_at"].toString();
        if (!uploadedAtStr.isEmpty()) {
            info.uploadedAt = QDateTime::fromString(uploadedAtStr, Qt::ISODate);
        }
    } else {
        // Direct media object
        info.id = json["id"].toString();
        info.filename = json["filename"].toString();
        info.size = json["size"].toInteger();
        info.mimeType = json["mime_type"].toString();
        
        QString uploadedAtStr = json["uploaded_at"].toString();
        if (!uploadedAtStr.isEmpty()) {
            info.uploadedAt = QDateTime::fromString(uploadedAtStr, Qt::ISODate);
        }
    }
    
    return info;
}

MediaInfo MediaInfo::fromBinary(const QByteArray &data, const QString &id, const QString &filename)
{
    MediaInfo info;
    info.id = id;
    info.filename = filename;
    info.binaryData = data;
    info.size = data.size();
    return info;
}

bool MediaInfo::isValid() const
{
    return !id.isEmpty() && !filename.isEmpty();
}

QString MediaInfo::getEndpoint() const
{
    if (deviceId.isEmpty()) {
        return "/api/media";
    }
    return QString("/api/media/%1").arg(deviceId);
}

QString MediaInfo::getCreateEndpoint() const
{
    if (deviceId.isEmpty()) {
        return "/api/media";
    }
    return QString("/api/media/%1/upload").arg(deviceId);
}

QString MediaInfo::getDeleteEndpoint() const
{
    if (deviceId.isEmpty() || id.isEmpty()) {
        return getEndpoint();
    }
    return QString("/api/media/%1/delete/%2").arg(deviceId, id);
}

QString MediaInfo::getFetchEndpoint() const
{
    if (deviceId.isEmpty() || id.isEmpty()) {
        return getEndpoint();
    }
    // Fetch JSON metadata from /{deviceId}/{id}
    return QString("/api/media/%1/%2").arg(deviceId, id);
}

QString MediaInfo::getBinaryDataEndpoint() const
{
    if (deviceId.isEmpty() || id.isEmpty()) {
        qDebug() << "Cannot get binary data endpoint: deviceId or id is empty";
        return getEndpoint();
    }
    // Download binary data from /download/{id}
    return QString("/api/media/%1/download/%2").arg(deviceId, id);
}

void MediaInfo::saveMetadata(const QString &directory) const
{
    QString filePath = getMetadataFilePath(directory, id);
    
    QJsonObject obj;
    obj["id"] = id;
    obj["filename"] = filename;
    obj["size"] = static_cast<qint64>(size);
    obj["uploaded_at"] = uploadedAt.toString(Qt::ISODate);
    obj["mime_type"] = mimeType;
    
    QJsonDocument doc(obj);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to save metadata for media" << id;
        return;
    }
    
    file.write(doc.toJson());
    file.close();
    
    qDebug() << "Saved metadata for media" << id << "to" << filePath;
}

MediaInfo MediaInfo::loadMetadata(const QString &directory, const QString &mediaId)
{
    QString filePath = getMetadataFilePath(directory, mediaId);
    QFile file(filePath);
    
    MediaInfo info;
    
    if (!file.exists()) {
        qDebug() << "Metadata file does not exist for media" << mediaId;
        return info;
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open metadata file for media" << mediaId;
        return info;
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qWarning() << "Invalid metadata format for media" << mediaId;
        return info;
    }
    
    QJsonObject obj = doc.object();
    info.id = obj["id"].toString();
    info.filename = obj["filename"].toString();
    info.size = obj["size"].toVariant().toULongLong();
    info.uploadedAt = QDateTime::fromString(obj["uploaded_at"].toString(), Qt::ISODate);
    info.mimeType = obj["mime_type"].toString();
    
    return info;
}

void MediaInfo::deleteMetadata(const QString &directory, const QString &mediaId)
{
    QString filePath = getMetadataFilePath(directory, mediaId);
    
    if (QFile::exists(filePath)) {
        if (QFile::remove(filePath)) {
            qDebug() << "Deleted metadata file for media" << mediaId;
        } else {
            qWarning() << "Failed to delete metadata file for media" << mediaId;
        }
    }
}

QString MediaInfo::getMetadataFilePath(const QString &directory, const QString &mediaId)
{
    return QDir(directory).absoluteFilePath(mediaId + ".json");
}
