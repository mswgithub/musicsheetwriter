#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include    <QObject>
#include    <Player.h>
#include    <MidiPlayer.h>
#include    <GUIDO2Midi.h>
#include    "Score.h"

class PlayerManager : public QObject
{
    Q_OBJECT

public:
    PlayerManager(QWidget *playerView, QObject* parent = 0);
    virtual ~PlayerManager();

    void    updatePlayer(Score *score);
    void    setTranslation();

public slots:
    void    slotStartClicked();
    void    slotPauseClicked();
    void    slotResumeClicked();
    void    slotStopClicked();
    void    slotEnd();

private:
    Player*     _player;
    MidiPlayer* _midiplayer;
    QWidget*    _playerview;
    Score*      _score;
};

#endif // PLAYERMANAGER_H
