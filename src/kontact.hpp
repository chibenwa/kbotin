/*!
* \file kontact.hpp
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

#ifndef KONTACT__HPP__
#define KONTACT__HPP__

#include <vector>
#include <QString>
#include <fstream>
#include <utility>

class Kontact
{
	public:
		Kontact();
		//Les fonctions de travail sur les attributs
		void setAttribute(QString, QString);
		void deleteAttribute(QString);
		void modifyAttribute(QString,QString);
		void save(std::ofstream*);
		void print(QString& p);
		void toDefault();
		QString findValue(QString);
		int size() const
		{
			return attributeList->size();
		}
		std::vector< std::pair<QString,QString> >* getAttributsList() const
		{
			return attributeList;
		}
		void renew()
		{
			image = "0";
			attributeList->clear();
		}
		QString getMail();
		bool operator==(const Kontact& plop)
		{
			return ( (*(this->attributeList))[0].second == (*(plop.attributeList))[0].second ) && ( (*(this->attributeList))[1].second == (*(plop.attributeList))[1].second );
		}
		bool operator<(const Kontact& plop);
		QString get_image() const
		{
			return image;
		}
		void set_image(const QString& i)
		{
			image = i;
		}
		int getId() const
		{
			return id;
		}
		void setId(int i)
		{
			id = i;
		}
		static void setCurrentId(int i)
		{
			currentId = i;
		}
		static int getCurrentId()
		{
			return currentId;
		}
		void updateId()
		{
			++currentId;
			id = currentId;
		}
	protected:
		static int currentId;
		int id;
		QString image;
		std::vector< std::pair<QString,QString> >* attributeList;
};

bool rel(Kontact*,Kontact*);

#endif 
