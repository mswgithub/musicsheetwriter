#include "Chord.h"
#include "MusicElementFactory.h"
#include <QDebug>

Chord::Chord()
{
    _notesVector = new QVector<AMusicElement*>();
}

Chord::Chord(Chord const* chord)
{
    _notesVector = chord->_notesVector;
}

Chord::~Chord()
{
    for (auto it = _notesVector->begin(); it != _notesVector->end(); ++it) {
        delete *it;
    }
    delete _notesVector;
}

void    Chord::abstract(ARFactoryHandler& arFactory)
{
    GuidoFactoryOpenChord(arFactory);
    for (auto it = _notesVector->begin(); it != _notesVector->end(); ++it)
    {
        (*it)->abstract(arFactory);
    }
    GuidoFactoryCloseChord(arFactory);
}

void    Chord::guidoNotationColor(ARFactoryHandler& arFactory)
{
    // TODO Implement
    GuidoFactoryOpenTag(arFactory, "chordFormat", 0);
    GuidoFactoryAddTagParameterString(arFactory, getColorString().toStdString().c_str());
    GuidoFactorySetParameterName(arFactory, "color");
    GuidoFactoryCloseTag(arFactory);
    GuidoFactoryEndTag(arFactory);
}

/*
 * Déséréalise le XML et initialise les variables à partir des valeurs déséréalisées
 */
void    Chord::readXML(QDomElement const& domElement)
{
    QDomNodeList tmp;
    QDomElement tmp2;

    if (!(tmp = domElement.elementsByTagName("musicelement")).isEmpty())//chord
    {
        if (!(tmp = tmp.at(0).toElement().elementsByTagName("musicelement")).isEmpty()) // note
        {
            int len = tmp.count();
            for (int i = 0; i < len; i++)
            {
                tmp2 = tmp.at(i).toElement();
                MusicElementFactory factory;
                Note* tmpNote;
                if ((tmpNote = dynamic_cast<Note*>(factory.newFromDomElement(tmp2))) == NULL){
                    //qDebug() << "erreur dans chord note cast";
                    return;
                }
                _notesVector->push_back(tmpNote);
            }
        }
    }
}

/*
 * Séréalise la note pour etre traiter en XML
 */
QDomElement Chord::writeXML(QDomDocument& document) const
{
    QDomElement chord;
    QDomElement element;
    int i = 0;

    chord = document.createElement("musicelement");
    chord.setAttribute("type", "chord");

    while (i < _notesVector->count()){
        Note* noteTmp = new Note();
        noteTmp = (Note*)_notesVector->at(i);
        element = noteTmp->writeXML(document);
        chord.appendChild(element);
        i++;
    }

    return chord;
}

AMusicElement*  Chord::duplicate() const
{
    return new Chord(this);
}

QVector<AMusicElement*>*  Chord::getNotes()
{
    return _notesVector;
}

void            Chord::addNote(AMusicElement* note)
{
    _notesVector->push_back(note);
}

void            Chord::setNote(QVector<AMusicElement*>* elem)
{
    for (auto it = _notesVector->begin(); it != _notesVector->end(); ++it) {
        delete *it;
    }
    delete _notesVector;
    _notesVector = (QVector<AMusicElement*>*)(elem);
}

QString         Chord::gmnAMusicElement() const
{
    QString str;

    str += "{";
    for (auto it = _notesVector->begin(); it != _notesVector->end(); ++it)
    {
        AMusicElement* elem = *it;
        str += elem->gmnAMusicElement();
        if (it != _notesVector->end())
            str += ",";
    }
    str += "}";
    qDebug() << str;
    return str;
}
