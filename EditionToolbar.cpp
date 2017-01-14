#include "EditionToolbar.h"
#include "ui_editionToolbar.h"
/*
 * connect tous les boutons à leurs slots
 */
EditionToolbar::EditionToolbar(QWidget *widget, IScoreEditor *scoreEditor,
                               QObject* parent) : QObject(parent)
{
    _ui = new Ui::EditionToolbar;
    _ui->setupUi(widget);
    _widgetTranslate = widget;
    _scoreEditor = scoreEditor;
    _previewedEvent = &_note;
    _w = new QGuidoWidget(_ui->previewnote);
    this->initSignalsNotes();
    this->initSignalsAccidental();
    this->initSignalsOctave();
    this->initSignalsDuration();
    this->initSignalsMesure();
    this->initSignalsPartition();
    connect(_ui->buttonAjouterNote, SIGNAL(clicked(bool)), SLOT(slotButtonAjouterNoteClicked()));
    previewMusicElement();
}

EditionToolbar::~EditionToolbar()
{
    delete _ui;
}

/*
 *  connecte tous les boutons de notes à leurs slots
*/
void    EditionToolbar::initSignalsNotes()
{
    connect(_ui->buttonDo, SIGNAL(clicked(bool)), SLOT(slotButtonDoClicked()));
    connect(_ui->buttonRe, SIGNAL(clicked(bool)), SLOT(slotButtonReClicked()));
    connect(_ui->buttonMi, SIGNAL(clicked(bool)), SLOT(slotButtonMiClicked()));
    connect(_ui->buttonFa, SIGNAL(clicked(bool)), SLOT(slotButtonFaClicked()));
    connect(_ui->buttonSol, SIGNAL(clicked(bool)), SLOT(slotButtonSolClicked()));
    connect(_ui->buttonLa, SIGNAL(clicked(bool)), SLOT(slotButtonLaClicked()));
    connect(_ui->buttonSi, SIGNAL(clicked(bool)), SLOT(slotButtonSiClicked()));
    connect(_ui->buttonPause, SIGNAL(clicked(bool)), SLOT(slotButtonPauseClicked()));
}

/*
 *  connecte tous les boutons accidental à leurs slots
*/
void    EditionToolbar::initSignalsAccidental()
{
    connect(_ui->buttonFlat, SIGNAL(clicked(bool)), SLOT(slotButtonFlatClicked()));
    connect(_ui->buttonDoubleFlat, SIGNAL(clicked(bool)), SLOT(slotButtonDoubleFlatClicked()));
    connect(_ui->buttonSharp, SIGNAL(clicked(bool)), SLOT(slotButtonSharpClicked()));
    connect(_ui->buttonDoubleSharp, SIGNAL(clicked(bool)), SLOT(slotButtonDoubleSharpClicked()));
    connect(_ui->buttonAucun, SIGNAL(clicked(bool)), SLOT(slotButtonAucunClicked()));
}

/*
 *  connecte tous les boutons octave à leurs slots
*/
void    EditionToolbar::initSignalsOctave()
{
    connect(_ui->buttonOctavePlus, SIGNAL(clicked(bool)), SLOT(slotButtonOctavePlusClicked()));
    connect(_ui->buttonOctaveMinus, SIGNAL(clicked(bool)), SLOT(slotButtonOctaveMinusClicked()));
}

/*
 *  connecte tous les boutons duration à leurs slots
*/
void    EditionToolbar::initSignalsDuration()
{
    connect(_ui->buttonQuarter, SIGNAL(clicked(bool)), SLOT(slotButtonQuarterClicked()));
    connect(_ui->buttonHalf, SIGNAL(clicked(bool)), SLOT(slotButtonHalfClicked()));
    connect(_ui->buttonWhole, SIGNAL(clicked(bool)), SLOT(slotButtonWholeClicked()));
    connect(_ui->buttonLonga, SIGNAL(clicked(bool)), SLOT(slotButtonLongaClicked()));
    connect(_ui->button8th, SIGNAL(clicked(bool)), SLOT(slotButton8thClicked()));
    connect(_ui->button16th, SIGNAL(clicked(bool)), SLOT(slotButton16thClicked()));
    connect(_ui->button32th, SIGNAL(clicked(bool)), SLOT(slotButton32thClicked()));
    connect(_ui->buttonDot, SIGNAL(clicked(bool)), SLOT(slotButtonDotClicked()));
    connect(_ui->buttonDoubleDot, SIGNAL(clicked(bool)), SLOT(slotButtonDoubleDotClicked()));
}

