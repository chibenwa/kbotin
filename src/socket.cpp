/*!
* \file socket.cpp
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

#include "socket.hpp"

#include "kontact.hpp"
#include <QTcpSocket>
#include "mainWindow.hpp"

#include <utility>
#include <QTextStream>
#include <QString>

///debug
#include <iostream>
#include "divers.hpp"

#include <unistd.h>

Socket::Socket(Preference* pref, MainWindow* w)
{
	mState = false;
	mW = w;
	can_destroy = true;
	constructed = 0;
	isAtt = true;
	mState = true;
	socket = new QTcpSocket;
	socket->connectToHost( pref->address, pref->distPort);
	waitTrt = new std::vector<Kontact*>;
	connect(socket, SIGNAL( error(QAbstractSocket::SocketError ) ), this, SLOT( errorTrt() ) );
	connect(socket, SIGNAL(readyRead()), this, SLOT( read() ));
}

void Socket::send(Kontact* kon, bool b)
{
	std::vector< std::pair<QString,QString> >* vect = kon->getAttributsList();
	int n = vect->size();
	int i;
	for(i=0; i<n; ++i)
	{
		QTextStream(socket)<< "0:" << (*vect)[i].first << endl << "0:" << (*vect)[i].second << endl;
		debug("0:"+(*vect)[i].first, mW);
		debug("0:"+(*vect)[i].second, mW);
	}
	QTextStream(socket) << "0:." << endl;
	if(b)
		QTextStream(socket) << "0:!" << endl;
}

void Socket::sendAll( std::vector<Kontact*>* all )
{
	int n = all->size();
	int i;
	for(i=0; i<n; ++i)
	{
		this->send( (*all)[i] );
	}
	QTextStream(socket) << "0:!" << endl;
}

void Socket::askDistantContactList(const QString& passWord)
{
	QTextStream(socket) << "1:" << passWord << endl;
}

Socket::~Socket()
{
	socket->deleteLater();
	if( can_destroy )
		delete waitTrt;
}

void Socket::read()
{
	if( mW != 0 )
	{
		QString line;
		while( socket->canReadLine() )
		{
			line = socket->readLine();
			line.remove(line.size()-1,1);
			//traitement des données reçues...
			if( line == "0:!" )
				this->deleteLater();
			if( line == "..")
			{
				can_destroy = false;
				mW->consult(waitTrt);
				delete this;
				return;
			}
			if(constructed == 0)
			{
				if(line != "." )
				{
					constructed = new Kontact;
					att = line;
					isAtt = false;
				}
			}
			else
			{
				if(isAtt)
				{
					if(line != "." )
					{
						att = line;
						isAtt = false;
					}
					else
					{
						waitTrt->push_back( constructed );
						constructed = 0;
						isAtt = true;
					}
				}
				else
				{
					isAtt = true;
					constructed->setAttribute(att, line);
				}
			}
		}
	}
}

void Socket::errorTrt()
{
	if( mW->getPref()->netError )
	{
		errorMessage( tr("<h2>Erreur réseau :</h2>") + socket->error() ); 
	}
}
