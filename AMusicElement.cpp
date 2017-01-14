#include "AMusicElement.h"

QMap<AMusicElement::ElementColor, QString> AMusicElement::elementColorMap = {
    {AMusicElement::ElementColor::BLACK, "black"},
    {AMusicElement::ElementColor::RED, "red"},
    {AMusicElement::ElementColor::GREEN, "green"},
    {AMusicElement::ElementColor::BLUE, "blue"}
};

AMusicElement::AMusicElement()
{
    _isSelected = false;
    _color = BLACK;
}

AMusicElement::AMusicElement(const AMusicElement& musicElement)
{
    _isSelected = musicElement._isSelected;
    _color = musicElement._color;
}

AMusicElement::~AMusicElement()
{

}

void    AMusicElement::initColorMap()
{
    /*this->elementColorMap.insert(AMusicElement::ElementColor::BLACK, "black");
    this->elementColorMap.insert(AMusicElement::ElementColor::RED, "red");
    this->elementColorMap.insert(AMusicElement::ElementColor::GREEN, "green");
    this->elementColorMap.insert(AMusicElement::ElementColor::BLUE, "blue");*/
}

bool    AMusicElement::isSelected() const
{
    return _isSelected;
}

QString const&  AMusicElement::getColorString() const
{
    return elementColorMap[_color];
}


AMusicElement::ElementColor AMusicElement::getColor() const
{
    return _color;
}

void    AMusicElement::select(ElementColor color)
{
    _isSelected = true;
    _color = color;
}

void    AMusicElement::deselect()
{
    _isSelected = false;
    _color = BLACK;
}
