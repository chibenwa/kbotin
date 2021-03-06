/*!
* \file exportWidget.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe affichant une fenêtre permettant de choisir les options d'export des contacts
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

#ifndef EXPORTWIDGET__HPP__
#define EXPORTWIDGET__HPP__

#include <QWidget>
#include <vector>

class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QRadioButton;
class QLabel;
class QPushButton;
class Kontact;
class MainWindow;

class ExportWidget : public QWidget
{
	Q_OBJECT
	public:
		ExportWidget( MainWindow*, std::vector<Kontact*>* );
	public slots:
		void selectDestination();
		void submit();
		void selectTemplate();
		void selectVcfTemplate();
	protected:
		MainWindow* mW;
		QVBoxLayout* mainLayout;
		QLabel* title;
		QLabel* instr1;
		QHBoxLayout* destLayout;
		QLabel* destLabel;
		QPushButton* mDest;
		QLabel* instr2;
		QGroupBox* group;
		QVBoxLayout* groupLayout;
		QRadioButton* radioStandard;
		QLabel* info;
		QRadioButton* radioThunderBird;
		QRadioButton* radioOutlook;
		QRadioButton* radioGoogle;
		QRadioButton* radioKDE;
		QRadioButton* radioVCard;
		QPushButton* mOk;
		std::vector<Kontact*>* mVect;
		QHBoxLayout* cvsTemplateLayout;
		QRadioButton* radioCvsTemplate;
		QLabel* cvsTemplateLabel;
		QPushButton* mCvsTemplate;
		
		QHBoxLayout* vcfTemplateLayout;
		QRadioButton* radioVcfTemplate;
		QLabel* vcfTemplateLabel;
		QPushButton* mVcfTemplate;
};

#endif 