void    EditionToolbar::initSignalsMesure()
{
    connect(_ui->ClefSol, SIGNAL(clicked(bool)), SLOT(slotButtonKeySolClicked()));
    connect(_ui->ClefFa, SIGNAL(clicked(bool)), SLOT(slotButtonKeyFaClicked()));
    connect(_ui->ClefUt, SIGNAL(clicked(bool)), SLOT(slotButtonKeyUtClicked()));
    connect(_ui->RythmeSlider, SIGNAL(valueChanged(int)), SLOT(slotRythmeChanged(int)));
    connect(_ui->RythmeSlider, SIGNAL(sliderPressed()), SLOT(slotSliderRythmePressed()));
    connect(_ui->BattementComboBox, SIGNAL(highlighted(QString)), SLOT(slotBattementChanged(QString)));
    connect(_ui->buttonDieseDo, SIGNAL(clicked(bool)), SLOT(slotButtonDieseDoClicked()));
    connect(_ui->buttonDieseSol, SIGNAL(clicked(bool)), SLOT(slotButtonDieseSolClicked()));
    connect(_ui->buttonDieseRe, SIGNAL(clicked(bool)), SLOT(slotButtonDieseReClicked()));
    connect(_ui->buttonDieseLa, SIGNAL(clicked(bool)), SLOT(slotButtonDieseLaClicked()));
    connect(_ui->buttonDieseMi, SIGNAL(clicked(bool)), SLOT(slotButtonDieseMiClicked()));
    connect(_ui->buttonDieseSi, SIGNAL(clicked(bool)), SLOT(slotButtonDieseSiClicked()));
    connect(_ui->buttonDieseFa, SIGNAL(clicked(bool)), SLOT(slotButtonDieseFaClicked()));
    connect(_ui->buttonDieseDoDiese, SIGNAL(clicked(bool)), SLOT(slotButtonDieseDoDieseClicked()));
    connect(_ui->buttonBemoleRe, SIGNAL(clicked(bool)), SLOT(slotButtonBemoleReClicked()));
    connect(_ui->buttonBemoleSol, SIGNAL(clicked(bool)), SLOT(slotButtonBemoleSolClicked()));
    connect(_ui->buttonBemoleDo, SIGNAL(clicked(bool)), SLOT(slotButtonBemoleDoClicked()));
    connect(_ui->buttonBemoleFa, SIGNAL(clicked(bool)), SLOT(slotButtonBemoleFaClicked()));
    connect(_ui->buttonBemoleSi, SIGNAL(clicked(bool)), SLOT(slotButtonBemoleSiClicked()));
    connect(_ui->buttonBemoleMi, SIGNAL(clicked(bool)), SLOT(slotButtonBemoleMiClicked()));
    connect(_ui->buttonBemoleLa, SIGNAL(clicked(bool)), SLOT(slotButtonBemoleLaClicked()));
    connect(_ui->buttonAddMesure, SIGNAL(clicked(bool)), SLOT(slotButtonAddMesureClicked()));
    connect(_ui->buttonDeleteMesure, SIGNAL(clicked(bool)), SLOT(slotButtonDeleteMesureClicked()));
}

void    EditionToolbar::initSignalsPartition()
{
    connect(_ui->titre, SIGNAL(textChanged(QString)), SLOT(slotTitreChanged(QString)));
    connect(_ui->compositeur, SIGNAL(textChanged(QString)), SLOT(slotCompositeurChanged(QString)));
}

