#ifndef VOICE_H
#define VOICE_H

#include    <QMap>
#include    <QtXml/QDomDocument>
#include    "IGuidoAbstractObject.h"
#include    "Note.h"
#include    "Rest.h"
#include    "Tag.h"

class Voice : public IGuidoAbstractObject
{
public:
    Voice();
    Voice(Voice const* voice);
    virtual ~Voice();

    // IGuidoAbstractObject implementation
    void    abstract(ARFactoryHandler& arFactory);

    bool        readXML(QDomElement const& element);
    QDomElement writeXML(QDomDocument& document) const;

    QVector<AMusicElement*>*    getMusicElements() const;
    void                        setMusicElements(QVector<AMusicElement*>* musicElements);
    void                        addElement(AMusicElement* element);
    void                        insertElement(int pos, AMusicElement* element);
    void                        removeElement(int pos);

private:
    QVector<AMusicElement*>*    _musicElements;
};

#endif // VOICE_H
