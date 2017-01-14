#include "MetronomeManager.h"

MetronomeManager::MetronomeManager(QWidget *metronomeView, QObject *parent) : QObject(parent)
{
    _metronomeview = metronomeView;
    _mPlayer = new MetronomePlayer(metronomeView);
    _metronome = new Metronome();
    _metronomeview->hide();

    connect(_mPlayer, SIGNAL(signalStartClicked(int)), SLOT(slotStartClicked(int)));
    connect(_mPlayer, SIGNAL(signalStopClicked()), SLOT(slotStopClicked()));
    connect(_mPlayer, SIGNAL(signalButtonAddClicked(QString)), SLOT(slotButtonAddClicked(QString)));
}

MetronomeManager::~MetronomeManager()
{
    delete _mPlayer;
}

void    MetronomeManager::updateMetronome(bool state)
{
    if (state)
    {
        _metronomeview->show();
    }
    else
    {
        _metronomeview->hide();
    }
}

void    MetronomeManager::slotStartClicked(int bmp)
{
    _metronome->setBmp(bmp);
    _metronome->start();
}

void    MetronomeManager::slotStopClicked()
{
    _metronome->stop();
}

void    MetronomeManager::slotButtonAddClicked(QString bmp)
{
    emit signalButtonAddClicked(bmp);
}

void    MetronomeManager::setTranslation()
{
    _mPlayer->setTranslation();
}
