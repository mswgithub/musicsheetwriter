#ifndef METRONOMEMANAGER_H
#define METRONOMEMANAGER_H

#include    <QObject>
#include    <QWidget>
#include    "metronomePlayer.h"
#include    "Metronome.h"

class MetronomeManager : public QObject
{
    Q_OBJECT

public:
    MetronomeManager(QWidget *metronomeView, QObject* parent = 0);
    virtual ~MetronomeManager();

    void    updateMetronome(bool state);
    void    setTranslation();

private:
    QWidget*            _metronomeview;
    MetronomePlayer*    _mPlayer;
    Metronome*          _metronome;

public slots:
    void    slotStartClicked(int bmp);
    void    slotStopClicked();
    void    slotButtonAddClicked(QString bmp);

signals:
    void    signalButtonAddClicked(QString bmp);
};

#endif // METRONOMEMANAGER_H
