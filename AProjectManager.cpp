#include    "AProjectManager.h"

AProjectManager::AProjectManager(QObject* parent) : QObject(parent)
{
    _projects = new QList<Project*>();
}

AProjectManager::~AProjectManager()
{
    delete _projects;
}

QList<Project*> const*    AProjectManager::getProjects() const
{
    return _projects;
}

Project const*  AProjectManager::getActiveProject() const
{
    return _activeProject;
}


QList<Project*>*    AProjectManager::getProjects()
{
    return _projects;
}

Project*    AProjectManager::getActiveProject()
{
    return _activeProject;
}

void    AProjectManager::setActiveProject(Project* project)
{
    _activeProject = project;
}
