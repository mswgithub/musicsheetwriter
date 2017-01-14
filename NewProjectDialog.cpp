#include "NewProjectDialog.h"
#include "ui_newProjectDialog.h"

/*
 * Constructeur pour la creation d'un nouveau projet
*/
NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent)
{
    _ui = new Ui::NewProjectDialog;
    _ui->setupUi(this);

    _project = NULL;
}

/*
 * Constructeur pour la modification d'un projet
*/
NewProjectDialog::NewProjectDialog(Project *modproject, QWidget *parent) : QDialog(parent)
{
    _ui = new Ui::NewProjectDialog;
    _ui->setupUi(this);

    _project = modproject;
}

NewProjectDialog::~NewProjectDialog()
{
    delete _ui;
}

/*
 * Ouvre la boite de dialog. Si un projet est déjà chargé, les informations
 * dans les champs sont préremplis.
 */
void    NewProjectDialog::launch()
{
    if (_project)
    {
        setWindowTitle(tr("Modifier le projet"));
        _ui->projectName->setText(_project->getProjectName());
        _ui->projectPath->setText(_project->getProjectPath());
    }
    else
    {
        setWindowTitle(tr("Création nouveau projet"));
        _ui->projectPath->setText(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }
    exec();
}

/*
 * Verifie que le formulaire est bien rempli, notamment si l'emplacement choisi n'est pas déja occupé par un projet du meme nom
*/
bool    NewProjectDialog::checkForm()
{
    if (_ui->projectName->text() == NULL)
    {
        QMessageBox::critical(this, tr("Erreur"), tr("Le champs Nom du projet est requis"));
        return false;
    }
    else if (_ui->projectPath->text() == NULL)
    {
        QMessageBox::critical(this, tr("Erreur"), tr("Le champs Créer dans est requis"));
        return false;
    }
    QString filename = _ui->projectPath->text();
    if (QFile(filename).exists()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Un projet portant le même nom existe déjà à l'emplacement indiqué"));
        return false;
    }
    return  true;
}

/*
 * Verifie si les informations du formulaire sont remplis et valide. Si c'est le cas, un projet
 * est créé en utilisant les informations entrées.
 */
void NewProjectDialog::on_Confirm_clicked()
{
   if (this->checkForm() != false)
   {
       QString filename = _ui->projectPath->text();
       if (_project != NULL)
       {
           if (_project->getProjectName() != _ui->projectName->text() || _project->getProjectPath() != _ui->projectPath->text())
           {
               QFile::remove(_project->getProjectPath());
               if (_project->getProjectName() != _ui->projectName->text())
               {
                   QStringList  path = QString(_project->getProjectPath()).split("/");
                   path.removeLast();
                   filename = path.join("/") + "/" + _ui->projectName->text() + ".msw";
               }
               _project->setProjectName(_ui->projectName->text());
               _project->setProjectPath(filename);
           }
       }
       else
       {
           _project = new Project(_ui->projectName->text(), /*dirname*/_ui->projectPath->text());
       }
       close();
   }
}

/*
 *  Ouvre le QFileDialog pour choisir la destination du projet
*/
void NewProjectDialog::on_Browse_clicked()
{
    QString path;
    QString pathbrowse;

    if (_ui->projectPath->text() != "")
    {
        path = _ui->projectPath->text();
    } else
    {
        path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }
    pathbrowse = QFileDialog::getExistingDirectory(this, tr("Choisir un dossier"),
                                                          path,
                                                          QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if (pathbrowse != "")
    {
        if (_ui->projectName->text() != "")
        {
            _ui->projectPath->setText(pathbrowse + "/" + _ui->projectName->text() + ".msw");
        }
        else
        {
            _ui->projectPath->setText(pathbrowse);
        }
    }
}

Project*    NewProjectDialog::getProject() const
{
    return _project;
}

void NewProjectDialog::on_projectName_textChanged(const QString &text)
{
    QStringList path = QString(_ui->projectPath->text()).split("/");
    if (text != "")
    {
        if (!QString(path.at(path.count() - 1)).contains(".msw"))
        {
            _ui->projectPath->setText(path.join("/") + "/" + text + ".msw");
        }
        else
        {
            path.removeLast();
            _ui->projectPath->setText(path.join("/") + "/" + text + ".msw");
        }
    }
    else
    {
        path.removeLast();
        _ui->projectPath->setText(path.join("/"));
    }
}
