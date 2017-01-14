#ifndef PROJECTFACTORY_H
#define PROJECTFACTORY_H

#include    <QObject>
#include    <QDialog>
#include    <QDir>
#include    <QPrinter>
#include    <QMessageBox>
#include    <QFileDialog>
#include    <GUIDO2Midi.h>
#include    "Project.h"
#include    "NewProjectDialog.h"
#include    "Parse.h"


class ProjectFactory : QObject
{
    Q_OBJECT
public:
    explicit ProjectFactory();
    virtual ~ProjectFactory();

    Project*    newProject() const;
    Project*    openProject() const;
    Project*    editProject(Project* project) const;
    Project*    saveProject(Project* project) const;
    Project*    saveProjectAs(Project* project) const;
    Project*    importProject() const;
    Project*    exportProject(Project* project) const;

private:
    void        exportPdf(Project* project, QString const& pathname) const;
    void        exportPng(Project* project, QString const& pathname) const;
    void        exportMidi(Project* project, QString const& pathname) const;
    void        initScore(Score* score) const;
    Project*    resetColorAMusicElements(Project* project) const;
};

#endif // PROJECTFACTORY_H
