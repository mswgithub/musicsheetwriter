#include "Voice.h"

Voice::Voice()
{
    _musicElements = new QVector<AMusicElement*>();
}

Voice::Voice(Voice const* voice)
{
    _musicElements = new QVector<AMusicElement*>();
    for (auto it = voice->_musicElements->begin(); it != voice->_musicElements->end(); ++it) {
        _musicElements->push_back((*it)->duplicate());
    }
}

Voice::~Voice()
{
    for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it) {
        delete *it;
    }
    delete _musicElements;
}

/*
 * Abstrait l'objet voice en utilisant le GUIDOFactoryAdapter.
 */
void   Voice::abstract(ARFactoryHandler& arFactory)
{
    // Abstrait les élements musicaux contenu dans le voice
    for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it) {
        (*it)->abstract(arFactory);
    }
}

/*
 * Déséréalise le XML et initialise les variables à partir des valeurs déséréalisées
 */
bool    Voice::readXML(QDomElement const& domElement)
{
    QDomNodeList    tmp;
    QDomElement     tmp2;

    if (domElement.tagName() == "voice")
    {
        QDomNode child = domElement.firstChild();
        while (!child.isNull())
        {
            if (child.toElement().tagName() != "musicelements")
            {
                return false;
            }
           child = child.nextSibling();
        }
        if (!(tmp = domElement.elementsByTagName("musicelements")).isEmpty())
        {
            if (!(tmp = tmp.at(0).toElement().elementsByTagName("musicelement")).isEmpty())
            {
                int len = tmp.count();
                for (int i = 0; i < len; i++)
                {
                    tmp2 = tmp.at(i).toElement();

                    QString type = tmp2.attribute("type");
                    if (type != "note" && type != "rest" && type != "tag" && type != "chord")
                    {
                        return false;
                    }
                    else
                    {
                        MusicElementFactory factory;
                        AMusicElement* musicElement = factory.newFromDomElement(tmp2);
                        _musicElements->push_back(musicElement);
                    }
                }
            }

        }
        return true;
    }
    else
    {
        return false;
    }
}

/*
 * Séréalise le voice pour etre traiter en XML
 */
QDomElement Voice::writeXML(QDomDocument& document) const
{
    QDomElement voice;
    QDomElement elements;
    QDomElement element;
    QDomText    text;

    voice = document.createElement("voice");

    elements = document.createElement("musicelements");
    for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it)
    {
        element = (*it)->writeXML(document);
        elements.appendChild(element);
    }

    voice.appendChild(elements);

    return voice;
}

QVector<AMusicElement*>* Voice::getMusicElements() const
{
    return _musicElements;
}

void    Voice::setMusicElements(QVector<AMusicElement*>* musicElements)
{
    for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it) {
        delete *it;
    }
    delete _musicElements;
    _musicElements = musicElements;
}

void    Voice::addElement(AMusicElement* element)
{
    _musicElements->push_back(element);
}

void    Voice::insertElement(int pos, AMusicElement* element)
{
    _musicElements->insert(pos, element);
}

void    Voice::removeElement(int pos)
{
    delete _musicElements->takeAt(pos);
}

