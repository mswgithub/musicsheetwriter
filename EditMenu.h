#ifndef EDITMENU_H
#define EDITMENU_H

#include    <QObject>
#include    <QMenu>
#include    <QAction>
#include    "AMenu.h"

class EditMenu : public AMenu
{
    Q_OBJECT

public:
    EditMenu(QMenu* menufile);
    virtual ~EditMenu();

private:
    void    createMenu();

private slots:

};

#endif // EDITMENU_H