/*
 *  Affiche une preview de la note
 *  On créer une nouvelle QGuidoWidget
 *  On ouvre la guidofactory
 *  On ouvre une nouvelle partition
 *  On ouvre une nouvelle portée
 *  On y ajoute le tag clé
 *  Si on a une note à affiché on créer un event qui correpond à la note et on set les differents attribut de la note
 *  On ferme la porté ainsi que la partition qui retourne un ARHandler qui est utilisé par QGuidoWidget pour afficher la partition
*/
void    EditionToolbar::previewMusicElement()
{
    _w->resize(150, 150);

    GuidoPageFormat format = _w->guidoPageFormat();
    format.marginright = 350;
    format.marginleft = 200;
    format.width = 150;
    format.height = 150;
    _w->setGuidoPageFormat(format);

    GuidoLayoutSettings layout = _w->guidoLayoutSettings();
    layout.optimalPageFill = 0;
    _w->setGuidoLayoutSettings(layout);

    ARFactoryHandler factory;
    GuidoFactoryOpen(&factory);
    GuidoFactoryOpenMusic(factory);
    GuidoFactoryOpenVoice(factory);
    _previewedEvent->abstract(factory);
    GuidoFactoryCloseVoice(factory);
    ARHandler ar = GuidoFactoryCloseMusic(factory);
    GuidoFactoryClose(factory);
    _w->setARHandler(ar);
    _w->show();
}

/*
 *  Toutes les actions rattachées aux boutons de note
 *  A chaque note cliqué on set la _currentNote de l'editeur avec celle cliqué
 *  On affiche la preview de la note
*/

void    EditionToolbar::slotButtonDoClicked()
{
    _previewedEvent = &_note;
    _note.setNoteName(Note::NoteName::C);
    previewMusicElement();
}

void    EditionToolbar::slotButtonReClicked()
{
    _previewedEvent = &_note;
    _note.setNoteName(Note::NoteName::D);
    previewMusicElement();

}

void    EditionToolbar::slotButtonMiClicked()
{
    _previewedEvent = &_note;
    _note.setNoteName(Note::NoteName::E);
    previewMusicElement();

}

void    EditionToolbar::slotButtonFaClicked()
{
    _previewedEvent = &_note;
    _note.setNoteName(Note::NoteName::F);
    previewMusicElement();

}

void    EditionToolbar::slotButtonSolClicked()
{
    _previewedEvent = &_note;
    _note.setNoteName(Note::NoteName::G);
    previewMusicElement();

}

void    EditionToolbar::slotButtonLaClicked()
{
    _previewedEvent = &_note;
    _note.setNoteName(Note::NoteName::A);
    previewMusicElement();

}

void    EditionToolbar::slotButtonSiClicked()
{
    _previewedEvent = &_note;
    _note.setNoteName(Note::NoteName::B);
    previewMusicElement();
}

void    EditionToolbar::slotButtonPauseClicked()
{
    _previewedEvent = &_rest;
    previewMusicElement();
}

/*
 *  Fonctions qui set l'accidental de la note
*/
void    EditionToolbar::slotButtonFlatClicked()
{
    _note.setAccidental(-1);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDoubleFlatClicked()
{
    _note.setAccidental(-2);
    previewMusicElement();
}

void    EditionToolbar::slotButtonSharpClicked()
{
    _note.setAccidental(1);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDoubleSharpClicked()
{
    _note.setAccidental(2);
    previewMusicElement();
}

void    EditionToolbar::slotButtonAucunClicked()
{
    _note.setAccidental(0);
    previewMusicElement();
}

void    EditionToolbar::slotButtonOctavePlusClicked()
{
    if (_note.getOctave() < 3)
    {
        _note.setOctave(_note.getOctave() + 1);
        previewMusicElement();
    }
}

void    EditionToolbar::slotButtonOctaveMinusClicked()
{
    if (_note.getOctave() > -1)
    {
        _note.setOctave(_note.getOctave() - 1);
        previewMusicElement();
    }
}

/*
 *  Fonctions  qui set la durée de la note
*/
void    EditionToolbar::slotButtonQuarterClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(4);
    _note.setDots(0);
    previewMusicElement();

}

void    EditionToolbar::slotButtonHalfClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(2);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButtonWholeClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(1);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButtonLongaClicked()
{
    _note.setNumDuration(2);
    _note.setDenomDuration(1);
    previewMusicElement();
}

void    EditionToolbar::slotButton8thClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(8);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButton16thClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(16);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButton32thClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(32);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDotClicked()
{
    if ((_note.getNumDuration() != 2) && (_note.getDenomDuration() >= 1))
    {
        _note.setDots(1);
        previewMusicElement();
    }
}

void    EditionToolbar::slotButtonDoubleDotClicked()
{
    if ((_note.getNumDuration() != 2) && (_note.getDenomDuration() >= 1))
    {
        _note.setDots(2);
        previewMusicElement();
    }
}

void    EditionToolbar::slotButtonHalfTripletClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(3);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButtonQuarterTripletClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(6);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButton8thTripletClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(12);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButtonQuintupletClicked()
{
    _note.setNumDuration(1);
    _note.setDenomDuration(5);
    _note.setDots(0);
    previewMusicElement();
}

void    EditionToolbar::slotButtonKeySolClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("clef");
    _tag.removeAllParams();
    _tag.addParam("g");
    previewMusicElement();
}

