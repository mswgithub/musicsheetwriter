#ifndef EDITIONMANAGER_H
#define EDITIONMANAGER_H

#include    "AEditionManager.h"
#include    "ScoreViewerWidget.h"
#include    "EditionToolbar.h"
#include    "ScoreEditor.h"
#include    "analysis/SoundAnalyzerManager.h"
#include    "MultipageToolbar.h"
#include    <QToolButton>
#include    <QScrollArea>
#include    <QObject>


class EditionManager : public AEditionManager
{
    Q_OBJECT
public:
    explicit EditionManager(QWidget *scoreViewerWidget, QWidget* editionToolbar,
                            QWidget* generatorToolbar,  QToolButton* btnMultipageLeft, QToolButton* btnMultipageRight, QObject* parent = 0);
    virtual ~EditionManager();

    void    updateScore(Score* score);
    void    keyReleaseEventHandler(QKeyEvent* event);
    void    setBPMFromMetronome(QString bpm);
    void    setTranslation();

private:
    ScoreViewerWidget*      _scoreViewer;
    EditionToolbar*         _editionToolbar;
    ScoreEditor*            _scoreEditor;
    SoundAnalyzerManager*   _soundAnalyzerManager;
    MultipageToolBar*       _multipageToolbar;

    // Widgets
    QWidget*    _uiScoreViewerWidget;
    QWidget*    _uiEditionToolbar;
    QWidget*    _uiGeneratorToolbar;

public slots:
    void    slotRefreshScore();
    void    slotAddMusicElement(AMusicElement* aMusic);
    void    slotCurrentPageScoreViewer(int currentPage);
    void    slotSetMultipageMaxPages(int maxpage);

signals:
    void    signalLecteurPartitionScore(Score *score);
    void    signalSetTranslation();
};

#endif // EDITIONMANAGER_H
