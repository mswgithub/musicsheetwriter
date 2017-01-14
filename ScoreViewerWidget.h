#ifndef SCOREVIEWWIDGET_H
#define SCOREVIEWWIDGET_H

#include    "QGuidoWidget.h"
#include    "GuidoFactory.h"
#include    "Score.h"
#include    <QObject>
#include    <QScrollArea>
#include    <QScrollBar>
#include    <QVBoxLayout>
#include    <QGraphicsScene>
#include    <GraphicItemsView.h>
#include    <QGraphicsView>

class ScoreViewerWidget : public QObject
{
    Q_OBJECT
public:
    ScoreViewerWidget(QWidget *scoreView, QObject* parent = 0);
    virtual ~ScoreViewerWidget();

    void    show(ARHandler score);
    void    setCurrentPage(int currentPage);
    int     getPages() const;

private:
    QScrollArea*        _scrollArea;
    QWidget*            _scoreWidget;
    QVBoxLayout*        _scoreLayout;
    QGraphicsScene*     _scene;
    GraphicItemsView*   _graphic;
    QGraphicsView*      _graphicview;
    int                 _currentPage;

signals:
    void    signalMaxPages(int maxPages);
};

#endif // SCOREVIEWWIDGET_H
