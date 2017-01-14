#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include    <QTreeWidget>
#include    "AProjectManager.h"
#include    "ProjectTreeWidget.h"
#include    "ProjectFactory.h"
#include    "Project.h"
#include    "Score.h"


class ProjectManager : public AProjectManager
{
    Q_OBJECT

public:
    ProjectManager(QTreeWidget *treewidget, QObject* parent = 0);
    virtual ~ProjectManager();

    Project*        newProject();
    Project*        openProject();

    void        selectProject(Project* project = NULL);
    void        editProject(Project* project = NULL);
    void        saveProject(Project* project = NULL);
    void        saveProjectAs(Project* project = NULL);
    void        saveAllProjects();
    Project*    importProject();
    void        exportProject(Project* project = NULL);
    void        closeProject(Project* project = NULL);
    void        closeAllProjects();

private:
    ProjectTreeWidget* _projectsTreeWidget;
    ProjectFactory*     _projectFactory;

signals:

private slots:

};

#endif // PROJECTMANAGER_H
