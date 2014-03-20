/*!
* \file mainWindow.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre principale
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

#include "mainWindow.hpp"

#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QScrollArea>
#include "Preference.hpp"
#include "server.hpp"
#include "kontact.hpp"
#include "group.hpp"
#include "actionGroup.hpp"

#include <QMenuBar>
#include <QString>
#include "toolBar.hpp"
#include <fstream>
#include <utility>
#include <string>
#include "AllWidget.hpp"
#include "AddWidget.hpp"
#include "massiveAdd.hpp"
#include "modifyWidget.hpp"
#include <QLabel>
#include <QApplication>
#include "preferencesWidget.hpp"
#include "queueWidget.hpp"
#include "socket.hpp"
#include "exportWidget.hpp"
#include "importWidget.hpp"
#include "doublonWidget.hpp"
#include "groupWidget.hpp"
#include "groupMng.hpp"
#include "addKToGroup.hpp"
#include <cstdlib>
#include <algorithm>
#include <QFile>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include "consultConfig.hpp"
#include "distantWidget.hpp"

#include "paths.hpp"

//Debugg
#include <iostream>
#include "divers.hpp"
#include <unistd.h>

MainWindow::MainWindow():QMainWindow()
{
	this->chek_install();
	up = false;
	//instanciation des conteneurs
	attributs = new std::map<QString,int>;
	kontacts = new std::vector<Kontact*>;
	groups = new std::vector<Group*>;
	//Une histoire de menus
	mFichier = menuBar()->addMenu(tr("&Fichier"));
	mPartage = menuBar()->addMenu(tr("&Partage"));
	mGroups = menuBar()->addMenu(tr("&Groupes"));
	mHelp = menuBar()->addMenu(tr("&Aide"));
	aPref = new QAction(tr("&Préférences"), this);
	aQuitter = new QAction(tr("&Quitter"), this);
	aViewQueue = new QAction(tr("&File d'attente"), this);
	aDist = new QAction(tr("&Envoyer massivement"), this);
	aHelp = new QAction(tr("&Aide"), this);
	aExport = new QAction(tr("&Sauvegarder / &Exporter"), this);
	aImport = new QAction(tr("&Importer"), this);
	aEmpty = new QAction(tr("&Vider la file d'attente"), this);
	aDoublon = new QAction(tr("&Rechercher les doublons"), this);
	aGroupMng = new QAction(tr("&Gestion des groupes de contacts"), this);
	aPrint = new QAction(tr("&Imprimer"), this);
	aConsult = new QAction(tr("&Consulter un KBotin distant"), this);
	mFichier->addAction(aPref);
	mFichier->addSeparator();
	mFichier->addAction(aExport);
	mFichier->addAction(aImport);
	mFichier->addSeparator();
	mFichier->addAction(aDoublon);
	mFichier->addSeparator();
	mFichier->addAction(aPrint );
	mFichier->addSeparator();
	mFichier->addAction(aQuitter);
	mPartage->addAction(aEmpty);
	mPartage->addAction(aViewQueue);
	mPartage->addSeparator();
	mPartage->addAction(aDist);
	mPartage->addSeparator();
	mPartage->addAction(aConsult);
	mGroups->addAction(aGroupMng );
	mGroups->addSeparator();
	mHelp->addAction(aHelp);
	connect(aQuitter, SIGNAL( triggered() ), qApp, SLOT( quit() ) );
	connect(aHelp, SIGNAL( triggered() ), this, SLOT( help() ) );
	connect(aPref, SIGNAL( triggered() ), this, SLOT( settings() ) );
	connect(aViewQueue, SIGNAL( triggered() ), this, SLOT( showQueue() ) );
	connect(aExport, SIGNAL( triggered() ), this, SLOT( askMassiveExports() ) );
	connect(aDist, SIGNAL( triggered() ), this, SLOT( askMassiveSend() ) );
	connect(aImport, SIGNAL( triggered() ), this, SLOT( import() ) );
	connect(aEmpty, SIGNAL( triggered() ), this, SLOT( emptyQueue() ) );
	connect(aDoublon, SIGNAL( triggered()), this, SLOT( doublon_search() ) );
	connect(aGroupMng, SIGNAL( triggered()), this, SLOT( groupMng() ) );
	connect(aPrint, SIGNAL( triggered() ), this, SLOT( emitPrintRequest() ) );
	connect(aConsult, SIGNAL( triggered() ), this, SLOT( consultConfig() ) );
	//On initialise la file d'attente...
	queue = new std::vector<Kontact*>;
	//On charge les contacts...
	this->load();
	this->createGroupsAction();
	//Et si on faisait des widgets dans un layout?
	scroll = new QScrollArea;
	scroll->setWidgetResizable(true);
	mainLayout = new QVBoxLayout;
	mTool = new ToolBar(this);
	mainLayout->addWidget(mTool);
	workingSpace = new AllWidget(kontacts, this);
//	workingSpace = new AddWidget(this);
//	workingSpace = new MassiveAdd(this);
//	workingSpace = new ModifyWidget(this, (*kontacts)[0] );
//	workingSpace = new QueueWidget(this, kontacts);
//	workingSpace = new GroupWidget((*groups)[0], this);
//	workingSpace = new GroupMng(groups, this);
//	workingSpace = new AddKToGroup((*groups)[0], this);
	centralWidget = new QWidget;
	mainLayout->addWidget(scroll);
	scroll->setWidget(workingSpace);
	centralWidget->setLayout(mainLayout);
	this->setCentralWidget(centralWidget);
	//Gros kiki
	constructSerchEngine();
	//Et on charge les préférences...
	mPref = new Preference;
	this->loadPref();
	networking = mPref->networking;
	oldPort = mPref->distPort;
	oldAddresse = mPref->address;
	if(networking)
	{
		server = new Serveur(this);
		server->listenWeb();
		can_send = true;
	}
	else
		can_send = false;
	aDist->setEnabled(can_send);
	can_export = true;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::load()
{
	// chargement des contacts
	this->basicLoad(QString( PERSONNAL_RESSOURCES_PATH ) + ".data/contactList.txt",kontacts);
	this->basicLoad(QString( PERSONNAL_RESSOURCES_PATH ) + ".data/queue.txt",queue);
	// relatif aux groupes
	this->loadGroups();
	sort( kontacts->begin(), kontacts->end(), rel);
}

void MainWindow::save()
{
	// sauvegarde des contacts
	this->basicSave(QString( PERSONNAL_RESSOURCES_PATH ) + "/.data/contactList.txt", kontacts);
	this->saveGroups();
	emit needSource();
}

void MainWindow::showContact()
{
	// montre les contacts
	delete workingSpace;
	workingSpace = new AllWidget(kontacts, this);
	scroll->setWidget(workingSpace);
	this->update();
	if( mPref->networking )
	{
		can_send = true;
		aDist->setEnabled(true);
	}
	can_export = true;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::add()
{
	//Interface d'ajout des contacts
	delete workingSpace;
	workingSpace = new MassiveAdd(this);
	scroll->setWidget(workingSpace);
	this->update();
	can_send = false;
	aDist->setEnabled(can_send);
	can_export = false;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::modify(Kontact* kon)
{
	//Permet de modifier un contact... (widget)
	delete workingSpace;
	workingSpace = new ModifyWidget(this,kon);
	scroll->setWidget(workingSpace);
	this->update();
	can_send = false;
	aDist->setEnabled(can_send);
	can_export = false;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::addAttribute(QString att)
{
	//Ajout d'un attribut à la liste des attributs.
	if(attributs->find(att) == attributs->end())
	{
		(*attributs)[att] = 0 ;
		up = true;
	}
}

MainWindow::~MainWindow()
{
	saveGroups();
	delete aHelp;
	delete aQuitter;
	delete mFichier;
	delete mPartage;
	delete mHelp;
	delete mGroups;
	delete mainLayout;
	delete aViewQueue;
	delete aDist;
	delete centralWidget;
	delete aPref;
	delete aEmpty;
	delete aDoublon;
	delete aGroupMng;
	this->deleteSearchEngine();
	int i;
	int n = groups->size();
	for(i=0; i<n; ++i)
		delete (*groups)[i];
	n = kontacts->size();
	for(i=0; i<n; ++i)
		delete (*kontacts)[i];
	n = queue->size();
	for(i=0; i<n; ++i)
		delete (*queue)[i];
	delete queue;
	delete kontacts;
	delete groups;
}

void MainWindow::addKontact(Kontact* kon)
{
	kontacts->push_back(kon);
}

void MainWindow::constructSerchEngine()
{
	//Cette fonction parcours l'ensemble des contacts et construit un indx de recherche propre!
	//A n'utiliser qu'une fois!!!
	int i;
	int j;
	int k;
	std::map<QString, std::vector<Kontact*>* >* myMap;
	std::vector< std::pair<QString,QString> >* att;
	int n = kontacts->size();
	for(i=0; i<n; ++i)
	{
		//on parcours l'ensemble des contacts...
		att = (*kontacts)[i]->getAttributsList();
		k=att->size();
		for(j=0; j<k; ++j)
		{

			/// Magic code starting here ...
			if( searchEngine.find( (*att)[j].first ) == searchEngine.end() )
			{
				myMap = new std::map<QString, std::vector<Kontact*>* >;
				searchEngine[(*att)[j].first] = myMap;
			}
			
			if( (searchEngine[(*att)[j].first])->find( (*att)[j].second ) == (searchEngine[(*att)[j].first])->end() )
			{
				(*(searchEngine[(*att)[j].first]))[(*att)[j].second] = new std::vector<Kontact*>;
			}
			(*(searchEngine[(*att)[j].first]))[(*att)[j].second]->push_back((*kontacts)[i]);
			///end of magic code...
		}
	}
}

void MainWindow::deleteSearchEngine()
{
	std::map<QString, std::map<QString,std::vector<Kontact*>* >* >::iterator it;
	for(it = searchEngine.begin(); it != searchEngine.end(); it++)
	{
		std::map<QString, std::vector<Kontact*>* >::iterator itbis;
		for( itbis = it->second->begin(); itbis != it->second->end(); itbis++)
		{
			delete itbis->second;
		}
		delete it->second;
	}
}

void MainWindow::search()
{
	QString field = mTool->getField();
	QString value = mTool->getValue();
	if(searchEngine[field]->find(value) != searchEngine[field]->end() )
	{
		delete workingSpace;
		workingSpace = new AllWidget((*(searchEngine[field]))[value], this);
		scroll->setWidget(workingSpace);
		this->update();
	}
	else
	{
		delete workingSpace;
		QLabel* lab = new QLabel;
		lab->setText(tr("<h2>Aucun résultat trouvé</h2>"));
		workingSpace = lab;
		scroll->setWidget(workingSpace);
		this->update();
		
	}
}

void MainWindow::updateSearchEngine()
{
	std::cout << "updating search engine" << std::endl;
	searchEngine.clear();
	constructSerchEngine();
	sort( kontacts->begin(), kontacts->end(), rel);
}

void MainWindow::deleteKontact(Kontact* kon, std::vector<Kontact*>* vect)
{
	int i;
	int n = kontacts->size();
	for(i=0; i<n; ++i)
	{
		if( (*kontacts)[i] == kon )
		{
			kontacts->erase(kontacts->begin()+i);
			QFile f;
			f.setFileName(kon->get_image());
			f.remove();
			delete kon;
			updateSearchEngine();
			emit needSource();
			this->save();
			break;
		}
	}
	if( vect->size() > 0 )
	{
		delete workingSpace;
		workingSpace = new AllWidget(vect, this);
		scroll->setWidget(workingSpace);
		this->update();
		if( mPref->networking )
		{
			can_send = true;
			aDist->setEnabled(true);
		}
		can_export = true;
		aExport->setEnabled(can_export);
		aPrint->setEnabled(can_export);
	}
	else
		showContact();
}

void MainWindow::help()
{
	delete workingSpace;
	workingSpace = new QWidget;
	scroll->setWidget(workingSpace);
	this->update();
	QVBoxLayout* ooo = new QVBoxLayout;
	QLabel* lab = new QLabel;
	lab->setText(tr("<h1><u>Aide</u></h1>Bienvenue dans ce programme de gestion des contacts!<br/>\
	La page de base permet la visualisation de l'ensemble de vos contacts. Vous <br/>\
	pouvez voir leurs noms et prénoms. Cliquer sur le bouton + permet de voir l'ensemble <br/>\
	du contact et d'accéder aux boutons de suppression et de modification du contact.<br/>\
	Le bouton - qui apparait alors permet de replier le contact.<br/>\
	Vous pouvez alors modifier et supprimer le contact via les boutons prévus à cet effet.<br/>\
	Vous pouvez en plus lier une image à ce contact.\
	<br/><br/>\
	<h3>Ajout de contacts</h3>\
	Pour celà, il suffi de cliquer sur le bouton add de la barre d'outils. Il vous ouvre une<br/>\
	interface permettat l'édition simultanée de plusieurs contacts. Vous pouvez de plus <br/>\
	leurs ajouter autant d'attributs que vous le désirez. Cliquez ensuite sur soummettre<br/>\
	pour les ajouter à votre ensemble de contacts.<br/><br/>\
	<h3>Barre de recherche</h3>\
	Vous pouvez taper une recherche dans la barre de recherche du programme. Pour celà<br/>\
	selectionnez le champs souhaité, puis la valeur correspondante. Les ontacts ayant <br/>\
	cette valeur pour ce champs seront alors affichés. Vous pouvez de plus envoyer un <br/>\
	mail à ce contact (nécéssite que thunderbird soit installé).<br/><br/>\
	<h3>Transfert de contact via internet</h3>\
	Ce programme vous offre la possibilité  d'échanger vos contacts entre différents ordinateurs.<br/>\
	Il vous faudra pour celà utiliser plusieurs outils :<br/><br/>\
	- Il vous faut déjà accepter de recevoir des contacts si tel est ce que vous voulez faire...<br/>\
	Pour cela, je vous invite à vous rendre dans les préférences du menu 'fichier'.<br/><br/>\
	- Suite à cela, vous pouvez visualiser les contacts que vous avez reçu et les modérer.<br/><br/>\
	- Pour envoyer des contacts, il vous suffit quand vous les regardez, d'appuyer sur le <br/>\
	bouton 'envoyer' après avoir règlé de même la configuration d'envoi.<br/><br/>\
	-Une options d'envoi massif est disponible. Elle se situe dans le menu fichier, et est<br/>\
	intitulée 'Envoyer massivement'. Elle permet d'envoyer l'ensemble des contacts affichés<br/><br/>\
	Par ailleurs vous pouvez aussi consulter vos contacts à distance. Pour celà, activez la bonne<br/>\
	option. Cette consultation à distance se fait via un mot de passe, que vous pouvez modifier.<br/>\
	Par défaut il s'agit de: plop<br/>\
	Vous ne pouvez ni supprimer ni modifier des contacts consultés à distance, mais vous pouvez <br/>\
	les imprimer, envoyer, exporter, leur envoyer des mails...<br/><br/>\
	<h3>Groupes de contacts</h3>\
	Vous pouvez former des groupes de contacts. Un contact peut appartenir à plusieurs groupes.<br/>\
	Ces groupes sont directement accessibles via le menu groupes. Une fenêtre peut être ouverte <br/>\
	via le menu fichier pour ajouter ou supprimer des groupes. Vous pouvez envyer un mail à <br/>\
	l'ensemble des contacts d'un groupe.<br/><br/>\
	Les groupes de contacts peuvent être inclus les uns dans les autres. Ainsi si le groupe A<br/>\
	inclus dans B est modifié, le groupe B subira alors ces modifications. Les groupes sont <br/>\
	protégés contre les inclusions circulaires. Une visualisation des inclusions est fournie <br/>\
	dans la fenêtre de gestion des groupes. Vous pouvez ajouter ou enlever des inclusions de<br/>\
	groupe via la fenêtre d'ajout de contacts.<br/><br/>\
	<h3>Exportation</h3>\
	Les options d'exportation vous permettent d'enregistrer vos contact dans un fichier (format<br/>\
	standard). Elles sont disponible dans le menu fichier, onglet exportation.<br/><br/>\
	Les autres options d'exports vous font perdre de l'informations mais sont compatibles <br/>\
	avec d'autres applications (Google+, ThunderBird, Outlook et KAdressBook).<br/><br/>\
	Dans tous les cas sélectionnez le fichier de destination ainsi que l'option choisie.<br/><br/>\
	<h3>Importation</h3>\
	Les options d'importations vous permettent de charger des contacts avec des fichiers <br/>\
	produits par d'autres applications de gestion de contacts, à savoir Google+, ThunderBird,<br/>\
	Outlook et KAdressBook. Elles sont disponible dans le menu fichier, onglet importation.<br/>\
	Dans tous les cas sélectionnez le fichier source ainsi que l'option choisie.<br/><br/>\
	<h2>Aide avancées</h2>\
	<h3> Templates d'import au format CVS</h3>\
	Ces templates permetent, via le langage que je vais vous expliquer, d'importer <br/>\
	potentiellement toute donnée au format CVS.<br/><br/>\
	Ce langage comporte 4 mots clé. Il contiennent des valeurs séparées par des retours <br/> \
	à la ligne au sein d'accolades. Ces valeurs peuvent aller par paire, ou être solitaires.<br/>\
	Voici donc les fameux 4 mots clés : <br/>\
	- <b>value</b> : (fonctionne par couple de valeurs) Il permet de définir le nom des attributs<br/>\
	 en fonction de leur colonne. Ces attributs sont ajoutés avant tout autre, dans leur<br/>\
	ordre de déclaration. Pensez de plus à définir via ce moyen les 8 attributs nécéssaire <br/>\
	 au bon fonctionnement de l'application, et dans le bon ordre... A savoir Nom, Prénom,<br/>\
	  Adresse, Code Postal, Ville, Site, e-mail, Téléphone, Portable. <br/> \
	- <b>forbid</b> : Interdit l'import de valeurs en fonction de leur position (entier) <br/> \
	- <b>forbidvalue</b> : Interdit l'import des colonnes ayant un valeur spécifiée ici. <br/> \
	- <b>assimilate</b> : (fonctionne par couple de valeurs) Lors de l'import, les colonnes <br/>\
	ayant pour valeur associée une chaine ici mentionnée sera remplacée par la chaine<br/>\
	 suivante dans la déclaration.<br/> \
	<br/> \
	Autres astuces : Une accolade se referme en début de ligne (afin d'autoriser une <br/> \
	compatibilité maximum... <br/><br/>\
	Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire <br/> \
	contenant l'éxécutable à cette position : <br/>\
	../exemples/templateCVS/import/exemple.template<br/><br/>\
	<h3> Templates d'export au format CVS</h3>\
	Ces templates permetent, via le langage que je vais vous expliquer, d'exporter <br/>\
	potentiellement toute donnée au format CVS.<br/><br/>\
	Ce language comporte un seul mot clé : combinaisons <br/>\
	Il est suivi de paires de valeurs séparées par des retours à la ligne au sein <br/>\
	d'accolades. La première valeur indique le nom de la colonne CVS, la deuxième <br/>\
	indique le nom de l'attribut à utiliser pour remplir la colonne.<br/>\
	Concernant celle ci, deux valeurs sont à noter : <br/>\
	- \"\" indique de laisser la colonne vide<br/>\
	- &#8249;-- force les valeurs contenues dans la colonne à être égales à la chaine de <br/>\
	caractère la succédant directement.<br/><br/>\
	Par ailleurs pour quoter les valeurs, rajouter simplement la directive <b>quote;</b><br/>\
	en dehors des accolades. Vous pouvez de plus via <b>coma;</b> forcer l'ajout d'une virgule en <br/>\
	fin de chaque ligne, ou via <b>comafields;</b> en fin de chaque ligne de donnée.<br/><br/>\
	Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire <br/> \
	contenant l'éxécutable à cette position : <br/>\
	../exemples/templateCVS/export/exemple.template<br/><br/>\
	<h3>Template d'export au language CVF</h3>\
	Ces templates permetent, via le langage que je vais vous expliquer, d'exporter <br/>\
	potentiellement toute donnée au format VCF.<br/><br/>\
	Ce language comporte un seul mot clé : <b>newline</b> <br/>\
	Vous pouvez trouver dans le dossier media une page html explicant comment fonctionne<br/>\
	le format VCF 3.0<br/>\
	Cette lecture (courte) est requise pour bien appréhender ce qui suit.<br/><br/>\
	Chaque block newline décrit une politique à adopter pour la ligne suivante pour un<br/>\
	contact. La première ligne caractérise le nom de l'attribut VCF (avant les : ) <br/>\
	Les lignes d'après caractérisent la valeur de cet attribut.<br/>\
	&#8249;-- force la valeur qui le suit<br/>\
	Rajouter une ligne avec un = pour ne pas ajouter des ;<br/>\
	Une ligne sans ces deux valeurs se verra remplacée par la valeur de l'attribut <br/>\
	lui correspondant chez le contact. \"\" correspond à une valeur nulle.<br/>\
	Il est nécéssaire de spécifier la version du format VCARD utilisé.<br/>\
	Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire <br/> \
	contenant l'éxécutable à cette position : <br/>\
	../exemples/templateVCF/export/exemple.template<br/><br/>\
	<h3>Template d'import au language CVF</h3>\
	Ces templates permetent, via le langage que je vais vous expliquer, d'importer <br/>\
	potentiellement toute donnée au format VCF.<br/><br/>\
	Vous pouvez trouver dans le dossier media une page html explicant comment <br/>\
	fonctionne le format VCF 3.0. Cette lecture (courte) est requise pour bien <br/>\
	appréhender ce qui suit.<br/>\
	Chaque block <b>ref</b> décrit une politique à adopter.La première ligne <br/>\
	caractérise le nom de l'attribut VCF (avant les : ). La politique sera<br/>\
	appliquée quand l'attrbut VCF sera rencontré... Les lignes d'après décrivie<br/>\
	cette politique. Elles fonctionnent par paire : On indique d'abord le séparateur<br/>\
	indiquant le champs de lecture à l'aide de <b>sep:</b> suivi du dit séparateur. Ensuite<br/>\
	 suit l'attribut kbotin auquel on souhaite associer cette valeur. Notons ici de<br/>\
	  \"\" constitue la valeur nulle.<br/><br/>\
	Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire <br/> \
	contenant l'éxécutable à cette position : <br/>\
	../exemples/templateVCF/import/exemple.template<br/><br/>\
	"));
	lab->setTextInteractionFlags( Qt::TextSelectableByMouse );
	ooo->addWidget(lab);
	workingSpace->setLayout(ooo);
	can_send = false;
	can_export = false;
	aDist->setEnabled(can_send);
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::settings()
{
	delete workingSpace;
	workingSpace = new PreferencesWidget(mPref, this);
	scroll->setWidget(workingSpace);
	this->update();
	can_send = false;
	aDist->setEnabled(can_send);
	can_export = false;
	aPrint->setEnabled(can_export);
	aExport->setEnabled(can_export);
}

void MainWindow::loadPref()
{
	using namespace std;
	QString pat = QString( PERSONNAL_RESSOURCES_PATH ) + ".data/settings.txt";
	ifstream ifs( pat.toStdString().c_str() );
	int read1;
	ifs >> read1;
	mPref->networking = read1;
	ifs >> read1;
	mPref->port = read1;
	ifs >> read1;
	mPref->tailSize = read1;
	ifs >> read1;
	mPref->contactSize = read1;
	std::string read2;
	ifs >> read2;
	mPref->address = read2.c_str();
	ifs >> read1;
	mPref->distPort = read1;
	ifs >> read2;
	mPref->passWord = read2.c_str();
	ifs >> read1;
	mPref->distantConfig = read1;
	ifs >> read1;
	mPref->netError = read1;
	ifs >> read1;
	mPref->debugger = read1;
}

void MainWindow::savePref()
{
	using namespace std;
	QString pat = QString( PERSONNAL_RESSOURCES_PATH ) + ".data/settings.txt";
	ofstream ofs(pat.toStdString().c_str() );
	ofs << mPref->networking << endl << mPref->port << endl << mPref->tailSize << endl << mPref->contactSize << endl << mPref->address.toStdString().c_str() << endl << mPref->distPort << std::endl << mPref->passWord.toStdString().c_str() << std::endl << mPref->distantConfig << std::endl << mPref->netError << std::endl << mPref->debugger;
}

void MainWindow::showQueue()
{
	delete workingSpace;
	workingSpace = new QueueWidget(this, queue);
	scroll->setWidget(workingSpace);
	this->update();
	if( mPref->networking )
	{
		can_send = true;
		aDist->setEnabled(true);
	}
	can_export = true;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::addToQueue(Kontact* kon)
{
	queue->push_back(kon);
}

void MainWindow::setNetworking()
{
	if( !networking && mPref->networking )
	{
		server = new Serveur(this);
		server->listenWeb();
		networking = true;
		std::cout << "Activating networking" << std::endl;
		return;
	}
	if( networking && !(mPref->networking) )
	{
		networking = false;
		delete server;
		std::cout << "Destroying networking" << std::endl;
		return;
	}
	if( networking && ( mPref->distPort != oldPort ) )
	{
		delete server;
		server = new Serveur(this);
		server->listenWeb();
		std::cout << "Restarting networking" << std::endl;
		return;
	}
	if( networking && ( mPref->address != oldAddresse ) )
	{
		delete server;
		server = new Serveur(this);
		server->listenWeb();
		std::cout << "Restarting networking" << std::endl;
		return;
	}
}

void MainWindow::sendKontact(Kontact* kon)
{
	Socket* soc = new Socket( mPref, this);
	soc->send( kon, true);
}

void MainWindow::sendAll( std::vector<Kontact*>* tut)
{
	Socket* soc = new Socket(mPref);
	soc->sendAll( tut );
}

void MainWindow::removeFromQueue(int i)
{
	queue->erase(queue->begin()+i);
}

void MainWindow::basicLoad(QString file, std::vector<Kontact*>* tut)
{
	using namespace std;
	ifstream ifs(file.toStdString().c_str());
	string read1;
	string read2;
	Kontact* kontact;
	bool readyRead = true;
	while(getline(ifs, read1))
	{
		if(read1 == "." )
		{
			tut->push_back(kontact);
			readyRead = true;			
		}
		else
		{
			if(readyRead)
			{
				kontact = new Kontact;
				readyRead = false;
				kontact->setId( QString(read1.c_str()).toInt() );
				getline( ifs, read1);
				kontact->set_image( read1.c_str() );
				getline( ifs, read1);
			}
			if(getline(ifs, read2))
				kontact->setAttribute(read1.c_str(), read2.c_str());
			this->addAttribute(read1.c_str());
		}
	}
	if(!readyRead)
		delete kontact;
}

void MainWindow::basicSave(QString file, std::vector<Kontact*>* tut)
{
	using namespace std;
	ofstream* ofs = new ofstream(file.toStdString().c_str());
	int i;
	int n = tut->size();
	for(i = 0; i < n; i++)
		(*tut)[i]->save(ofs);
}

void MainWindow::saveQueue()
{
	debug("Saving queue...", this);
	this->basicSave(QString( PERSONNAL_RESSOURCES_PATH ) + ".data/queue.txt", queue);
}

void MainWindow::massiveExports( std::vector<Kontact*>* tut)
{
	ExportWidget* exportW = new ExportWidget(this,tut);
	exportW->setWindowTitle(tr("KBotin : Export"));
	exportW->show();
}

void MainWindow::massiveSend( std::vector<Kontact*>* tut)
{
	this->sendAll(tut);
}

void MainWindow::askMassiveExports()
{
	emit needMassiveExport();
}

void MainWindow::askMassiveSend()
{
	emit needMassiveSend();
}

void MainWindow::import()
{
	can_send = false;
	aDist->setEnabled(can_send);
	can_export = false;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
	delete workingSpace;
	workingSpace = new ImportWidget(this);
	scroll->setWidget(workingSpace);
	this->update();
}

void MainWindow::emptyQueue()
{
	debug("empty queue", this);
	int i;
	int n = queue->size();
	for(i=0; i<n; ++i)
	{
		delete (*queue)[i];
	}
	queue->clear();
	emit refreshQueue();
	this->saveQueue();
}

void MainWindow::doublon_search()
{
	std::vector<Kontact*> plop = *kontacts;
	std::vector<Kontact*>* res = new std::vector<Kontact*>;
	int j = 0;
	bool wrong = false;
	while( plop.size() > 0 )
	{
		j=1;
		wrong = false;
		while( j < plop.size() )
		{
			if( *(plop[j]) == *(plop[0]) )
			{
				wrong = true;
				res->push_back(plop[j]);
				plop.erase(plop.begin()+j);
			}
			else
			 ++j;
		}
		if( wrong )
			res->push_back(plop[0]);
		plop.erase(plop.begin());
	}
	delete workingSpace;
	workingSpace = new DoublonWidget(res, this);
	scroll->setWidget(workingSpace);
	this->update();
	if( mPref->networking )
	{
		can_send = true;
		aDist->setEnabled(true);
	}
	can_export = true;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::loadGroups()
{
	using namespace std;
	QString pat = QString( PERSONNAL_RESSOURCES_PATH ) + ".data/groups.txt";
	ifstream ifs( pat.toStdString().c_str());
	int a;
	std::string read;
	getline(ifs, read);
	a = QString( read.c_str() ).toInt();
	Kontact::setCurrentId(a);
	getline(ifs, read);
	a = QString( read.c_str() ).toInt();
	Group::setIdDef(a);
	Group* g;
	bool f = true;
	//Maintenant chargement des groupes
	bool mode = false;
	while( getline(ifs, read))
	{
		if(!mode)
		{
			if( read != "-" )
				if(f)
				{
					g = new Group;
					g->setId(QString( read.c_str() ).toInt());
					getline(ifs, read);
					g->setName( read.c_str());
					getline(ifs, read);
					g->setImage( read.c_str() );
					f = false;
				}
				else
				{
					g->add( QString( read.c_str() ).toInt() );
				}
			else
			{
				mode = true;
			}
		}
		else
		{
			if( read != "." )
			{
				g->addInclusion(QString( read.c_str() ).toInt());
			}
			else
			{
				mode = false;
				f = true;
				groups->push_back(g);
			}
		}
	}
	if(!f)
		delete g;
}

void MainWindow::saveGroups()
{
	using namespace std;
	QString pat = QString( PERSONNAL_RESSOURCES_PATH ) + ".data/groups.txt";
	ofstream* ofs = new ofstream( pat.toStdString().c_str() );
	(*ofs) << Kontact::getCurrentId() << endl;
	(*ofs) << Group::getIdDef() << endl;
	int i;
	int n = groups->size();
	for(i=0; i<n; ++i)
		(*groups)[i]->save(ofs);
	delete ofs;
}

Kontact* MainWindow::askKontact(int a)
{
	int i;
	int n = kontacts->size();
	for(i=0; i<n; ++i)
		if((*kontacts)[i]->getId() == a)
			return (*kontacts)[i];
	return 0;
}

void MainWindow::showGroup(Group* g)
{
	delete workingSpace;
	workingSpace = new GroupWidget(g, this);
	scroll->setWidget(workingSpace);
	this->update();
	if( mPref->networking )
	{
		can_send = true;
		aDist->setEnabled(true);
	}
	can_export = true;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::groupMng()
{
	can_send = false;
	aDist->setEnabled(can_send);
	can_export = false;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
	delete workingSpace;
	workingSpace = new GroupMng(groups,this);
	scroll->setWidget(workingSpace);
	this->update();
}

void MainWindow::createGroupsAction()
{
	int i;
	int n = groups->size();
	for(i=0; i<n; ++i)
	{
		this->addGroupAction( (*groups)[i] );
	}
}

void MainWindow::addGroupAction(Group* g)
{
	ActionGroup* act;
	act = new ActionGroup( g->getName(), this, g);
	mGroups->addAction(act);
	aGroups.push_back(act);
}

void MainWindow::removeGroupAction(Group* g)
{
	int i;
	int n = aGroups.size();
	for(i=0; i<n; ++i)
		if( aGroups[i]->getGroup() == g )
		{
			delete aGroups[i];
			aGroups.erase( aGroups.begin() + i);
			return;
		}
}

void MainWindow::addKToG(Group* g)
{
	can_send = false;
	aDist->setEnabled(can_send);
	can_export = false;
	aExport->setEnabled(can_export);
	delete workingSpace;
	workingSpace = new AddKToGroup(g, this);
	scroll->setWidget(workingSpace);
	this->update();
}

void MainWindow::emitPrintRequest()
{
	emit printRequest();
}

void MainWindow::print(const QString& p)
{
	QPrinter printer;
	QTextDocument doc;
	//C'est ici que se situe la difficultée pour moi...
	doc.setHtml( p );
	QPrintDialog *dialog = new QPrintDialog( &printer, this);
	dialog->setWindowTitle(tr("Impression des contacts"));
	if( dialog->exec() == QDialog::Accepted )
	{
		doc.print(&printer);
		debug("Printing", this);
	}
}

Group* MainWindow::getGroupById(int a)
{
	int i;
	int n = groups->size();
	for(i=0; i<n; ++i)
	{
		if( (*groups)[i]->getId() == a)
			return (*groups)[i];
	}
	return 0;
}

void MainWindow::consultConfig()
{
	can_send = false;
	aDist->setEnabled(can_send);
	can_export = false;
	aPrint->setEnabled( can_export );
	aExport->setEnabled(can_export);
	delete workingSpace;
	workingSpace = new ConsultWidget(this);
	scroll->setWidget(workingSpace);
	this->update();
}

void MainWindow::consult( std::vector<Kontact*>* kkons)
{
	delete workingSpace;
	QString l;
	l.setNum( mPref->distPort );
	workingSpace = new DistantWidget(kkons, mPref->address+ " port " + l, this);
	scroll->setWidget(workingSpace);
	this->update();
	if( mPref->networking )
	{
		can_send = true;
		aDist->setEnabled(true);
	}
	can_export = true;
	aExport->setEnabled(can_export);
	aPrint->setEnabled(can_export);
}

void MainWindow::chek_install()
{
	if( INSTALL_VERSION )
	{
		QDir dir( QDir::homePath() + "/.kbotin");
		if( ! dir.exists() )
		{
			dir = QDir::home();
			dir.mkdir( ".kbotin" );
			dir.cd( ".kbotin" );
			dir.mkdir( ".data" );
			dir.cd( ".data" );
			QString base = QDir::homePath() + "/.kbotin/.data/";
			{
				QString path = base + "groups.txt";
				std::ofstream ofs( path.toStdString().c_str() );
				ofs << 0 << std::endl;
			}
			{
				QString path = base + "settings.txt";
				std::ofstream ofs( path.toStdString().c_str() );
				ofs << 0 << std::endl;;
				ofs << 1447 << std::endl;
				ofs << 9100 << std::endl;
				ofs << 850 << std::endl;
				ofs << "127.0.0.1" << std::endl;
				ofs << "1447" << std::endl;
				ofs << "plop" << std::endl;
				ofs << 0 << std::endl;
				ofs << 0 << std::endl;
				ofs << 0 << std::endl;
			}
			dir.mkdir( "images" );
		}
	}
	else
	{
		QDir dir( ".data" );
		if( ! dir.exists() )
		{
			QDir dira( "." );
			dira.mkdir( ".data" );
			dira.cd( ".data" );
			QString base =".data/";
			{
				QString path = base + "groups.txt";
				std::ofstream ofs( path.toStdString().c_str() );
				ofs << 0 << std::endl;
			}
			{
				QString path = base + "settings.txt";
				std::ofstream ofs( path.toStdString().c_str() );
				ofs << 0 << std::endl;;
				ofs << 1447 << std::endl;
				ofs << 9100 << std::endl;
				ofs << 850 << std::endl;
				ofs << "127.0.0.1" << std::endl;
				ofs << "1447" << std::endl;
				ofs << "plop" << std::endl;
				ofs << 0 << std::endl;
				ofs << 0 << std::endl;
				ofs << 0 << std::endl;
			}
			dira.mkdir( "images" );
		}
	}
}
