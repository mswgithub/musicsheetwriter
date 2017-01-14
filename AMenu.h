#ifndef AMENU_H
#define AMENU_H

#include <QObject>
#include <QMenu>

class AMenu : public QObject
{
    Q_OBJECT
public:
    explicit AMenu(QMenu* menu);
    virtual ~AMenu();

    QMenu const*   getMenu() const;

protected:
    QMenu*          getMenu();
    virtual void    createMenu() = 0;


private:
    QMenu*  _menu;
};

#endif // AMENU_H
