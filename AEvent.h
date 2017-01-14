#ifndef AEVENT_H
#define AEVENT_H

#include    "AMusicElement.h"

class AEvent : public AMusicElement
{
public:
    AEvent();
    AEvent(AEvent const& event);
    virtual ~AEvent();

    // Accessors
    int         getNumDuration() const;
    void        setNumDuration(int numDuration);
    int         getDenomDuration() const;
    void        setDenomDuration(int denomDuration);
    int         getDots() const;
    void        setDots(int dots);

private:
    // The duration of the event as defined in *_numDuration/denomDuration
    int _numDuration;
    int _denomDuration;
    // The number of dots of the event
    int _dots;
};

#endif // AEVENT_H
