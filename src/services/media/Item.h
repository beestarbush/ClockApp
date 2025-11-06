#ifndef SERVICES_MEDIA_ITEM_H
#define SERVICES_MEDIA_ITEM_H

#include <QObject>
#include <QString>

namespace Services::Media
{
class Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename CONSTANT)
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(bool isValid READ isValid CONSTANT)

  public:
    Item(QObject* parent = nullptr);
    Item(const QString& filename, const QString& path, QObject* parent = nullptr);

    QString filename() const
    {
        return m_filename;
    }
    QString path() const
    {
        return m_path;
    }
    bool isValid() const
    {
        return m_isValid;
    }

    void setFilename(const QString& filename)
    {
        m_filename = filename;
    }
    void setPath(const QString& path)
    {
        m_path = path;
    }
    void setIsValid(bool valid)
    {
        m_isValid = valid;
    }

  private:
    QString m_filename;
    QString m_path;
    bool m_isValid;
};
} // namespace Services::Media

#endif // SERVICES_MEDIA_ITEM_H
