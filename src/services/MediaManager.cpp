#include "MediaManager.h"
#include "RemoteApi.h"
#include "remoteapi/MediaInfo.h"
#include "remoteapi/MediaList.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#ifdef PLATFORM_IS_TARGET
const QString MEDIA_PATH = QStringLiteral("/usr/share/bee/media");
#else
const QString MEDIA_PATH = QStringLiteral("/workdir/media");
#endif
const QString DEFAULT_MEDIA = QStringLiteral("qrc:/media/test.gif");
constexpr int MIN_MEDIA_FILE_SIZE = 50; // Minimum reasonable file size in bytes
constexpr int SCAN_DELAY_MS = 500; // Delay in milliseconds for scanning directory
constexpr int SYNC_INTERVAL_MS = 5 * 60 * 1000; // 5 minutes

MediaManager::MediaManager(RemoteApi* remoteApi, QObject *parent)
    : QObject(parent)
    , m_fileWatcher(this)
    , m_scanTimer(this)
    , m_syncTimer(this)
    , m_remoteApi(remoteApi)
    , m_syncing(false)
    , m_activeDownloads(0)
{
    // Setup delayed scanning to avoid excessive file system operations
    m_scanTimer.setSingleShot(true);
    m_scanTimer.setInterval(SCAN_DELAY_MS);
    connect(&m_scanTimer, &QTimer::timeout, this, &MediaManager::scanDirectory);

    // Setup sync timer
    m_syncTimer.setInterval(SYNC_INTERVAL_MS);
    connect(&m_syncTimer, &QTimer::timeout, this, &MediaManager::onSyncTimerTimeout);
    
    // Connect file watcher
    connect(&m_fileWatcher, &QFileSystemWatcher::directoryChanged, this, &MediaManager::onDirectoryChanged);

    // Setup file watcher and initial scan
    setupFileWatcher();
    scanDirectory();
    
    // Start sync timer if RemoteApi is enabled
    if (m_remoteApi && m_remoteApi->enabled()) {
        m_syncTimer.start();
        // Trigger initial sync after a short delay
        QTimer::singleShot(5000, this, &MediaManager::triggerSync);
    }
}

QStringList MediaManager::availableMedia() const
{
    return m_availableMedia;
}

bool MediaManager::syncing() const
{
    return m_syncing;
}

QDateTime MediaManager::lastSyncTime() const
{
    return m_lastSyncTime;
}

QString MediaManager::lastError() const
{
    return m_lastError;
}

void MediaManager::triggerSync()
{
    if (m_syncing) {
        qDebug() << "Sync already in progress";
        return;
    }
    
    if (!m_remoteApi || !m_remoteApi->enabled()) {
        qDebug() << "RemoteApi not enabled, skipping media sync";
        return;
    }
    
    startMediaSync();
}

QString MediaManager::getMediaPath(const QString &name) const
{
    if (name.isEmpty()) {
        return QString();
    }

    QString mediaDir = getMediaDirectory();
    QString fullPath = QDir(mediaDir).absoluteFilePath(name);

    // Verify the file exists and is valid
    if (QFileInfo::exists(fullPath) && isValidFile(fullPath)) {
        return fullPath;
    }

    qWarning() << "Requested media not found or invalid:" << fullPath << ", falling back to default.";
    return DEFAULT_MEDIA;
}

void MediaManager::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)
    // Use delayed scanning to avoid excessive updates when multiple files change
    m_scanTimer.start();
}

void MediaManager::onSyncTimerTimeout()
{
    triggerSync();
}

void MediaManager::setupFileWatcher()
{
    QString mediaDir = getMediaDirectory();

    QDir dir(mediaDir);
    if (dir.exists()) {
        if (!m_fileWatcher.directories().contains(mediaDir)) {
            m_fileWatcher.addPath(mediaDir);
        }
    } else {
        qWarning() << "Media directory does not exist:" << mediaDir;
    }
}

void MediaManager::scanDirectory()
{
    QStringList newMedia;
    QString mediaDir = getMediaDirectory();

    QDir dir(mediaDir);
    if (dir.exists()) {
        // Get all GIF, PNG, JPG, and JPEG files in the directory
        QStringList nameFilters;
        nameFilters << QStringLiteral("*.gif") << QStringLiteral("*.GIF")
                    << QStringLiteral("*.png") << QStringLiteral("*.PNG")
                    << QStringLiteral("*.jpg") << QStringLiteral("*.JPG")
                    << QStringLiteral("*.jpeg") << QStringLiteral("*.JPEG");
        
        QFileInfoList fileList = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable);
        
        for (const QFileInfo &fileInfo : fileList) {
            if (isValidFile(fileInfo.absoluteFilePath())) {
                newMedia.append(fileInfo.fileName());
            }
        }
        
        // Sort alphabetically for consistent ordering
        newMedia.sort();

        qDebug() << "Found" << newMedia.size() << "media files in" << mediaDir;
    } else {
        qWarning() << "Media directory does not exist:" << mediaDir;

        // Try to create the directory
        if (dir.mkpath(mediaDir)) {
            qDebug() << "Created media directory:" << mediaDir;
            setupFileWatcher();
        } else {
            qWarning() << "Failed to create media directory:" << mediaDir;
        }
    }

    // Update available media if changed
    if (newMedia != m_availableMedia) {
        m_availableMedia = newMedia;
        emit availableMediaChanged();
    }
}

