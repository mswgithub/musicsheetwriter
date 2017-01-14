#include "EditionManager.h"

/*
 * Création d'un gestionnaire d'édition de partition. Il instantie toutes
 * les variables nécessaires et créer les classes Widget en les associant
 * aux widgets de la vue.
 */
EditionManager::EditionManager(QWidget *scoreViewerWidget, QWidget* editionToolbar,
                               QWidget* generatorToolbar, QToolButton* btnMultipageLeft, QToolButton* btnMultipageRight, QObject* parent) : AEditionManager(parent)
{
    _scoreEditor = new ScoreEditor();
    _scoreViewer = new ScoreViewerWidget(scoreViewerWidget, this);
    _editionToolbar = new EditionToolbar(editionToolbar, _scoreEditor, this);
    _soundAnalyzerManager = new SoundAnalyzerManager(generatorToolbar, _scoreEditor, this);
    _multipageToolbar = new MultipageToolBar(btnMultipageLeft, btnMultipageRight);

    _uiScoreViewerWidget = scoreViewerWidget;
    _uiEditionToolbar = editionToolbar;
    _uiGeneratorToolbar = generatorToolbar;

    _uiScoreViewerWidget->hide();
    _uiEditionToolbar->hide();
    _uiGeneratorToolbar->hide();

    connect(_scoreEditor, SIGNAL(signalRefreshScore()), SLOT(slotRefreshScore()));
    connect(_editionToolbar, SIGNAL(signalAddMusicElement(AMusicElement*)), SLOT(slotAddMusicElement(AMusicElement*)));
    connect(_scoreViewer, SIGNAL(signalMaxPages(int)), SLOT(slotSetMultipageMaxPages(int)));
    connect(_multipageToolbar, SIGNAL(signalSetCurrentPage(int)), SLOT(slotCurrentPageScoreViewer(int)));
}

EditionManager::~EditionManager()
{
    delete  _editionToolbar;
    delete  _scoreViewer;
    delete  _scoreEditor;
}

void    EditionManager::updateScore(Score *score)
{
    setScore(score);
    _scoreEditor->setScore(score);
    if (score)
    {
        _uiScoreViewerWidget->show();
        _uiEditionToolbar->show();
        _editionToolbar->initTitleComposer(score->getTitle(), score->getComposer());
        _uiGeneratorToolbar->show();
    }
    else
    {
        _uiScoreViewerWidget->hide();
        _uiEditionToolbar->hide();
        _uiGeneratorToolbar->hide();
    }
}

#include <QDebug>

void    EditionManager::keyReleaseEventHandler(QKeyEvent* event)
{
    if (QApplication::focusWidget()->objectName() != "SliderBpm" && QApplication::focusWidget()->objectName() != "RythmeSlider"
            && QApplication::focusWidget()->objectName() != "titre" && QApplication::focusWidget()->objectName() != "compositeur")
    {
        _uiScoreViewerWidget->setFocus();
        if (event->key() == Qt::Key_Left)
        {
            _scoreEditor->moveCursorNoteOf(-1);
        }
        if (event->key() == Qt::Key_Right)
        {
            _scoreEditor->moveCursorNoteOf(1);
        }
        if (event->key() == Qt::Key_Up)
        {
            _scoreEditor->moveCursorVoiceOf(-1);
        }
        if (event->key() == Qt::Key_Down)
        {
            _scoreEditor->moveCursorVoiceOf(1);
        }
        if(event->key() == Qt::Key_Delete)
        {
            QPair<int,int> pos = _scoreEditor->getCursorIndex();
            _scoreEditor->deleteMusicElement(pos.first, pos.second);
        }
    }
}

void    EditionManager::slotRefreshScore()
{
    Score* score = getScore();
    if (score)
    {
        ARFactoryHandler arFactory;
        GuidoFactoryOpen(&arFactory);
        ARHandler scoreAR = score->abstract(arFactory);
        GuidoFactoryClose(arFactory);
        _scoreViewer->show(scoreAR);
         emit signalLecteurPartitionScore(score);
    }
}

void    EditionManager::slotAddMusicElement(AMusicElement* aMusic)
{
    QPair<int, int> pos = _scoreEditor->getCursorIndex();
    _scoreEditor->addMusicElement(aMusic, pos.first, pos.second);
}

void    EditionManager::slotCurrentPageScoreViewer(int currentPage)
{
    _scoreViewer->setCurrentPage(currentPage);
    slotRefreshScore();
}

void    EditionManager::setBPMFromMetronome(QString bpm)
{
    Tag*    tag;

    tag = new Tag("tempo");
    tag->addParam("bpm=" + bpm);
    QPair<int, int> pos = _scoreEditor->getCursorIndex();
    _scoreEditor->deleteMusicElement(pos.first, 0);
    _scoreEditor->addMusicElement(tag, pos.first, 0);
    _scoreEditor->moveCursorTo(pos.first, pos.second);
}

void    EditionManager::slotSetMultipageMaxPages(int maxpage)
{
    _multipageToolbar->setMaxPages(maxpage);
}

void    EditionManager::setTranslation()
{
   _editionToolbar->setTranslation();
   _soundAnalyzerManager->setTranslation();
}
