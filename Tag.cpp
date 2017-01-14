#include "Tag.h"

Tag::Tag()
{
    _name = "";
    _hasRange = false;
    _params = new QVector<QPair<QString, ParameterType>>();
    _musicElements = new QVector<AMusicElement*>();
}

Tag::Tag(QString const& name)
{
    _name = name;
    _hasRange = false;
    _params = new QVector<QPair<QString, ParameterType>>();
    _musicElements = new QVector<AMusicElement*>();
}

Tag::Tag(QString const& name, bool hasRange)
{
    _name = name;
    _hasRange = hasRange;
    _params = new QVector<QPair<QString, ParameterType>>;
    _musicElements = new QVector<AMusicElement*>();
}

Tag::Tag(Tag const& tag) : AEvent(tag)
{
    _name = tag._name;
    _hasRange = tag._hasRange;
    _params = new QVector<QPair<QString, ParameterType> >();
    for (auto it = tag._params->begin(); it != tag._params->end(); ++it) {
        _params->push_back(*it);
    }
    _musicElements = new QVector<AMusicElement*>();
    for (auto it = tag._musicElements->begin(); it != tag._musicElements->end(); ++it) {
        _musicElements->push_back((*it)->duplicate());
     }
}

Tag::~Tag()
{

    delete _params;

    for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it) {
        delete *it;
    }
    delete _musicElements;
}

/*
 * Abstrait l'objet tag en utilisant le GUIDOFactoryAdapter. Selon si
 * le tag contient d'autre éléments musicaux, il va les appeller afin
 * de les abstraire.
 */
void    Tag::abstract(ARFactoryHandler& arFactory)
{
    if (_hasRange)
    {
        // The ID is set to 0
        GuidoFactoryOpenRangeTag(arFactory, _name.toStdString().c_str(), 0);
    }
    else
    {
        // The ID is set to 0
        GuidoFactoryOpenTag(arFactory, _name.toStdString().c_str(), 0);
    }

    // Abstract the parameters of the tag
    for (auto it = _params->begin(); it != _params->end(); ++it)
    {
        // Call the proper function depending on the type of the parameter
        if (it->second == STRING)
        {
            GuidoFactoryAddTagParameterString(arFactory, it->first.toStdString().c_str());
        }
        else if (it->second == INTEGER)
        {
            GuidoFactoryAddTagParameterInt(arFactory, atoi(it->first.toStdString().c_str()));
        }
        else if (it->second == FLOAT)
        {
            GuidoFactoryAddTagParameterFloat(arFactory, atof(it->first.toStdString().c_str()));
        }
    }
    guidoNotationColor(arFactory);
    GuidoFactoryCloseTag(arFactory);

    if (_hasRange)
    {
        // Abstrait les élements musicaux contenu dans le tag (range seulement)
        for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it) {
            (*it)->abstract(arFactory);
        }
        GuidoFactoryEndTag(arFactory);
    }
}

void    Tag::guidoNotationColor(ARFactoryHandler& arFactory)
{
    GuidoFactoryAddTagParameterString(arFactory, getColorString().toStdString().c_str());
    GuidoFactorySetParameterName(arFactory, "color");
}

/*
 * Déséréalise le XML et initialise les variables à partir des valeurs déséréalisées
 */
