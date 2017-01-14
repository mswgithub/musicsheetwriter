#include "ProjectTreeWidget.h"

ProjectTreeWidget::ProjectTreeWidget(QTreeWidget *treewidget,
                                     AProjectManager* projectManager,
                                     QObject* parent) : QObject(parent)
{
    _treewidget = treewidget;
    _treewidget->setContextMenuPolicy(Qt::CustomContextMenu);

    _projectManager = projectManager;
    _projects = NULL;
    _activeProject = NULL;

    connect(_treewidget,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(slotRighClick(const QPoint&)));
    connect(_treewidget,
            SIGNAL(clicked(QModelIndex)),
            SLOT(slotTreeItemClicked(QModelIndex)));
    connect(_projectManager,
            SIGNAL(signalUpdateProjectList(QList<Project*> const*)),
            SLOT(slotUpdateProjectsList(QList<Project*> const*)));
    connect(_projectManager,
            SIGNAL(signalUpdateActiveProject(Project const*)),
            SLOT(slotUpdateActiveProject(Project const*)));
    updateTreeWidget();
}

ProjectTreeWidget::~ProjectTreeWidget()
{
}

/*
 * Met à jour le TreeWidget en parcourant la liste des projets ouverts.
 * Le widget est d'abords vider de tous les projets et puis reremplis avec
 * la liste des projets ouverts. Le projet actif est mis en gras.
 * Si la liste n'a pas été initialisé ou si elle est vide, le widget se cache.
 */
void    ProjectTreeWidget::updateTreeWidget()
{
    _treewidget->clear();
    if (_projects && !_projects->empty())
    {
        QTreeWidgetItem* itm;
        for(auto it = _projects->begin(); it != _projects->end(); ++it)
        {
            Project* const project = *it;
            itm = new QTreeWidgetItem(_treewidget);
            itm->setText(0, project->getProjectName());
            itm->setWhatsThis(1, project->getProjectPath());
            if (project == _activeProject)
            {
                QFont font;
                font.setBold(true);
                itm->setFont(0, font);
                itm->setSelected(true);
            }
            else
            {
                itm->setSelected(false);
            }
            _treewidget->addTopLevelItem(itm);
        }
        _treewidget->show();
    }
    else
    {
        _treewidget->hide();
    }
}

/*
 * Affiche un menu contextuel
 * On verifie que le clic a bien eu lieu sur un élément du TreeWidget. Si c'est le cas,
 * L'utilisateur peut choisir d'afficher la partition, d'exporter le projet ou modifier
 * ses informations. Si l'utilisateur n'a pas cliqué sur un élément du TreeWidget, il
 * pourra choisir de créer un nouveau projet.
 */
void    ProjectTreeWidget::slotRighClick(QPoint const& pos)
{
    QTreeWidgetItem *itm = _treewidget->itemAt(pos);
    QMenu menu(_treewidget);

    // Si on a cliqué sur un élément du TreeWidget
    if (itm != NULL) {
        QAction* display = menu.addAction(tr("Afficher dans l'éditeur"), this, SLOT(slotSelectProject()));
        display->setData(itm->whatsThis(1));
        QFont font = display->font();
        font.setBold(true);
        display->setFont(font);
        menu.addAction(tr("Modifier la projet"), this, SLOT(slotEditProject()))->setData(itm->whatsThis(1));
        menu.addAction(tr("Exporter")+"...", this, SLOT(slotExportProject()))->setData(itm->whatsThis(1));
        menu.addAction(tr("Fermer le projet"), this, SLOT(slotCloseProjet()))->setData(itm->whatsThis(1));
    }
    // Sinon
    else
    {
        menu.addAction(tr("Nouveau projet")+"...", this, SLOT(slotNewProject()));
    }
    menu.setStyleSheet("QMenu::item:selected{background-color:rgb(144,200,246);}");
    menu.exec(_treewidget->mapToGlobal(pos));
}

/*
 * Gère le clic sur un item du TreeWidget. L'item selectionné est notifié au project manager
 * afin d'être affiché dans l'éditeur.
 */
void    ProjectTreeWidget::slotTreeItemClicked(QModelIndex const& index)
{
    QString projectpath = _treewidget->currentItem()->whatsThis(1);

    if (_projects)
        for(auto it = _projects->begin(); it != _projects->end(); ++it)
        {
            Project* const project = *it;
            if (project->getProjectPath() == projectpath)
            {
                _projectManager->selectProject(project);
            }
        }
}

/*
 * Met à jour la liste des projets ouverts dans le TreeWidget.
 */
void    ProjectTreeWidget::slotUpdateProjectsList(QList<Project*> const* projects)
{
    _projects = projects;
    if (_projects)
        updateTreeWidget();
}

/*
 * Met à jour le projet actif dans le TreeWidget.
 */
void    ProjectTreeWidget::slotUpdateActiveProject(Project const* activeProject)
{
    _activeProject = activeProject;
    if (_activeProject)
        updateTreeWidget();
}


void    ProjectTreeWidget::slotSelectProject()
{
    QAction *obj = static_cast<QAction*>(sender());
    QString projectPath = obj->data().toString();

    if (_projects)
        for(auto it = _projects->begin(); it != _projects->end(); ++it)
        {
            Project* const project = *it;
            if (project->getProjectPath() == projectPath)
            {
                _projectManager->selectProject(project);
            }
        }
}

void    ProjectTreeWidget::slotEditProject()
{
    QAction *obj = static_cast<QAction*>(sender());
    QString projectPath = obj->data().toString();

    if (_projects)
        for(auto it = _projects->begin(); it != _projects->end(); ++it)
        {
            Project* const project = *it;
            if (project->getProjectPath() == projectPath)
            {
                _projectManager->editProject(project);
            }
        }
}

void    ProjectTreeWidget::slotExportProject()
{
    QAction *obj = static_cast<QAction*>(sender());
    QString projectPath = obj->data().toString();

    if (_projects)
        for(auto it = _projects->begin(); it != _projects->end(); ++it)
        {
            Project* const project = *it;
            if (project->getProjectPath() == projectPath)
            {
                _projectManager->exportProject(project);
            }
        }
}

void    ProjectTreeWidget::slotCloseProjet()
{
    QAction *obj = static_cast<QAction*>(sender());
    QString projectPath = obj->data().toString();

    if (_projects)
        for(auto it = _projects->begin(); it != _projects->end(); ++it)
        {
            Project* const project = *it;
            if (project->getProjectPath() == projectPath)
            {
                _projectManager->closeProject(project);
                break;
            }
        }
}

void    ProjectTreeWidget::slotNewProject()
{
    _projectManager->newProject();
}