void    EditionToolbar::slotButtonKeyFaClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("clef");
    _tag.removeAllParams();
    _tag.addParam("fa");
    previewMusicElement();
}

void    EditionToolbar::slotButtonKeyUtClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("clef");
    _tag.removeAllParams();
    _tag.addParam("alto");
    previewMusicElement();
}

void    EditionToolbar::slotRythmeChanged(int value)
{
    _previewedEvent = &_tag;
    _tag.setName("meter");
    _tag.removeAllParams();
    _tag.addParam(QString::number(value) + "/" + _ui->BattementComboBox->itemText(_ui->BattementComboBox->currentIndex()));
    previewMusicElement();
}

void    EditionToolbar::slotSliderRythmePressed()
{
    _previewedEvent = &_tag;
    _tag.setName("meter");
    _tag.removeAllParams();
    _tag.addParam(QString::number(_ui->RythmeSlider->value()) + "/" + _ui->BattementComboBox->itemText(_ui->BattementComboBox->currentIndex()));
    previewMusicElement();
}

void    EditionToolbar::slotBattementChanged(QString value)
{
    _previewedEvent = &_tag;
    _tag.setName("meter");
    _tag.removeAllParams();
    _tag.addParam(QString::number(_ui->RythmeSlider->value()) + "/" + value);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseDoClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("0", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseSolClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("1", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseReClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("2", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseLaClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("3", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseMiClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("4", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseSiClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("5", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseFaClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("6", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonDieseDoDieseClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("7", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonBemoleReClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-1", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonBemoleSolClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-2", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonBemoleDoClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-3", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonBemoleFaClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-4", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonBemoleSiClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-5", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonBemoleMiClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-6", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotButtonBemoleLaClicked()
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-7", Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotKeyDieseChanged(int value)
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam(QString::number(value), Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotKeyBemolChanged(int value)
{
    _previewedEvent = &_tag;
    _tag.setName("key");
    _tag.removeAllParams();
    _tag.addParam("-" + QString::number(value), Tag::ParameterType::INTEGER);
    previewMusicElement();
}

void    EditionToolbar::slotTempoChanged(QString value)
{
    _previewedEvent = &_tag;
    _tag.setName("tempo");
    _tag.removeAllParams();
    _tag.addParam("bpm=" + value);
    previewMusicElement();
}

void    EditionToolbar::slotButtonAddMesureClicked()
{
    _scoreEditor->addMeasure();
}

bool    EditionToolbar::slotButtonDeleteMesureClicked()
{
    QPair<int, int> pos = _scoreEditor->getCursorIndex();
    QMessageBox::StandardButton reply;

    if (pos.first > 0)
    {
        reply = QMessageBox::question(0, "Supprimer la portée", "Supprimer la portée où se trouve le curseur d'édition ?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
        {
            _scoreEditor->deleteMeasure();
        }
    }
    else
    {
        QMessageBox msgBox;

        msgBox.setText("Impossible de supprimer la 1ère portée");
        msgBox.exec();
    }
    return reply != QMessageBox::Cancel;
}

void    EditionToolbar::slotTitreChanged(QString value)
{
    _scoreEditor->setTitle(value);
}

void    EditionToolbar::slotCompositeurChanged(QString value)
{
    _scoreEditor->setComposer(value);
}

/*
 * Appelle l'éditeur de partition en lui passant une copy de la note à ajouter.
 */

void    EditionToolbar::slotButtonAjouterNoteClicked()
{
    emit signalAddMusicElement(_previewedEvent->duplicate());
}

void    EditionToolbar::initTitleComposer(QString title, QString composer)
{
    _ui->titre->setText(title);
    _ui->compositeur->setText(composer);
}

#include    <QDebug>

void    EditionToolbar::setTranslation()
{
    _ui->retranslateUi(_widgetTranslate);
}
