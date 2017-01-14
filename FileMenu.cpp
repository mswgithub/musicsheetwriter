#include "FileMenu.h"

/*
 * Création du menu Fichier.
 * Prepare les signaux qui doivent être recus.
 */
FileMenu::FileMenu(QMenu* menufile, AProjectManager* projectManager) : AMenu(menufile)
{
    _projectManager = projectManager;
    _projects = NULL;
    _activeProject = NULL;

    createMenu();
    setActionsState();

    connect(_projectManager,
            SIGNAL(signalUpdateProjectList(QList<Project*> const*)),
            SLOT(slotUpdateProjectsList(QList<Project*> const*)));
    connect(_projectManager,
            SIGNAL(signalUpdateActiveProject(const Project*)),
            SLOT(slotUpdateActiveProject(const Project*)));
}

FileMenu::~FileMenu()
{

}

/*
 * Crée le menu "Fichier". Les sous-menus et les actions sont crées et ajoutés au menu.
 * Les actions sont associés au fonction du project factory (see ProjectFactory.h)
 */
void    FileMenu::createMenu()
{
    _new = getMenu()->addMenu(QIcon(":/images/FileMenu/new.png"), tr("Nouveau"));
    _newProject = _new->addAction(tr("Projet"), this, SLOT(slotNewProject()), QKeySequence::New);
    _openProject = getMenu()->addAction(QIcon(":/images/FileMenu/open.png"), tr("Ouvrir un projet"),
                                        this, SLOT(slotOpenProject()), QKeySequence::Open);

    getMenu()->addSeparator();

    _saveProject = getMenu()->addAction(QIcon(":/images/FileMenu/save.png"), tr("Enregistrer"),
                                        this, SLOT(slotSaveProject()), QKeySequence::Save);
    _saveProjectAs = getMenu()->addAction(tr("Enregistrer sous"), this, SLOT(slotSaveProjectAs()),
                                          QKeySequence::SaveAs);
    _saveAllProjects = getMenu()->addAction(tr("Enregistrer tous les projets"), this, SLOT(slotSaveAllProjects()));

    getMenu()->addSeparator();

    _importProject = getMenu()->addAction(tr("Importer")+"...", this, SLOT(slotImportProject()));
    _exportProject = getMenu()->addAction(tr("Exporter")+"...", this, SLOT(slotExportProject()));

    getMenu()->addSeparator();

    _closeProject = getMenu()->addAction(tr("Fermer"), this, SLOT(slotCloseProject()));
    _closeAllProjects = getMenu()->addAction(tr("Fermer tous les projets"), this, SLOT(slotCloseAllProjects()));
}

/*
 * Configure le status des boutons du menu. Si un projet est actif, les boutons sauvegarder,
 * exporter et fermer un projet seront selectionnable. Si une liste de projet est disponible,
 * les boutons pour tous les sauvegarder ou les fermer sont selectionnable.
 */
void    FileMenu::setActionsState()
{
    if (_activeProject)
    {
        _saveProject->setEnabled(true);
        _saveProjectAs->setEnabled(true);
        _exportProject->setEnabled(true);
        _closeProject->setEnabled(true);
        _saveProject->setText(tr("Enregistrer")+" \""+_activeProject->getProjectName()+"\"");
        _saveProjectAs->setText(tr("Enregistrer")+" \""+_activeProject->getProjectName()+"\" "+tr("sous"));
        _exportProject->setText(tr("Exporter")+" \""+_activeProject->getProjectName()+"\"...");
        _closeProject->setText(tr("Fermer")+" \""+_activeProject->getProjectName()+"\"");
    }
    else
    {
        _saveProject->setEnabled(false);
        _saveProjectAs->setEnabled(false);
        _exportProject->setEnabled(false);
        _closeProject->setEnabled(false);
        _saveProject->setText(tr("Enregistrer"));
        _saveProjectAs->setText(tr("Enregistrer sous"));
        _exportProject->setText(tr("Exporter")+"...");
        _closeProject->setText(tr("Fermer"));
    }

    if (_projects && !_projects->isEmpty())
    {
        _saveAllProjects->setEnabled(true);
        _closeAllProjects->setEnabled(true);
    }
    else
    {
        _saveAllProjects->setEnabled(false);
        _closeAllProjects->setEnabled(false);
    }
}

/*
 * Met à jour la liste des projets ouverts. Si la liste n'est ni NULL ni vide, les actions
 * d'enregistrement et de fermeture de tous les projets depuis le menu sont activées.
 */
void    FileMenu::slotUpdateProjectsList(QList<Project*> const* projects)
{
    _projects = projects;
    setActionsState();
}

/*
 * Met à jour le projet active. Si le project n'est pas NULL, les actions
 * d'enregistrement, d'export et de fermeture depuis le menu sont activées.
 */
void    FileMenu::slotUpdateActiveProject(Project const* activeProject)
{
    _activeProject = activeProject;
    setActionsState();
}

/*
 * Demande au project manager de créer un projet
 */
void    FileMenu::slotNewProject()
{
    _projectManager->newProject();
}

/*
 * Demande au project manager d'ouvrir un projet
 */
void    FileMenu::slotOpenProject()
{
    _projectManager->openProject();
}

/*
 * Demande au project manager d'enregistrer le projet actif.
 */
void    FileMenu::slotSaveProject()
{
    _projectManager->saveProject();
}

/*
 * Demande au project manager d'enregistrer sous le projet actif.
 */
void    FileMenu::slotSaveProjectAs()
{
    _projectManager->saveProjectAs();
}

/*
 * Demande au project manager d'enregistrer tous les projets.
 */
void    FileMenu::slotSaveAllProjects()
{
    _projectManager->saveAllProjects();
}

/*
 * Demande au project manager d'importer un projet
 */
void    FileMenu::slotImportProject()
{
    _projectManager->importProject();
}

/*
 * Demande au project manager d'exporter le projet actif.
 */
void    FileMenu::slotExportProject()
{
    _projectManager->exportProject();
}

/*
 * Demande au project manager de fermer le projet actif.
 */
void    FileMenu::slotCloseProject()
{
    _projectManager->closeProject();
}

/*
 * Demande au project manager de fermer tous les projets.
 */
void    FileMenu::slotCloseAllProjects()
{
    _projectManager->closeAllProjects();
}

