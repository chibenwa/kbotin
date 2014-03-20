/*!
* \file socket.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Socket d'envoi des contzacts
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

#ifndef SOCKET__HPP__
#define SOCKET__HPP__

#include "Preference.hpp"
#include <vector>
#include <QObject>

class Kontact;
class QTcpSocket;
class MainWindow;

class Socket: public QObject
{
		Q_OBJECT
	public:
		Socket(Preference*, MainWindow* w = 0);
		~Socket();
		void send(Kontact*, bool b = false);
		void sendAll( std::vector<Kontact*>*);
		void askDistantContactList(const QString&);
	public slots:
		void errorTrt();
		void read();
	protected:
		MainWindow* mW;
		bool can_destroy;
		QTcpSocket* socket;
		std::vector<Kontact*>* waitTrt;
		bool mState;
		Kontact* constructed;
		QString att;
		bool isAtt;
};

#endif 
