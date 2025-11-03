#include "MenuItem.h"

MenuItem::MenuItem(QString label, QObject* parent)
    : QObject(parent),
      m_label(std::move(label))
{
}
