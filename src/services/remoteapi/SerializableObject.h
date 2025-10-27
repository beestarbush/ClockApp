#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <QJsonObject>
#include <QString>
#include <QByteArray>

/**
 * Base class for all serializable domain objects
 * Provides common interface for JSON serialization/deserialization and API routing
 */
class SerializableObject
{
public:
    virtual ~SerializableObject() = default;
    
    // Common field for all domain objects
    QString deviceId;
    
    // Pure virtual methods that all domain objects must implement
    virtual QJsonObject toJson() const = 0;
    virtual bool isValid() const = 0;
    virtual QString typeName() const = 0;
    
    // Endpoint routing - domain objects know their own API endpoints
    virtual QString getEndpoint() const = 0;
    virtual QString getCreateEndpoint() const { return getEndpoint(); }
    virtual QString getUpdateEndpoint() const { return getEndpoint(); }
    virtual QString getDeleteEndpoint() const { return getEndpoint(); }
    virtual QString getFetchEndpoint() const { return getEndpoint(); }
    
    // Binary data support - override these for objects that include binary data
    virtual bool hasBinaryData() const { return false; }
    virtual QString getBinaryDataEndpoint() const { return getFetchEndpoint(); }
    virtual void setBinaryData(const QByteArray &data) { Q_UNUSED(data); }
};

#endif // SERIALIZABLEOBJECT_H
