#include "AnimationManager.h"
#include <QDebug>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCoreApplication>

AnimationManager::AnimationManager(QObject *parent)
    : QObject(parent)
    , m_fileWatcher(new QFileSystemWatcher(this))
    , m_scanTimer(new QTimer(this))
{
    // Setup delayed scanning to avoid excessive file system operations
    m_scanTimer->setSingleShot(true);
    m_scanTimer->setInterval(500); // 500ms delay
    connect(m_scanTimer, &QTimer::timeout, this, &AnimationManager::scanDirectory);

    // Connect file watcher
    connect(m_fileWatcher, &QFileSystemWatcher::directoryChanged,
            this, &AnimationManager::onDirectoryChanged);

    // Setup file watcher and initial scan
    setupFileWatcher();
    scanAnimationDirectory();
}

QStringList AnimationManager::availableAnimations() const
{
    return m_availableAnimations;
}

QString AnimationManager::selectedAnimation() const
{
    return m_selectedAnimation;
}

void AnimationManager::setSelectedAnimation(const QString &animation)
{
    if (m_selectedAnimation == animation) {
        return;
    }

    m_selectedAnimation = animation;
    emit selectedAnimationChanged();
}

void AnimationManager::scanAnimationDirectory()
{
    scanDirectory();
}

QString AnimationManager::getAnimationPath(const QString &animationName) const
{
    if (animationName.isEmpty()) {
        return QString();
    }

    QString animationDir = getAnimationDirectory();
    QString fullPath = QDir(animationDir).absoluteFilePath(animationName);
    
    // Verify the file exists and is valid
    if (QFileInfo::exists(fullPath) && isValidAnimationFile(fullPath)) {
        return fullPath;
    }

    return QString();
}

bool AnimationManager::isValidAnimationFile(const QString &filePath) const
{
    return isGifFile(filePath);
}

void AnimationManager::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)
    // Use delayed scanning to avoid excessive updates when multiple files change
    m_scanTimer->start();
}

void AnimationManager::delayedScan()
{
    scanDirectory();
}

void AnimationManager::setupFileWatcher()
{
    QString animationDir = getAnimationDirectory();
    
    QDir dir(animationDir);
    if (dir.exists()) {
        if (!m_fileWatcher->directories().contains(animationDir)) {
            m_fileWatcher->addPath(animationDir);
        }
    } else {
        qWarning() << "Animation directory does not exist:" << animationDir;
    }
}

void AnimationManager::scanDirectory()
{
    QStringList newAnimations;
    QString animationDir = getAnimationDirectory();

    QDir dir(animationDir);
    if (dir.exists()) {
        // Get all GIF files in the directory
        QStringList nameFilters;
        nameFilters << "*.gif" << "*.GIF";
        
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

        // If current selection is no longer available, clear it
        if (!m_selectedAnimation.isEmpty() && !m_availableAnimations.contains(m_selectedAnimation)) {
            setSelectedAnimation(QString());
        }
    }
}

QString AnimationManager::getAnimationDirectory() const
{
    // Fixed animation directory path relative to application
    return QCoreApplication::applicationDirPath() + "/animations";
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
    if (fileInfo.size() < 100) { // Minimum reasonable GIF size
        return false;
    }

    // Could add more sophisticated validation here (e.g., reading GIF header)
    // For now, extension and size check should be sufficient

    return true;
}