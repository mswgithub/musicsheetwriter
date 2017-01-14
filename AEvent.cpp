#include "AEvent.h"

AEvent::AEvent()
{
    _numDuration = 1;
    _denomDuration = 4;
    _dots = 0;
}

AEvent::AEvent(const AEvent& event) : AMusicElement(event)
{
    _numDuration = event._numDuration;
    _denomDuration = event._denomDuration;
    _dots = event._dots;
}

AEvent::~AEvent()
{

}


int AEvent::getNumDuration() const
{
    return _numDuration;
}

void    AEvent::setNumDuration(int numDuration)
{
    _numDuration = numDuration;
}

int AEvent::getDenomDuration() const
{
    return _denomDuration;
}

void    AEvent::setDenomDuration(int denomDuration)
{
    _denomDuration = denomDuration;
}

int AEvent::getDots() const
{
    return _dots;
}

void    AEvent::setDots(int dots)
{
    _dots = dots;
}

