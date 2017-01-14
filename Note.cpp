#include "Note.h"

QMap<Note::NoteName, QString> Note::noteNameMap = {
    {Note::NoteName::A, "a"},
    {Note::NoteName::B, "b"},
    {Note::NoteName::C, "c"},
    {Note::NoteName::D, "d"},
    {Note::NoteName::E, "e"},
    {Note::NoteName::F, "f"},
    {Note::NoteName::G, "g"}
};

Note::Note()
{
    _noteName = NoteName::A; // La note par defaut est 'A'
    _accidental = 0;
    _octave = 1;
}

Note::Note(NoteName noteName)
{
    _noteName = noteName;
    _accidental = 0;
    _octave = 1;
}

Note::Note(Note const& note) : AEvent(note)
{
    _noteName = note._noteName;
    _accidental = note._accidental;
    _octave = note._octave;
}

Note::Note(NoteName noteName, int accidental, int octave)
{
    _noteName = noteName;
    _accidental = accidental;
    _octave = octave;
}

Note::~Note()
{

}

bool    Note::operator==(Note const& note) const
{
    return this->_noteName == note._noteName &&
            this->_accidental == note._accidental &&
            this->_octave == note._octave &&
            this->getNumDuration() == note.getNumDuration() &&
            this->getDenomDuration() == note.getDenomDuration() &&
            this->getDots() == note.getDots();
}

/*
 * Abstrait l'objet note en utilisant le GUIDOFactoryAdapter
 */
void    Note::abstract(ARFactoryHandler& arFactory)
{
    guidoNotationColor(arFactory);
    GuidoFactoryOpenEvent(arFactory, noteNameMap[_noteName].toStdString().c_str());
    GuidoFactorySetEventAccidentals(arFactory, _accidental);
    GuidoFactorySetOctave(arFactory, _octave);
    GuidoFactorySetDuration(arFactory, getNumDuration(), getDenomDuration());
    GuidoFactorySetEventDots(arFactory, getDots());
    GuidoFactoryCloseEvent(arFactory);
}

void    Note::guidoNotationColor(ARFactoryHandler& arFactory)
{
    GuidoFactoryOpenTag(arFactory, "noteFormat", 0);
    GuidoFactoryAddTagParameterString(arFactory, getColorString().toStdString().c_str());
    GuidoFactorySetParameterName(arFactory, "color");
    GuidoFactoryCloseTag(arFactory);
    GuidoFactoryEndTag(arFactory);
}

/*
 * Déséréalise le XML et initialise les variables à partir des valeurs déséréalisées
 */
void    Note::readXML(QDomElement const& domElement)
{
    QDomNodeList    tmp;

    if (!(tmp = domElement.elementsByTagName("notename")).isEmpty())
        _noteName = noteNameMap.key(tmp.at(0).toElement().text());
    if (!(tmp = domElement.elementsByTagName("accidental")).isEmpty())
        _accidental = tmp.at(0).toElement().text().toInt();
    if (!(tmp = domElement.elementsByTagName("octave")).isEmpty())
        _octave = tmp.at(0).toElement().text().toInt();
    if (!(tmp = domElement.elementsByTagName("numduration")).isEmpty())
        setNumDuration(tmp.at(0).toElement().text().toInt());
    if (!(tmp = domElement.elementsByTagName("denomduration")).isEmpty())
        setDenomDuration(tmp.at(0).toElement().text().toInt());
    if (!(tmp = domElement.elementsByTagName("dots")).isEmpty())
        setDots(tmp.at(0).toElement().text().toInt());
}

/*
 * Séréalise la note pour etre traiter en XML
 */
QDomElement Note::writeXML(QDomDocument& document) const
{
    QDomElement note;
    QDomElement element;
    QDomText    text;

    note = document.createElement("musicelement");
    note.setAttribute("type", "note");

    element = document.createElement("notename");
    text = document.createTextNode(noteNameMap[_noteName]);
    element.appendChild(text);
    note.appendChild(element);

    element = document.createElement("accidental");
    text = document.createTextNode(QString::number(_accidental));
    element.appendChild(text);
    note.appendChild(element);

    element = document.createElement("octave");
    text = document.createTextNode(QString::number(_octave));
    element.appendChild(text);
    note.appendChild(element);

    element = document.createElement("numduration");
    text = document.createTextNode(QString::number(getNumDuration()));
    element.appendChild(text);
    note.appendChild(element);

    element = document.createElement("denomduration");
    text = document.createTextNode(QString::number(getDenomDuration()));
    element.appendChild(text);
    note.appendChild(element);

    element = document.createElement("dots");
    text = document.createTextNode(QString::number(getDots()));
    element.appendChild(text);
    note.appendChild(element);

    return note;
}

AMusicElement*  Note::duplicate() const
{
    return new Note(*this);
}

Note::NoteName    Note::getNoteName() const
{
    return _noteName;
}

void        Note::setNoteName(NoteName noteName)
{
    if (_noteName != noteName)
    {
        _accidental = 0;
        _octave = 1;
        setNumDuration(1);
        setDenomDuration(4);
    }
    _noteName = noteName;
}

int  Note::getAccidental() const
{
    return _accidental;
}

/*
 * Les valeurs positive sont utilisées pour le nombre de dièzes et
 * les négatives pour le nombre de bémols.
 */
void    Note::setAccidental(int accidental)
{
    _accidental = accidental;
}

int Note::getOctave() const
{
    return _octave;
}

void    Note::setOctave(int octave)
{
    _octave = octave;
}

QString Note::gmnAMusicElement() const
{
    return noteNameMap[_noteName] + QString::number(_accidental) + QString::number(_octave) + QString::number(getNumDuration()) + "/" + QString::number(getDenomDuration());
}
