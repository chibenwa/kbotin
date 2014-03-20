/*!
* \file distantWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe permettant de visualiser le contenu d'un KBotin distant...
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

#include "distantWidget.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include "kontact.hpp"
#include "mainWindow.hpp"

#include "customWidget.hpp"
#include <algorithm>
#include <QChar>

DistantWidget::DistantWidget( std::vector<Kontact*>* kons, QString q, MainWindow* w ):QWidget()
{
	mW = w;
	mKons = kons;
	title = new QLabel;
	sort( kons->begin(), kons->end(), rel);
	title->setText(tr("<h1>Contacts sur ") + q + "</h1>");
	mainLayout = new QVBoxLayout;
	mainLayout->addWidget( title );
	int i;
	int n = kons->size();
	CustomWidget* cu;
	QChar c = '!';
	QLabel* tg;
	Kontact* k;
	for(i=0; i<n; ++i)
	{
		k = (*kons)[i];
		if( k->findValue("Nom")[0] != c && k->findValue("Nom")[0].isLetter() )
		{
			c = k->findValue("Nom")[0];
			tg = new QLabel;
			tg->setText("<h1>"+ k->findValue("Nom")[0] +"</h1>");
			mainLayout->addWidget(tg);
		}
		cu = new CustomWidget((*kons)[i],mW,0, false, true);
		mainLayout->addWidget( cu );
	}
	this->setLayout( mainLayout );
	connect(mW, SIGNAL( needMassiveSend() ), this, SLOT( sendAll() ) );
	connect(mW, SIGNAL( needMassiveExport() ), this, SLOT( exportAll() ) );
	connect(mW, SIGNAL( printRequest() ), this, SLOT( print() ) );
}

DistantWidget::~DistantWidget()
{
	int i;
	int n = mKons->size();
	for(i=0; i<n; ++i)
	{
		delete (*mKons)[i];
	}
	delete mKons;
}


void DistantWidget::sendAll()
{
	mW->sendAll(mKons);
}

void DistantWidget::exportAll()
{
	mW->massiveExports(mKons);
}

void DistantWidget::print()
{
	QString p = tr("<h1>Liste des contacts à imprimer</h1>");
	int i;
	int n = mKons->size();
	for(i=0; i<n;++i)
	{
		(*mKons)[i]->print(p);
	}
	mW->print(p);
}
