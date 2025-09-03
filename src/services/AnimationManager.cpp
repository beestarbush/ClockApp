#include "AnimationManager.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#ifdef PLATFORM_IS_TARGET
const QString ANIMATION_PATH = QStringLiteral("/usr/share/bee/animations");
#else
const QString ANIMATION_PATH = QStringLiteral("/workdir/animations");
#endif
const QString DEFAULT_ANIMATION = QStringLiteral("qrc:/animations/test.gif");
constexpr int MIN_GIF_FILE_SIZE = 100; // Minimum reasonable GIF file size in bytes
constexpr int SCAN_DELAY_MS = 500; // Delay in milliseconds for scanning directory

AnimationManager::AnimationManager(QObject *parent)
    : QObject(parent)
    , m_fileWatcher(this)
    , m_scanTimer(this)
{
    // Setup delayed scanning to avoid excessive file system operations
    m_scanTimer.setSingleShot(true);
    m_scanTimer.setInterval(SCAN_DELAY_MS);
    connect(&m_scanTimer, &QTimer::timeout, this, &AnimationManager::scanDirectory);

    // Connect file watcher
    connect(&m_fileWatcher, &QFileSystemWatcher::directoryChanged, this, &AnimationManager::onDirectoryChanged);

    // Setup file watcher and initial scan
    setupFileWatcher();
    scanDirectory();
}

QStringList AnimationManager::availableAnimations() const
{
    return m_availableAnimations;
}

QString AnimationManager::getAnimationPath(const QString &animationName) const
{
    if (animationName.isEmpty()) {
        return QString();
    }

    QString animationDir = ANIMATION_PATH;;
    QString fullPath = QDir(animationDir).absoluteFilePath(animationName);
    
    // Verify the file exists and is valid
    if (QFileInfo::exists(fullPath) && isGifFile(fullPath)) {
        return fullPath;
    }

    qWarning() << "Requested animation not found or invalid:" << fullPath << ", falling back to default.";
    return DEFAULT_ANIMATION;
}

void AnimationManager::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)
    // Use delayed scanning to avoid excessive updates when multiple files change
    m_scanTimer.start();
}

void AnimationManager::setupFileWatcher()
{
    QString animationDir = ANIMATION_PATH;
    
    QDir dir(animationDir);
    if (dir.exists()) {
        if (!m_fileWatcher.directories().contains(animationDir)) {
            m_fileWatcher.addPath(animationDir);
        }
    } else {
        qWarning() << "Animation directory does not exist:" << animationDir;
    }
}

void AnimationManager::scanDirectory()
{
    QStringList newAnimations;
    QString animationDir = ANIMATION_PATH;

    QDir dir(animationDir);
    if (dir.exists()) {
        // Get all GIF files in the directory
        QStringList nameFilters;
        nameFilters << QStringLiteral("*.gif") << QStringLiteral("*.GIF");
        
        QFileInfoList fileList = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable);
        
        for (const QFileInfo &fileInfo : fileList) {
            if (isGifFile(fileInfo.absoluteFilePath())) {
                newAnimations.append(fileInfo.fileName());
            }
        }
        
        // Sort alphabetically for consistent ordering
        newAnimations.sort();
        
        qDebug() << "Found" << newAnimations.size() << "animation files in" << animationDir;
    } else {
        qWarning() << "Animation directory does not exist:" << animationDir;
        
        // Try to create the directory
        if (dir.mkpath(animationDir)) {
            qDebug() << "Created animation directory:" << animationDir;
            setupFileWatcher();
        } else {
            qWarning() << "Failed to create animation directory:" << animationDir;
        }
    }

    // Update available animations if changed
    if (newAnimations != m_availableAnimations) {
        m_availableAnimations = newAnimations;
        emit availableAnimationsChanged();
    }
}

bool AnimationManager::isGifFile(const QString &filePath) const
{
    QFileInfo fileInfo(filePath);
    
    // Check file extension
    QString suffix = fileInfo.suffix().toLower();
    if (suffix != "gif") {
        return false;
    }

    // Basic file size check (avoid empty files)
    if (fileInfo.size() < MIN_GIF_FILE_SIZE) { // Minimum reasonable GIF size
        return false;
    }

    // Could add more sophisticated validation here (e.g., reading GIF header)
    // For now, extension and size check should be sufficient

    return true;
}