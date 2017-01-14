#ifndef TAG_H
#define TAG_H

#include    <QMap>
#include    <QVector>
#include    <QPair>
#include    <cstdlib> // for the atoi function
#include    <string>
#include    "AMusicElement.h"
#include    "MusicElementFactory.h"

class Tag : public AEvent
{
public:

    enum ParameterType{
        STRING = 0,
        INTEGER,
        FLOAT
    };

    Tag();
    Tag(QString const& name);
    Tag(QString const& name, bool hasRange);
    Tag(Tag const& tag);
    virtual ~Tag();

    // IGuidoAbstractObject implementation
    void    abstract(ARFactoryHandler& arFactory);

    // AMusicElement implementation
    void            readXML(QDomElement const& domElement);
    QDomElement     writeXML(QDomDocument& document) const;
    void            guidoNotationColor(ARFactoryHandler& arFactory);
    AMusicElement*  duplicate() const;
    QString         gmnAMusicElement() const;

    // Accessors
    QString const&                  getName() const;
    void                            setName(QString const& name);
    bool                            hasRange() const;
    void                            setHasRange(bool hasRange);

    // Params
    QVector<QPair<QString, Tag::ParameterType>>*   getParams();
    void                setParams(QVector<QPair<QString, Tag::ParameterType>>* params);
    void                addParam(QString param, ParameterType isInt = STRING);
    void                insertParam(int pos, QString param, ParameterType isInt = STRING);
    void                removeParam(int pos);
    void                removeAllParams();

    // Range (Music elements)
    QVector<AMusicElement*>*    getMusicElements() const;
    void                        setMusicElements(QVector<AMusicElement*>* musicElements);
    void                        addElement(AMusicElement* element);
    void                        insertElement(int pos, AMusicElement* element);
    void                        removeElement(int pos);

private:
    QString                                 _name;
    // true if the tag has a range, false otherwise
    bool                                    _hasRange;
    // This is the list of parameters.
    // The String in each pair is the actual value of the parameter as string
    // The bool represents whether the parameter value was initially an integer
    QVector<QPair<QString, ParameterType>>* _params;
    // The list of the music element within the Tag object
    QVector<AMusicElement*>*                _musicElements;
};

#endif // TAG_H
