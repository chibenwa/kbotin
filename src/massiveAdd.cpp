/*!
* \file massiveAdd.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre d'ajout massif de contacts
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

#include "massiveAdd.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "AddWidget.hpp"
#include "mainWindow.hpp"

#include <iostream>

MassiveAdd::MassiveAdd(MainWindow* w):QWidget()
{
	nbAddW = 0;
	mW =w;
	mainLayout = new QVBoxLayout;
	buttonLayout = new QHBoxLayout;
	editLayout = new QHBoxLayout;
	mAdd = new QPushButton(tr("Ajouter un contact"));
	mSubmit = new QPushButton(tr("Soumettre"));
	mAdd->setMaximumSize(mAdd->sizeHint());
	mSubmit->setMaximumSize(mSubmit->sizeHint());
	buttonLayout->addWidget(mAdd);
	buttonLayout->addWidget(mSubmit);
	this->setLayout(mainLayout);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addLayout(editLayout);
	AddWidget* plop = new AddWidget(w,nbAddW);
	widgetVect.push_back(plop);
	editLayout->addWidget(plop);
	connect(mAdd, SIGNAL( clicked() ), this, SLOT( add() ) );
	connect(mSubmit, SIGNAL(clicked() ), this, SLOT( submit() ) );
	connect(plop, SIGNAL( dellDemande(int) ), this, SLOT( dell(int) ) );
	++nbAddW;
}

void MassiveAdd::add()
{
	AddWidget* plop = new AddWidget(mW,nbAddW);
	widgetVect.push_back(plop);
	editLayout->addWidget(plop);
	++nbAddW;
	connect(plop, SIGNAL( dellDemande(int) ), this, SLOT( dell(int) ) );
}

void MassiveAdd::submit()
{
	int i;
	int n = widgetVect.size();
	for(i=0; i<n; ++i)
	{
		widgetVect[i]->addKontact();
	}
	mW->save();
	mW->showContact();
}

void MassiveAdd::dell(int n)
{
	//Supprime le widget d'édition en position n
	editLayout->removeItem(editLayout->itemAt(n));
	delete widgetVect[n];
	widgetVect.erase(widgetVect.begin()+n);
	int youpi;
	for( youpi=n; youpi<widgetVect.size(); youpi++)
	{
		widgetVect[youpi]->decrease();
	}
	--nbAddW;
}
