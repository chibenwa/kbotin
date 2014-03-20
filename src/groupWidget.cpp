/*!
* \file groupWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe permettant la visualisation d'un groupe
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

#include "groupWidget.hpp" 

#include "group.hpp"
#include "mainWindow.hpp"
#include <QLabel>
#include "doublonWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "kontact.hpp"

#include <cstdlib>
#include <QFileDialog>
#include <QFile>
#include <algorithm>
#include "divers.hpp"

#include "paths.hpp"

GroupWidget::GroupWidget(Group* g, MainWindow* w):QWidget()
{
	group = g;
	mW = w;
	std::vector<int> ploplop;
	std::vector<int>* vect = g->getVect();
	kontacts = new std::vector<Kontact*>;
	Kontact* kon;
	int i;
	int n = vect->size();
	for(i=0; i<n; ++i)
	{
		kon = w->askKontact( (*vect)[i] );
		if(kon != 0)
			kontacts->push_back(kon);
		ploplop.push_back( (*vect)[i] );
	}
	//ici algo foireux
	std::vector<int>* ginc = g->getInclusion();
	i = 0;
	int k,j;
	Group* gBis;
	std::vector<int> protection;
	protection.push_back( g->getId() );
	while( i  < ginc->size() )
	{
		//On inclut les contact du groupe
		gBis = mW->getGroupById((*ginc)[i]);
		if( gBis!= 0 )
		{
			k = gBis->getVect()->size();
			protection.push_back( gBis->getId() );
			for(j=0; j<k; ++j)
			{
				if( !contains( ploplop, (*(gBis->getVect()))[j] ) )
				{
					kon = w->askKontact( (*(gBis->getVect()))[j] );
					if(kon != 0)
					kontacts->push_back(kon);
					ploplop.push_back( (*(gBis->getVect()))[j] );
				}
			}
			k= gBis->getInclusion()->size();;
			for(j=0; j<k; ++j)
			{
				if( !contains(protection, (*(gBis->getInclusion()))[j]) )
					ginc->push_back( (*(gBis->getInclusion()))[j] );
			}
		}
		++i;
	}
	//retour à la normale
	sort( kontacts->begin(), kontacts->end(), rel);
	titre = new QLabel;
	titre->setText(tr("<h1>Groupe ")+ g->getName() +"</h1>");
	titre->setTextInteractionFlags( Qt::TextSelectableByMouse );
	titleLayout = new QHBoxLayout;
	imageButton = new QPushButton();
	imageLabel = new QLabel(imageButton);
	imageLabel->setMaximumSize(50,50);
	imageLabel->setScaledContents(true);
	if(group->getImage() != "0")
	{
		imageLabel->setPixmap( group->getImage() );
		debug( "Group image path :: " + group->getImage() );
	}
	imageLabel->setMargin(5);
	imageButton->setMaximumSize(50,50);
	imageButton->setMinimumSize(50,50);
	titleLayout->addWidget(imageButton);
	titleLayout->addWidget(titre);
	doublonWidget = new DoublonWidget(kontacts, w, true);
	mainLayout = new QVBoxLayout;
	mainLayout->addLayout( titleLayout );
	mainLayout->addWidget( doublonWidget );
	buttonLayout = new QHBoxLayout;
	mAdd = new QPushButton(tr("Ajouter des kontacts"));
	mAdd->setMaximumSize(mAdd->sizeHint());
	buttonLayout->addWidget(mAdd);
	mMailAll = new QPushButton(tr("Envoyer un mail à tous"));
	mMailAll->setMaximumSize(mMailAll->sizeHint());
	buttonLayout->addWidget(mMailAll);
	mainLayout->addLayout(buttonLayout);
	this->setLayout( mainLayout );
	connect(mMailAll, SIGNAL( clicked() ), this, SLOT( mailAll() ) );
	connect(doublonWidget, SIGNAL( please(int) ), this, SLOT( pleaseTrt(int)));
	connect(mAdd, SIGNAL( clicked() ), this, SLOT( addK() ));
	connect(imageButton, SIGNAL( clicked() ), this, SLOT( changeImage() ) );
	connect(mW, SIGNAL( printRequest() ), this, SLOT( print() ) );
}

void GroupWidget::mailAll()
{
	int i;
	int n = kontacts->size();
	QString plop = "thunderbird -compose to='";
	for(i=0; i<n-1; ++i)
		plop += (*kontacts)[i]->getMail() + ";";
	plop += (*kontacts)[n-1]->getMail();
	plop += "'";
	system(plop.toStdString().c_str());
}

void GroupWidget::pleaseTrt(int a)
{
	group->remove(a);
	mW->saveGroups();
}

void GroupWidget::addK()
{
	mW->addKToG(group);
}

void GroupWidget::changeImage()
{
	debug( "try changing image ...", mW );
	QString t = QFileDialog::getOpenFileName(this, tr("Choisissez une image"), "~", "Images (*.png *.jpg *.bmp)");
	if(t != "")
	{
		debug( "Changing image...", mW );
		int a,b,c,d;
		a=rand();
		b=rand();
		c=rand();
		d=rand();
		QString sa, sb, sc, sd;
		sa.setNum(a);
		sb.setNum(b);
		sc.setNum(c);
		sd.setNum(d);
		QString old = group->getImage();
		debug( "Old : " + old , mW);
		QString newName = QString( PERSONNAL_RESSOURCES_PATH ) + ".data/images/"+sa+"_"+sb+"_"+sc+"_"+sd;
		debug( "new : "+ newName );
		debug( "origin : " + t , mW);
		QFile file_image;
		file_image.setFileName(t);
		if(file_image.copy(newName))
		{
			debug( "Copy OK", mW );
			group->setImage(newName);
			if( old != "0" )
			{
				debug("removing previous", mW);
				QFile f;
				f.setFileName(old);
				f.remove();
			}
			imageLabel->setPixmap(newName);
			imageLabel->setScaledContents(true);
		}
		else
		{
			debug( "Erreur de copie", mW );
		}
	}
}

void GroupWidget::print()
{
	QString p = tr("<h1>Groupe ")+group->getName()+"</h1>";
	int i;
	int n = kontacts->size();
	for(i=0; i<n;++i)
	{
		(*kontacts)[i]->print(p);
	}
	mW->print(p);
}
