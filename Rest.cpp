#include "Rest.h"

Rest::Rest()
{
}

Rest::Rest(Rest const& rest) : AEvent(rest)
{
}

Rest::~Rest()
{

}

bool    Rest::operator==(Rest const& rest) const
{
    return this->getNumDuration() == rest.getNumDuration() &&
            this->getDenomDuration() == rest.getDenomDuration() &&
            this->getDots() == rest.getDots();
}

/*
 * Abstrait l'objet silence en utilisant le GUIDOFactoryAdapter
 */
void    Rest::abstract(ARFactoryHandler& arFactory)
{
    guidoNotationColor(arFactory);
    GuidoFactoryOpenEvent(arFactory, "_");
    GuidoFactorySetDuration(arFactory, getNumDuration(), getDenomDuration());
    GuidoFactorySetEventDots(arFactory, getDots());
    GuidoFactoryCloseEvent(arFactory);
}

void    Rest::guidoNotationColor(ARFactoryHandler& arFactory)
{
    GuidoFactoryOpenTag(arFactory, "restFormat", 0);
    GuidoFactoryAddTagParameterString(arFactory, getColorString().toStdString().c_str());
    GuidoFactorySetParameterName(arFactory, "color");
    GuidoFactoryCloseTag(arFactory);
    GuidoFactoryEndTag(arFactory);
}

/*
 * Déséréalise le XML et initialise les variables à partir des valeurs déséréalisées
 */
void    Rest::readXML(QDomElement const& domElement)
{
    QDomNodeList    tmp;

    if (!(tmp = domElement.elementsByTagName("numduration")).isEmpty())
        setNumDuration(tmp.at(0).toElement().text().toInt());
    if (!(tmp = domElement.elementsByTagName("denomduration")).isEmpty())
        setDenomDuration(tmp.at(0).toElement().text().toInt());
    if (!(tmp = domElement.elementsByTagName("dots")).isEmpty())
        setDots(tmp.at(0).toElement().text().toInt());
}

/*
 * Séréalise le silence pour etre traiter en XML
 */
QDomElement Rest::writeXML(QDomDocument& document) const
{
    QDomElement rest;
    QDomElement element;
    QDomText    text;

    rest = document.createElement("musicelement");
    rest.setAttribute("type", "rest");

    element = document.createElement("numduration");
    text = document.createTextNode(QString::number(getNumDuration()));
    element.appendChild(text);
    rest.appendChild(element);

    element = document.createElement("denomduration");
    text = document.createTextNode(QString::number(getDenomDuration()));
    element.appendChild(text);
    rest.appendChild(element);

    element = document.createElement("dots");
    text = document.createTextNode(QString::number(getDots()));
    element.appendChild(text);
    rest.appendChild(element);

    return rest;
}

AMusicElement*  Rest::duplicate() const
{
    return new Rest(*this);
}

QString Rest::gmnAMusicElement() const
{
    return "_" + QString::number(getNumDuration()) + "/" + QString::number(getDenomDuration());
}
