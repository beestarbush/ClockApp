#ifndef HAL_STORAGE_H
#define HAL_STORAGE_H

#include <QObject>

class Storage : public QObject
{
    Q_OBJECT

  public:
    Storage(QObject* parent = nullptr);
};

#endif // HAL_STORAGE_H