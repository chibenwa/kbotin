/*!
* \file group.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe définissant un groupe de contact par leur id
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

#include "group.hpp"

#include "divers.hpp"

#include <string>

int Group::id_def = -1;

Group::Group()
{
	id = -1;
	mVect = new std::vector<int>;
	included = new std::vector<int>;
	mName = "";
	image = "0";
}

bool Group::isInGroup(int a)
{
	int i; 
	int n = mVect->size();
	for(i=0; i<n; ++i)
	{
		if((*mVect)[i] == a)
			return true;
	}
	return false;
}

void Group::remove(int a)
{
	int i;
	int n = mVect->size();
	for(i=0; i<n; ++i)
		if((*mVect)[i] == a)
		{
			mVect->erase(mVect->begin()+i);
			return;
		}
}

void Group::save(std::ofstream* ofs)
{
	int i;
	int n = mVect->size();
	(*ofs) << id << std::endl;
	(*ofs) << mName.toStdString() << std::endl;
	(*ofs) << image.toStdString() << std::endl;
	for(i=0; i<n; ++i)
		(*ofs) << (*mVect)[i] << std::endl;
	(*ofs) << "-" << std::endl;
	n = included->size();
	for(i=0; i<n; ++i)
		(*ofs) << (*included)[i] << std::endl;
	(*ofs) << "." << std::endl;
}

Group::~Group()
{
	emit deleteMe(this);
}

void Group::removeInclusion(int a)
{
	int i;
	int n = included->size();
	for(i=0; i<n; ++i)
	{
		if( (*included)[i] == a)
		{
			included->erase(included->begin()+i);
			return;
		}
	}
}

void Group::addInclusion(int a)
{
	if(a != id && !contains( (*included), a) )
	{
		included->push_back(a);
	}
}
