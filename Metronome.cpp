#include "Metronome.h"

Metronome::Metronome()
{
    _sound = new QSound(":/sounds/tick.wav");
    _stop = false;
}


Metronome::~Metronome()
{
    delete _sound;
}

void    Metronome::run()
{
    _stop = false;
    while (!_stop)
    {
        _sound->play();
        float ms = (60.0 / _bmp) * 1000;
        msleep(ms);
    }
}

void    Metronome::stop()
{
    _stop = true;
}

void    Metronome::setBmp(float bmp)
{
    _bmp = bmp;
}
