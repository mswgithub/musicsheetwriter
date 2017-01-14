#ifndef SCORE_H
#define SCORE_H

#include    <QString>
#include    <QtXml/QDomDocument>
#include    <QVector>
#include    "IGuidoAbstractObject.h"
#include    "Voice.h"

class Score
{
public:
    Score();
    Score(QString const& title, QString const& composer);
    ~Score();

    ARHandler  abstract(ARFactoryHandler& arFactory);

    bool        readXML(QDomElement const& domElement);
    QDomElement writeXML(QDomDocument& document) const;

    // Accessors
    QString const&  getTitle() const;
    void            setTitle(QString const& title);
    QString const&  getComposer() const;
    void            setComposer(QString const& composer);
    // Voices
    QVector<Voice*>*    getVoices();
    void                setVoices(QVector<Voice*>* voices);
    void                addVoice(Voice* voice);
    void                insertVoice(int pos, Voice* voice);
    void                removeVoice(int pos);

private:
    QString             _title;
    QString             _composer;
    QVector<Voice*>*    _voices;
};

#endif // SCORE_H
