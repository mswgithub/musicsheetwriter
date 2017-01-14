#ifndef GRAPHICITEMSVIEW_H
#define GRAPHICITEMSVIEW_H

#include    "QGuidoGraphicsItem.h"

class GraphicItemsView : public QGuidoGraphicsItem
{
public:
    GraphicItemsView();

    void    setARHandler(ARHandler score);
};

#endif // GRAPHICITEMSVIEW_H
