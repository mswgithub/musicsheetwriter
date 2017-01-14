#include "ProjectManager.h"

/*
 *  Création du QTreeWidget
 *  On indique qu'on souhaite pouvoir avoir un menu contextuel(1er connect) on connecte à la fonction de création du menu contextuel
 *  On connect l'action du click sur un element du QTreeWidget avec le slot slotTreewidgetClicked
 *  On chaque le QTreeWidget si la liste des projets ouverts est vide
*/
ProjectManager::ProjectManager(QTreeWidget *treewidget, QObject* parent) :
    AProjectManager(parent)
{
    _projectsTreeWidget = new ProjectTreeWidget(treewidget, this, this);
    _projectFactory = new ProjectFactory();
}

ProjectManager::~ProjectManager()
{
    closeAllProjects();

    delete _projectsTreeWidget;
    delete _projectFactory;
}

/*
 * Appelle le project factory afin de créer un nouveau projet. Si la création
 * a réussi, alors le projet est ajouté à la liste des projets ouverts.
 * Un signal est envoyé pour notifier que la liste des projets ouverts a changé.
 */
Project*    ProjectManager::newProject()
{
    Project* project = _projectFactory->newProject();
    if (project)
    {
        getProjects()->append(project);
        emit signalUpdateActiveProject(project);
        emit signalUpdateProjectList(getProjects());
        return project;
    }
    else
    {
        return NULL;
    }
}

/*
 * Appelle le project factory afin de récuperer un projet existant sur l'ordinateur.
 * Si l'ouverture a réussi et qu'aucun projet déjà ouvert ne possède le même nom et
 * le même path, alors the projet est ajouté à la liste des projets ouverts.
 * Un signal est envoyé pour notifier que la liste des projets ouverts a changé.
 */
Project*    ProjectManager::openProject()
{
    Project* project = _projectFactory->openProject();

    if (project)
    {
        bool isAlreadyOpened = false;

        QList<Project*>* projects = getProjects();
        for(auto it = projects->begin(); it != projects->end(); ++it)
        {
            Project* tmp = *it;
            if (project->getProjectPath() == tmp->getProjectPath()
                    && project->getProjectName() == tmp->getProjectName())
                isAlreadyOpened = true;
        }

        if (!isAlreadyOpened)
        {
            getProjects()->append(project);
            emit signalUpdateActiveProject(project);
            emit signalUpdateProjectList(getProjects());
        }
        else
        {
            QMessageBox::critical(0, tr("Erreur"),
                                  tr("Le projet") + " \"" + project->getProjectName() + "\" " + tr("est déjà ouvert"));
        }
        project->setgmnScore(project->checkGmnScore());
        return project;
    }
    else
    {
        return NULL;
    }
}

/*
 * Le projet passé en parametre devient le projet actif.
 * Un signal est envoyé pour notifier que le projet actif a changé.
 */
void    ProjectManager::selectProject(Project* project)
{
    if (project == NULL)
        project = getActiveProject();

    if (project != NULL)
    {
        setActiveProject(project);
        emit signalUpdateActiveProject(project);
    }
}

/*
 * Appelle le project factory afin d'editer les informations du projet. Si aucun projet
 * n'est spécifié, l'édition s'effectue sur le projet actif. Un fois l'edition terminé,
 * un signal est envoyé pour notifier que la liste des projets ouverts a changé.
 */
void    ProjectManager::editProject(Project* project)
{
    if (project == NULL)
        project = getActiveProject();

    if (project != NULL)
    {
        _projectFactory->editProject(project);
        emit signalUpdateProjectList(getProjects());
    }
}

/*
 * Appelle le project factory afin d'enregistrer le projet. Si aucun projet n'est
 * spécifié, l'enregistrement s'effectue sur le projet actif. Une fois l'enregistrement
 * terminé, un signal est envoyé pour notifier que la liste des projets ouverts a changé.
 */
