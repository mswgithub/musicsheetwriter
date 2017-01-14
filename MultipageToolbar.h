#ifndef MULTIPAGETOOLBAR_H
#define MULTIPAGETOOLBAR_H

#include    <QObject>
#include    <QToolButton>

class MultipageToolBar : public QObject
{
    Q_OBJECT
public:
    MultipageToolBar(QToolButton* btnLeft, QToolButton* btnRight);
    void    setMaxPages(int maxpage);
    int     getMaxPages() const;
    int     getCurrentPage() const;

private:
    QToolButton*    _btnLeft;
    QToolButton*    _btnRight;
    int             _currentPage;
    int             _maxPages;

    void    checkMultipageToolButton();

public slots:
    void    slotBtnLeftClicked();
    void    slotBtnRightClicked();

signals:
    void    signalSetCurrentPage(int currentPage);
};

#endif // MULTIPAGETOOLBAR_H
