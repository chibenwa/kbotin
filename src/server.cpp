/*!
* \file serveur.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Serveur permettant la réception des contacts
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

#include "server.hpp"
#include <QTextStream>
#include <iostream>
#include "mainWindow.hpp"
#include "clientSocket.hpp"
#include "kontact.hpp"

#include "Preference.hpp"

///debug
#include "divers.hpp"

Serveur::Serveur(MainWindow* w):QTcpServer()
{
	mW = w;
	Preference* pref = mW->getPref();
	mPort = pref->port;
}

Serveur::~Serveur()
{
	int i;
	int n = connectionVect.size();
	for (i=0; i<n; ++i)
	{
		delete connectionVect[i];
	}
}

void Serveur::listenWeb()
{
	Preference* pref = mW->getPref();
	bool a = listen(QHostAddress::Any, pref->port);
	if(a)
	{
		emit connectionOk();
		connect(this, SIGNAL(newConnection()), this, SLOT(newConnectionTraitement()));
	}
	else
		emit connectionError(this->errorString());
}

void Serveur::newConnectionTraitement()
{
	debug("new connection received by server...", mW);
	QTcpSocket* income = nextPendingConnection();
	ClientSocket* plop = new ClientSocket(connectionVect.size(), income, mW);
	connectionVect.push_back(plop);
	emit newConnectionSignal();
	connect(plop, SIGNAL( discRequest(int)), this, SLOT( closeSocket(int) ));
}

void Serveur::closeSocket(int a)
{
	int n = connectionVect.size();
	if( a < n)
	{
		delete connectionVect[a];
		connectionVect.erase(connectionVect.begin()+a);
	}
}
