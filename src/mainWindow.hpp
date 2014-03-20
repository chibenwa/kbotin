/*!
* \file mainWindow.hpp
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

#ifndef MAINWINDOW__HPP__
#define MAINWINDOW__HPP__

#include <QMainWindow>

#include <map>
#include <QString>

class QMenu;
class QVBoxLayout;
class ToolBar;
class QWidget;
class Kontact;
class QScrollArea;
class Serveur;
class Group;
class ActionGroup;

#include "Preference.hpp"

class MainWindow: public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow();
		~MainWindow();
		std::map<QString, int>* getAttributs() const
		{
			return attributs;
		}
		Preference* getPref() const
		{
			return mPref;
		}
		int getQueueSize() const
		{
			return queue->size();
		}
		void setNetworking();
		bool getNetworking() const
		{
			return networking;
		}
		std::vector<Kontact*>* getQueue() const
		{
			return queue;
		}
		std::vector<Kontact*>* getKontacts() const
		{
			return kontacts;
		}
		std::vector<Group*>* getGroups() const
		{
			return groups;
		}
		void addKontact(Kontact*);
		void addToQueue(Kontact*);
		void loadPref();
		void savePref();
		void sendKontact(Kontact*);
		void sendAll( std::vector<Kontact*>*);
		void removeFromQueue(int);
		void basicLoad(QString file, std::vector<Kontact*>* );
		void basicSave(QString file, std::vector<Kontact*>* );
		void saveQueue();
		void massiveSend( std::vector<Kontact*>* );
		void massiveExports( std::vector<Kontact*>* );
		void loadGroups();
		void saveGroups();
		Kontact* askKontact(int);
		void showGroup(Group* g);
		void createGroupsAction();
		void addGroupAction(Group*);
		void removeGroupAction(Group*);
		void addKToG(Group*);
		void deleteSearchEngine();
		Group* getGroupById(int);
		void consult( std::vector<Kontact*>* );
		void chek_install();
	public slots:
		void load();
		void save();
		void add();
		void showContact();
		void modify(Kontact*);
		void addAttribute(QString);
		void constructSerchEngine();
		void search();
		void updateSearchEngine();
		void deleteKontact(Kontact*, std::vector<Kontact*>*);
		void help();
		void settings();
		void showQueue();
		void askMassiveSend();
		void askMassiveExports();
		void import();
		void emptyQueue();
		void doublon_search();
		void groupMng();
		void print(const QString&);
		void emitPrintRequest();
		void consultConfig();
	signals:
		void needSource();
		void needMassiveSend();
		void needMassiveExport();
		void refreshQueue();
		void printRequest();
	protected:
		bool up;
		QMenu* mFichier;
		QMenu* mPartage;
		QMenu* mGroups;
		QMenu* mHelp;
		QAction* aPref;
		QAction* aQuitter;
		QAction* aViewQueue;
		QAction* aDist;
		QAction* aHelp;
		QAction* aExport;
		QAction* aImport;
		QAction* aEmpty;
		QAction* aDoublon;
		QAction* aGroupMng;
		QAction* aPrint;
		QAction* aConsult;
		QVBoxLayout* mainLayout;
		ToolBar* mTool;
		QWidget* centralWidget;
		std::map<QString,int>* attributs;
		std::vector<Kontact*>* kontacts;
		QWidget* workingSpace;
		QScrollArea* scroll;
		//Et maintenant un chouilla de recherche...
		std::map<QString, std::map<QString, std::vector<Kontact*>* >* > searchEngine;
		Preference* mPref;
		std::vector<Kontact*>* queue;
		bool networking;
		Serveur* server;
		int oldPort;
		QString oldAddresse;
		bool can_send;
		bool can_export;
		std::vector<Group*>* groups;
		std::vector<ActionGroup*> aGroups;
};

#endif 