void    Tag::readXML(QDomElement const& domElement)
{
    QDomNodeList    tmp;
    QDomElement     tmp2;

    if (!(tmp = domElement.elementsByTagName("name")).isEmpty())
        _name = tmp.at(0).toElement().text();
    if (!(tmp = domElement.elementsByTagName("range")).isEmpty())
        _hasRange = (bool) tmp.at(0).toElement().text().toInt();

    if (!(tmp = domElement.elementsByTagName("params")).isEmpty())
    {
        if (!(tmp = tmp.at(0).toElement().elementsByTagName("param")).isEmpty())
        {
            int len = tmp.count();
            for (int i = 0; i < len; i++)
            {
                tmp2 = tmp.at(i).toElement();
                addParam(tmp2.attribute("value"), (ParameterType) atoi(tmp2.attribute("type").toStdString().c_str()));
            }
        }
    }

    if (!(tmp = domElement.elementsByTagName("musicelements")).isEmpty())
    {
        if (!(tmp = tmp.at(0).toElement().elementsByTagName("musicelement")).isEmpty())
        {
            int len = tmp.count();
            for (int i = 0; i < len; i++)
            {
                tmp2 = tmp.at(i).toElement();
                MusicElementFactory factory;
                AMusicElement* musicElement = factory.newFromDomElement(tmp2);
                _musicElements->push_back(musicElement);
            }
        }
    }
}

/*
 * Séréalise le tag pour etre traiter en XML
 */
QDomElement Tag::writeXML(QDomDocument& document) const
{
    QDomElement tag;
    QDomElement elements;
    QDomElement element;
    QDomText    text;

    tag = document.createElement("musicelement");
    tag.setAttribute("type", "tag");

    element = document.createElement("name");
    text = document.createTextNode(_name);
    element.appendChild(text);
    tag.appendChild(element);

    element = document.createElement("range");
    text = document.createTextNode(QString::number((int) _hasRange));
    element.appendChild(text);
    tag.appendChild(element);

    elements = document.createElement("params");
    for (auto it = _params->begin(); it != _params->end(); ++it)
    {
        element = document.createElement("param");
        element.setAttribute("value", it->first);
        element.setAttribute("type", QString::fromStdString(std::to_string(it->second)));
        elements.appendChild(element);
    }
    tag.appendChild(elements);

    elements = document.createElement("musicelements");
    for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it)
    {
        element = (*it)->writeXML(document);
        elements.appendChild(element);
    }

    tag.appendChild(elements);

    return tag;
}

AMusicElement*  Tag::duplicate() const
{
    return new Tag(*this);
}

QString const&  Tag::getName() const
{
    return _name;
}

void    Tag::setName(QString const& name)
{
    _name = name;
}

bool    Tag::hasRange() const
{
    return _hasRange;
}

void    Tag::setHasRange(bool hasRange)
{
    _hasRange = hasRange;
}

/*
 * Accesseurs pour les parametres
 */
QVector<QPair<QString, Tag::ParameterType>>*    Tag::getParams()
{
    return _params;
}

void    Tag::setParams(QVector<QPair<QString, Tag::ParameterType>>* params)
{
    delete _params;
    _params = params;
}

void    Tag::addParam(QString param, ParameterType isInt)
{
    _params->push_back(QPair<QString, ParameterType>(param, isInt));
}

void    Tag::insertParam(int pos, QString param, ParameterType isInt)
{
    _params->insert(pos, QPair<QString, ParameterType>(param, isInt));
}

void    Tag::removeParam(int pos)
{
    _params->removeAt(pos);
}

void    Tag::removeAllParams()
{
    _params->clear();
}

/*
 * Accesseurs pour les élements musicaux
 */
QVector<AMusicElement*>* Tag::getMusicElements() const
{
    return _musicElements;
}

void    Tag::setMusicElements(QVector<AMusicElement*>* musicElements)
{
    for (auto it = _musicElements->begin(); it != _musicElements->end(); ++it) {
        delete *it;
    }
    delete _musicElements;
    _musicElements = musicElements;
}

void    Tag::addElement(AMusicElement* element)
{
    _musicElements->push_back(element);
}

void    Tag::insertElement(int pos, AMusicElement* element)
{
    _musicElements->insert(pos, element);
}

void    Tag::removeElement(int pos)
{
    delete _musicElements->takeAt(pos);
}

QString Tag::gmnAMusicElement() const
{
    QString taggmn;

    taggmn = _name;
    for (auto it = _params->begin(); it != _params->end(); ++it)
    {
        taggmn += it->first;
    }
    return taggmn;
}

