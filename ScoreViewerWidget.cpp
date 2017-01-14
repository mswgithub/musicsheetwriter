#include "ScoreViewerWidget.h"

ScoreViewerWidget::ScoreViewerWidget(QWidget* scoreView, QObject* parent) : QObject(parent)
{
    _currentPage = 1;
    _scoreWidget = scoreView;
    _scoreLayout = new QVBoxLayout(_scoreWidget);
    _graphic = new GraphicItemsView;
    _scene = new QGraphicsScene;
    _graphic->setScale(3.1);
    _graphicview = new QGraphicsView(_scene);
    _graphicview->setFixedWidth(650);
    _graphicview->setRenderHint(QPainter::Antialiasing, true);
}

ScoreViewerWidget::~ScoreViewerWidget()
{
    delete _scrollArea;
    delete _scoreLayout;
    delete _scoreWidget;
    delete _scene;
    delete _graphic;
}

#include <QDebug>

void    ScoreViewerWidget::show(ARHandler score)
{
    _graphic->setARHandler(score);
    if (_scene->items().count() >= 1)
    {
        _scene->removeItem(_graphic);
    }
    _scene->addItem(_graphic);
    _graphic->setPos(0,0);
    GuidoLayoutSettings settings = _graphic->guidoLayoutSettings();
    settings.optimalPageFill = 0;
    _graphic->setGuidoLayoutSettings(settings);
    _graphic->setPage(_currentPage);
    _graphicview->setSceneRect(0, 0, 0, 875);
    _graphicview->setStyleSheet("background-color:white;");
    _scoreLayout->addWidget(_graphicview);
    _scoreLayout->setAlignment(Qt::AlignCenter);
    _scoreWidget->setLayout(_scoreLayout);
    _graphicview->setAlignment(Qt::AlignLeft);
    _graphicview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    emit signalMaxPages(_graphic->pageCount());
}

int ScoreViewerWidget::getPages() const
{
    return _graphic->pageCount();
}

void ScoreViewerWidget::setCurrentPage(int currentPage)
{
    _currentPage = currentPage;
}
