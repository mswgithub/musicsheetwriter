#ifndef CHORD_H
#define CHORD_H

#include "AMusicElement.h"
#include "Note.h"

class Chord : public AMusicElement
{
public:
    Chord();
    Chord(Chord const* chord);
    virtual ~Chord();
    //from the interface
    void    abstract(ARFactoryHandler& arFactory);

    void            readXML(QDomElement const& domElement);
    QDomElement     writeXML(QDomDocument& document) const;
    void            guidoNotationColor(ARFactoryHandler& arFactory);
    AMusicElement*  duplicate() const;
    QString         gmnAMusicElement() const;


    QVector<AMusicElement*>*  getNotes();
    void            addNote(AMusicElement* note);
    void            setNote(QVector<AMusicElement*>* elem);

private:
    QVector<AMusicElement*>* _notesVector;
};

#endif // CHORD_H
