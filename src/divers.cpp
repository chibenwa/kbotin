/*!
* \file divers.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Diverses fonctions
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

#include "divers.hpp" 

#include <iostream>
#include <QString>
#include <QMessageBox>
#include "mainWindow.hpp"

void debug(QString s, MainWindow* w)
{
	if( (w != 0 && w->getPref()->debugger ) || w == 0 )
		std::cout <<"Debug :--: "<< s.toStdString() << std::endl;
}

void errorMessage(QString s)
{
	std::cout << "Error <--> " << s.toStdString() << std::endl;
	QMessageBox::critical(0,"Erreur",s);
}

void removingSpaces(QString& Read)
{
	while( Read.startsWith(" ") || Read.startsWith("	") )
	{
		Read.remove(0,1);
		debug("removing spacing caracter");
	}
}

bool isThereSomethingAfterSpaces( QString Read )
{
	removingSpaces( Read );
	return Read[0] == '}';
}

bool contains(const std::vector<int>& vect, int a)
{
	int i;
	int n = vect.size();
	for(i=0; i < n; ++i)
	{
		if( vect[i] == a )
			return true;
	}
	return false;
}

bool contains(const std::vector<QString>& vect, QString a)
{
	int i;
	int n = vect.size();
	for(i=0; i < n; ++i)
	{
		if( vect[i] == a )
			return true;
	}
	return false;
}

QString memberOf(const std::vector< std::pair<QString, QString> >& corr, QString& val)
{
	int n = corr.size();
	int i;
	for(i=0; i<n; ++i)
	{
		if( corr[i].first == val )
		{
			debug("Assimilation of "+val+" in "+corr[i].second);
			return corr[i].second;
		}
	}
}

bool contains(const std::vector<std::pair<int, QString> >& plop, int a)
{
	int i;
	int n = plop.size();
	for(i=0; i < n; ++i)
	{
		if( plop[i].first == a )
			return true;
	}
	return false;
}

QString traitement(QString q)
{
	if( q[0]=='"' && q[q.size()-1] == '"')
	{
		q.remove(q.size()-1,1);
		q.remove(0,1);
	}
	return q;
}

QString quoter(QString q, bool b)
{
	if(b)
	{
		q.insert(0,"\"");
		q.insert(q.size(), "\"");
	}
	return q;
}

QString r3(QString q)
{
	q.remove(0,3);
	return q;
}
