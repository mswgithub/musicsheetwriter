#ifndef NOTE_H
#define NOTE_H

#include    <QMap>
#include    "AEvent.h"

class Note : public AEvent
{
public:

    enum NoteName
    {
        A = 0,  // la
        B,  // si
        C,  // do
        D,  // re
        E,  // mi
        F,  // fa
        G   // sol
    };

    Note();
    Note(NoteName noteName);
    Note(Note const& note);
    Note(NoteName noteName, int accidental, int octave);
    virtual ~Note();

    bool    operator==(Note const& note) const;

    // IGuidoAbstractObject implementation
    void    abstract(ARFactoryHandler& arFactory);

    // AMusicElement implementation
    void            readXML(QDomElement const& domElement);
    QDomElement     writeXML(QDomDocument& document) const;
    void            guidoNotationColor(ARFactoryHandler& arFactory);
    AMusicElement*  duplicate() const;
    QString         gmnAMusicElement() const;



    // Accessors
    NoteName    getNoteName() const;
    void        setNoteName(NoteName noteName);
    int         getAccidental() const;
    void        setAccidental(int accidental);
    int         getOctave() const;
    void        setOctave(int octave);

private:
    // This the name of the note in the english notation using the enum
    NoteName    _noteName;
    // It is the modifier of the note # or b (bemol).
    // 1 for #, 2 for ##, etc. or -1 for b, -2 for bb, etc.
    int         _accidental;
    // Represent the octave in the english notation (ref. A4 = La3 = 440 Hz)
    int         _octave;

    static QMap<NoteName, QString>      noteNameMap;

};

#endif // NOTE_H
