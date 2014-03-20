/*!
* \file divers.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Modules de diverses fonctions
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

#ifndef DIVERS__HPP__
#define DIVERS__HPP__

#include <QString>
#include <QStringList>
#include <vector>
#include <utility>

class MainWindow;

void debug(QString s,MainWindow* w = 0);

void errorMessage(QString s);

void removingSpaces(QString& Read);

bool isThereSomethingAfterSpaces( QString Read );

bool contains(const std::vector<int>& vect, int a);

bool contains(const std::vector<QString>& vect, QString a);

bool contains(const std::vector<std::pair<int, QString> >& plop, int a);

QString memberOf(const std::vector< std::pair<QString, QString> >& corr, QString& val);

QString traitement(QString q);

QString quoter(QString q, bool b);

QString r3(QString q);

#endif 
