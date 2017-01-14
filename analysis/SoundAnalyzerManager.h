#ifndef SOUNDANALIZERMANAGER_H
#define SOUNDANALIZERMANAGER_H

#include    <QObject>
#include    "../IScoreEditor.h"
#include    "AccousticAnalyzer.h"
#include    "ui_generatorToolbar.h"
#include "midi/MidiAnalyser.h"
#include <QMessageBox>
#include <exception>
#include    <QLabel>
#include "AndroidSwitch.h"

namespace Ui {
class GeneratorToolbar;
}

class SoundAnalyzerManager : public QObject
{
    Q_OBJECT

private:
    typedef enum
    {
        STARTED,
        IDLE
    } State;

    typedef enum
    {
        ACCOUSTIC,
        MIDI
    } Mode;
    IScoreEditor*   _scoreEditor;

public:
    explicit SoundAnalyzerManager(QWidget *widget, IScoreEditor* scoreEditor,
                                  QObject* parent = 0);
    virtual ~SoundAnalyzerManager();

    bool    isStarted();
    bool    isIdle();
    bool    isInMidi();
    bool    isInAccoustic();
    void    setTranslation();

public slots:
    void    startAccousticGeneration();
    void    stopAccousticGeneration();
    void    switchMode();
    void    startMidiGeneration();
    void    stopMidiGeneration();

private:
    void    refreshUi();
    void    setAdditionalWidgets();

    AccousticAnalyzer*          _accousticAnalizer;
    MidiAnalyser *              _midiAnalyzer;
    Ui::GeneratorToolbar*       _ui;
    State                       _state;
    Mode                        _mode;
    AndroidSwitch*              _androidSwitch;
    QLabel*                     _label;
    QWidget*                    _widgetTranslate;
};

#endif // SOUNDANALIZERMANAGER_H
