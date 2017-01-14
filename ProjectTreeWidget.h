#ifndef PROJECTSTREEWIDGET_H
#define PROJECTSTREEWIDGET_H

#include    <QObject>
#include    <QTreeWidget>
#include    <QTreeWidgetItem>
#include    <QAction>
#include    <QMenu>
#include    <QPoint>
#include    <QFont>
#include    <QModelIndex>
#include    "AProjectManager.h"
#include    "Project.h"


class ProjectTreeWidget : public QObject
{
    Q_OBJECT
public:
    explicit ProjectTreeWidget(QTreeWidget *treewidget, AProjectManager* projectManager,
                               QObject* parent = 0);
    virtual ~ProjectTreeWidget();

private:
    void                            updateTreeWidget();

    QTreeWidget*                    _treewidget;
    // Projects and their manager
    AProjectManager*                _projectManager;
    QList<Project*> const*          _projects;
    Project const*                  _activeProject;

public slots:
    // Interaction avec le Tree Widget
    void    slotRighClick(QPoint const& pos);
    void    slotTreeItemClicked(QModelIndex const& index);

    // Mis à jour de la liste des projets
    void    slotUpdateProjectsList(QList<Project*> const* projects);
    void    slotUpdateActiveProject(Project const* activeProject);

private slots:
    // Menu contextuel
    void    slotSelectProject();
    void    slotEditProject();
    void    slotExportProject();
    void    slotNewProject();
    void    slotCloseProjet();
};

#endif // PROJECTSTREEWIDGET_H
