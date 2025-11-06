#include "Item.h"
using namespace Applications::Menu;

Item::Item(QString label, QObject* parent)
    : QObject(parent),
      m_label(std::move(label))
{
}
