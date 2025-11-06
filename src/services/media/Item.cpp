#include "Item.h"

using namespace Services::Media;

Item::Item(QObject* parent)
    : QObject(parent),
      m_isValid(false)
{
}

Item::Item(const QString& filename, const QString& path, QObject* parent)
    : QObject(parent),
      m_filename(filename),
      m_path(path),
      m_isValid(true)
{
}
