#include "MusicElementFactory.h"

MusicElementFactory::MusicElementFactory()
{

}

MusicElementFactory::~MusicElementFactory()
{

}

AMusicElement*  MusicElementFactory::newFromDomElement(QDomElement const& domElement) const
{
    QString type = domElement.attribute("type");
    AMusicElement* musicElement;

    if (type == "note")
    {
        musicElement = new Note;
    }
    else if (type == "rest")
    {
        musicElement = new Rest;

    }
    else if (type == "tag")
    {
        musicElement = new Tag;

    }
    else if (type == "chord")
    {
        musicElement = new Chord;
    }
    musicElement->readXML(domElement);
    return musicElement;
}
