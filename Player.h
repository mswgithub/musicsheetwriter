#ifndef PLAYER_H
#define PLAYER_H

#include    <QObject>
#include    <QThread>
#include    <QElapsedTimer>
#include    <QMidiOut.h>
#include    <QMidiFile.h>
#include    <QFile>
#include    <QStandardPaths>
#include    "Score.h"
#include    "GUIDOEngine.h"
#include    "GUIDO2Midi.h"
#include    "GUIDOParse.h"

class Player : public QThread
{
    Q_OBJECT

public:
    explicit Player();
    virtual ~Player();

    void    run();
    void    pause();
    void    resume();
    void    stop();

    void    setMidiFile(Score *score);
    void    setTimeElapsed(int timer);
    void    setPause(bool state);
    void    setStop(bool state);

signals:
    void    signalEnd();

private:
    QString         _outfile;
    QWidget*        _uiPlayerView;
    QMidiFile*      _midiFile;
    QMidiOut*       _midiOut;
    QElapsedTimer   _t;
    bool            _paused;
    bool            _stop;
    int             _timeElapsed;
};

#endif // PLAYER_H
