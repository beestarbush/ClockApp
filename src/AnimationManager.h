#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QDir>
#include <QTimer>

class AnimationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableAnimations READ availableAnimations NOTIFY availableAnimationsChanged)
    Q_PROPERTY(QString selectedAnimation READ selectedAnimation WRITE setSelectedAnimation NOTIFY selectedAnimationChanged)

public:
    explicit AnimationManager(QObject *parent = nullptr);

    QStringList availableAnimations() const;

    QString selectedAnimation() const;
    void setSelectedAnimation(const QString &animation);

    Q_INVOKABLE void scanAnimationDirectory();
    Q_INVOKABLE QString getAnimationPath(const QString &animationName) const;
    Q_INVOKABLE bool isValidAnimationFile(const QString &filePath) const;

signals:
    void availableAnimationsChanged();
    void selectedAnimationChanged();

private slots:
    void onDirectoryChanged(const QString &path);
    void delayedScan();

private:
    void setupFileWatcher();
    void scanDirectory();
    bool isGifFile(const QString &filePath) const;
    QString getAnimationDirectory() const;

    QStringList m_availableAnimations;
    QString m_selectedAnimation;
    QFileSystemWatcher *m_fileWatcher;
    QTimer *m_scanTimer;
};

#endif // ANIMATIONMANAGER_H