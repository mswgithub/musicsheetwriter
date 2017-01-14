#ifndef EDITIONTOOLBAR_H
#define EDITIONTOOLBAR_H

#include    <QObject>
#include    <QWidget>
#include    <GUIDOFactory.h>
#include    <QGuidoWidget.h>
#include    <QMessageBox>
#include    <QGraphicsView>
#include    "IScoreEditor.h"
#include    "AEvent.h"
#include    "Note.h"
#include    "Rest.h"
#include    "Score.h"
#include    "GraphicItemsView.h"
#include    "QGraphicsScene"
#include    "ScoreViewerWidget.h"
#include    "GraphicItemsView.h"

namespace Ui {
class EditionToolbar;
}

class EditionToolbar : public QObject
{
    Q_OBJECT

public:
    explicit EditionToolbar(QWidget *widget, IScoreEditor *scoreEditor,
                            QObject* parent = 0);
    ~EditionToolbar();

    void    previewMusicElement();
    void    initTitleComposer(QString title, QString composer);
    void    setTranslation();

signals:
    void    signalAddMusicElement(AMusicElement* aMusic);

private slots:
    void slotButtonDoClicked();
    void slotButtonReClicked();
    void slotButtonMiClicked();
    void slotButtonFaClicked();
    void slotButtonSolClicked();
    void slotButtonLaClicked();
    void slotButtonSiClicked();
    void slotButtonPauseClicked();

    void slotButtonFlatClicked();
    void slotButtonDoubleFlatClicked();
    void slotButtonSharpClicked();
    void slotButtonDoubleSharpClicked();
    void slotButtonAucunClicked();

    void    slotButtonOctavePlusClicked();
    void    slotButtonOctaveMinusClicked();

    void slotButtonQuarterClicked();
    void slotButtonHalfClicked();
    void slotButtonWholeClicked();
    void slotButtonLongaClicked();
    void slotButton8thClicked();
    void slotButton16thClicked();
    void slotButton32thClicked();
    void slotButtonDotClicked();
    void slotButtonDoubleDotClicked();
    void slotButtonHalfTripletClicked();
    void slotButtonQuarterTripletClicked();
    void slotButton8thTripletClicked();
    void slotButtonQuintupletClicked();

    void slotButtonKeySolClicked();
    void slotButtonKeyFaClicked();
    void slotButtonKeyUtClicked();
    void slotRythmeChanged(int value);
    void slotSliderRythmePressed();
    void slotBattementChanged(QString value);
    void slotButtonDieseDoClicked();
    void slotButtonDieseReClicked();
    void slotButtonDieseMiClicked();
    void slotButtonDieseFaClicked();
    void slotButtonDieseSolClicked();
    void slotButtonDieseLaClicked();
    void slotButtonDieseSiClicked();
    void slotButtonDieseDoDieseClicked();
    void slotButtonBemoleReClicked();
    void slotButtonBemoleSolClicked();
    void slotButtonBemoleDoClicked();
    void slotButtonBemoleFaClicked();
    void slotButtonBemoleSiClicked();
    void slotButtonBemoleMiClicked();
    void slotButtonBemoleLaClicked();

    void slotKeyDieseChanged(int value);
    void slotKeyBemolChanged(int value);
    void slotButtonAddMesureClicked();
    bool slotButtonDeleteMesureClicked();
    void slotTempoChanged(QString value);

    void slotTitreChanged(QString value);
    void slotCompositeurChanged(QString value);

    void slotButtonAjouterNoteClicked();

private:
    void initSignalsNotes();
    void initSignalsAccidental();
    void initSignalsOctave();
    void initSignalsDuration();
    void initSignalsMesure();
    void initSignalsPartition();

    Ui::EditionToolbar* _ui;
    IScoreEditor*       _scoreEditor;
    AEvent*             _previewedEvent;
    Note                _note;
    Rest                _rest;
    Tag                 _tag;
    QGuidoWidget*       _w;
    QWidget*            _widgetTranslate;
};

#endif // EDITIONTOOLBAR_H
