#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include "SerializableObject.h"
#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QByteArray>
#include <QList>

/**
 * Media Info domain object
 * Represents metadata about a media file
 */
class MediaInfo : public SerializableObject
{
public:
    MediaInfo();
    
    QString id;
    QString filename;
    qint64 size;
    QDateTime uploadedAt;
    QString mimeType;
    QByteArray binaryData; // The actual media file content (not serialized to JSON)
    
    // SerializableObject interface
    QJsonObject toJson() const override;
    bool isValid() const override;
    QString typeName() const override { return "MediaInfo"; }
    
    // Endpoint routing
    QString getEndpoint() const override;
    QString getCreateEndpoint() const override;
    QString getUpdateEndpoint() const override { return getEndpoint(); }
    QString getDeleteEndpoint() const override;
    QString getFetchEndpoint() const override;  // Fetches JSON metadata from /{deviceId}/{id}
    
    // Binary data endpoint - separate from metadata
    QString getBinaryDataEndpoint() const override;  // Downloads binary from /download/{id}
    
    // Deserialization
    static MediaInfo fromJson(const QJsonObject &json);
    static MediaInfo fromBinary(const QByteArray &data, const QString &id, const QString &filename);
    
    // Local persistence (metadata JSON files)
    void saveMetadata(const QString &directory) const;
    static MediaInfo loadMetadata(const QString &directory, const QString &mediaId);
    static void deleteMetadata(const QString &directory, const QString &mediaId);
    static QString getMetadataFilePath(const QString &directory, const QString &mediaId);
    
    // Check if binary data is loaded
    bool hasBinaryData() const override { return true; }
    void setBinaryData(const QByteArray &data) override { binaryData = data; }
};

#endif // MEDIAINFO_H
