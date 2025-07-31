#include "Version.h"
#include "git_version.h"

Version::Version(QObject *parent) :
    QObject(parent)
{
}

QString Version::tag() const
{
    return QStringLiteral(GIT_TAG);
}

QString Version::commitHash() const
{
    return QStringLiteral(GIT_COMMIT_HASH);
}

QString Version::shortCommitHash() const
{
    return QStringLiteral(GIT_COMMIT_HASH_SHORT);
}

QString Version::dirty() const
{
    return QStringLiteral(GIT_DIRTY);
}