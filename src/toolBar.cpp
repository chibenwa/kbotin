/*!
* \file toolBar.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Barre d'outils
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

#include "toolBar.hpp"

#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QComboBox>
#include "mainWindow.hpp"
#include <map>
#include <utility>
#include <QString>

#include <iostream>

ToolBar::ToolBar(MainWindow* w):QWidget()
{
	mW = w;
	layout = new QHBoxLayout;
	mAdd = new QPushButton(tr("Add"));
	mSearch = new QLineEdit;
	mAtt = new QComboBox;
	std::map<QString, int>*  index = w->getAttributs();
	std::map<QString, int>::iterator it;
	for(it = index->begin(); it != index->end(); it++)
	{
		mAtt->addItem((*it).first);
	}
	mOk = new QPushButton(tr("Ok"));
	mAll = new QPushButton(tr("All"));
	mAdd->setMaximumSize(mAdd->sizeHint());
	mOk->setMaximumSize(mOk->sizeHint());
	layout->addWidget(mAdd);
	layout->addWidget(mAll);
	layout->addWidget(mAtt); 
	layout->addWidget(mSearch);
	layout->addWidget(mOk);
	this->setLayout(layout);
	connect(mAll, SIGNAL( clicked() ), mW, SLOT( showContact() ) );
	connect(mAdd, SIGNAL( clicked() ), mW, SLOT( add() ) );
	connect(mW, SIGNAL( needSource() ), this, SLOT( majCombo() ) );
	connect(mOk, SIGNAL( clicked() ), mW, SLOT( search() ) );
} 

void ToolBar::majCombo()
{
	//on enlève tout...
	int n = mAtt->count();
	int i;
	std::cout << "C'est partit : " << n << std::endl;
	for(i=0; i<n; ++i)
	{
		mAtt->removeItem(0);
	}
	n = mAtt->count();
	std::cout << "Et là : " << n << std::endl;
	//puis on ajoute...
	std::map<QString, int>*  index = mW->getAttributs();
	std::map<QString, int>::iterator it;
	for(it = index->begin(); it != index->end(); it++)
	{
		mAtt->addItem((*it).first);
	}
}

QString ToolBar::getValue()
{
	return mSearch->text();
}

QString ToolBar::getField()
{
	return mAtt->currentText();
}

ToolBar::~ToolBar()
{
	delete layout;
	delete mSearch;
	delete mAtt;
	delete mAll;
}
