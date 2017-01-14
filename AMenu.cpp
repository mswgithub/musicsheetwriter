#include "AMenu.h"

AMenu::AMenu(QMenu* menu)
{
    _menu = menu;
}

AMenu::~AMenu()
{

}

QMenu const*    AMenu::getMenu() const
{
    return _menu;
}

QMenu*  AMenu::getMenu()
{
    return _menu;
}
