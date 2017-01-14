#ifndef MidiAnalyser_H
#define MidiAnalyser_H

#include <time.h>
#include "midi/rtmidi.h"
#include "midi/rtmidi_c.h"
#include "Note.h"
#include "analysis/ASoundAnalyzer.h"
#include "userexception.h"
#include <QDebug>
#include <list>

typedef struct MidiNote{
    int note;
    clock_t start;
    clock_t end;
} MidiNote;

class MidiAnalyser : public ASoundAnalyzer
{
private:
    RtMidiIn *midiIn;
    std::list<MidiNote *> tmp_notes;//List of current note event
    bool silenceOn;
    clock_t silence_start;
    clock_t silence_end;

protected:
    // inherited from QThread
    void    run();

public:
    void    init();//Must be called before start()
    void    start();//Start the analyze
    void    stop();//Stop it
    void addNote(const int note);//Add a new event noteON
    MidiNote *findNote(const int note);//Add a new event noteOff
    void startSilence();//start a silence
    double endSilence();//return the silence duration
    bool isSilence() const;
    bool noEvent() const;
    MidiAnalyser(IScoreEditor* scoreEditor);
    ~MidiAnalyser();
    bool chooseMidiPort( RtMidiIn *rtmidi );
};

#endif // MidiAnalyser_H
