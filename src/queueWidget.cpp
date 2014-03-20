/*!
* \file queueWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Widget montrant la file d'attente des contacts importés/reçus
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

#include "queueWidget.hpp"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include "kontact.hpp"
#include "mainWindow.hpp"
#include "kontactWidget.hpp"
#include <QPushButton>

#include <QLabel>
#include <iostream>

QueueWidget::QueueWidget(MainWindow* w, std::vector<Kontact*>* kon) : QWidget()
{
	mW = w;
	kontacts = kon;
	int i;
	mainLayout = new QVBoxLayout;
	if( kon->size() > 0 )
	{
		QHBoxLayout* hb;
		QGroupBox* gro;
		QVBoxLayout* yop;
		QRadioButton* qcb1;
		QRadioButton* qcb2;
		QRadioButton* qcb3;
		KontactWidget* wid;
		int n = kontacts->size();
		for(i=0; i<n; ++i)
		{
			hb = new QHBoxLayout;
			gro = new QGroupBox(tr("Choisissez une des options : "), this);
			yop = new QVBoxLayout;
			qcb1 = new QRadioButton(tr("Accepter"));
			qcb2 = new QRadioButton(tr("Différer"));
			qcb3 = new QRadioButton(tr("Détruire"));
			accept.push_back(qcb1);
			differ.push_back(qcb2);
			destroy.push_back(qcb3);
			qcb2->setChecked(true);
			yop->addWidget(qcb1);
			yop->addWidget(qcb2);
			yop->addWidget(qcb3);
			gro->setLayout(yop);
			hb->addWidget(gro);
			wid = new KontactWidget( (*kontacts)[i], mW, 0,false);
			hb->addWidget(wid);
			mainLayout->addLayout(hb);
		}
		mSubmit = new QPushButton(tr("Soumettre"));
		mSubmit->setMaximumSize(mSubmit->sizeHint());
		mainLayout->addWidget(mSubmit);
		connect( mSubmit, SIGNAL( clicked() ), this, SLOT( submit() ) );
	}
	else
	{
		QLabel* lab = new QLabel;
		lab->setText(tr("<b>Aucun contact dans la file d'attente</b>"));
		mainLayout->addWidget(lab);
	}
	this->setLayout(mainLayout);
	connect( mW, SIGNAL( refreshQueue() ), this, SLOT( refresh() ) );
}

void QueueWidget::submit()
{
	int n = kontacts->size();
	int i;
	for(i=0; i<n; ++i)
	{
		if( accept[i]->isChecked() )
		{
			mW->addKontact( (*kontacts)[i] );
			(*kontacts)[i]->updateId();
		}
		if( accept[i]->isChecked() || destroy[i]->isChecked())
		{
			std::cout << (*kontacts)[i]->size() << std::endl;
			mW->removeFromQueue(i);
			destroy.erase(destroy.begin()+i);
			accept.erase(accept.begin()+i);
			--i;
			--n;
		}
	}
	mW->saveQueue();
	mW->save();
	mW->updateSearchEngine();
	mW->showQueue();
}

void QueueWidget::refresh()
{
	mW->showQueue();
}
