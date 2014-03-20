/*!
* \file groupMng.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Widget de gestion des groupes de contacts
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

#include "groupMng.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "group.hpp"
#include "mainWindow.hpp"
#include "buttonMod.hpp"

#include "divers.hpp"
#include <QFile>

GroupMng::GroupMng(std::vector<Group*>* gs, MainWindow* w)
{
	mW = w;
	groups = gs;
	mainLayout = new QVBoxLayout;
	titre = new QLabel;
	titre->setText(tr("<h1>Gestion des groupes : </h1><br/>Liste des groupes : <br/> <br/>"));
	groupsLayout = new QVBoxLayout;
	buttonLayout = new QHBoxLayout;
	instr = new QLabel;
	instr->setText(tr("<br/>Ajoutez des groupes : "));
	mainLayout->addWidget(titre);
	mainLayout->addLayout( groupsLayout );
	mainLayout->addWidget(instr);
	mainLayout->addLayout( buttonLayout );
	int i,j,k;
	int n = groups->size();
	QHBoxLayout* plop;
	QPushButton* del;
	QLabel* lab;
	QPushButton* see;
	QLabel* name;
	QLabel* inclImage;
	QHBoxLayout* inclLayout;
	QLabel* inclTitle;
	QLabel* incl;
	Group* temp;
	for(i=0; i<n; ++i)
	{
		plop = new QHBoxLayout;
		del = new ButtonMod(tr("Supprimer"), (*groups)[i], true);
		del->setMaximumSize(del->sizeHint());
		plop->addWidget(del);
		if( (*groups)[i]->getImage() != "0" )
		{
			lab =new QLabel;
			lab->setPixmap( (*groups)[i]->getImage() );
			debug( "Group image path : " + (*groups)[i]->getImage() , mW);
			lab->setMaximumSize(50,50);
			lab->setScaledContents(true);
			plop->addWidget(lab);
		}
		name = new QLabel;
		name->setText("<b>"+(*groups)[i]->getName()+"</b>");
		name->setTextInteractionFlags( Qt::TextSelectableByMouse );
		plop->addWidget(name);
		if( (*groups)[i]->getInclusion()->size() > 0 )
		{
			inclLayout = new QHBoxLayout;
			inclTitle = new QLabel;
			inclTitle->setText(tr("Groupes inclus :"));
			inclLayout->addWidget(inclTitle);
			k = (*groups)[i]->getInclusion()->size();
			for(j=0; j<k; ++j)
			{
				temp = mW->getGroupById( (*((*groups)[i]->getInclusion()))[j] );
				if(temp->getImage() != "0")
				{
					inclImage = new QLabel;
					inclImage->setPixmap( temp->getImage() );
					debug( "Group image path : " + temp->getImage() , mW);
					inclImage->setMaximumSize(50,50);
					inclImage->setScaledContents(true);
					inclLayout->addWidget( inclImage );
				}
				incl = new QLabel;
				incl->setText(temp->getName() + "; ");
				inclLayout->addWidget( incl );
			}
			plop->addLayout( inclLayout );
		}
		see = new ButtonMod(tr("Voir"), (*groups)[i], false);
		see->setMaximumSize(see->sizeHint());
		plop->addWidget(see);
		groupsLayout->addLayout(plop);
		layouts.push_back(plop);
		connect((*groups)[i], SIGNAL( deleteMe(Group*) ), this, SLOT( remove(Group*) ) );
		connect(see, SIGNAL( askShow(Group*)), this, SLOT( showGroup(Group*)));
		labels.push_back(name);
		showb.push_back(see);
		delb.push_back(del);
	}
	edit = new QLineEdit;
	mAdd = new QPushButton(tr("Ajouter"));
	mAdd->setMaximumSize(mAdd->sizeHint());
	buttonLayout->addWidget(edit);
	buttonLayout->addWidget(mAdd);
	this->setLayout( mainLayout );
	connect(mAdd, SIGNAL( clicked() ), this, SLOT( add() ) );
} 

void GroupMng::add()
{
	if( edit->text() != "" )
	{
		Group* g = new Group;
		g->newId();
		QHBoxLayout* plop;
		QPushButton* del;
		QPushButton* see;
		QLabel* name;
		g->setName(edit->text());
		plop = new QHBoxLayout;
		del = new ButtonMod(tr("Supprimer"), g, true);
		del->setMaximumSize(80,25);
		plop->addWidget(del);
		name = new QLabel;
		name->setText("<b>"+g->getName()+"</b>");
		plop->addWidget(name);
		see = new ButtonMod(tr("Voir"), g, false);
		see->setMaximumSize(40,25);
		plop->addWidget(see);
		groupsLayout->addLayout(plop);
		layouts.push_back(plop);
		groups->push_back(g);
		mW->saveGroups();
		labels.push_back(name);
		showb.push_back(see);
		delb.push_back(del);
		connect(g, SIGNAL( deleteMe(Group*) ), this, SLOT( remove(Group*) ) );
		connect(see, SIGNAL( askShow(Group*)), this, SLOT( showGroup(Group*)));
		mW->addGroupAction(g);
		edit->setText("");
	}
}

void GroupMng::remove(Group* g)
{
	int i;
	int n = groups->size();
	QFile f;
	f.setFileName(g->getImage());
	f.remove();
	for(i=0; i<n; ++i)
	{
		if( g == (*groups)[i] )
		{
			mW->removeGroupAction(g);
			groups->erase( groups->begin() + i);
			delete layouts[i];
			layouts.erase( layouts.begin() + i);
			delete delb[i];
			delb.erase( delb.begin() + i);
			delete showb[i];
			showb.erase(showb.begin() + i);
			delete labels[i];
			labels.erase( labels.begin() + i);
			mW->saveGroups();
		}
	}
}

void GroupMng::showGroup(Group* g)
{
	mW->showGroup(g);
}
	
