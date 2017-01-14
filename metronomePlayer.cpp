#include "MetronomePlayer.h"
#include "ui_metronomePlayer.h"

MetronomePlayer::MetronomePlayer(QWidget *metronomeView)
{
    _ui = new Ui::MetronomePlayer;
    _ui->setupUi(metronomeView);
    _widgetTranslate = metronomeView;
    _ui->bpm->setValidator( new QIntValidator);
    _ui->ButtonExec->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    _ui->bpm->setText("120");
    _state = false;
    connect(_ui->ButtonExec, SIGNAL(clicked(bool)), SLOT(slotButtonClicked()));
    connect(_ui->bpm,SIGNAL(textChanged(QString)), SLOT(slotBmpChanged(QString)));
    connect(_ui->SliderBpm, SIGNAL(valueChanged(int)), SLOT(slotSetBmpValue(int)));
    connect(_ui->btnAddBPM, SIGNAL(clicked(bool)), SLOT(slotBtnAddBPMClicked()));
}

MetronomePlayer::~MetronomePlayer()
{
    delete _ui;
}

void    MetronomePlayer::slotButtonClicked()
{
    if (_state)
    {
        _ui->ButtonExec->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        _state = false;
        emit signalStopClicked();
    }
    else
    {
        _ui->ButtonExec->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
        _state = true;
        bool ok;
        emit signalStartClicked(_ui->bpm->text().toInt(&ok, 10));
    }
}

void    MetronomePlayer::slotBmpChanged(QString value)
{
    if (value != "")
    {
        _ui->ButtonExec->setEnabled(true);
    }
    else
    {
        _ui->ButtonExec->setEnabled(false);
    }
}

void    MetronomePlayer::slotSetBmpValue(int value)
{
    _ui->bpm->setText(QString::number(value));
}

void    MetronomePlayer::slotBtnAddBPMClicked()
{
    emit signalButtonAddClicked(_ui->bpm->text());
}

void    MetronomePlayer::setTranslation()
{
    _ui->retranslateUi(_widgetTranslate);
}
