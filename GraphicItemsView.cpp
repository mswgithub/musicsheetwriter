#include "GraphicItemsView.h"

GraphicItemsView::GraphicItemsView()
{

}

void    GraphicItemsView::setARHandler(ARHandler score)
{
    mGuidoPainter->setARHandler(score);
    updateGuidoPagesSizes();
    guidoItemUpdate();
}
