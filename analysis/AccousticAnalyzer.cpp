#include "AccousticAnalyzer.h"
#include <QDebug>
#include <float.h>

AccousticAnalyzer::AccousticAnalyzer(IScoreEditor* scoreEditor) :
    ASoundAnalyzer(scoreEditor)
{
    connect(this, SIGNAL(finished()), SLOT(freeData()));
    this->_file = fopen ("recordedc.raw", "wb");
    _note.frequency = -1;
    int nfft = (int)pow(2.0, ceil(log((double)FFT_BUFFER_SIZE)/log(2.0)));
    _data = new SAMPLE[2*nfft];
//    _data = new SAMPLE[FFT_BUFFER_SIZE];
}

AccousticAnalyzer::~AccousticAnalyzer()
{
    fclose(this->_file);
}

void AccousticAnalyzer::init(){
    _frameIndex = 0;
    _audioInput = new AudioInput();
}

void    AccousticAnalyzer::run()
{
    // Start Port audio
    qDebug() << "Initialaizing audio input";
    try {
        this->_audioInput->init();
        qDebug() << "Setting the device";
        this->_audioInput->setDevice();
        qDebug() << "Start listening";
        this->_audioInput->startListening();
        qDebug() << "listening started";
    } catch (UserException &e){
        qDebug() << e.what();
        this->_audioInput->stopListening();
        qDebug() << "Closing the stream";
        this->_audioInput->closeStream();
        _state = STOPPED;
        qDebug() << "Closing the threads 1";
        return;

    } catch (exception &e){
        qDebug() << e.what();
        this->_audioInput->stopListening();
        qDebug() << "Closing the stream";
        this->_audioInput->closeStream();
        _state = STOPPED;
        qDebug() << "Closing the threads 2";
        return;
    }

    unsigned int buf_len;
    int diff;



    while (this->_audioInput->data.out != NULL){
        //Alloc more buffer
        if (this->_audioInput->data.in->next == NULL){
            this->_audioInput->allocBuffer();
            qDebug() << "Alloc new link";
        }
        diff = this->_audioInput->data.out->frameIndex - _frameIndex;
            if (diff >= FFT_BUFFER_SIZE){
                 buf_len = FFT_BUFFER_SIZE;
                 //ON different buffer
                 if (this->_audioInput->data.out != this->_audioInput->data.in){
                     analyse(&(this->_audioInput->data.out->recordedSamples[_frameIndex]), buf_len);
                     _frameIndex += buf_len;
                     if (_frameIndex == _audioInput->data.maxFrameIndex ){ //end of the buffer
                         _frameIndex = 0;
                         _audioInput->moveBuffer();
                     }
                 }
                 else//On the same buffer
                 {
                     analyse(&(_audioInput->data.out->recordedSamples[_frameIndex]), buf_len);
                     _frameIndex += buf_len;
                 }
            }
            else {
                if (_audioInput->data.out == _audioInput->data.in){
                    if (_state == STOPPED)
                        return;
                    else
                        Pa_Sleep(100);
                }
                else {
                    qDebug() << "Will probably never append but who know";
                    _frameIndex = 0;
                    _audioInput->moveBuffer();
                }
        }
    }
}

void    AccousticAnalyzer::start()
{
    // Start the thread by calling the start method contained by
    // the QThread class. Since AccousticAnalyser inherite from
    // QThread, it will actually execute the thread instance within
    // AccousticAnalyzer.
    this->_state = STARTED;
    QThread::start();
    _firstRest = true;
}

void    AccousticAnalyzer::stop()
{
    this->_state = STOPPED;
    this->_audioInput->stopListening();
    this->_audioInput->closeStream();
}

void AccousticAnalyzer::free(){
     delete this->_audioInput;
}

