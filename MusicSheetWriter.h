#ifndef MUSICSHEETWRITER_H
#define MUSICSHEETWRITER_H

#include    <QMainWindow>
#include    <QList>
#include    <QKeyEvent>
#include    <QGraphicsDropShadowEffect>
#include    <QTranslator>
#include    "ProjectManager.h"
#include    "FileMenu.h"
#include    "EditMenu.h"
#include    "EditionManager.h"
#include    "PlayerManager.h"
#include    "MetronomeManager.h"
#include    "ui_musicsheetwriter.h"

namespace Ui {
class MusicSheetWriter;
}

class MusicSheetWriter : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicSheetWriter(QWidget *parent = 0);
    virtual ~MusicSheetWriter();

    void    keyReleaseEvent(QKeyEvent* event);

private:
    void    setProjectList(QList<Project*> projectlist);
    void    createMenus();
    void    createMenusTranslations();
    void    createProjectManager();
    void    createEditionManager();
    void    setDropShadowEffect();
    void    setTopCenterWidgetShadow();
    void    setTreeWidgetShadow();
    void    setEditorWidgetShadow();
    void    setScoreViewerWidgetShadow();


    Ui::MusicSheetWriter*   _ui;
    FileMenu*               _fileMenu;
    EditMenu*               _editMenu;

    AProjectManager*        _projectManager;
    AEditionManager*        _editionManager;
    PlayerManager*          _playerManager;
    MetronomeManager*       _metronomeManager;
    QTranslator*            _translator;
    QMenuBar*               _menubar;
    QAction*                _menuFr;
    QAction*                _menuUk;

protected:
     void closeEvent(QCloseEvent *event);

public slots:
    void    slotUpdateActiveProject(Project const* activeProjet);
    void    slotWelcomeCreateProject();
    void    slotWelcomeOpenProject();
    void    slotWelcomeImportProject();
    void    slotLecteurPartitionScore(Score* score);
    void    slotButtonAddBPMClicked(QString bpm);
    void    slotTranslateFr();
    void    slotTranslateUk();
};

#endif // MUSICSHEETWRITER_H
