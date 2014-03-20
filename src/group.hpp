/*!
* \file group.hpp
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

#ifndef GROUP__HPP__
#define GROUP__HPP__

#include <vector>
#include <QString>
#include <fstream>
#include <QObject>

class Group: public QObject
{
	Q_OBJECT
	public:
		Group();
		~Group();
		void setName(const QString& name)
		{
			mName = name;
		}
		QString getName() const
		{
			return mName;
		}
		bool isInGroup(int);
		void remove(int);
		void add(int a)
		{
			if(!isInGroup(a))
				mVect->push_back(a);
		}
		void save(std::ofstream*);
		std::vector<int>* getVect() const
		{
			return mVect;
		}
		void setImage(const QString& q)
		{
			image = q;
		}
		QString getImage() const
		{
			return image;
		}
		static void setIdDef(int a)
		{
			id_def = a;
		}
		void newId()
		{
			++id_def;
			id = id_def;
		}
		void setId(int a)
		{
			id=a;
		}
		int getId() const
		{
			return id;
		}
		static int getIdDef()
		{
			return id_def;
		}
		void addInclusion(int a);
		void removeInclusion(int a);
		std::vector<int>* getInclusion() const
		{
			return included;
		}
	public slots:
	signals:
		void deleteMe(Group*);
	protected:
		static int id_def;
		int id;
		QString mName;
		QString image;
		std::vector<int>* mVect;
		std::vector<int>* included;
};

#endif 
