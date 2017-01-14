#ifndef ACCOUSTICANALIZER_H
#define ACCOUSTICANALIZER_H

// Must be at the top of the include tree
#define _USE_MATH_DEFINES
#include    <cmath>

#include    <QElapsedTimer>
#include    <QCoreApplication>
#include    <algorithm>
#include    <stdio.h>
#include    <QMap>
#include    <sstream>
#include    <QString>
#include    <stdexcept>
#include    "analysis/AsoundAnalyzer.h"
#include    "analysis/AudioInput.h"
#include    "analysis/AudioData.h"
#include    "analysis/FrequencyTable.h"
#include    "Note.h"

#define AMPLITUDE_FF_THRESHOLD (10.f) //Threshold to detect a note. Before : (double)100.f
#define AMPLITUDE_HAR_THRESHOLD (5.f) //Threshold to detect a note. Before : (double)75.f

typedef struct NumericNote {
    float           frequency; //In hertz
    unsigned long   duration; //in milliseconds
    float           magnitude;
} NumericNote;

class AccousticAnalyzer : public ASoundAnalyzer
{
public:
    explicit AccousticAnalyzer(IScoreEditor* scoreEditor);
    virtual ~AccousticAnalyzer();

    void    init();
    // inherited from ASoundAnalyzer
    void    start();
    void    stop();
    void    free();

protected:
    // inherited from QThread
    void    run();

    void    analyse(SAMPLE *buffer, const unsigned int len);
    void    hanning(SAMPLE *buffer, const unsigned int len);
    void    fft(SAMPLE* data, const unsigned int nn) const;
    double  getFundamentalFrequency(const SAMPLE *fft, const unsigned int len, float &m) const;

    unsigned int     getFirstFreqOverThreshold(const SAMPLE* fft, int startIndex, const unsigned int len) const;
    unsigned int     getLastFreqOverThreshold(const SAMPLE* fft, int startIndex, const unsigned int len) const;
    unsigned int     getBiggestPeakIndex(const SAMPLE* fft, int startIndex, int lastIndex) const;
    bool             hasEnoughHarmonic(const SAMPLE* fft, const unsigned int peakIndex, const unsigned int len) const;
    float            getMagnitude(const SAMPLE* fft, int index) const;
    int              getFrequence(unsigned int peakIndex, const unsigned int len) const;

private slots:
    void    freeData();

private:
    typedef enum
    {
        STARTED,
        STOPPED
    } State;

    QPair<int, int> deciToFraction(float decimal);

    AudioInput*             _audioInput;
    SAMPLE                  *_data;//complex buffer
    FrequencyTable const    _frequencyTable;
    // the last fundamental frequency return by the fft
    double                  _lastFreq;
    // Used to ignore the first rest when the analysis is launched
    bool                    _firstRest;

    // The current index in the frame
    long    _frameIndex;
    // Just to test, to be removed in release
    FILE*   _file;
    // The state of the analyser (started, stopped)
    State   _state;
    NumericNote _note;//The numeric data of the current note
};

#endif // ACCOUSTICANALIZER_H
