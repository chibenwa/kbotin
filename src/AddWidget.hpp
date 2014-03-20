/*!
* \file AddWidget.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe affichant une fenêtre permettant la saisie d'un contact
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

#ifndef ADDWIDGET__HPP__
#define ADDWIDGET__HPP__

#include <QWidget>
#include <vector>
#include <QString>

class QLineEdit;
class QPushButton;
class QFormLayout;
class MainWindow;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QPUWithInt;

class AddWidget: public QWidget
{
	Q_OBJECT
	public:
		AddWidget(MainWindow*, int);
		void addKontact();
		void decrease()
		{
			--mN;
		}
	public slots:
		void AddAttribute();
		virtual void dell();
		void dellAtribute(int n);
		void chooseImage();
	signals:
		void dellDemande(int);
	void needUpdate();
	protected:
		int mN;
		MainWindow* mW;
		QFormLayout* formLayout;
		QVBoxLayout* mainLayout;
		QHBoxLayout* buttonLayout;
		QVBoxLayout* editLayout;
		QPushButton* mAddAttribute;
		QPushButton* mDelete;
		QString imageName;
		QLabel* imageLabel;
		QLabel* imageInstr;
		QPushButton* mChoose;
		QHBoxLayout* imageLayout;
		std::vector<QLineEdit*> values;
		std::vector<QLineEdit*> fields;
		std::vector<QPUWithInt*> QPUV;
		std::vector<QHBoxLayout*> tempV;
};

#endif 
