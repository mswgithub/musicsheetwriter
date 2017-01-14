#include "audioinput.h"

int Stream_status__ = paContinue; // Control the thread status

/*
 * Callback device input function to retrieve data from the device input stream and store it into a buffer.
 * Buffer version
 *
 */
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    AudioData *data = (AudioData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr;
    long framesToCalc;
    long i;
    unsigned long framesLeft = data->maxFrameIndex - data->in->frameIndex;

    if( framesLeft < framesPerBuffer )
    {
        if (data->in->next == NULL){
            fprintf( stderr, "Too fast\n" );
            return paComplete;
        }
        data->in = data->in->next;
    }

    framesToCalc = framesPerBuffer;
    wptr = &(data->in->recordedSamples[data->in->frameIndex]);

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = SAMPLE_SILENCE;
            if( NUM_CHANNELS == 2 )
            {
                *wptr++ = SAMPLE_SILENCE;
            }
        }
    }
    else
    {
        for( i=0; i<framesToCalc; i++ )
        {
                *wptr++ = *rptr++;
            if( NUM_CHANNELS == 2 )
            {
                *wptr++ = *rptr++;
            }
        }
    }
    data->in->frameIndex += framesToCalc * NUM_CHANNELS;
    if (Stream_status__ == paComplete)
        return paComplete;
    return paContinue;
}

/*
 * initialise input parameters and buffer size
 * */
AudioInput::AudioInput()
{
    err = paNoError;
    this->stream = NULL;
    this->data.maxFrameIndex = FFT_BUFFER_SIZE * 50;
    AudioBuffer *first = allocBuffer();
    AudioBuffer *second = allocBuffer();
    if (first == NULL || second == NULL){
        printf("Could not allocate record array.\n");
    }
    first->next = second;
    this->data.in = first;
    this->data.out = first;
}

/*
 * free buffer and terminate
 */
AudioInput::~AudioInput(){
 Pa_Terminate();
 AudioBuffer *b = this->data.out;
 while (b != NULL){
     if (this->data.out->recordedSamples != NULL){
         delete this->data.out->recordedSamples;
     }
     b = this->data.out->next;
     delete this->data.out;
     this->data.out = b;
 }
}

/*
 * Must be called to initialise portaudio library befor performing anythings.
 * */
void AudioInput::init(){
    err = Pa_Initialize();
    if( err != paNoError ) throw UserException("Error cannot initialize audio generation!");
}

/*
 * Set the input to listen to this function is not complete and must take as parameter a device.
 * Currently use the default input device.
 * */
void AudioInput::setDevice(){
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
       throw UserException("Error: No input device.\n");
    }
    inputParameters.channelCount = NUM_CHANNELS;    /* stereo or mono input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
}

/*
 * Start listening into an audio stream start the listening thread controled by the callback function.
 */
void AudioInput::startListening(){
    Stream_status__ = paContinue;
    err = Pa_OpenStream(
                 &stream,
                 &inputParameters,
                 NULL,
                 SAMPLE_RATE,
                 FRAMES_PER_BUFFER,
                 paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                 recordCallback,
                 &data );
    if( err != paNoError ) throw UserException("Error open stream 1");
    err = Pa_StartStream( stream );
    if( err != paNoError ) throw UserException("Error open stream 2");
   printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);
}

/*
 * No comment
 */
void AudioInput::stopListening(){
    printf("\n=== Now stopping!! ===\n"); fflush(stdout);
    Stream_status__ = paComplete;
}

void AudioInput::closeStream(){
    if (stream != NULL){
        err = Pa_CloseStream( stream );
        if( err != paNoError ) printf("error");
        if( err != paNoError )
        {
            fprintf( stderr, "An error occured while using the portaudio stream\n" );
            fprintf( stderr, "Error number: %d\n", err );
            fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
            err = 1;          /* Always return 0 or 1, but no other return codes. */
        }
    }
}

/*
 * Here find out how to get a jack input device and send it to setDevice <----- for someone who as a jack input device!!!!!!!!!
 */
void AudioInput::getDevices(){
    const PaDeviceInfo *deviceInfo;
    const int numDevices = Pa_GetDeviceCount();

    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );//log error
        throw UserException("No device available! Try to connect a microphone.");
    }
    for(int i=0; i<numDevices; i++ ) //Get the input devices and print their names
    {
        deviceInfo = Pa_GetDeviceInfo( i );
        if (i == Pa_GetDefaultInputDevice()){
             printf( "Default input Device host api name : %s\n", deviceInfo->hostApi );
            #ifdef WIN32
                    {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
                        wchar_t wideName[MAX_PATH];
                        MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
                        wprintf( L"Default Name = %s\n", wideName );
                    }
            #else
                    printf( "Default Name    = %s\n", deviceInfo->name );
            #endif
        }
        else if (deviceInfo->maxInputChannels > 0){ //if it's an input device
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
            printf( "Device host api name : %s\n", hostInfo->name );
        }

        #ifdef WIN32
                {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
                    wchar_t wideName[MAX_PATH];
                    MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
                    wprintf( L"Name = %s\n", wideName );
                }
        #else
                printf( "Name    = %s\n", deviceInfo->name );
        #endif

    }
}

AudioBuffer *AudioInput::allocBuffer(){
    AudioBuffer *a = new AudioBuffer;
    if (a == NULL){
        UserException("No memory available");
        return NULL;
    }
    int   totalFrames,numBytes;

    a->frameIndex = 0;
    totalFrames = FFT_BUFFER_SIZE * 50;
    numBytes = totalFrames * sizeof(SAMPLE);
    a->recordedSamples = new SAMPLE[numBytes];
    if (a->recordedSamples == NULL) return NULL;
    for(int i=0; i<totalFrames; i++ ) a->recordedSamples[i] = 0;
    a->next = NULL;
    return a;
}

//Move first buffer to the end
void AudioInput::moveBuffer(){
    AudioBuffer *toMove = this->data.out;
    toMove->frameIndex = 0;
    for(int i=0; i<numSamples; i++ ) toMove->recordedSamples[i] = 0;
    AudioBuffer *end = this->data.in;
    while (end->next != NULL){
        end = end->next;
    }
    this->data.out = toMove->next;
    toMove->next = NULL;
    end->next = toMove;
}

//Add a new buffer to the end of the list.
void AudioInput::addBuffer(){
    AudioBuffer *n = allocBuffer();
    AudioBuffer *end = this->data.in;
    while (end->next != NULL){
        end = end->next;
    }
    end->next = n;
}
