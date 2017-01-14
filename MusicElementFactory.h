#ifndef MUSICELEMENTFACTORY_H
#define MUSICELEMENTFACTORY_H

#include    <QDomElement>
#include    "AMusicElement.h"
#include    "Note.h"
#include    "Rest.h"
#include    "Tag.h"
#include    "Chord.h"

class MusicElementFactory
{
public:
    MusicElementFactory();
    virtual ~MusicElementFactory();

    AMusicElement*    newFromDomElement(QDomElement const& domElement) const;
    AMusicElement*    newFromInstance(AMusicElement* domElement) const;
};

#endif // MUSICELEMENTFACTORY_H