#include <cstdio>
void AccousticAnalyzer::analyse(SAMPLE *buffer, const unsigned int len){
    //fwrite( bb, sizeof(SAMPLE), len, this->file );
    //Copying the buffer into the data
    hanning(buffer, len / 2);
    unsigned int i = 0;
    for (i = 0; i < len / 2 - 1;i++){
        _data[i*2] = buffer[i];
        _data[i*2+1] = 0;
    }
    for(;i<FFT_BUFFER_SIZE;i++){
        _data[2*i] = 0.0;
        _data[2*i+1] = 0.0;
    }
    fft(_data, len);// perform FFT on the read sample

    FILE *file = fopen("../software/debug_fft", "w");
    int _i = 0;
    float   _re;
    float   _im;
    string          s;
    while (_i < FFT_BUFFER_SIZE)
    {
        stringstream    ss;

        _re = _data[_i];
        _im = _data[_i+1];
        ss << sqrt(_re*_re+_im*_im);
        ss >> s;
        fwrite(s.c_str(), 1, s.length(), file);
        fwrite("\n", 1, 1, file);
        _i += 2;
    }
    fclose(file);


    //}
    float magnitude;
    float frequency = getFundamentalFrequency(_data, len, magnitude);// get fundamental frequency (fo)

    // Store the first note
    if (_note.frequency == -1)
    {
        _note.frequency = frequency;
        _note.duration = 0;
    }
    //If the frequency has changed, send the note that just ended to the score editor
    if (frequency != _note.frequency /*|| magnitude > _note.magnitude*/)
    {
        // Add to the score the corresponding musicElement
        if (_note.frequency != 0.f)
        {
            _firstRest = false;
            try {
                // Find out the note by comparing with a frequency-note table
                Note note = _frequencyTable.getNote(_note.frequency);

                // Find out the duration and the length-bpm rate
                this->storeDuration(note, _note.duration);

                // Send the note to the score editor
                if (note.getNumDuration() != 0) {
                    getEditor()->addMusicElement(note.duplicate());
                }
            } catch (std::range_error& e) {
            }
        }
        else if (!_firstRest)
        {
            Rest rest;

            // Find out the duration and the length-bpm rate
            this->storeDuration(rest, _note.duration);

            // Send the rest to the score editor
            if (rest.getNumDuration() != 0) {
                getEditor()->addMusicElement(rest.duplicate());
            }
        }
        _note.duration = 0;
    }
    _note.duration += 1000.0 / (SAMPLE_RATE / FFT_BUFFER_SIZE);
    _note.magnitude = magnitude;
    _note.frequency = frequency;
}

void    AccousticAnalyzer::fft(SAMPLE* data, const unsigned int nn) const
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    SAMPLE tempr, tempi;

    // reverse-binary reindexing
    n = nn<<1;
    j=1;
    for (i=1; i<n; i+=2) {
        if (j>i) {
            SAMPLE tmp;
            // swap data[j-1] and data[i-1]
            tmp = data[j-1];
            data[j-1] = data[i-1];
            data[i-1] = tmp;

            // swap data[j] and data[i])
            tmp = data[j];
            data[j] = data[i];
            data[i] = tmp;
        }
        m = nn;
        while (m>=2 && j>m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };

    // here begins the Danielson-Lanczos section
    mmax=2;
    while (n>mmax) {
        istep = mmax<<1;
        theta = -(2*M_PI/mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m=1; m < mmax; m += 2) {
            for (i=m; i <= n; i += istep) {
                j=i+mmax;
                tempr = wr*data[j-1] - wi*data[j];
                tempi = wr * data[j] + wi*data[j-1];

                data[j-1] = data[i-1] - tempr;
                data[j] = data[i] - tempi;
                data[i-1] += tempr;
                data[i] += tempi;
            }
            wtemp=wr;
            wr += wr*wpr - wi*wpi;
            wi += wi*wpr + wtemp*wpi;
        }
        mmax=istep;
    }
}

