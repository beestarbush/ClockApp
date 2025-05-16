#include "FunkyClass.h"
#include <QDebug>

FunkyClass::FunkyClass(QObject *parent) : QObject(parent)
{
}

QString FunkyClass::getGreeting() const
{
    qDebug() << "getGreeting() called";
    return "Hello, World!";
}