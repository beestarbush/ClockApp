#include "FunkyClass.h"
#include <QDebug>

FunkyClass::FunkyClass(QObject *parent) :
    QObject(parent),
    m_rotationDegrees(0)
{
}

QString FunkyClass::getGreeting() const
{
    qDebug() << "getGreeting() called";
    return "Hello, World!";
}

quint32 FunkyClass::rotationDegrees() const
{
    return m_rotationDegrees;
}

void FunkyClass::setRotationDegrees(quint32 degrees)
{
    if (m_rotationDegrees != degrees)
    {
        m_rotationDegrees = degrees;
        emit rotationDegreesChanged(m_rotationDegrees);
    }
}