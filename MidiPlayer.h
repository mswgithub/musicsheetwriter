#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <QWidget>

namespace Ui {
class MidiPlayer;
}

class MidiPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MidiPlayer(QWidget *playerView);
    ~MidiPlayer();

    void    setStart(bool start);
    void    setTranslation();

private slots:
    void    slotButtonStartClicked();
    void    slotButtonPauseClicked();
    void    slotButtonStopClicked();
    void    slotButtonClicked();

signals:
    void    signalStartClicked();
    void    signalPauseClicked();
    void    signalResumeClicked();
    void    signalStopClicked();

private:
    Ui::MidiPlayer* _ui;
    bool            _state;
    bool            _start;
    QWidget*        _widgetTranslate;

    void    initSignalsPlayer();

};

#endif // MIDIPLAYER_H
