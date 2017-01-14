#ifndef APROJECTMANAGER_H
#define APROJECTMANAGER_H

#include    <QObject>
#include    <QList>
#include    "Project.h"
#include    "Score.h"

class AProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit AProjectManager(QObject* parent = 0);
    virtual ~AProjectManager();

    QList<Project*> const*    getProjects() const;
    Project const*            getActiveProject() const;

    virtual Project*    newProject() = 0;
    virtual Project*    openProject() = 0;
    virtual Project*    importProject() = 0;

    virtual void    selectProject(Project* project = NULL) = 0;
    virtual void    editProject(Project* project = NULL) = 0;
    virtual void    saveProject(Project* project = NULL) = 0;
    virtual void    saveProjectAs(Project* project = NULL) = 0;
    virtual void    saveAllProjects() = 0;
    virtual void    exportProject(Project* project = NULL) = 0;
    virtual void    closeProject(Project* project = NULL) = 0;
    virtual void    closeAllProjects() = 0;

signals:
    void    signalUpdateProjectList(QList<Project*> const* _projects);
    void    signalUpdateActiveProject(Project const* activeProject);

protected:
    QList<Project*>*    getProjects();
    Project*            getActiveProject();
    void                setActiveProject(Project* project);

private:
    QList<Project*>*    _projects;
    Project*            _activeProject;
};

#endif // APROJECTMANAGER_H
