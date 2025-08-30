#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QTimer>

class AnimationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableAnimations READ availableAnimations NOTIFY availableAnimationsChanged)

public:
    explicit AnimationManager(QObject *parent = nullptr);

    QStringList availableAnimations() const;

    Q_INVOKABLE QString getAnimationPath(const QString &animationName) const;

signals:
    void availableAnimationsChanged();

private slots:
    void onDirectoryChanged(const QString &path);

private:
    void setupFileWatcher();
    void scanDirectory();
    bool isGifFile(const QString &filePath) const;

    QStringList m_availableAnimations;
    QFileSystemWatcher m_fileWatcher;
    QTimer m_scanTimer;
};

#endif // ANIMATIONMANAGER_H