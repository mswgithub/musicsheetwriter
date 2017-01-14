#ifndef PROJECT_H
#define PROJECT_H

#include    <QList>
#include    <QDir>
#include    <QFile>
#include    <QMessageBox>
#include    <QTextStream>
#include    <QtXml/QDomDocument>
#include    <QBuffer>
#include    <QByteArray>
#include    <QGuidoPainter.h>
#include    "Score.h"

//#include    "Midiplayer.h"

class Project
{
public:
    Project();
    Project(QString const& projectName, QString const& projectPath = "");
    Project(QString const& projectName, QString const& projectPath, Score *score);
    virtual ~Project();

    QString const&  getProjectName() const;
    QString const&  getProjectPath() const;
    Score*          getScore() const;
    bool            isActive() const;
    void            setProjectName(QString projectName);
    void            setProjectPath(QString projectpath);
    void            setScore(Score* score);
    void            setIsActive(bool active);
    void            setgmnScore(QString gmnScore);

    void            save();
    bool            checkSave();
    void            serialized(QString path);
    QDomElement     serializedScorePng(QDomDocument doc);
    bool            deserialize(QString path);
    QDomElement     writeXML(QDomDocument doc);
    bool            readXML(QDomElement elem);
    static bool     fileExists(QString path);
    QString         checkGmnScore();

    // TODO put as private and provide accessors
    //MidiPlayer      *player;

private:
    QString _projectName;
    QString _projectPath;
    Score*  _score;
    bool    _isActive;
    QString _gmnScore;
};

#endif // PROJECT_H