bool MediaManager::isValidFile(const QString &filePath) const
{
    QFileInfo fileInfo(filePath);
    
    // Check file extension
    QString suffix = fileInfo.suffix().toLower();
    if (suffix != "gif" && suffix != "png" && suffix != "jpg" && suffix != "jpeg") {
        qDebug() << "Invalid media file extension:" << filePath;
        return false;
    }

    // Basic file size check (avoid empty files)
    if (fileInfo.size() < MIN_MEDIA_FILE_SIZE) { // Minimum reasonable file size
        qDebug() << "Invalid media file size:" << filePath;
        return false;
    }

    // Could add more sophisticated validation here (e.g., reading file header)
    // For now, extension and size check should be sufficient

    return true;
}

QString MediaManager::getMediaDirectory() const
{
    return MEDIA_PATH;
}

void MediaManager::startMediaSync()
{
    m_syncing = true;
    m_lastError.clear();
    emit syncingChanged();
    emit lastErrorChanged();
    
    qDebug() << "Starting media sync...";
    fetchMediaList();
}

void MediaManager::fetchMediaList()
{
    MediaList listTemplate;
    listTemplate.deviceId = m_remoteApi->deviceId();
    
    m_remoteApi->fetchObject(listTemplate, [this](bool success, const MediaList &list, const QString &error) {
        if (!success) {
            completeSyncWithError("Failed to fetch media list: " + error);
            return;
        }
        
        qDebug() << "Fetched" << list.count() << "media IDs from server";
        
        // Load local media list
        MediaList localList = MediaList::loadFromFile(getMediaDirectory());
        QStringList localMediaIds = localList.mediaIds;
        QStringList serverMediaIds = list.mediaIds;
        
        qDebug() << "Local media.json has" << localMediaIds.count() << "media IDs";
        
        // Determine what to add and what to remove
        QStringList mediaToDownload;
        QStringList mediaToDelete;
        
        // Find media to download (in server but not in local)
        for (const QString &serverId : serverMediaIds) {
            if (!localMediaIds.contains(serverId)) {
                mediaToDownload.append(serverId);
            }
        }
        
        // Find media to delete (in local but not in server)
        for (const QString &localId : localMediaIds) {
            if (!serverMediaIds.contains(localId)) {
                mediaToDelete.append(localId);
            }
        }
        
        qDebug() << "Media to download:" << mediaToDownload.count();
        qDebug() << "Media to delete:" << mediaToDelete.count();
        
        // Delete old media files first (using metadata to find filenames)
        if (!mediaToDelete.isEmpty()) {
            for (const QString &mediaId : mediaToDelete) {
                // Load metadata to get filename
                MediaInfo metadata = MediaInfo::loadMetadata(getMediaDirectory(), mediaId);
                if (!metadata.filename.isEmpty()) {
                    QString filePath = getMediaDirectory() + "/" + metadata.filename;
                    if (QFile::exists(filePath)) {
                        if (QFile::remove(filePath)) {
                            qDebug() << "Deleted old media file:" << metadata.filename;
                        } else {
                            qWarning() << "Failed to delete old media file:" << metadata.filename;
                        }
                    }
                }
                // Delete metadata file
                MediaInfo::deleteMetadata(getMediaDirectory(), mediaId);
            }
        }
        
        // If nothing to download and nothing changed, we're done
        if (mediaToDownload.isEmpty() && mediaToDelete.isEmpty()) {
            qDebug() << "Media lists are in sync, no changes needed";
            completeSyncWithSuccess();
            return;
        }
        
        // If we have media to download, start downloading
        if (!mediaToDownload.isEmpty()) {
            m_pendingDownloads.clear();
            m_downloadedFilenames.clear();
            m_currentServerMediaIds = serverMediaIds;  // Store for final save
            m_activeDownloads = 0;
            
            for (const QString &mediaId : mediaToDownload) {
                downloadMedia(mediaId);
            }
        } else {
            // No downloads, but we had deletions
            // Update local media.json to match server
            MediaList updatedList;
            updatedList.deviceId = m_remoteApi->deviceId();
            updatedList.mediaIds = serverMediaIds;
            updatedList.saveToFile(getMediaDirectory());
            completeSyncWithSuccess();
        }
    });
}

