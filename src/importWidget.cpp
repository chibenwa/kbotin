/*!
* \file importWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre permettant de choisir les options d'import des contacts
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

#include "importWidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include "mainWindow.hpp"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <fstream>
#include <string>
#include <vector>
#include "kontact.hpp"
#include "cvs.hpp"
#include "VCF.hpp"

#include "paths.hpp"

ImportWidget::ImportWidget( MainWindow* w): QWidget()
{
	mW = w;
	mainLayout = new QVBoxLayout;
	title = new QLabel;
	title->setText(tr("<h1>Choix des options d'importation :</h1>"));
	instr1 = new QLabel;
	instr1->setText(tr("<br/><u>Choix de la source :</u>"));
	destLayout = new QHBoxLayout;
	destLabel = new QLabel;
	destLabel->setText(tr("<aucune>"));
	mDest = new QPushButton(tr("Modifier"));
	mDest->setMaximumSize(mDest->sizeHint());
	destLayout->addWidget(destLabel);
	destLayout->addWidget(mDest);
	instr2 = new QLabel;
	instr2->setText(tr("<br/><u>Choisissez un format :</u>"));
	group = new QGroupBox;
	groupLayout = new QVBoxLayout;
	group->setLayout(groupLayout);
	radioStandard = new QRadioButton(tr("Format KBotin standard"));
	groupLayout->addWidget(radioStandard);
	info = new QLabel;
	info->setText(tr("<br/>Les formats suivant sont compatible avec d'autre application :<br/><br/>"));
	groupLayout->addWidget(info);
	radioThunderBird = new QRadioButton(tr("Format de compatibilité thunderbird (CSV)"));
	groupLayout->addWidget(radioThunderBird);
	radioOutlook = new QRadioButton(tr("Format de compatibilité Outlook (CSV) (En)"));
	groupLayout->addWidget(radioOutlook);
	radioGoogle = new QRadioButton(tr("Format de compatibilité Google+ (CSV) (En)"));
	groupLayout->addWidget(radioGoogle);
	radioKDE = new QRadioButton(tr("Format de compatibilité KAddressBook (CVS)"));
	groupLayout->addWidget(radioKDE);
	radioVCard = new QRadioButton(tr("Format de compatibilité VCard (VCF)"));
	groupLayout->addWidget(radioVCard);
	cvsTemplateLayout = new QHBoxLayout;
	cvsTemplateLabel = new QLabel;
	radioCvsTemplate = new QRadioButton(tr("Utiliser un template pour lire le CVS : "));
	cvsTemplateLayout->addWidget( radioCvsTemplate );
	cvsTemplateLayout->addWidget( cvsTemplateLabel );
	mCvsTemplate = new QPushButton(tr("Choisir"));
	mCvsTemplate->setMaximumSize(mCvsTemplate->sizeHint());
	cvsTemplateLayout->addWidget( mCvsTemplate );
	groupLayout->addLayout( cvsTemplateLayout );
	vcfTemplateLayout = new QHBoxLayout;
	vcfTemplateLabel = new QLabel;
	radioVcfTemplate = new QRadioButton(tr("Utiliser un template pour lire le VCF 3.0 : "));
	vcfTemplateLayout->addWidget( radioVcfTemplate );
	vcfTemplateLayout->addWidget( vcfTemplateLabel );
	mVcfTemplate = new QPushButton(tr("Choisir"));
	mVcfTemplate->setMaximumSize(mVcfTemplate->sizeHint());
	vcfTemplateLayout->addWidget( mVcfTemplate );
	groupLayout->addLayout( vcfTemplateLayout );
	mOk = new QPushButton(tr("Ok"));
	mOk->setMaximumSize(mOk->sizeHint());
	mainLayout->addWidget(title);
	mainLayout->addWidget(instr1);
	mainLayout->addLayout(destLayout);
	mainLayout->addWidget(instr2);
	mainLayout->addWidget(group);
	mainLayout->addWidget(mOk);
	this->setLayout(mainLayout);
	connect(mOk, SIGNAL( clicked() ), this, SLOT( submit() ) );
	connect(mDest, SIGNAL (clicked()), this, SLOT( selectDestination() )  );
	connect(mCvsTemplate, SIGNAL( clicked() ), this, SLOT( chooseTemplate() ) );
	connect(mVcfTemplate, SIGNAL( clicked() ), this, SLOT( chooseVcfTemplate() ) );
} 

void ImportWidget::selectDestination()
{
	QString dest = QFileDialog::getOpenFileName();
	if( dest != "" )
	{
		destLabel->setText(dest);
	}
}

void ImportWidget::submit()
{
	if(destLabel->text() == tr("<aucune>"))
	{
		QMessageBox::critical(this, tr("Erreur"), tr("Veuillez sélectionner une source..."));
		return;
	}
	if(radioStandard->isChecked())
	{		
		std::vector<Kontact*>* plop = new std::vector<Kontact*>;
		mW->basicLoad(destLabel->text(), plop);
		int i;
		int n = plop->size();
		for(i=0; i<n; ++i)
		{
			mW->addToQueue( (*plop)[i] );
		}
		delete plop;
	}
	if(radioThunderBird->isChecked())
		Cvs::imports( destLabel->text(), ThunderBird, mW->getQueue() , mW);
	if(radioGoogle->isChecked())
		Cvs::imports( destLabel->text(), Google, mW->getQueue() , mW);
	if(radioOutlook->isChecked())
		Cvs::imports( destLabel->text(), Outlook , mW->getQueue() , mW);
	if(radioCvsTemplate->isChecked())
	{
		if( cvsTemplateLabel->text() != "" )
		{
			Cvs::importsFromTemplate(destLabel->text(), cvsTemplateLabel->text(), mW->getQueue() , mW );
		}
		else
		{
			QMessageBox::critical(this, tr("Erreur"), tr("Veuillez sélectionner un template VCF..."));
			return;
		}
	}
	if(radioVcfTemplate->isChecked())
	{
		if( vcfTemplateLabel->text() != "" )
		{
			Vcf::importFromTemplate(vcfTemplateLabel->text(),destLabel->text(), mW->getQueue() , mW);
		}
		else
		{
			QMessageBox::critical(this, tr("Erreur"), tr("Veuillez sélectionner un template CVS..."));
			return;
		}
	}
	if(radioKDE->isChecked())
		Cvs::importsFromTemplate(destLabel->text(), QString( COMMON_RESSOURCES_PATH) + "templates/CVS/import/KAdressBook.template",mW->getQueue(), mW);
	mW->saveQueue();
	mW->showQueue();
}

void ImportWidget::chooseTemplate()
{
	QString dest = QFileDialog::getOpenFileName();
	if( dest != "" )
	{
		cvsTemplateLabel->setText(dest);
	}
}

void ImportWidget::chooseVcfTemplate()
{
	QString dest = QFileDialog::getOpenFileName();
	if( dest != "" )
	{
		vcfTemplateLabel->setText(dest);
	}
}
