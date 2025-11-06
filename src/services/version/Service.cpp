#include "Service.h"
#include "git_version.h"
using namespace Services::Version;

Service::Service(QObject* parent)
    : QObject(parent)
{
}

QString Service::tag() const
{
    return QStringLiteral(GIT_TAG);
}

QString Service::commitHash() const
{
    return QStringLiteral(GIT_COMMIT_HASH);
}

QString Service::shortCommitHash() const
{
    return QStringLiteral(GIT_COMMIT_HASH_SHORT);
}

QString Service::dirty() const
{
    return QStringLiteral(GIT_DIRTY);
}
