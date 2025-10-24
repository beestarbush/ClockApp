#ifndef SERVICES_MEDIAMANAGER_H
#define SERVICES_MEDIAMANAGER_H

#include <QObject>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QTimer>

class MediaManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableMedia READ availableMedia NOTIFY availableMediaChanged)

public:
    explicit MediaManager(QObject *parent = nullptr);

    QStringList availableMedia() const;

    Q_INVOKABLE QString getMediaPath(const QString &name) const;

signals:
    void availableMediaChanged();

private slots:
    void onDirectoryChanged(const QString &path);

private:
    void setupFileWatcher();
    void scanDirectory();
    bool isValidFile(const QString &filePath) const;

    QStringList m_availableMedia;
    QFileSystemWatcher m_fileWatcher;
    QTimer m_scanTimer;
};

#endif // SERVICES_MEDIAMANAGER_H
