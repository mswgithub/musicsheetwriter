#include "Project.h"
#include "Converter.h"
#include <string>

Project::Project()
{
    _projectName = "";
    _projectPath = "";
    _score = NULL;
    _isActive = false;
}

Project::Project(QString const& projectName, QString const& projectPath)
{
    _projectName = projectName;
    _projectPath = projectPath;
    _score = NULL;
    _isActive = false;
}

Project::Project(QString const& projectName, QString const& projectPath, Score *score)
{
    _projectName = projectName;
    _projectPath = projectPath;
    _score = score;
    _isActive = false;
}

Project::~Project()
{
    delete _score;
}

bool Project::fileExists(QString path) {
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

/*
 *  Fonction de sauvegarde du projet
 *  On créer le dossier si celui-ci n'existe pas
 *  Si il ya une partition on créer son gmn (on supprime a chaque sauvegarde le fichier pour le réécrire)
 *  On ecrit dans le fichier le code gmn de la partition
 *  On sérialise les infos du projet
 */
void    Project::save()
{
    this->serialized(_projectPath);
}

/*
 *  Fonction qui demande si on souhaite enregister le projet -> est appelé lors de la fermeture du projet
 */

bool    Project::checkSave()
{
    QMessageBox::StandardButton reply;

    QString tmp = checkGmnScore();
    int check = QString::compare(_gmnScore, tmp, Qt::CaseSensitive);
    if (check != 0)
    {
        reply = QMessageBox::question(0, "Ferme le projet", "Souhaitez-vous enregistrer le projet " + _projectName + " avant la fermeture ?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
        {
            this->save();
        }
    }
    return reply != QMessageBox::Cancel;
}

/*
 *  Fonction qui ecrit dans le .msw tous les attributs du projet au format XML
 *  Pour chaque element qu'on souahite ajouter, on créer un QDomElement et on lui donne le nom souhaité
 *  Si on souhaite ajouter un attribut comme le type il suffit d'appeler setAttribut("type", "letypequonveut")
 *  On peut egalement ajouter du text entre les crochets à l'aide de QDomText
 */
QDomElement Project::writeXML(QDomDocument doc)
{
    QDomElement project = doc.createElement("project");
    project.setAttribute("type","Project");

    QDomElement projectname = doc.createElement("projectname");
    projectname.setAttribute("type","QString");
    project.appendChild(projectname);
    QDomText nametext = doc.createTextNode(_projectName);
    projectname.appendChild(nametext);

    if (_score != NULL) {
        QDomElement score = _score->writeXML(doc);
        score.setAttribute("type","Score");
        project.appendChild(score);
        QDomElement pngdata = serializedScorePng(doc);
        project.appendChild(pngdata);

    }

    return project;
}

/*
 *  Fonction qui reçoit un QDomElement qui correspond à un des elements créer dans le writeXML
 *  Ici il suffit juste de faire correspondre le nom de l'element récupéré avec l'attribut qui lui correspond
 */
bool        Project::readXML(QDomElement elem)
{
    if (elem.tagName() == "projectname")
    {
        _projectName = elem.text();
    }
    else if (elem.tagName() == "score")
    {
        Score *tmp = new Score();
        if (!tmp->readXML(elem))
        {
            return false;
        }
        _score = tmp;
    }
    else if (elem.tagName() != "Picture")
    {
        return false;
    }
    return true;
}

/*
 *  Serialise les infos de l'image de la partition
 *  Ici on créer une image png temporaire de la partition
 *  On recupere les infos que l'on souhaite et on detruit l'image
 */
QDomElement    Project::serializedScorePng(QDomDocument doc)
{
    QString filename = QString(_projectPath).replace(".msw", ".png");
    int pageIndex = 1;
    QGuidoPainter *ptr = QGuidoPainter::createGuidoPainter();

    ARFactoryHandler arFactory;
    GuidoFactoryOpen(&arFactory);
    ptr->setARHandler(_score->abstract(arFactory));
    GuidoFactoryClose(arFactory);

    QSizeF s = ptr->pageSizeMM( pageIndex );
    QImage image( s.toSize() * 10 , QImage::Format_ARGB32 );
    image.fill( QColor(Qt::white).rgb() );
    QPainter painter( &image );
    ptr->draw( &painter , pageIndex , image.rect() );
    image.save( filename );

    QImage imagepng(filename);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    imagepng.save(&buffer, "png");

    QDomElement picture = doc.createElement("Picture");
    picture.setAttribute("type", "png");

    QDomElement width = doc.createElement("width");
    width.setAttribute("type", "int");
    picture.appendChild(width);
    QDomText widthtext = doc.createTextNode(QString::number(imagepng.size().rwidth()));
    width.appendChild(widthtext);

    QDomElement height = doc.createElement("height");
    height.setAttribute("type", "int");
    picture.appendChild(height);
    QDomText heighttext = doc.createTextNode(QString::number(imagepng.size().rheight()));
    height.appendChild(heighttext);

    QDomElement picturedata = doc.createElement("PictureData");
    picturedata.setAttribute("type", "QString");
    picture.appendChild(picturedata);
    QDomText data = doc.createTextNode(byteArray.toBase64());
    picturedata.appendChild(data);

    QFile::remove(filename);

    return picture;
}

/*
 *  Fonction qui serialize les attributs du projet ainsi que l'image de la partition
 */
void    Project::serialized(QString path)
{
    QDomDocument doc;
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement project = writeXML(doc);
    doc.appendChild(project);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, "Sauvegarde échouée", "Impossible d'ouvrir le fichier " + path );
        return;
    }
    QTextStream ts(&file);
    int indent = 2;
    _gmnScore = checkGmnScore();
    doc.save(ts, indent);
}

