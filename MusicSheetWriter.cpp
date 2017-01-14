#include "MusicSheetWriter.h"

/*
 *  Instantie tous les elements graphiques et fonctionnels du logiciel.
 *  connect les éléments entre eux avec les signaux et slots
 */
MusicSheetWriter::MusicSheetWriter(QWidget *parent) : QMainWindow(parent)
{
    // Start Quido Engine
    QGuidoPainter::startGuidoEngine();

    // Create and Setup the UI
    _ui = new Ui::MusicSheetWriter;
    _ui->setupUi(this);
    _translator = new QTranslator;

    createMenusTranslations();

    // Show the welcome widget. It's the first thing the user will see on the program launch.
    _ui->welcomeWidget->show();

    _ui->stackedWidget->setCurrentWidget(_ui->page);

    createProjectManager();
    createEditionManager();
    setDropShadowEffect();

    connect(_ui->welcomeCreateNewProject,
            SIGNAL(clicked(bool)),
            SLOT(slotWelcomeCreateProject()));

    connect(_ui->welcomeOpenProject,
            SIGNAL(clicked(bool)),
            SLOT(slotWelcomeOpenProject()));

    connect(_ui->welcomeImportProject,
            SIGNAL(clicked(bool)),
            SLOT(slotWelcomeImportProject()));

    QString locale = QLocale::system().name().section('_', 0, 0);
    if (locale != "fr")
    {
        slotTranslateUk();
    }
}

MusicSheetWriter::~MusicSheetWriter()
{
    // Destroy the menu bar elements
    delete _fileMenu;
    delete _editMenu;
    // Destroy managers
    delete _projectManager;
    delete _editionManager;
    delete _playerManager;

    // Destroy the UI
    delete _ui;

    // Stop
    QGuidoPainter::stopGuidoEngine();
}

/*
 * Met à jour l'éditeur de partition avec le nouveau projet actif. Si aucun projet
 * n'est actif, l'éditor est caché.
 */
void    MusicSheetWriter::slotUpdateActiveProject(Project const* activeProjet)
{
    if (activeProjet)
    {
        _editionManager->updateScore(activeProjet->getScore());
        _playerManager->updatePlayer(activeProjet->getScore());
        _metronomeManager->updateMetronome(true);
    }
    else
    {
        _editionManager->updateScore(NULL);
        _playerManager->updatePlayer(NULL);
        _metronomeManager->updateMetronome(false);
        _ui->welcomeWidget->show();
        _ui->stackedWidget->setCurrentWidget(_ui->page);
        menuBar()->clear();
        _menubar->show();
    }
}

void    MusicSheetWriter::keyReleaseEvent(QKeyEvent* event)
{
    if (_ui->scoreView->isVisible())
    {
        _editionManager->keyReleaseEventHandler(event);
    }
}

void    MusicSheetWriter::slotWelcomeCreateProject()
{
    Project *project = _projectManager->newProject();
    if (project != NULL)
    {
        createMenus();
        menuBar()->show();
        _ui->stackedWidget->setCurrentWidget(_ui->page_2);
        _ui->welcomeWidget->hide();
         slotUpdateActiveProject(project);
         _projectManager->selectProject(project);
    }
}

void    MusicSheetWriter::slotWelcomeOpenProject()
{
    Project *project = _projectManager->openProject();
    if (project != NULL)
    {
        createMenus();
        menuBar()->show();
        _ui->stackedWidget->setCurrentWidget(_ui->page_2);
        _ui->centralWidget->setStyleSheet("background-color: #ffffff;");
        _ui->welcomeWidget->hide();
        slotUpdateActiveProject(project);
        _projectManager->selectProject(project);
    }
}

void    MusicSheetWriter::slotWelcomeImportProject()
{
    Project *project = _projectManager->importProject();
    if (project != NULL)
    {
        createMenus();
        menuBar()->show();
        _ui->stackedWidget->setCurrentWidget(_ui->page_2);
        _ui->centralWidget->setStyleSheet("background-color: #ffffff;");
        _ui->welcomeWidget->hide();
        slotUpdateActiveProject(project);
        _projectManager->selectProject(project);
    }
}

