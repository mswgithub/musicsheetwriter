#ifndef FILEMENU_H
#define FILEMENU_H

#include <AMenu.h>

#include    <QObject>
#include    <QMenu>
#include    <QAction>
#include    <QList>
#include    "AProjectManager.h"
#include    "Project.h"

/*
 * Voici le menu qui est crée:
 * [ Fichier ]
 *   ======================
 *   Nouveau
 *     |-Projet
 *     `-Partition
 *   Ouvir
 *   ---------
 *   Enregistrer "[]"
 *   Enregistrer "[]" sous
 *   Enregistrer tous les projets
 *   ---------
 *   Importer...
 *   Exporter "[]"...
 *   ---------
 *   Fermer "[]"
 *   Fermer tous les projets
 *   ======================
 */

class FileMenu : public AMenu
{
    Q_OBJECT

public:
    FileMenu(QMenu* menufile, AProjectManager* projectManager);
    virtual ~FileMenu();

private:
    void                createMenu();
    void                setActionsState();

    // Menu
    // Creation/ouverture
    QMenu*              _new;
    QAction*            _newProject;
    QAction*            _openProject;

    // Enregistrement
    QAction*            _saveProject;
    QAction*            _saveProjectAs;
    QAction*            _saveAllProjects;

    // Importer/Exporter
    QAction*            _importProject;
    QAction*            _exportProject;

    // Fermer
    QAction*            _closeProject;
    QAction*            _closeAllProjects;

    // Projects and their manager
    AProjectManager*                _projectManager;
    QList<Project*> const*          _projects;
    Project const*                  _activeProject;

signals:

public slots:
    // mis à jour de la liste des projets
    void    slotUpdateProjectsList(QList<Project*> const* projects);
    void    slotUpdateActiveProject(Project const* activeProject);

private slots:
    // slots pour les boutons du menu
    void    slotNewProject();
    void    slotOpenProject();
    void    slotSaveProject();
    void    slotSaveProjectAs();
    void    slotSaveAllProjects();
    void    slotImportProject();
    void    slotExportProject();
    void    slotCloseProject();
    void    slotCloseAllProjects();
};

#endif // FILEMENU_H