void    ProjectManager::saveProject(Project* project)
{
    if (project == NULL)
        project = getActiveProject();

    if (project != NULL)
    {
        _projectFactory->saveProject(project);
        emit signalUpdateProjectList(getProjects());
    }
}

/*
 * Appelle le project factory afin d'enregistrer le projet sous. Si aucun projet n'est
 * spécifié, l'enregistrement s'effectue sur le projet actif. Une fois l'enregistrement
 * terminé, un signal est envoyé pour notifier que la liste des projets ouverts a changé.
 */
void    ProjectManager::saveProjectAs(Project* project)
{
    if (project == NULL)
        project = getActiveProject();

    if (project != NULL)
    {
        _projectFactory->saveProjectAs(project);
        (getProjects());
    }
}

/*
 * Appelle le project factory sur tous les projets afin de tous les enregister un par un.
 * Une fois les enregistrements terminés, un signal est envoyé pour notifier que la liste
 * des projets ouverts a changé.
 */
void    ProjectManager::saveAllProjects()
{
    QList<Project*>* projects = getProjects();
    for(auto it = projects->begin(); it != projects->end(); ++it)
    {
        Project* project = *it;
        _projectFactory->saveProject(project);
    }
    emit signalUpdateProjectList(getProjects());
}

/*
 * Appelle le project factory afin d'importer un projet. Si l'importation a réussi,
 * alors le projet est ouvert et est ajouté à la liste des projets ouverts.
 */
Project*    ProjectManager::importProject()
{
    Project* project = _projectFactory->importProject();
    if (project)
    {
        getProjects()->append(project);
        emit signalUpdateProjectList(getProjects());
        return project;
    }
    else
    {
        return NULL;
    }
}

/*
 * Appelle le project factory afin d'exporter un projet. Si aucun projet n'est spécifié,
 * L'exportation s'effectue sur le projet actif. Une fois l'exportation terminé,
 * un signal est envoyé pour notifier que la liste des projets ouverts a changé.
 */
void    ProjectManager::exportProject(Project* project)
{
    if (project == NULL)
        project = getActiveProject();

    if (project != NULL)
    {
        _projectFactory->exportProject(project);
        emit signalUpdateProjectList(getProjects());
    }
}

/*
 * Retire le projet de la liste des projets ouverts. Avant la fermeture, une verification
 * d'un nécéssaire enregistrement est faite. Si aucun project n'est spécifié, la fermeture
 * s'effectue sur le projet actif. Si le projet actif est fermé, un signal est envoyé pour
 * notifier que le projet actif a changé. Un signal est égalament envoyé pour notifier que
 * la liste des projets ouverts a changé.
 */
void    ProjectManager::closeProject(Project* project)
{
    if (project == NULL)
        project = getActiveProject();

    if (project != NULL)
    {
        if (project->checkSave())
        {
            getProjects()->removeOne(project);
            if (project == getActiveProject())
            {
                if (getProjects()->count() >= 1)
                {
                    QList<Project*> *tmp = getProjects();
                    setActiveProject(*tmp->begin());
                }
                else
                {
                    setActiveProject(NULL);
                }
                emit signalUpdateActiveProject(getActiveProject());
            }
            delete project;
            emit signalUpdateProjectList(getProjects());
        }
    }
}

/*
 * Retire tous les projets de la liste des projets ouverts. Avant chaque fermeture, une
 * verification d'un nécéssaire enregistrement est effectué. Si le projet actif est fermé,
 * un signal est envoyé pour notifier que le projet actif a changé. Un signal est
 * égalament envoyé pour notifier que la liste des projets ouverts a changé.
 */
void    ProjectManager::closeAllProjects()
{
    QList<Project*>* projects = getProjects();
    for(auto it = projects->begin(); it != projects->end(); ++it)
    {
        Project* project = *it;
        if (project->checkSave())
        {
            getProjects()->removeOne(project);
            if (project == getActiveProject())
            {
                setActiveProject(NULL);
                emit signalUpdateActiveProject(getActiveProject());
            }
            delete project;
        }
    }
    emit signalUpdateProjectList(getProjects());
}
