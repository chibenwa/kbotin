/*!
* \file main.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fonction principale
*
* Le programme suivant est distribué sous la license Creative Commun (Attribution - Partage
*       dans les Mêmes conditions 3.0 France)
*
*       Ceci signifie que vous êtes libres de :
*               - partager (reproduire, distribuer et communiquer le présent contenu )
*               - remixer ( adapter ce contenu )
*       Selon les conditions suivantes :
*               - attribution : Vous devez attribuer l'oeuvre de la manière indiquée par l'auteur
*               (mais pas d'une manière qui suggère qu'il vous approuve)
*               Ici, il s'agit d'ajoutéer la mention : "D'après le travail de Mr Tellier Benoit"
*               partout ou la présente license est mentionnée. Vous pouvez bien entendu y ajouter
*               vos commentaires.
*               - partage selon les mêmes conditions.
*       Comprenant bien que :
*               - Chacunes des conditions suivantes peut être levée si vous obtenez l'autorisation
*               du titulaire des droits.
*               - Les droits suivants ne sont en aucun cas affecté par la licence:
*                       - fair use (prérogatives issues des exceptions et limitations
*                       aux droits exclusifs
*                       - Les droits moraux de l'auteur
*               - A chaque réutilisation / distribution de cette oeuvre, la présente licence
*               Doit apparaitre clairement.
*       Plus d'informations sur http://creativecommons.org/licences/by-sa/3.0/fr
*/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QString>
#include <ctime>
#include <cstdlib>
#include <QIcon>
#include <QPixmap>

#include <QPushButton>
#include <QString>

#include "mainWindow.hpp"

#include "paths.hpp"

int main(int argc, char **argv)
{
	srand(time(0));
	
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	
	QApplication app(argc, argv);
        QApplication::setApplicationName("KBotin");
    
        QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator *translator = new QTranslator;
	translator->load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	translator->load( QString( COMMON_RESSOURCES_PATH ) + QString("trad/kbotin_") + locale + QString(".qm"));
	app.installTranslator(translator);
	
	app.setWindowIcon( QIcon( QPixmap( QString( COMMON_RESSOURCES_PATH ) + "media/address_book_last.png") ) );
    
	MainWindow l;
	l.show();
	
	return app.exec();
}
