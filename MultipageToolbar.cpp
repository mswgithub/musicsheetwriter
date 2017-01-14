#include "MultipageToolbar.h"

#include    <QDebug>

MultipageToolBar::MultipageToolBar(QToolButton *btnLeft, QToolButton *btnRight)
{
    _btnLeft = btnLeft;
    _btnRight = btnRight;
    _currentPage = 1;
    _maxPages = 4;
    _btnLeft->setArrowType(Qt::LeftArrow);
    _btnRight->setArrowType(Qt::RightArrow);
    connect(_btnLeft, SIGNAL(clicked(bool)), SLOT(slotBtnLeftClicked()));
    connect(_btnRight, SIGNAL(clicked(bool)), SLOT(slotBtnRightClicked()));
}

void    MultipageToolBar::slotBtnLeftClicked()
{
    if (_currentPage > 1)
    {
        _currentPage--;
    }
    checkMultipageToolButton();
    emit signalSetCurrentPage(_currentPage);
}

void    MultipageToolBar::slotBtnRightClicked()
{
    if (_currentPage < _maxPages)
    {
        _currentPage++;
    }
    checkMultipageToolButton();
    emit signalSetCurrentPage(_currentPage);
}

void    MultipageToolBar::setMaxPages(int maxpage)
{
    _maxPages = maxpage;
    if (_currentPage > _maxPages)
    {
        _currentPage = _maxPages;
        emit signalSetCurrentPage(_currentPage);
    }
    checkMultipageToolButton();
}

int     MultipageToolBar::getMaxPages() const
{
    return _maxPages;
}

void    MultipageToolBar::checkMultipageToolButton()
{
    if (_maxPages == 1)
    {
        _btnLeft->hide();
        _btnRight->hide();
    }
    else if (_maxPages == 2)
    {
        if (_currentPage == 1)
        {
            _btnLeft->hide();
            _btnRight->show();
        }
        else if (_currentPage == 2)
        {
            _btnRight->hide();
            _btnLeft->show();
        }
    }
    else if (_maxPages > 2)
    {
        if (_currentPage == 1)
        {
            _btnLeft->hide();
            _btnRight->show();
        }
        else if (_currentPage == _maxPages)
        {
            _btnRight->hide();
            _btnLeft->show();
        }
        else
        {
            _btnLeft->show();
            _btnRight->show();
        }
    }
}

int MultipageToolBar::getCurrentPage() const
{
    return _currentPage;
}
