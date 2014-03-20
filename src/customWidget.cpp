/*!
* \file customWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Une fenêtre montrant les noms prénoms du contact. Si on la déplie, on peu consulter le contact intégralement.
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

#include "customWidget.hpp"

#include "kontact.hpp"
#include "kontactWidget.hpp"
#include "mainWindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <vector>
#include <QString>
#include <utility>
#include "divers.hpp"

CustomWidget::CustomWidget(Kontact* kon, MainWindow* w, AllWidget* all, bool isG, bool dist):QWidget()
{
	mIsG = isG;
	mAll = all;
	b=false;
	mW = w;
	mKon = kon;
	mainLayout = new QVBoxLayout;
	presLayout = new QHBoxLayout;
	if(mIsG)
	{
		mDeleteFromGroup = new QPushButton(tr("Enlever"));
		mDeleteFromGroup->setMaximumSize(mDeleteFromGroup->sizeHint());
	}
	contactOverview = new QLabel;
	contactOverview->setText( "<b>"+((*(kon->getAttributsList()))[0]).second + " " + (((*(kon->getAttributsList()))[1]).second ) + "</b>");
	contactOverview->setTextInteractionFlags( Qt::TextSelectableByMouse );
	mDeplie = new QPushButton("+");
	mDeplie->setMaximumSize(mDeplie->sizeHint());
	mainLayout->addLayout(presLayout);
	if( dist)
	{
		details = new KontactWidget(kon, w, mAll, false, true);
	}
	else
	{
		details = new KontactWidget(kon, w, mAll);
	}
	details->setVisible(false);
	mainLayout->addWidget(details);
	this->setLayout(mainLayout);
	imageLabel = new QLabel;
	imageLabel->setMaximumSize(50,50);
	imageLabel->setScaledContents(true);
	if(kon->get_image() != "0")
	{
		imageLabel->setPixmap(kon->get_image());
	//	debug("Image path : " + kon->get_image(), mW);
	}
	presLayout->addWidget(imageLabel);
	if(mIsG)
	{
		presLayout->addWidget(mDeleteFromGroup);
		connect(mDeleteFromGroup, SIGNAL( clicked() ), this, SLOT( removeKontactFromGroup() ) );
	}
	presLayout->addWidget(contactOverview);
	presLayout->addWidget(mDeplie);
	connect(mDeplie, SIGNAL( clicked() ), this, SLOT( showContact() ) );
}

void CustomWidget::showContact()
{
	if(b)
	{
		details->setVisible(false);
		b=false;
		mDeplie->setText("+");
	}
	else
	{
		details->setVisible(true);
		b=true;
		mDeplie->setText("-");
	}
}

void CustomWidget::removeKontactFromGroup()
{
	emit please(mKon->getId());
	delete this;
}