// Private methods

#include <QDebug>

void    MusicSheetWriter::createMenus()
{
    _menubar->hide();
    _fileMenu = new FileMenu(menuBar()->addMenu(tr("Fichier")), _projectManager);
}

void    MusicSheetWriter::createMenusTranslations()
{
    _menubar = new QMenuBar(_ui->menuBar);
    _menuFr = new QAction("fr", _ui->menuBar);
    _menuUk = new QAction("uk", _ui->menuBar);
    _menuFr->setIcon(QIcon(":/images/Accueil/flag-france.png"));
    _menuUk->setIcon(QIcon(":/images/Accueil/flag-united-kingdom.png"));
    connect(_menuFr,SIGNAL(triggered(bool)), SLOT(slotTranslateFr()));
    connect(_menuUk,SIGNAL(triggered(bool)), SLOT(slotTranslateUk()));
    _menubar->addAction(_menuFr);
    _menubar->addAction(_menuUk);
    _ui->menuBar->setCornerWidget(_menubar);
}

void    MusicSheetWriter::createProjectManager()
{
    _projectManager = new ProjectManager(_ui->treeWidget, this);

    connect(_projectManager,
            SIGNAL(signalUpdateActiveProject(const Project*)),
            SLOT(slotUpdateActiveProject(const Project*)));
}

void    MusicSheetWriter::createEditionManager()
{
    _editionManager = new EditionManager(_ui->scoreView, _ui->editorView, _ui->generatorView, _ui->btnMultipageLeft, _ui->btnMultipageRight, this);
    connect(_editionManager, SIGNAL(signalLecteurPartitionScore(Score*)), SLOT(slotLecteurPartitionScore(Score*)));
    _playerManager = new PlayerManager(_ui->playerView);
    _metronomeManager = new MetronomeManager(_ui->metronomeView);
    connect(_metronomeManager, SIGNAL(signalButtonAddClicked(QString)), SLOT(slotButtonAddBPMClicked(QString)));
}

void    MusicSheetWriter::setDropShadowEffect()
{
    setTopCenterWidgetShadow();
    setTreeWidgetShadow();
    setEditorWidgetShadow();
    setScoreViewerWidgetShadow();
}

void    MusicSheetWriter::setTopCenterWidgetShadow()
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(15);
    effect->setXOffset(4);
    effect->setYOffset(4);

    _ui->topWidgets->setGraphicsEffect(effect);
}

void    MusicSheetWriter::setTreeWidgetShadow()
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(15);
    effect->setXOffset(4);
    effect->setYOffset(4);

    _ui->treeWidget->setGraphicsEffect(effect);
}

void    MusicSheetWriter::setEditorWidgetShadow()
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(15);
    effect->setXOffset(4);
    effect->setYOffset(4);

    _ui->editorView->setGraphicsEffect(effect);
}

void    MusicSheetWriter::setScoreViewerWidgetShadow()
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(15);
    effect->setXOffset(4);
    effect->setYOffset(4);

    _ui->centerWidgets->setGraphicsEffect(effect);
}

void    MusicSheetWriter::slotLecteurPartitionScore(Score *score)
{
  _playerManager->updatePlayer(score);
}

void    MusicSheetWriter::slotButtonAddBPMClicked(QString bpm)
{
    _editionManager->setBPMFromMetronome(bpm);
}

void    MusicSheetWriter::closeEvent(QCloseEvent *event)
{
    _projectManager->closeAllProjects();
}

void    MusicSheetWriter::slotTranslateFr()
{
    qApp->removeTranslator(_translator);
    _editionManager->setTranslation();
    _metronomeManager->setTranslation();
    _playerManager->setTranslation();
    _ui->retranslateUi(this);
}

void    MusicSheetWriter::slotTranslateUk()
{
    QString file_translator = ":/translations/englishTranslation.qm";
    _translator->load(file_translator);
    qApp->installTranslator(_translator);
    _editionManager->setTranslation();
    _metronomeManager->setTranslation();
    _playerManager->setTranslation();
    _ui->retranslateUi(this);
}
