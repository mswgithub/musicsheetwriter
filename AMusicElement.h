#ifndef AMUSICELEMENT_H
#define AMUSICELEMENT_H

#include    <QDomElement>
#include    <QMap>
#include    "IGuidoAbstractObject.h"

class AMusicElement : public IGuidoAbstractObject
{    
public:

    enum ElementColor
    {
        BLACK = 0,
        RED,
        GREEN,
        BLUE
    };

    AMusicElement();
    AMusicElement(AMusicElement const& musicElement);
    virtual ~AMusicElement();

    void initColorMap();

    virtual void            readXML(QDomElement const& domElement) = 0;
    virtual QDomElement     writeXML(QDomDocument& document) const = 0;
    virtual AMusicElement*  duplicate() const = 0;
    virtual void            guidoNotationColor(ARFactoryHandler &arFactory) = 0;
    virtual QString         gmnAMusicElement() const = 0;

    bool            isSelected() const;
    QString const&  getColorString() const;
    ElementColor    getColor() const;
    void            select(ElementColor color = RED);
    void            deselect();

private:
    bool            _isSelected;
    ElementColor    _color;
    int             _page;

    static QMap<ElementColor, QString>  elementColorMap;
};

#endif // AMUSICELEMENT_H
