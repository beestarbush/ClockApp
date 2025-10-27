#ifndef MEDIALIST_H
#define MEDIALIST_H

#include "SerializableObject.h"
#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QByteArray>
#include <QList>

class MediaInfo;

/**
 * Media List domain object
 * Represents a list of media IDs available for a device
 */
class MediaList : public SerializableObject
{
public:
    MediaList();
    
    QList<QString> mediaIds;  // List of media IDs (not full MediaInfo objects)
    
    // SerializableObject interface
    QJsonObject toJson() const override;
    bool isValid() const override;
    QString typeName() const override { return "MediaList"; }

    // Endpoint routing
    QString getEndpoint() const override;
    QString getFetchEndpoint() const override;
    
    // Deserialization
    static MediaList fromJson(const QJsonObject &json);
    
    // Local persistence (media.json file)
    void saveToFile(const QString &directory) const;
    static MediaList loadFromFile(const QString &directory);
    static QString getMediaListFilePath(const QString &directory);
    
    // Convenience methods
    int count() const;
    bool isEmpty() const;
};

#endif // MEDIALIST_H