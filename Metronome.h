#ifndef METRONOME_H
#define METRONOME_H

#include    <QThread>
#include    <QSound>

class Metronome : public QThread
{
    Q_OBJECT

public:
    explicit Metronome();
    virtual ~Metronome();

    void    run();
    void    stop();

    void    setBmp(float bmp);

private:
    QSound* _sound;
    bool    _stop;
    float   _bmp;
};

#endif // METRONOME_H
