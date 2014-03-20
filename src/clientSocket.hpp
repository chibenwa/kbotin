/*!
* \file clientSocket.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe simplifiant la gestion des clients TCP entrant
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

#ifndef CLIENTSOCKET__H__
#define CLIENTSOCKET__H__

#include <QTcpSocket>
#include <QObject>
#include <QString>

class Kontact;
class MainWindow;

class ClientSocket:public QObject
{
	Q_OBJECT
	public:
		ClientSocket(int a, QTcpSocket* socket, MainWindow* w);
		~ClientSocket();
		QTcpSocket* socket() const
		{
			return mSocket;
		}
		void dec()
		{
			--num;
		}
		bool state() const
		{
			return mState;
		}
		void close()
		{
			mState = false;
		}
		void valid(Kontact*);
		void send(Kontact* kon);
	public slots:
		void emmission();
		void disconnexion();
		void read();
		void errorTrt();
	signals:
		void giveNb(int);
		void discRequest(int);
	protected:
		MainWindow* mW;
		int num;
		QTcpSocket* mSocket;
		bool mState;
		Kontact* constructed;
		QString att;
		bool isAtt;
};

#endif
