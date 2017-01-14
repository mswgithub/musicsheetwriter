#include "PlayerManager.h"

PlayerManager::PlayerManager(QWidget *playerView, QObject *parent) : QObject(parent)
{
    _player = new Player();
    _midiplayer = new MidiPlayer(playerView);
    _playerview = playerView;
    _playerview->hide();

    connect(_midiplayer, SIGNAL(signalStartClicked()), SLOT(slotStartClicked()));
    connect(_midiplayer, SIGNAL(signalPauseClicked()), SLOT(slotPauseClicked()));
    connect(_midiplayer, SIGNAL(signalResumeClicked()), SLOT(slotResumeClicked()));
    connect(_midiplayer, SIGNAL(signalStopClicked()), SLOT(slotStopClicked()));
    connect(_player, SIGNAL(signalEnd()), SLOT(slotEnd()));
}

PlayerManager::~PlayerManager()
{
    delete _player;
    delete _midiplayer;
}

void    PlayerManager::updatePlayer(Score *score)
{
    if (score)
    {
        _score = score;
        _playerview->show();
    }
    else
    {
        _playerview->hide();
    }
}

void    PlayerManager::slotStartClicked()
{
    _player->setMidiFile(_score);
    _player->setStop(false);
    _player->start();
}

void    PlayerManager::slotPauseClicked()
{
    _player->pause();
}

void    PlayerManager::slotResumeClicked()
{
    _player->resume();
}

void    PlayerManager::slotStopClicked()
{
    _player->stop();
    this->slotEnd();
}

void    PlayerManager::slotEnd()
{
    _midiplayer->setStart(false);
    _player->setTimeElapsed(0);
    _player->setPause(false);
}

void    PlayerManager::setTranslation()
{
    _midiplayer->setTranslation();
}
