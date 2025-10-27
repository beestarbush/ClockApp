#include "DateTime.h"
#include <QDebug>
#include <cassert>

DateTime::DateTime(QObject* parent)
    : QObject(parent),
      m_timeZone(QTimeZone("Europe/Amsterdam"))
{
    assert(m_timeZone.isValid()); // Time zones not available on system (install tzdata).
}

QString DateTime::localTime() const
{
    QDateTime now = QDateTime::currentDateTimeUtc().toTimeZone(m_timeZone);
    return now.toString("HH:mm:ss");
}

QString DateTime::utcTime() const
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    return now.toString("HH:mm:ss");
}