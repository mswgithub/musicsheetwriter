#include "SoundAnalyzerManager.h"

SoundAnalyzerManager::SoundAnalyzerManager(QWidget *widget, IScoreEditor* scoreEditor,
                                           QObject* parent) : QObject(parent)
{
    // Setup the UI
    _ui = new Ui::GeneratorToolbar;
    _ui->setupUi(widget);
    _widgetTranslate = widget;
    // Set default settings
    _state = IDLE;
    _mode = MIDI;
    _scoreEditor = scoreEditor;

    // Instantiate the different analyzer that will be used
    _accousticAnalizer = new AccousticAnalyzer(scoreEditor);

    // Link the button from the UI to the slots
    connect(_ui->startGenerator, SIGNAL(clicked(bool)), SLOT(startMidiGeneration()));
    connect(_ui->stopGenerator, SIGNAL(clicked(bool)), SLOT(stopMidiGeneration()));
//    connect(_ui->switch_2, SIGNAL(clicked(bool)),SLOT(switchMode()));

    setAdditionalWidgets();

    refreshUi();
}

SoundAnalyzerManager::~SoundAnalyzerManager()
{
    delete _ui;
    delete _accousticAnalizer;
}

void    SoundAnalyzerManager::startAccousticGeneration()
{
    if (!isStarted())
    {
        _state = STARTED;
        try {
            qDebug() << "Initialazing accoustic";
            this->_accousticAnalizer->init();
            qDebug() << "Starting accoustic generation";
            this->_accousticAnalizer->start();
            qDebug() << "Accoustic generation started";
        }
        catch (UserException &e){
            qDebug() << "Error occured stopping accoustic generation!";
            this->stopAccousticGeneration();
            QMessageBox msgBox;
            msgBox.setText(e.getMessage());
            msgBox.exec();
        } catch(exception &e){
            qDebug() << "Unknow error!";
            qDebug() << e.what();
        }
    }
    refreshUi();
}

#include "QDebug"

void    SoundAnalyzerManager::stopAccousticGeneration()
{
    if (isStarted())
    {
        this->_accousticAnalizer->stop();
        _state = IDLE;
        while (this->_accousticAnalizer->isFinished() == false){
            QThread::msleep(100);
        }
        qDebug() << "Accoustic analysed ended!";
        qDebug() << "Free the memory for the accoustic analyser!";
        this->_accousticAnalizer->free();
        qDebug() << "Memory is now free!";
    }
    qDebug() << "Refreshing the UI!";
    refreshUi();
}


void    SoundAnalyzerManager::startMidiGeneration()
{
    try {
        _state = STARTED;
        this->_midiAnalyzer = new MidiAnalyser(this->_scoreEditor);
        this->_midiAnalyzer->init();
        this->_midiAnalyzer->start();
        refreshUi();
    } catch (UserException &e){
        this->stopMidiGeneration();
        QMessageBox msgBox;
        msgBox.setText(e.getMessage());
        msgBox.exec();
    } catch(exception &e){
        qDebug() << e.what();
    }
}

void    SoundAnalyzerManager::stopMidiGeneration()
{
    _state = IDLE;
    qDebug() << "Stopping Midi generation!";
    this->_midiAnalyzer->stop();
    qDebug() << "Midi generation stopped!";
    delete this->_midiAnalyzer;
    qDebug() << "midiAnalyser memory has been free!";
    refreshUi();//Ca plante ici.
}

bool    SoundAnalyzerManager::isStarted()
{
    return _state == STARTED;
}

bool    SoundAnalyzerManager::isIdle()
{
    return _state == IDLE;

}

bool    SoundAnalyzerManager::isInMidi()
{
    return _mode == MIDI;

}

bool    SoundAnalyzerManager::isInAccoustic()
{
    return _mode == ACCOUSTIC;
}

void SoundAnalyzerManager::switchMode(){
    _ui->startGenerator->disconnect(SIGNAL(clicked(bool)));
    _ui->stopGenerator->disconnect(SIGNAL(clicked(bool)));
    if (isInAccoustic()){
        _mode = MIDI;
        _label->setText("MIDI");
        connect(_ui->startGenerator, SIGNAL(clicked(bool)), SLOT(startMidiGeneration()));
        connect(_ui->stopGenerator, SIGNAL(clicked(bool)), SLOT(stopMidiGeneration()));
    }
    else{
        _mode = ACCOUSTIC;
        _label->setText("Audio");
        connect(_ui->startGenerator, SIGNAL(clicked(bool)), SLOT(startAccousticGeneration()));
        connect(_ui->stopGenerator, SIGNAL(clicked(bool)), SLOT(stopAccousticGeneration()));
    }
}

void    SoundAnalyzerManager::refreshUi()
{
        if (isStarted())
        {
            _ui->startGenerator->setEnabled(false);
            _ui->stopGenerator->setEnabled(true);
        }
        else
        {
            _ui->startGenerator->setEnabled(true);
            _ui->stopGenerator->setEnabled(false);
        }
}

void    SoundAnalyzerManager::setAdditionalWidgets()
{
    _androidSwitch = new AndroidSwitch(QColor(76, 175, 80));
    _androidSwitch->setMaximumSize(QSize(100, 20));
    _ui->horizontalLayout->addWidget(_androidSwitch);
    connect(_androidSwitch, SIGNAL(toggled()), SLOT(switchMode()));

    _label = new QLabel();
    _label->setText("MIDI");
    _label->setObjectName("modeLabel");
    _ui->horizontalLayout->addWidget(_label);
}

void    SoundAnalyzerManager::setTranslation()
{
    _ui->retranslateUi(_widgetTranslate);
}
