#ifndef PARSE_H
#define PARSE_H

#include    <QDebug>
#include    <stdio.h>
#include    <stdlib.h>
#include    <iostream>
#include    <fstream>
#include    <algorithm>
#include    <string>
#include    "Project.h"
#include    "Score.h"
#include    "Voice.h"
#include    "Note.h"
#include    "Tag.h"
#include    "Rest.h"


class Parse
{
private:
    std::string     FName;
    std::ifstream   GmnFile;
    std::string     content;
    Project*        _project;
    Score*          _score;
    Note*           _lastNote;
    Rest*           _lastRest;
    bool            _lastIsNote = false;

    void                        clean();
    std::vector<std::string>    CutPartVoice();
    Voice*                      ParseVoice(std::string);
    AMusicElement*              ParseTag(std::string voice);
    AMusicElement*              ParseNote(std::string voice);
    int                         count(std::string string, char delim, int pos);
    AMusicElement*              ParseRest(std::string voice);
    QVector<AMusicElement*>*    ParseParenthese(std::string voice);
    int                         number(std::string voice, int pos);


public:
    Parse(std::string FName);
    Score*                      getScoreFromImport();
};

#endif // PARSE_H