/*
 *  Fonction qui recupère tous les attributs du projet
 */
bool    Project::deserialize(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Ouverture échouée", "Impossible d'ouvrir le fichier " + path);
        return false;
    }
    QDomDocument doc;
    if (!doc.setContent(&file, false))
    {
        QMessageBox::critical(0, "Ouverture échouée", "Le fichier a subi des modifications en dehors du logiciel et ne peut donc être ouvert dans celui-ci.");
        return false;
    }
    QDomElement racine = doc.documentElement();
    racine = racine.firstChildElement();
    while (!racine.isNull())
    {
        if (!this->readXML(racine))
        {
            QMessageBox::critical(0, "Ouverture échouée", "Le fichier a subi des modifications en dehors du logiciel et ne peut donc être ouvert dans celui-ci.");
            return false;
        }
        racine = racine.nextSiblingElement();
    }
    return true;
}

/*
 *  Fonctions set et get
 */
void    Project::setProjectName(QString projectName)
{
    _projectName = projectName;
}

void    Project::setProjectPath(QString projectPath)
{
    _projectPath = projectPath;
}

void    Project::setScore(Score* score)
{
    _score = score;
}

void    Project::setIsActive(bool active)
{
    _isActive = active;
}

void    Project::setgmnScore(QString gmnScore)
{
    _gmnScore = gmnScore;
}

QString const& Project::getProjectName() const
{
    return _projectName;
}

QString const& Project::getProjectPath() const
{
    return _projectPath;
}

Score *Project::getScore() const
{
    return _score;
}

bool    Project::isActive() const
{
    return _isActive;
}

QString  Project::checkGmnScore()
{
    QString gmn;
    QVector<Voice*>*  voices = _score->getVoices();

    gmn = _score->getTitle() + _score->getComposer();
    for (auto i = voices->begin(); i != voices->end(); ++i)
    {
        QVector<AMusicElement*>* musicElements = (*i)->getMusicElements();
        for (auto j = musicElements->begin(); j != musicElements->end(); ++j)
        {
            gmn += (*j)->gmnAMusicElement();
        }
    }
    return gmn;
}

