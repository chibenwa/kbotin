/*!
* \file kontact.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe représentant un contact.
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

#include "kontact.hpp"

#include <utility>
#include <string>
#include <iostream>
#include <QTranslator>

int Kontact::currentId = 0;

Kontact::Kontact()
{
	image = "0";
	attributeList = new std::vector< std::pair<QString,QString> >;
}

void Kontact::toDefault()
{
	image = "0";
	attributeList->clear();
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Nom"),"Dupont") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Prénom"),"Georges") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Adresse"),"1 rue du Pont") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Code Postal"),"45896") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Ville"),"Sarges") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Téléphone"),"0123456789") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Portable"),"0612345789") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("e-mail"),"dupont@geo.fr") );
	attributeList->push_back( std::pair<QString,QString>(QTranslator::tr("Site"),"geo.fr") );
}

void Kontact::setAttribute(QString nomAtt, QString val)
{
	int i;
	int n = attributeList->size();
	bool tut = false;
	for(i=0; i <n; ++i)
	{
		if( (*attributeList)[i].first == nomAtt )
		{
			(*attributeList)[i].second = val;
			tut = true;
		}
	}
	if(!tut)
	{
		attributeList->push_back( std::pair<QString,QString>(nomAtt, val) );
	}
	n = attributeList->size();
}

void Kontact::deleteAttribute(QString nomAtt)
{
	int i;
	int n = attributeList->size();
	for(i=0; i <n; ++i)
	{
		if( (*attributeList)[i].first == nomAtt )
		{
			attributeList->erase(attributeList->begin()+i);
			return;
		}
	}
}

void Kontact::save(std::ofstream* ifs)
{
	int i;
	int n = attributeList->size();
	(*ifs) << id << std::endl;
	(*ifs) << image.toStdString() << std::endl;
	for(i = 0; i < n; ++i)
	{
		(*ifs) << (*attributeList)[i].first.toStdString() << std::endl;
		(*ifs) << (*attributeList)[i].second.toStdString() << std::endl;
	}
	(*ifs) << "." << std ::endl;
}

void Kontact::print(QString& p)
{
	int i;
	int n = attributeList->size();
	p += "<b>"+(*attributeList)[0].second+" "+(*attributeList)[1].second+"</b><br/>";
	for(i = 2; i < n; ++i)
	{
		if( (*attributeList)[i].second != "" )
		p += (*attributeList)[i].first+" : "+(*attributeList)[i].second + "<br/>";
	}
	p+="<br/>";
}

QString Kontact::getMail()
{
	return (*attributeList)[6].second;
}

QString Kontact::findValue(QString att)
{
	int i;
	int n = attributeList->size();
	for(i=0; i<n; ++i)
	{
		if( att == (*attributeList)[i].first )
		{
			return (*attributeList)[i].second;
		}
	}
	return "";
}

bool Kontact::operator<(const Kontact& plop)
{
	if( (*(this->attributeList))[0].second == (*(plop.attributeList))[0].second )
	{
		return (*(this->attributeList))[1].second.localeAwareCompare((*(plop.attributeList))[1].second) < 0 ;
	}
	else
	{
		return  (*(this->attributeList))[0].second.localeAwareCompare((*(plop.attributeList))[0].second) < 0 ;
	}
}

bool rel(Kontact* k1,Kontact* k2)
{
	return (*k1) < (*k2);
}
