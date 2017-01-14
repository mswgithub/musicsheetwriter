#include "Score.h"

Score::Score()
{
    _title = "";
    _composer = "";
    _voices = new QVector<Voice*>();
}

Score::Score(QString const& title, QString const& composer)
{
    _title = title;
    _composer = composer;
    _voices = new QVector<Voice*>();
}

Score::~Score()
{
    for (auto it = _voices->begin(); it != _voices->end(); ++it) {
        delete *it;
    }
    delete _voices;
}

/*
 * Abstrait l'objet partition en utilisant le GUIDOFactoryAdapter.
 */
ARHandler  Score::abstract(ARFactoryHandler& arFactory)
{
    GuidoFactoryOpenMusic(arFactory);

    // Abstrait les voices contenues dans la partition
    for (auto it = _voices->begin(); it != _voices->end(); ++it) {

        GuidoFactoryOpenVoice(arFactory);

        // Le titre et le compositeur doivent ête dans la premiere voice
        // (et uniquement la premiere).
        if (it == _voices->begin())
        {
            GuidoFactoryOpenTag(arFactory, "composer", 0);
            GuidoFactoryAddTagParameterString(arFactory, _composer.toStdString().c_str());
            GuidoFactoryCloseTag(arFactory);

            GuidoFactoryOpenTag(arFactory, "title", 0);
            GuidoFactoryAddTagParameterString(arFactory, _title.toStdString().c_str());
            GuidoFactoryCloseTag(arFactory);
        }

        (*it)->abstract(arFactory);

        GuidoFactoryCloseVoice(arFactory);
    }

    return GuidoFactoryCloseMusic(arFactory);
}

#include <QDebug>

/*
 * Déséréalise le XML et initialise les variables à partir des valeurs déséréalisées
 */
bool    Score::readXML(QDomElement const& domElement)
{
    QDomNodeList    tmp;

    if (domElement.toElement().tagName() == "score")
    {
        QDomNode child = domElement.firstChild();
        while (!child.isNull())
        {
            if (child.toElement().tagName() != "title" && child.toElement().tagName() != "composer"
                    && child.toElement().tagName() != "voices")
                return false;
           child = child.nextSibling();
       }
        if (!(tmp = domElement.elementsByTagName("title")).isEmpty())
            _title = tmp.at(0).toElement().text();
        if (!(tmp = domElement.elementsByTagName("composer")).isEmpty())
            _composer = tmp.at(0).toElement().text();
        if (!(tmp = domElement.elementsByTagName("voices")).isEmpty())
        {
            if (!(tmp = tmp.at(0).toElement().elementsByTagName("voice")).isEmpty())
            {
                int len = tmp.count();
                for (int i = 0; i < len; i++)
                {
                    Voice* voice = new Voice;
                    if (!voice->readXML(tmp.at(i).toElement()))
                    {
                        return false;
                    }
                    _voices->push_back(voice);
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
QDomElement Score::writeXML(QDomDocument& document) const
{
    QDomElement score;
    QDomElement elements;
    QDomElement element;
    QDomText    text;

    score = document.createElement("score");

    element = document.createElement("title");
    text = document.createTextNode(_title);
    element.appendChild(text);
    score.appendChild(element);

    element = document.createElement("composer");
    text = document.createTextNode(_composer);
    element.appendChild(text);
    score.appendChild(element);

    elements = document.createElement("voices");
    for (auto it = _voices->begin(); it != _voices->end(); ++it)
    {
        element = (*it)->writeXML(document);
        elements.appendChild(element);
    }
    score.appendChild(elements);

    return score;
}

QString const&  Score::getTitle() const
{
    return _title;
}

void            Score::setTitle(QString const& title)
{
    _title = title;
}

QString const&  Score::getComposer() const
{
    return _composer;
}

void            Score::setComposer(QString const& composer)
{
    _composer = composer;
}

QVector<Voice*>*    Score::getVoices()
{
    return _voices;
}

void    Score::setVoices(QVector<Voice*>* voices)
{
    for (auto it = _voices->begin(); it != _voices->end(); ++it) {
        delete *it;
    }
    delete _voices;
    _voices = voices;
}

void    Score::addVoice(Voice* voice)
{
    _voices->push_back(voice);
}

void    Score::insertVoice(int pos, Voice* voice)
{
    _voices->insert(pos, voice);
}

void    Score::removeVoice(int pos)
{
    delete _voices->takeAt(pos);
}