double AccousticAnalyzer::getFundamentalFrequency(const SAMPLE *fft, const unsigned int len, float &m) const{
    unsigned int firstFreqOverThresholdIndex = 0;
    unsigned int lastFreqOverThresholdIndex = 0;
    unsigned int biggestPeakIndex = 0;

    firstFreqOverThresholdIndex = getFirstFreqOverThreshold(fft, 0, len);
    lastFreqOverThresholdIndex = getLastFreqOverThreshold(fft, firstFreqOverThresholdIndex, len);
    biggestPeakIndex = getBiggestPeakIndex(fft, firstFreqOverThresholdIndex, lastFreqOverThresholdIndex);

    bool hasHarmonic = hasEnoughHarmonic(fft, biggestPeakIndex, len);

    if (hasHarmonic)
    {
        int freq = getFrequence(biggestPeakIndex, len);

        m = getMagnitude(fft, biggestPeakIndex * 2);
        return freq;
    }
    return 0.0;
}

// peakIndex is already divided by 2
int AccousticAnalyzer::getFrequence(unsigned int peakIndex, const unsigned int len) const
{
    return (peakIndex * SAMPLE_RATE / (len * 4));
}

float AccousticAnalyzer::getMagnitude(const SAMPLE* fft, int index) const
{
    float magnitude = 0.f;
    float re = 0.f;
    float im = 0.f;

    re = fft[index];
    im = fft[index + 1];
    magnitude = sqrtf(re*re + im*im);

    return magnitude;
}

unsigned int AccousticAnalyzer::getFirstFreqOverThreshold(const SAMPLE* fft, int startIndex, const unsigned int len) const
{
    unsigned int i = startIndex;
    float magnitude = 0.f;

    while ((i + 1) < len)
    {
        magnitude = getMagnitude(fft, i);
        if (magnitude >= AMPLITUDE_FF_THRESHOLD)
        {
            return i/2;
        }
        i += 2;
    }
    return 0;
}

unsigned int AccousticAnalyzer::getLastFreqOverThreshold(const SAMPLE* fft, int startIndex, const unsigned int len) const
{
    unsigned int i = startIndex * 2;
    float magnitude = 0.f;

    while ((i + 1) < len)
    {
        magnitude = getMagnitude(fft, i);
        if (magnitude <= AMPLITUDE_FF_THRESHOLD)
        {
            return (i > 0) ? ((i - 1)/2) : (0);
        }
        i += 2;
    }
    return 0;
}

unsigned int AccousticAnalyzer::getBiggestPeakIndex(const SAMPLE* fft, int startIndex, int lastIndex) const
{
    float magnitude = 0.f;
    float calc_magnitude = 0.f;
    unsigned int index = 0;
    unsigned int i = startIndex * 2;
    unsigned int j = lastIndex * 2;

    while ((i + 1) < j)
    {
        calc_magnitude = getMagnitude(fft, i);
        if (calc_magnitude >= magnitude)
        {
            magnitude = calc_magnitude;
            index = i / 2;
        }
        i += 2;
    }
    return index;
}

bool AccousticAnalyzer::hasEnoughHarmonic(const SAMPLE* fft, const unsigned int peakIndex, const unsigned int len) const
{
    unsigned int firstHarmonic = peakIndex * 4;
    unsigned int secondHarmonic = peakIndex * 8;
    float firstMagnitude = 0.f;
    float secondMagnitude = 0.f;

    if ((secondHarmonic + 1) < len)
    {
        firstMagnitude = getMagnitude(fft, firstHarmonic);
        secondMagnitude = getMagnitude(fft, secondHarmonic);
    }

    if ((firstMagnitude >= AMPLITUDE_HAR_THRESHOLD) && (secondMagnitude >= AMPLITUDE_HAR_THRESHOLD))
    {
        return true;
    }
    return false;
}

void AccousticAnalyzer::hanning(SAMPLE *buffer, const unsigned int len){
    for (unsigned int i=0;i<len;i++){
        buffer[i] *= (1-cos(2*i*M_PI/(len-1)))/2;
    }
}

void    AccousticAnalyzer::freeData(){

}
