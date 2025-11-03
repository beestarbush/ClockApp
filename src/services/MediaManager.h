#ifndef SERVICES_MEDIAMANAGER_H
#define SERVICES_MEDIAMANAGER_H

#include <QDateTime>
#include <QFileSystemWatcher>
#include <QObject>
#include <QStringList>
#include <QTimer>

class RemoteApi;
class MediaInfo;

class MediaManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableMedia READ availableMedia NOTIFY availableMediaChanged)
    Q_PROPERTY(bool syncing READ syncing NOTIFY syncingChanged)
    Q_PROPERTY(QDateTime lastSyncTime READ lastSyncTime NOTIFY lastSyncTimeChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

  public:
    explicit MediaManager(RemoteApi& remoteApi, QObject* parent = nullptr);

    QStringList availableMedia() const;
    bool syncing() const;
    QDateTime lastSyncTime() const;
    QString lastError() const;

    Q_INVOKABLE QString getMediaPath(const QString& name) const;
    Q_INVOKABLE void triggerSync();

  signals:
    void availableMediaChanged();
    void syncingChanged();
    void lastSyncTimeChanged();
    void lastErrorChanged();
    void syncCompleted();

  private slots:
    void onDirectoryChanged(const QString& path);
    void onSyncTimerTimeout();

  private:
    void setupFileWatcher();
    void scanDirectory();
    bool isValidFile(const QString& filePath) const;
    QString getMediaDirectory() const;

    void fetchMediaList();
    void downloadMedia(const QString& mediaId);
    void cleanupOldMedia(const QStringList& serverMediaList);
    void completeSyncWithSuccess();
    void completeSyncWithError(const QString& error);

    QStringList m_availableMedia;
    QFileSystemWatcher m_fileWatcher;
    QTimer m_scanTimer;
    QTimer m_syncTimer;
    RemoteApi& m_remoteApi;

    bool m_syncing;
    QDateTime m_lastSyncTime;
    QString m_lastError;
    QStringList m_pendingDownloads;
    QStringList m_downloadedFilenames;
    QStringList m_currentServerMediaIds; // Store server media IDs for final save
    int m_activeDownloads;
};

#endif // SERVICES_MEDIAMANAGER_H
