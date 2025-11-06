#include "Service.h"
#include <QDebug>
#include <cassert>
using namespace Services::DateTime;

Service::Service(QObject* parent)
    : QObject(parent),
      m_timeZone(QTimeZone("Europe/Amsterdam"))
{
    assert(m_timeZone.isValid()); // Time zones not available on system (install tzdata).
}

QString Service::localTime() const
{
    QDateTime now = QDateTime::currentDateTimeUtc().toTimeZone(m_timeZone);
    return now.toString("HH:mm:ss");
}

QString Service::utcTime() const
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    return now.toString("HH:mm:ss");
}
