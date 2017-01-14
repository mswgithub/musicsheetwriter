#ifndef AUDIODATA_H
#define AUDIODATA_H

#include    <portaudio.h>
#include    <list>

#define SAMPLE_RATE         (44100)
#define FRAMES_PER_BUFFER   (2048)
#define NUM_SECONDS         (5)
#define NUM_CHANNELS        (1) //for mono, set 1 and 2 for stereo
#define DITHER_FLAG         (0) /**/

/* Select sample format. */
/* Check with the device informations to check compatibility*/
#if 1
#define PA_SAMPLE_TYPE      paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE      (0.0f)
#define PRINTF_S_FORMAT     "%.8f"
#elif 1
#define PA_SAMPLE_TYPE      paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE      (0)
#define PRINTF_S_FORMAT     "%d"
#elif 0
#define PA_SAMPLE_TYPE      paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE      (0)
#define PRINTF_S_FORMAT     "%d"
#else
#define PA_SAMPLE_TYPE      paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE      (128)
#define PRINTF_S_FORMAT     "%d"
#endif


#define FFT_BUFFER_SIZE (FRAMES_PER_BUFFER*2) // Size of the buffer sent to the fft

#include <sstream>

typedef struct AudioBuffer{
    int         frameIndex;  /* Index into sample array. */
    SAMPLE      *recordedSamples; /* recorded data */
    AudioBuffer *next;
} AudioBuffer;

typedef struct AudioData
{
  int         maxFrameIndex;
  AudioBuffer *in;
  AudioBuffer *out;
} AudioData;

#endif // AUDIODATA_H
