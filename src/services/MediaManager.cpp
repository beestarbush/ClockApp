#include "MediaManager.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#ifdef PLATFORM_IS_TARGET
const QString MEDIA_PATH = QStringLiteral("/usr/share/bee/media");
#else
const QString MEDIA_PATH = QStringLiteral("/workdir/media");
#endif
const QString DEFAULT_MEDIA = QStringLiteral("qrc:/media/test.gif");
constexpr int MIN_MEDIA_FILE_SIZE = 100; // Minimum reasonable file size in bytes
constexpr int SCAN_DELAY_MS = 500; // Delay in milliseconds for scanning directory

MediaManager::MediaManager(QObject *parent)
    : QObject(parent)
    , m_fileWatcher(this)
    , m_scanTimer(this)
{
    // Setup delayed scanning to avoid excessive file system operations
    m_scanTimer.setSingleShot(true);
    m_scanTimer.setInterval(SCAN_DELAY_MS);
    connect(&m_scanTimer, &QTimer::timeout, this, &MediaManager::scanDirectory);

    // Connect file watcher
    connect(&m_fileWatcher, &QFileSystemWatcher::directoryChanged, this, &MediaManager::onDirectoryChanged);

    // Setup file watcher and initial scan
    setupFileWatcher();
    scanDirectory();
}

QStringList MediaManager::availableMedia() const
{
    return m_availableMedia;
}

QString MediaManager::getMediaPath(const QString &name) const
{
    if (name.isEmpty()) {
        return QString();
    }

    QString mediaDir = MEDIA_PATH;
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

void MediaManager::setupFileWatcher()
{
    QString mediaDir = MEDIA_PATH;

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
    QString mediaDir = MEDIA_PATH;

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
        return false;
    }

    // Basic file size check (avoid empty files)
    if (fileInfo.size() < MIN_MEDIA_FILE_SIZE) { // Minimum reasonable file size
        return false;
    }

    // Could add more sophisticated validation here (e.g., reading file header)
    // For now, extension and size check should be sufficient

    return true;
}
