#include "Item.h"
using namespace Menu;

Item::Item(QString label, QObject* parent)
    : QObject(parent),
      m_label(std::move(label))
{
}
