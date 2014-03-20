/*!
* \file AlWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre d'affichage massif de contacts
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

#include "AllWidget.hpp"
 
#include <QLabel>
#include <QVBoxLayout>
#include "kontact.hpp"
#include "customWidget.hpp"
#include "mainWindow.hpp"
#include <QChar>

#include <iostream>

AllWidget::AllWidget(std::vector<Kontact*>* plop, MainWindow* w, bool b):QWidget()
{
	mB = b;
	mVect = plop;
	mW = w;
	mainLayout = new QVBoxLayout;
	int i;
	int n = plop->size();
	QChar stock='!';
	QLabel* letter;
	CustomWidget* kw;
	if( n!= 0 )
	{
		for(i=0; i<n; ++i)
		{
			if((*plop)[i]->findValue("Nom")[0] != stock && (*plop)[i]->findValue("Nom")[0].isLetter())
			{
				letter = new QLabel;
				letter->setText("<h1>"+(*plop)[i]->findValue("Nom")[0]+"</h1>");
				stock = (*plop)[i]->findValue("Nom")[0];
				mainLayout->addWidget(letter);
			}
			kw = new CustomWidget( (*plop)[i], w, this, b);
			mainLayout->addWidget(kw);
			if(b)
			{
				connect(kw, SIGNAL(please(int)), this, SLOT( pleaseTrt(int)));
			}
		}
	}
	else
	{
		QLabel* plop = new QLabel;
		plop->setText(tr("<h1>Aucun contact pour l'instant</h1>"));
		mainLayout->addWidget(plop);
	}
	this->setLayout(mainLayout);
	mVect = plop;
	connect(mW, SIGNAL( needMassiveSend() ), this, SLOT( sendAll() ) );
	connect(mW, SIGNAL( needMassiveExport() ), this, SLOT( exportAll() ) );
	if( !mB )
		connect(mW, SIGNAL( printRequest() ), this, SLOT( print() ) );
}

AllWidget::~AllWidget()
{
	delete mainLayout;
}

void AllWidget::sendAll()
{
	mW->sendAll(mVect);
}

void AllWidget::exportAll()
{
	mW->massiveExports(mVect);
}

void AllWidget::deleteKontact(Kontact* k)
{
	int i;
	int n = mVect->size();
	for(i=0; i<n; ++i)
	{
		if( (*mVect)[i] ==  k )
		{
			mVect->erase(mVect->begin()+i);
			break;
		}
	}
	mW->deleteKontact(k, mVect);
	mW->updateSearchEngine();
	mW->save();
}

void AllWidget::pleaseTrt(int a)
{
	if(mB)
		emit please(a);
}

void AllWidget::print()
{
	if(!mB)
	{
		QString p = tr("<h1>Liste des contacts à imprimer</h1>");
		int i;
		int n = mVect->size();
		for(i=0; i<n;++i)
		{
			(*mVect)[i]->print(p);
		}
		mW->print(p);
	}
}
