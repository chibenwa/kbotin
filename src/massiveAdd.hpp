/*!
* \file massiveAdd.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre d'ajout massif de contacts
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

#ifndef MASSIVEADD__HPP__
#define MASSIVEADD__HPP__

#include <QWidget>
#include <vector>

class AddWidget;
class QVBoxLayout;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class MainWindow;

class MassiveAdd: public QWidget
{
	Q_OBJECT
	public:
		MassiveAdd(MainWindow*);
	public slots:
		void add();
		void submit();
		void dell(int);
	signals:
	protected:
		MainWindow* mW;
		QVBoxLayout* mainLayout;
		QHBoxLayout* buttonLayout;
		QHBoxLayout* editLayout;
		QPushButton* mAdd;
		QPushButton* mSubmit;
		std::vector<AddWidget*> widgetVect;
		int nbAddW;
};

#endif
