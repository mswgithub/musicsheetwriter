#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include    <string>
#include    <iostream>
#include    <fstream>
#include <QDebug>


#include    "AudioData.h"

#include "userexception.h"

#ifdef WIN32
#include    <windows.h>

#if PA_USE_ASIO
#include    "pa_asio.h"
#endif
#endif

/*
 * Use as in Interface to control input device actions
 */
class AudioInput
{
public:
    explicit AudioInput();
    virtual ~AudioInput();

    void    init();
    void    setDevice();
    void    startListening();
    void    stopListening();
    void    closeStream();
    void    getDevices();
    void    addBuffer();
    void    moveBuffer();
    AudioBuffer    *allocBuffer();
    AudioData            data;

private:
    PaStreamParameters   inputParameters;
    PaStreamParameters   outputParameters;
    PaStream*            stream;
    PaError              err;
    int                  numSamples;
};

#endif // AUDIOINPUT_H
