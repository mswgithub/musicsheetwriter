#ifndef REST_H
#define REST_H

#include    "AEvent.h"

class Rest : public AEvent
{
public:

    Rest();
    Rest(Rest const& rest);
    virtual ~Rest();

    bool    operator==(Rest const&) const;

    // IGuidoAbstractObject implementation
    void    abstract(ARFactoryHandler& arFactory);

    // AMusicElement implementation
    void            readXML(QDomElement const& domElement);
    QDomElement     writeXML(QDomDocument& document) const;
    void            guidoNotationColor(ARFactoryHandler& arFactory);
    AMusicElement*  duplicate() const;
    QString         gmnAMusicElement() const;
};

#endif // REST_H
