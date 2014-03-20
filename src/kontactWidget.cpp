/*!
* \file kontactWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre affichant un contact.
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

#include "kontactWidget.hpp"

#include <QFormLayout>
#include <QLabel> 
#include "kontact.hpp"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "mainWindow.hpp"
#include "AllWidget.hpp"
#include "group.hpp"

#include <QString>
#include <map>
#include <cstdlib>
#include <string>
#include "divers.hpp"

//debug
#include <iostream>

KontactWidget::KontactWidget(Kontact* kon, MainWindow* w, AllWidget* all, bool b, bool dist):QWidget()
{
	mAll = all;
	mB = b;
	mW = w;
	mKontact = kon;
	mainLayout = new QFormLayout;
	QLabel* l;
	std::vector< std::pair<QString,QString> >* attributeList = kon->getAttributsList();
	int i;
	int n = attributeList->size();
	int a;
	if(b)
		a = 2;
	else
		a=0;
	for(i = a; i < n; ++i)
	{
		l = new QLabel;
		l -> setText((*attributeList)[i].second);
		l->setTextInteractionFlags( Qt::TextSelectableByMouse );
		mainLayout->addRow("<u><b>"+(*attributeList)[i].first+"</b></u>", l);
	}
	box = new QVBoxLayout;
	box->addLayout(mainLayout);
	if(b)
	{
		{
			QVBoxLayout* groupBox = new QVBoxLayout;
			QLabel* label1 = new QLabel;
			groupBox->addWidget(label1);
			int cpt = 0;
			//affichage de la liste des groupes avec les images...
			std::vector<Group*>* groups = mW->getGroups();
			int i;
			int n = groups->size();
			QHBoxLayout* hbb;
			QLabel* pppp;
			for(i=0; i<n; ++i)
			{
				if( (*groups)[i]->isInGroup( kon->getId() ) )
				{
					++cpt;
					if( (*groups)[i]->getImage() != "0" )
					{
						hbb = new QHBoxLayout;
						pppp = new QLabel;
						pppp->setMaximumSize(50, 50);
						pppp->setScaledContents(true);
						pppp->setPixmap( QPixmap( (*groups)[i]->getImage() ) );
						hbb->addWidget( pppp );
						pppp = new QLabel;
						pppp->setText( (*groups)[i]->getName() );
						hbb->addWidget( pppp );
						groupBox->addLayout( hbb );
					}
					else
					{
						pppp = new QLabel;
						pppp->setText( (*groups)[i]->getName() );
						groupBox->addWidget( pppp );
					}
				}
			}
			if( cpt > 0 )
			{
				if( cpt > 1 )
					label1->setText( tr("<br/><b>Groupes auxquels appartient ce contact :</b>") );
				else
					label1->setText( tr("<br/><b>Groupe auquel appartient ce contact :</b>") );
				box->addLayout( groupBox );
			}
			else
			{
				delete label1;
				delete groupBox;
			}
		}
		HBox = new QHBoxLayout;
		mModify = new QPushButton(tr("Modifier"));
		mModify->setMaximumSize(mModify->sizeHint());
		mDelete = new QPushButton(tr("Supprimer"));
		mDelete->setMaximumSize(mDelete->sizeHint());
		mSend = new QPushButton(tr("Envoyer"));
		mSend->setMaximumSize(mSend->sizeHint());
		mMail = new QPushButton(tr("Envoyer un mail"));
		mMail->setMaximumSize( mMail->sizeHint());
		HBox->addWidget(mModify);
		HBox->addWidget(mDelete);
		HBox->addWidget(mSend);
		HBox->addWidget(mMail);
		box->addLayout(HBox);
		connect(mModify, SIGNAL(clicked()), this, SLOT( modify() ) );
		connect(mDelete, SIGNAL(clicked()), this, SLOT( deleteKontact() ) );
		connect(mSend, SIGNAL( clicked() ), this, SLOT( send() ));
		connect(mMail, SIGNAL( clicked() ), this, SLOT( sendMail() ) );
	}
	if(dist)
	{
		HBox = new QHBoxLayout;
		mSend = new QPushButton(tr("Envoyer"));
		mSend->setMaximumSize(mSend->sizeHint());
		mMail = new QPushButton(tr("Envoyer un mail"));
		mMail->setMaximumSize( mMail->sizeHint());
		HBox->addWidget(mSend);
		HBox->addWidget(mMail);
		box->addLayout(HBox);
		connect(mSend, SIGNAL( clicked() ), this, SLOT( send() ));
		connect(mMail, SIGNAL( clicked() ), this, SLOT( sendMail() ) );
	}
	this->setLayout(box);
}

void KontactWidget::modify()
{
	if(mB)
	{
		mW->modify(mKontact);
	}
}

void KontactWidget::deleteKontact()
{
	if(mB)
	{
		mAll->deleteKontact(mKontact);
	}
}

void KontactWidget::send()
{
	mW->sendKontact(mKontact);
}

void KontactWidget::sendMail()
{
	QString plop("thunderbird -compose to='"+mKontact->getMail()+"'");
	system(plop.toStdString().c_str());
}
