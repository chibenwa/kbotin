/*!
* \file cvs.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe permettant de travailler sur l'import / export de fichiers CVS types
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

#include "cvs.hpp"

#include <QStringList>
#include <fstream>
#include <string>
#include "cvsTemplate.hpp"
#include <QTranslator>

#include "mainWindow.hpp"

#include "divers.hpp"

/// debug
#include <iostream>

Cvs::Cvs()
{
	
}

void Cvs::imports(QString fileName, TypeCvs type, std::vector<Kontact*>* dest, MainWindow* w)
{
	debug( "getting started" , w );
	std::ifstream ifs(fileName.toStdString().c_str());
	std::string read;
	getline(ifs, read);
	std::cout << "        Reading : " << read << std::endl;
	QStringList headers = QString(read.c_str()).split(",");
	std::vector<QStringList> qsl;
	while( getline(ifs, read) )
	{
		std::cout << "        Reading : " << read << std::endl;
		qsl.push_back( QString(read.c_str()).split(",") );
	}
	int n = qsl.size();
	int i, j, k;
	int secu = headers.size();
	Kontact* kon;
	if(type == ThunderBird)
	{
		for( i=0; i<n; ++i)
		{
			k = qsl[i].size();
			kon = new Kontact;
			if(k>27)
			{
				kon->setAttribute( QTranslator::tr("Nom"), qsl[i].at(1) );
				kon->setAttribute( QTranslator::tr("Prénom"), qsl[i].at(0) );
				kon->setAttribute( QTranslator::tr("Adresse"), qsl[i].at(12) );
				kon->setAttribute( QTranslator::tr("Code Postal"), qsl[i].at(16) );
				kon->setAttribute( QTranslator::tr("Ville"), qsl[i].at(14) );
				kon->setAttribute( QTranslator::tr("Site"), qsl[i].at(27) );
				kon->setAttribute( QTranslator::tr("e-mail"), qsl[i].at(4) );
				kon->setAttribute( QTranslator::tr("Téléphone"), qsl[i].at(8) );
				kon->setAttribute( QTranslator::tr("Portable"), qsl[i].at(11) );
			}
			k = qsl[i].size();
			for( j=0; j<k; ++j)
			{
				if(j != 0 && j != 1 && j != 4 && j != 8 && j != 11 && j != 12 && j != 14 && j != 16 && j != 27 && j < secu)
				{
					if( qsl[i].at(j) != "" )
					{
						kon->setAttribute( headers.at(j), qsl[i].at(j) );
					}
				}
			}
			dest->push_back( kon );
		}
	}
	if(type == Google)
	{
		for( i=0; i<n; ++i)
		{
			k = qsl[i].size();
			kon = new Kontact;
			k = qsl[i].size();
			if(k>3)
				kon->setAttribute( QTranslator::tr("Nom"), qsl[i].at(3) );
			if(k>0)
				kon->setAttribute( QTranslator::tr("Prénom"), qsl[i].at(0) );
			if(k>35)
				kon->setAttribute( QTranslator::tr("Adresse"), qsl[i].at(35) );
			if(k>37)
				kon->setAttribute( QTranslator::tr("Code Postal"), qsl[i].at(37) );
			if(k>36)
				kon->setAttribute( QTranslator::tr("Ville"), qsl[i].at(36) );
			if(k>52)
				kon->setAttribute( QTranslator::tr("Site"), qsl[i].at(52) );
			if(k>28)
				kon->setAttribute( QTranslator::tr("e-mail"), qsl[i].at(28) );
			if(k>30)
				kon->setAttribute( QTranslator::tr("Téléphone"), qsl[i].at(30) );
			if(k>32)
				kon->setAttribute( QTranslator::tr("Portable"), qsl[i].at(32) );
			for( j=0; j<k; ++j)
			{
				if(j != 0 && j != 3 && j != 28 && j != 30 && j != 32 && j != 35 && j != 36 && j != 37 && j != 52 && j < secu)
				{
					if( qsl[i].at(j) != "" )
					{
						kon->setAttribute( headers.at(j), qsl[i].at(j) );
					}
				}
			}
			dest->push_back( kon );
		}
	}
	if(type == Outlook)
	{
		for( i=0; i<n; ++i)
		{
			k = qsl[i].size();
			kon = new Kontact;
			if(k>29)
			{
				kon->setAttribute( QTranslator::tr("Nom"), qsl[i].at(2) );
				kon->setAttribute( QTranslator::tr("Prénom"), qsl[i].at(0) );
				kon->setAttribute( QTranslator::tr("Adresse"), qsl[i].at(23) );
				kon->setAttribute( QTranslator::tr("Code Postal"), qsl[i].at(29) );
				kon->setAttribute( QTranslator::tr("Ville"), qsl[i].at(27) );
				kon->setAttribute( QTranslator::tr("Site"), qsl[i].at(6) );
				kon->setAttribute( QTranslator::tr("e-mail"), qsl[i].at(14) );
				kon->setAttribute( QTranslator::tr("Téléphone"), qsl[i].at(18) );
				kon->setAttribute( QTranslator::tr("Portable"), qsl[i].at(20) );
			}
			k = qsl[i].size();
			for( j=0; j<k; ++j)
			{
				if(j != 0 && j != 2 && j != 6 && j != 14 && j != 18 && j != 20 && j != 23 && j != 27 && j != 29 && j < secu)
				{
					if( qsl[i].at(j) != "" )
					{
						kon->setAttribute( headers.at(j), qsl[i].at(j) );
					}
				}
			}
			dest->push_back( kon );
		}
	}
} 

void Cvs::importsFromTemplate(QString filePath, QString templatePath, std::vector<Kontact*>* dest, MainWindow* w)
{
	CvsTemplate cvsTemplate( templatePath, w );
	cvsTemplate.imports(filePath, dest);
}

bool Cvs::exportsFromTemplate(QString filePath, QString templatePath, std::vector<Kontact*>* orig, MainWindow* w)
{
	CvsTemplate cvsTemplate( templatePath, w, false);
	if(cvsTemplate.isOk())
	{
		cvsTemplate.exports(filePath, orig);
		return true;
	}
	return false;
}
