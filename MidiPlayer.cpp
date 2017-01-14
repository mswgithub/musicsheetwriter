#include "MidiPlayer.h"
#include "ui_midiPlayer.h"
MidiPlayer::MidiPlayer(QWidget *playerView)
{
    _ui = new Ui::MidiPlayer;
    _ui->setupUi(playerView);
    _widgetTranslate = playerView;
    initSignalsPlayer();
    _ui->playButton->setIcon(QIcon(":/images/Player/play.png"));
    _ui->stopButton->setIcon(QIcon(":/images/Player/stop.png"));
    _ui->stopButton->setEnabled(false);
    _state = false;
    _start = false;
}

MidiPlayer::~MidiPlayer()
{
    delete _ui;
}

void    MidiPlayer::initSignalsPlayer()
{
    connect(_ui->playButton, SIGNAL(clicked(bool)), SLOT(slotButtonClicked()));
    connect(_ui->stopButton, SIGNAL(clicked(bool)), SLOT(slotButtonStopClicked()));
}

void    MidiPlayer::slotButtonClicked()
{
    if (_state)
    {
        _ui->playButton->setIcon(QIcon(":/images/Player/play.png"));
        _state = false;
        _start = true;
        emit signalPauseClicked();
    }
    else
    {
        _ui->stopButton->setEnabled(true);
        _ui->playButton->setIcon(QIcon(":/images/Player/pause.png"));
        _state = true;
        if (_start)
        {
            emit signalResumeClicked();
        }
        else
        {
            emit signalStartClicked();
        }
    }
}

void    MidiPlayer::slotButtonStartClicked()
{
    emit signalStartClicked();
}

void    MidiPlayer::slotButtonPauseClicked()
{
     emit signalPauseClicked();
}

void    MidiPlayer::slotButtonStopClicked()
{
    emit signalStopClicked();
}

void    MidiPlayer::setStart(bool start)
{
    _start = start;
    if (!_start)
    {
       _ui->stopButton->setEnabled(false);
       _ui->playButton->setIcon(QIcon(":/images/Player/play.png"));
       _state = false;
    }
}

void    MidiPlayer::setTranslation()
{
    _ui->retranslateUi(_widgetTranslate);
}
