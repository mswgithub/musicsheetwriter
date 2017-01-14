#include "ProjectFactory.h"
#include "libmusicxml.h"
#include <iostream>
#include <fstream>

using namespace MusicXML2;
using namespace std;

ProjectFactory::ProjectFactory()
{

}

ProjectFactory::~ProjectFactory()
{

}

Project*    ProjectFactory::newProject() const
{
    NewProjectDialog* projectDialog = new NewProjectDialog();
    projectDialog->launch();
    Project* project = projectDialog->getProject();
    delete projectDialog;

    if (project != NULL)
    {
        Score* score = new Score();
        if (score != NULL)
        {
            initScore(score);
            project->setScore(score);
        }
        // Si la partition n'a pas pu être créée, alors le projet est détruit.
        else
        {
            delete project;
            project = NULL;
        }
    }
    return project;
}

Project*    ProjectFactory::openProject() const
{
    QString filename = QFileDialog::getOpenFileName(0, tr("Ouvrir le projet"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Config (*.msw)");
    if (filename != NULL)
    {
        Project *project = new Project();
        if (project->deserialize(filename))
        {
            QStringList path = QString(filename).split("/");
            path.removeLast();
            QString pathstring = path.join("/") + "/" + project->getProjectName() + ".msw";
            project->setProjectPath(pathstring);
            return project;
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}

Project*    ProjectFactory::editProject(Project* project) const
{
    NewProjectDialog* projectDialog = new NewProjectDialog(project);
    projectDialog->launch();
    project = projectDialog->getProject();
    if (project != NULL)
    {
        project->save();
    }

    projectDialog->close();
    delete projectDialog;
    return project;
}

Project*    ProjectFactory::saveProject(Project* project) const
{
    Project* tmp = resetColorAMusicElements(project);
    tmp->save();
    return project;
}

Project*    ProjectFactory::saveProjectAs(Project* project) const
{
    Project* tmp = resetColorAMusicElements(project);
    QString dirname = QFileDialog::getExistingDirectory(0,
                                                        tr("Dossier de destination"),
                                                        "/home") + "/" + tmp->getProjectName() + "/";
    tmp->setProjectPath(dirname);
    tmp->save();
    return project;
}

Project*    ProjectFactory::importProject() const
{
    NewProjectDialog* projectDialog = new NewProjectDialog();
    projectDialog->launch();
    Project* project = projectDialog->getProject();
    delete projectDialog;

    if (project != NULL)
    {
        //

        QString pathFile = QFileDialog::getOpenFileName(0, tr("Ouvrir le fichier"), "", "MusicXML (*.xml);;GuidoFile (*.gmn)");

        char drive[_MAX_DRIVE];
        char dir[_MAX_DIR];
        char fname[_MAX_FNAME];
        char ext[_MAX_EXT];
        _splitpath(pathFile.toStdString().c_str(), drive, dir, fname, ext);
        std::string fn = pathFile.toStdString();

        if(fn.substr(fn.find_last_of(".") + 1) == "xml"){
            std::ofstream myfile;
            myfile.open(strcat(fname, ".gmn"));
            qDebug() << pathFile.toStdString().c_str();
            MusicXML2::xmlErr err = MusicXML2::kNoErr;
            err = MusicXML2::musicxmlfile2guido(pathFile.toStdString().c_str(), false, myfile);
            if (err != NULL)
            {
                qDebug() << "error in musicxmlfile2guido()";
                return NULL;
            }

            Parse* parse = new Parse(fname);
            //do parse
            Score* score = parse->getScoreFromImport();

            myfile.close();

            if (score != NULL)
            {
                //initScore(score);
                project->setScore(score);
                project->save();
            }
            // Si la partition n'a pas pu être créée, alors le projet est détruit.
            else
            {
                delete project;
                project = NULL;
            }
        }
        else  if(fn.substr(fn.find_last_of(".") + 1) == "gmn"){
            Parse* parse = new Parse(pathFile.toStdString());
            //do parse
            Score* score = parse->getScoreFromImport();


            if (score != NULL)
            {
                //initScore(score);
                project->setScore(score);
                project->save();
            }
            // Si la partition n'a pas pu être créée, alors le projet est détruit.
            else
            {
                delete project;
                project = NULL;
            }
        }
    }

    return project;
}

Project*    ProjectFactory::exportProject(Project* project) const
{
    QString selectedFilter;
    QString pathfile = QFileDialog::getSaveFileName(0, tr("Exporter vers..."),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + project->getProjectName(), "PNG;;PDF;;MIDI", &selectedFilter);
    if (selectedFilter == "PNG")
    {
        pathfile = pathfile + ".png";
        if (QFile(pathfile).exists()) {
            QMessageBox::critical(0, tr("Erreur"), tr("Un fichier portant le même nom existe déjà à l'emplacement indiqué"));
            return NULL;
        } else {
            exportPng(project, pathfile);
        }
    }
    else if (selectedFilter == "PDF")
    {
        pathfile = pathfile + ".pdf";
        if (QFile(pathfile).exists()) {
            QMessageBox::critical(0, tr("Erreur"), tr("Un fichier portant le même nom existe déjà à l'emplacement indiqué"));
            return NULL;
        } else {
            exportPdf(project, pathfile);
        }
    }
    else if (selectedFilter == "MIDI")
    {
        pathfile = pathfile + ".midi";
        if (QFile(pathfile).exists()) {
            QMessageBox::critical(0, tr("Erreur"), tr("Un fichier portant le même nom existe déjà à l'emplacement indiqué"));
            return NULL;
        } else {
            exportMidi(project, pathfile);
        }
    }
    return NULL;
}

/*
 *  Export PDF qui fait appel aux fonctions de la lib guido
 *
 *  TODO : adapter width et height a la taille de la fenetre d'affichage de la partition
 */
void    ProjectFactory::exportPdf(Project* project, QString const& pathname) const
{
    Project* tmp = resetColorAMusicElements(project);
    QGuidoPainter *ptr = QGuidoPainter::createGuidoPainter();
    ARFactoryHandler arFactory;
    GuidoFactoryOpen(&arFactory);
    ptr->setARHandler(tmp->getScore()->abstract(arFactory));
    GuidoLayoutSettings settings = ptr->guidoLayoutSettings();
    settings.optimalPageFill = 0;
    ptr->setGuidoLayoutSettings(settings);
    GuidoFactoryClose(arFactory);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat( QPrinter::PdfFormat );
    printer.setOutputFileName( QString(pathname) );
    printer.setPaperSize( QPrinter::A4 );
    QPainter painter (&printer);
    painter.setRenderHint( QPainter::Antialiasing );
    int firstPage = 1;
    int lastPage = ptr->pageCount();
    if ( printer.fromPage() )
        firstPage = printer.fromPage();
    if ( printer.toPage() )
        lastPage = printer.toPage();
    for (int page = firstPage; page <= lastPage ; ++page) {
        ptr->draw(&painter , page , printer.paperRect() );
        if (page != lastPage)
            printer.newPage();
    }
}

/*
 *  Export PNG qui fait appel aux fonctions de la lib guido
 */
void    ProjectFactory::exportPng(Project* project,
                                  QString const& pathname) const
{
    Project* tmp = resetColorAMusicElements(project);
    QGuidoPainter *ptr = QGuidoPainter::createGuidoPainter();
    GuidoLayoutSettings settings = ptr->guidoLayoutSettings();
    settings.optimalPageFill = 0;
    ptr->setGuidoLayoutSettings(settings);
    ARFactoryHandler arFactory;
    GuidoFactoryOpen(&arFactory);
    ptr->setARHandler(tmp->getScore()->abstract(arFactory));
    GuidoFactoryClose(arFactory);
    QString tmppath;
    int pageIndex = ptr->pageCount();
    for (int i = 1; i <= pageIndex; i++)
    {
        tmppath = pathname;
        if (pageIndex > 1)
        {
            tmppath = tmppath.left(tmppath.length() - 4);
            tmppath = tmppath + "_page_" + QString::number(i) + ".png";
        }
        QSizeF s = ptr->pageSizeMM(1);
        QImage image(s.toSize() * 10, QImage::Format_ARGB32);
        image.fill(QColor(Qt::white).rgb());
        QPainter painter(&image);
        ptr->draw(&painter, i, image.rect());
        image.save(tmppath);
    }
}

/*
 *  Export Midi qui fait appel aux fonctions de la lib guido
 */
void    ProjectFactory::exportMidi(Project* project,
                                   QString const& pathname) const
{
    QString outfile = pathname;
    QGuidoPainter *ptr = QGuidoPainter::createGuidoPainter();
    ARFactoryHandler arFactory;
    GuidoFactoryOpen(&arFactory);
    ptr->setARHandler(project->getScore()->abstract(arFactory));
    GuidoFactoryClose(arFactory);
    CARHandler ar = ptr->getARHandler();
    GuidoAR2MIDIFile((const ARHandler)ar, outfile.toStdString().c_str(), 0);
}

/*
 * Créer une partition basique avec une voice et quelques notes.
 */
void    ProjectFactory::initScore(Score *score) const
{
    Tag*    tag;

    // Ajoute une voice
    score->addVoice(new Voice);

    // Ajoute un tempo de base
    tag = new Tag("tempo");
    tag->addParam("bpm=120");
    (*(score->getVoices()))[0]->addElement(tag);

    // Ajoute le tag clef
    tag = new Tag("clef");
    tag->addParam("g");
    (*(score->getVoices()))[0]->addElement(tag);

    // Ajoute le tag de mesure
    tag = new Tag("meter");
    tag->addParam("4/4");
    (*(score->getVoices()))[0]->addElement(tag);
}

Project*    ProjectFactory::resetColorAMusicElements(Project* project) const
{
    Project* tmp = project;
    QVector<Voice*>* voices = tmp->getScore()->getVoices();
    for (auto it = voices->begin(); it != voices->end(); ++it)
    {
        QVector<AMusicElement*>* musicElements = (*it)->getMusicElements();
        for (auto itr = musicElements->begin(); itr != musicElements->end(); ++itr)
        {
            (*itr)->deselect();
        }
    }
    return tmp;
}
