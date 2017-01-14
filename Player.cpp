#include "Player.h"

Player::Player()
{
    _midiFile = new QMidiFile();
    _midiOut = new QMidiOut();
    _paused = false;
    _stop = false;
    _timeElapsed = 0;
}

Player::~Player()
{
    QFile file(_outfile);
    if (file.exists())
    {
        file.remove(_outfile);
    }
    delete _midiFile;
    delete _midiOut;
}

void    Player::run()
{
    QList<QMidiEvent*> events = _midiFile->events();
    int i = 0;
    _midiOut->connect("0");
    _t.start();
    while (i < events.count() && !_stop)
    {
        while(_paused)
        {
            msleep(100);
        }
        if (events[i]->type() != QMidiEvent::Meta)
        {
            qint64 event_time = _midiFile->timeFromTick(events[i]->tick()) * 1000;
            qint32 waitTime = event_time - (_t.elapsed() + _timeElapsed);
            if (waitTime > 0)
            {
                msleep(waitTime);
            }
            if (events[i]->type() == QMidiEvent::SysEx)
            {
                  // TODO: sysex
            }
            else
            {
                qint32 message = events[i]->message();
                _midiOut->sendMsg(message);
            }
        }
        i++;
    }
    _midiOut->disconnect();
   QFile::remove(_outfile);
    emit signalEnd();
}

void    Player::pause()
{
    _midiOut->stopAll();
    _paused = true;
    _timeElapsed += _t.elapsed();
    _t.invalidate();
}

void    Player::resume()
{
    _paused = false;
    _t.restart();
}

void    Player::stop()
{
    _stop = true;
    QFile::remove(_outfile);
}

void    Player::setMidiFile(Score *score)
{
    _midiFile = new QMidiFile();
    _outfile = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+ "/" + score->getTitle() + ".mid";
    ARFactoryHandler arFactory;
    GuidoFactoryOpen(&arFactory);
    ARHandler arh = score->abstract(arFactory);
    GuidoFactoryClose(arFactory);
    GuidoAR2MIDIFile(arh, _outfile.toStdString().c_str(), 0);
    _midiFile->load(_outfile);
}

void Player::setTimeElapsed(int timer)
{
    _timeElapsed = timer;
}

void    Player::setPause(bool state)
{
    _paused = state;
}

void    Player::setStop(bool state)
{
    _stop = state;
}
