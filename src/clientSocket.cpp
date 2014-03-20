/*!
* \file clientSocket.cpp
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

#include "clientSocket.hpp"

#include "kontact.hpp"
#include "mainWindow.hpp"
#include "Preference.hpp"

#include <utility>
#include "divers.hpp"

///debug
#include <iostream>
#include <string>

ClientSocket::ClientSocket(int a, QTcpSocket* socket, MainWindow* w):QObject()
{
	debug("traiting client", w);
	mW = w;
	mState = true;
	mSocket = socket;
	num = a;
	connect(socket, SIGNAL(readyRead()), this, SLOT( read() ));
	connect(socket, SIGNAL(disconnected()), this, SLOT( disconnexion() ) );
	connect(mSocket, SIGNAL( error(QAbstractSocket::SocketError ) ), this, SLOT( errorTrt() ) );
	constructed = 0;
	isAtt = true;
}

void ClientSocket::emmission()
{
	emit giveNb(num);
}

ClientSocket::~ClientSocket()
{
	mSocket->deleteLater();
	if(constructed != 0 )
	{
		delete constructed;
	}
}

void ClientSocket::disconnexion()
{
	emit discRequest(num);
}

void ClientSocket::read()
{
	debug("lecture d'un flux", mW);
	QString line;
	while( mSocket->canReadLine() )
	{
	//	std::cout << "Reading a line..." << std::endl;
		line = mSocket->readLine();
		debug(line, mW);
		line.remove(line.size()-1,1);
		//traitement des données reçues...
		if( line  == "0:!" )
		{
			QTextStream(mSocket) << "0:!" << endl;
			return;
		}
		if(line.startsWith("0:"))
		{
			line.remove(0,2);
			if(constructed == 0)
			{
				if(line != "." )
				{
//					std::cout << "receiving a new Kontact" << std::endl;
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
//						std::cout << "Reading a new attribute" << std::endl;
						att = line;
						isAtt = false;
					}
					else
					{
//						std::cout << "Adding kontact to queue" << std::endl;
						this->valid(constructed);
						constructed = 0;
						isAtt = true;
					}
				}
				else
				{
//					std::cout << "Reading attribute value :" << line.toStdString() << std::endl;
					isAtt = true;
					constructed->setAttribute(att, line);
				}
			}
			continue;
		}
		if(line.startsWith("1:") && mW->getPref()->distantConfig)
		{
			line.remove(0,2);
			if( line == mW->getPref()->passWord )
			{
				//traitement de la demande...
				std::vector<Kontact*>* grub = mW->getKontacts();
				int i;
				int n = grub->size();
				for(i=0; i<n; ++i)
				{
					this->send( (*grub)[i]);	
				}
				QTextStream(mSocket) << ".." << endl;
				continue;
			}
			else
				debug("Passwords doesn't match... "+ line +"/"+ mW->getPref()->passWord, mW);
		}
	}
}

void ClientSocket::valid(Kontact* c)
{
	Preference* pref = mW->getPref();
	int nmax = pref->tailSize;
	int mlength = pref->contactSize;
	int n = mW->getQueueSize();
	if(nmax > n)
	{
		n = c->size();
		if( mlength > n )
		{
			mW->addToQueue(c);
			mW->saveQueue();
		}
	}
}

void ClientSocket::send(Kontact* kon)
{
	std::vector< std::pair<QString,QString> >* vect = kon->getAttributsList();
	int n = vect->size();
	int i;
	for(i=0; i<n; ++i)
	{
		QTextStream(mSocket) << (*vect)[i].first << endl << (*vect)[i].second << endl;
	}
	QTextStream(mSocket) << "." << endl;
}

void ClientSocket::errorTrt()
{
	if( mW->getPref()->netError )
	{
		errorMessage( tr("<h2> Erreur réseau sur le serveur : </h2>") + mSocket->errorString() );
	} 
}