void MediaManager::downloadMedia(const QString &mediaId)
{
    m_activeDownloads++;
    
    if (!m_remoteApi || !m_remoteApi->enabled()) {
        m_activeDownloads--;
        m_pendingDownloads.removeOne(mediaId);
        qWarning() << "RemoteApi not enabled";
        return;
    }

    // Create MediaInfo template with just the ID
    MediaInfo infoTemplate;
    infoTemplate.id = mediaId;
    infoTemplate.deviceId = m_remoteApi->deviceId();

    m_remoteApi->fetchObject(infoTemplate, [this, mediaId](bool success, const MediaInfo &fetchedInfo, const QString &error) {
        m_activeDownloads--;
        m_pendingDownloads.removeOne(mediaId);
        
        if (!success) {
            qWarning() << "Failed to fetch media" << mediaId << ":" << error;
            if (m_activeDownloads == 0) {
                // Cleanup with filenames we did manage to fetch
                // Save updated media.json even on partial failure
                MediaList updatedList;
                updatedList.deviceId = m_remoteApi->deviceId();
                updatedList.mediaIds = m_currentServerMediaIds;
                updatedList.saveToFile(getMediaDirectory());
                cleanupOldMedia(m_downloadedFilenames);
                completeSyncWithError("Failed to download some media files");
            }
            return;
        }
        
        // Track this filename for cleanup
        m_downloadedFilenames.append(fetchedInfo.filename);
        
        // Check if file already exists
        QString mediaDir = getMediaDirectory();
        QString filePath = QDir(mediaDir).absoluteFilePath(fetchedInfo.filename);
        
        if (QFile::exists(filePath)) {
            qDebug() << "Media file already exists:" << fetchedInfo.filename;
            
            // Save/update metadata even if file exists
            fetchedInfo.saveMetadata(getMediaDirectory());
            
            // File exists, skip download
            if (m_activeDownloads == 0) {
                if (m_pendingDownloads.isEmpty()) {
                    // Save updated media.json with server's media list
                    MediaList updatedList;
                    updatedList.deviceId = m_remoteApi->deviceId();
                    updatedList.mediaIds = m_currentServerMediaIds;
                    updatedList.saveToFile(getMediaDirectory());
                    cleanupOldMedia(m_downloadedFilenames);
                    completeSyncWithSuccess();
                } else {
                    // Save updated media.json even on partial failure
                    MediaList updatedList;
                    updatedList.deviceId = m_remoteApi->deviceId();
                    updatedList.mediaIds = m_currentServerMediaIds;
                    updatedList.saveToFile(getMediaDirectory());
                    cleanupOldMedia(m_downloadedFilenames);
                    completeSyncWithError("Some downloads failed");
                }
            }
            return;
        }
        
        // Save the binary data to file
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(fetchedInfo.binaryData);
            file.close();
            qDebug() << "Downloaded media:" << fetchedInfo.filename << "(" << fetchedInfo.binaryData.size() << "bytes)";
            
            // Save metadata for this media
            fetchedInfo.saveMetadata(getMediaDirectory());
        } else {
            qWarning() << "Failed to save media file:" << filePath;
        }
        
        // Check if all downloads are complete
        if (m_activeDownloads == 0) {
            if (m_pendingDownloads.isEmpty()) {
                // Save updated media.json with server's media list
                MediaList updatedList;
                updatedList.deviceId = m_remoteApi->deviceId();
                updatedList.mediaIds = m_currentServerMediaIds;
                updatedList.saveToFile(getMediaDirectory());
                cleanupOldMedia(m_downloadedFilenames);
                completeSyncWithSuccess();
            } else {
                // Save updated media.json even on partial failure
                MediaList updatedList;
                updatedList.deviceId = m_remoteApi->deviceId();
                updatedList.mediaIds = m_currentServerMediaIds;
                updatedList.saveToFile(getMediaDirectory());
                cleanupOldMedia(m_downloadedFilenames);
                completeSyncWithError("Some downloads failed");
            }
        }
    });
}

void MediaManager::cleanupOldMedia(const QStringList &serverMediaList)
{
    QString mediaDir = getMediaDirectory();
    QDir dir(mediaDir);
    
    if (!dir.exists()) {
        return;
    }
    
    QStringList nameFilters;
    nameFilters << "*.gif" << "*.GIF" << "*.png" << "*.PNG" 
                << "*.jpg" << "*.JPG" << "*.jpeg" << "*.JPEG";
    
    QFileInfoList localFiles = dir.entryInfoList(nameFilters, QDir::Files);
    
    for (const QFileInfo &fileInfo : localFiles) {
        QString fileName = fileInfo.fileName();
        if (!serverMediaList.contains(fileName)) {
            qDebug() << "Removing old media file:" << fileName;
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }
}

void MediaManager::completeSyncWithSuccess()
{
    m_syncing = false;
    m_lastSyncTime = QDateTime::currentDateTime();
    m_lastError.clear();
    
    emit syncingChanged();
    emit lastSyncTimeChanged();
    emit lastErrorChanged();
    
    qDebug() << "Media sync completed successfully";
    
    // Trigger rescan to update available media list
    m_scanTimer.start();
}

void MediaManager::completeSyncWithError(const QString &error)
{
    m_syncing = false;
    m_lastError = error;
    
    emit syncingChanged();
    emit lastErrorChanged();
    
    qWarning() << "Media sync failed:" << error;
}
