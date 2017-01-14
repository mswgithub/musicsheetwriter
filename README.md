# Music Sheet Writer

## Epitech Innovative Project

## Guide d'installation
Voici les instructions pour l'installation des différents outils pour pouvoir
développer sur le projet. Les differentes bibliothèques se trouvent dans 
le dépôt SVN dans /Software/bibliothèques. Vous devez d'abord décomprésser tous
les fichiers compréssé avant d'éxécuter les instructions ci-dessous

Pour compiler GuidoEngine :

	1. Installer CMake : https://cmake.org/download/.
	2. Créer un nouveau répertoire dans le dossier guidolib/build.
	3. À l'aide d'un terminal aller à l'emplacement du dossier précédemment créé.
	4. Taper : cmake .. -G "Le nom de votre compilateur" -DMIDIEXPORT=yes
		- Si vous utilisez Visual Studio 2013 le nom de votre compilateur devrait être : Visual Studio 12 2013 Win64.
		- En cas d'erreur CMake vous indique la liste de tous les compilateurs pris en compte.
	5. Aller dans guidolib/build/nomdossier et executer le VC++ Project GuidoEngine64 (si compilé avec Visual Studio).
	6. Générer le projet en allant dans Build > Générer la solution.
	7. Créer un dossier GuidoEngine et y copier les fichiers et dossiers suivants:
		- guidolib/build/win64/GUIDOEngine64D.lib
		- guidolib/build/win64/GUIDOEngine64D.dll
		- guidolib/src/engine/include

Pour compiler GuidoQt :

	1. Lancer le .pro qui se trouve dans guidolib/environments/Qt/libs/GuidoQt.
	2. Faire executer qmake. (Une erreur se produit si vous tentez de compiler le projet)
	3. Un nouveau dossier est créé dans guidolib/environments/Qt/libs ayant un nom du type *build-GuidoQt-....
	4. Executer le VC++ Project appelé GuidoQt (si votre compilateur est Visual Studio).
	5. Générer le projet en allant dans Build > Générer la solution.
	6. Créer un dossier du nom de GuidoQt et y copier les fichiers et dossiers suivants:
		- source/environments/Qt/libs/GuidoQt.lib
		- guidolib/source/environments/Qt/libs/GuidoQt/include

Pour compiler portAudio :

	1. Lancer portaudio.sln dans portaudio/build/msvc
	2. Copier le dossier ASIOSDK dans portaudio/src/hostapi/asio en renommant le dossier (le dossier DOIT s'appeller ASIOSDK)
	3. Compiler le projet depuis Visual Studio
	4. Si la compilation a fonctionné, passez à l'étape 7. En revanche, si vous avez une erreur indiquant que vous ne pouvez pas ouvrir le fichier ksguid.lib
		continuer à suivre la procedure normalement.
	5. Vous devez localiser le fichier ksguid.lib sur votre ordinateur. Cette lib est un fichier du SDK Windows 7.
		Il devrait se trouver probablement dans C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64.
		S'il n'y est pas, vous pouvez le télécharger ici: http://www.microsoft.com/en-us/download/confirmation.aspx?id=8279
	6. Dans Visual Studio, allez dans Projet > Propriétés > Propriétés de configuration > Editeur de liens > Entrée.
		Editez les Dépendances Supplémentaires en y ajouter le chemin de ksguid.lib
	7. Créer un dossier du nom de PortAudio et y copier les fichiers et dossiers:
		- portaudio/build/msvc/x64/Debug/portaudio_x64.lib
		- portaudio/build/msvc/x64/Debug/portaudio_x64.lib
		- portaudio/include

Pour midisharlight (requis pour l'export midi) le .lib se trouve dans le
dossier guidolib/src/midisharelight/win64. Il faut ensuite créer un dossier include
et y mettre les deux fichier .h qui se trouvent dans guido/src/midisharelight.

Pour compiler la Lib MusicXML:

	1. Recuperer le contenu de la lib MUsicXML contenu dans le svn (Software/libMusicXML)
	2. Dans le dossier de la libmusicXMl grace a un temrinal faites: pathDeVotreCmake\cmake.exe .\cmake\ -G "Visual Studio 12 2013 Win64"
	3. Des nouveaux fichiers ont eté créé, il faut ouvrir ALL_BUILD si vous avez utilisé Visual Studio
	4. Cliquer droit sur ALL_BUILD et generer.
	5. Des erreurs vont apparaitres, il faut simplement les regler.
		6. Dans le fichier transposition, renommer la fonction round en round2 et faites un ctrl-f, cherchez round et modifier la seul occurence de round en round2
		7. Il se peu aussi qu'un erreur pour un probleme d'include de afxres.h, remplacer le par windows.h.
	8. Regenerer
	9. Dans un dossier Debug (dans le dossier du dezzipage de la lib XML) il faut copier les fichiers libmusicxml2.*
	10. Il faut coller les fichiers dans le dossier LibMusicXML (Git repository)

Link des libs dans QtCreator :

	1. Dans le .pro s'il existe d'éventuelle link vers les librairies précédemment créés, supprimez les.
	2. Remplacer les éventuels dossiers des librairies guidoengine et guidoqt par ceux précédemment créés.
	3. Clique-droit sur le nom du projet -> ajouter une bibliothèque -> choisir bibliothèque externe.
	4. Fichier de bibliothèque : Cliquez sur Parcourir et aller chercher le .lib de notre librairie.
	5. Chemin d'inclusion : Cliquez sur Parcourir et aller chercher le dossier include de notre librairie
	6. Plateforme : Selectionnez uniquement Windows et laisser en Dynamique. Décocher les trois cases du bas.
	7. Répétez pour chaque librairies. Vous devriez avoir quelque chose de plus ou moins similaire :
		///// MusicSheetWriter.pro /////
		win32: LIBS += -L$$PWD/GuidoEngine/ -lGUIDOEngine64D
		INCLUDEPATH += $$PWD/GuidoEngine/include
		DEPENDPATH += $$PWD/GuidoEngine/include

		win32: LIBS += -L$$PWD/GuidoQt/ -lGuidoQt
		INCLUDEPATH += $$PWD/GuidoQt/include
		DEPENDPATH += $$PWD/GuidoQt/include

		win32: LIBS += -L$$PWD/GuidoEngine/midisharelight/ -lmidisharelight64
		INCLUDEPATH += $$PWD/GuidoEngine/midisharelight/include
		DEPENDPATH += $$PWD/GuidoEngine/midisharelight/include

Si c'est la première fois que vous compilez les libs, il est impératif d'installer
la police guido2. Sinon vous risquez d'avoir des problèmes d'affichages.

	1. Telecharger la derniere version à cette adresse : https://sourceforge.net/projects/guidolib/files/Qt%20Applications/Qt%20appls%20Win32/
	2. Décompresser le dossier
	3. Executer guido2.ttf se trouvant à la racine du dossier

## Team Member
- Jonathan Racaud: Team Leader
- Antoine Simon: iOS and Software developer
- Jeremy Harrault: Android and Software developer
- Simon Daguenet: Windows Phone and Software developer
- Julien Blondeel: Software Developer
- Florian Corradin: Web Site and Software developer

Email : musicsheetwriter@gmail.com
