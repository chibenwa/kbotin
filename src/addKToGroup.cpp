/*!
* \file actionGroup.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre d'ajout de contacts au groupe.
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

#include "addKToGroup.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox> 
#include "mainWindow.hpp"
#include "kontact.hpp"
#include "group.hpp"

#include "divers.hpp"
#include <QChar>

AddKToGroup::AddKToGroup(Group* g, MainWindow* w)
{
	mW = w;
	mGroup = g;
	mainLayout = new QVBoxLayout;
	secLayout = new QVBoxLayout;
	titre = new QLabel;
	titre->setText(tr("<h1>Ajoutez des contacts à <b>")+g->getName()+tr("</b></h1>"));
	mainLayout->addWidget( titre );
	std::vector<Kontact*>* kontacts = mW->getKontacts();
	groups = mW->getGroups();
	int i;
	QHBoxLayout* qhb;
	QCheckBox* qcb;
	QLabel* img;
	QLabel* noms;
	QChar stock ='!';
	QLabel* sep;
	int n = kontacts->size();
	for(i=0; i<n; ++i)
	{
		if( ! mGroup->isInGroup( (*kontacts)[i]->getId() ) )
		{
			qhb = new QHBoxLayout;
			qcb = new QCheckBox;
			qhb->addWidget(qcb);
			if( stock != (*((*kontacts)[i]->getAttributsList()))[0].second[0] && (*((*kontacts)[i]->getAttributsList()))[0].second[0].isLetter() )
			{
				stock = (*((*kontacts)[i]->getAttributsList()))[0].second[0];
				sep = new QLabel;
				sep->setText("<h1>"+(*((*kontacts)[i]->getAttributsList()))[0].second[0]+"</h1>");
				secLayout->addWidget(sep);
			}
			if( (*kontacts)[i]->get_image() != "0")
			{
				img = new QLabel;
				img->setPixmap( (*kontacts)[i]->get_image() );
				img->setMaximumSize(50,50);
				img->setScaledContents(true);
				qhb->addWidget(img);
			}
			else
			{
				QWidget* spacer = new QWidget;  
				spacer->setMinimumSize(50,1);
				qhb->addWidget(spacer);
			}
			noms = new QLabel;
			noms->setText( (*((*kontacts)[i]->getAttributsList()))[0].second +" "+ (*((*kontacts)[i]->getAttributsList()))[1].second );
			qhb->addWidget(noms, Qt::AlignLeft);
			secLayout->addLayout(qhb);
			local.push_back((*kontacts)[i]);
			qcbs.push_back(qcb);
		}
	}
	mainLayout->addLayout(secLayout);
	mSubmit = new QPushButton(tr("Ajouter"));
	mSubmit->setMaximumSize(mSubmit->sizeHint());
	connect(mSubmit, SIGNAL( clicked() ), this, SLOT( submit() ) );
	n = groups->size();
	include = new QLabel;
	include->setText(tr("<h1>Inclure un groupe</h1>"));
	mainLayout->addWidget(include);
	thirdLayout = new QVBoxLayout;
	for(i=0; i<n; ++i)
	{
		if( (*groups)[i] != mGroup && !contains( (*(mGroup->getInclusion())), (*groups)[i]->getId() ) )
		{
			qhb = new QHBoxLayout;
			qcb = new QCheckBox;
			qhb->addWidget(qcb);
			if( (*groups)[i]->getImage() != "0")
			{
				img = new QLabel;
				img->setPixmap( (*groups)[i]->getImage() );
				img->setMaximumSize(50,50);
				img->setScaledContents(true);
				qhb->addWidget(img);
			}
			noms = new QLabel;
			noms->setText( (*groups)[i]->getName() );
			qhb->addWidget(noms);
			thirdLayout->addLayout(qhb);
			localg.push_back((*groups)[i]);
			gqcbs.push_back(qcb);
		}
	}
	mainLayout->addLayout( thirdLayout );
	if(mGroup->getInclusion()->size() != 0)
	{
		Group* temp;
		fourthLayout = new QVBoxLayout;
		rmInc = new QLabel;
		rmInc->setText(tr("<h3>Supprimer les inclusions existantes de ce groupe</h3>"));
		fourthLayout->addWidget(rmInc);
		n = mGroup->getInclusion()->size();
		for(i=0; i<n; ++i)
		{
			temp = mW->getGroupById( (*(mGroup->getInclusion()))[i] );
			qhb = new QHBoxLayout;
			qcb = new QCheckBox;
			qhb->addWidget(qcb);
			if( temp->getImage() != "0")
			{
				img = new QLabel;
				img->setPixmap( temp->getImage() );
				img->setMaximumSize(50,50);
				img->setScaledContents(true);
				qhb->addWidget(img);
			}
			noms = new QLabel;
			noms->setText( temp->getName() );
			qhb->addWidget(noms);
			fourthLayout->addLayout(qhb);
			locali.push_back(temp);
			iqcbs.push_back(qcb);
		}
		mainLayout->addLayout(fourthLayout);
	}
	mainLayout->addWidget(mSubmit);
	this->setLayout( mainLayout );
}

void AddKToGroup::submit()
{
	int i;
	int n = local.size();
	for(i=0; i<n; ++i)
	{
		 if( qcbs[i]->isChecked() )
			mGroup->add(local[i]->getId());
	}
	n = localg.size();
	for(i=0; i<n; ++i)
	{
		 if( gqcbs[i]->isChecked() )
			mGroup->addInclusion(localg[i]->getId());
	}
	n = locali.size();
	for(i=0; i<n; ++i)
	{
		 if( iqcbs[i]->isChecked() )
			mGroup->removeInclusion(locali[i]->getId());
	}
	mW->showGroup(mGroup);
}
