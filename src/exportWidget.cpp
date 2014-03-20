/*!
* \file exportWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre permettant de choisir les options d'export des contacts
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

#include "exportWidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include "kontact.hpp"
#include "mainWindow.hpp"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <fstream>
#include <string>
#include "cvs.hpp"
#include "VCF.hpp"

#include "paths.hpp"

ExportWidget::ExportWidget( MainWindow* w, std::vector<Kontact*>* tut): QWidget()
{
	mW = w;
	mVect = tut;
	mainLayout = new QVBoxLayout;
	title = new QLabel;
	title->setText(tr("<h1>Choix des options d'exortation :</h1>"));
	instr1 = new QLabel;
	instr1->setText(tr("<br/><u>Choix de la destination :</u>"));
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
	info->setText(tr("<br/>Les formats suivant sont compatible avec d'autre application <br/>\
	mais sont plus pauvres que le format natif:<br/><br/>"));
	groupLayout->addWidget(info);
	radioThunderBird = new QRadioButton(tr("Format de compatibilité thunderbird (CSV)"));
	groupLayout->addWidget(radioThunderBird);
	radioOutlook = new QRadioButton(tr("Format de compatibilité Outlook (CSV) (En)"));
	groupLayout->addWidget(radioOutlook);
	radioGoogle = new QRadioButton(tr("Format de compatibilité Google+ (CSV) (En)"));
	groupLayout->addWidget(radioGoogle);
	radioKDE = new QRadioButton(tr("Format de compatibilité KAddressBook (CSV) (En)"));
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
	radioVcfTemplate = new QRadioButton(tr("Utiliser un template VCF 3.0 : "));
	vcfTemplateLabel = new QLabel;
	mVcfTemplate = new QPushButton(tr("Choisir"));
	vcfTemplateLayout->addWidget(radioVcfTemplate);
	vcfTemplateLayout->addWidget(vcfTemplateLabel);
	vcfTemplateLayout->addWidget(mVcfTemplate);
	groupLayout->addLayout(vcfTemplateLayout);
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
	connect(mCvsTemplate, SIGNAL( clicked() ), this, SLOT( selectTemplate() ) );
	connect(mVcfTemplate, SIGNAL( clicked() ), this, SLOT( selectVcfTemplate() ) );
}

void ExportWidget::selectDestination()
{
	QString dest = QFileDialog::getSaveFileName();
	if( dest != "" )
	{
		destLabel->setText(dest);
	}
}

void ExportWidget::submit()
{
	using namespace std;
	if(destLabel->text() == tr("<aucune>"))
	{
		QMessageBox::critical(this, tr("Erreur"), tr("Veuillez sélectionner une destination..."));
		return;
	}
	if(radioStandard->isChecked())
	{		
		mW->basicSave(destLabel->text(), mVect );
		this->close();
		return;
	}
	if(radioGoogle->isChecked())
	{
		if(Cvs::exportsFromTemplate(destLabel->text(), QString( COMMON_RESSOURCES_PATH ) + "templates/CVS/export/google.template", mVect, mW))
			this->close();
		return;
	}
	if(radioThunderBird->isChecked())
	{
		if(Cvs::exportsFromTemplate(destLabel->text(), QString( COMMON_RESSOURCES_PATH ) + "templates/CVS/export/thunderbird.template", mVect, mW))
			this->close();
		return;
	}
	if(radioOutlook->isChecked())
	{
		if(Cvs::exportsFromTemplate(destLabel->text(), QString( COMMON_RESSOURCES_PATH ) + "templates/CVS/export/outlook.template", mVect, mW))
			this->close();
		return;
	}
	if(radioKDE->isChecked())
	{
		if(Cvs::exportsFromTemplate(destLabel->text(), QString( COMMON_RESSOURCES_PATH ) + "templates/CVS/export/KAddressBook.template", mVect, mW))
			this->close();
		return;
	}
	if(radioCvsTemplate->isChecked())
	{
		if( cvsTemplateLabel->text() != "" )
		{
			if(Cvs::exportsFromTemplate(destLabel->text(), cvsTemplateLabel->text(), mVect, mW))
				this->close();
			return;
		}
		else
		{
			QMessageBox::critical(this, tr("Erreur"), tr("Veuillez sélectionner un template CVS..."));
			return;
		}
	}
	if( radioVcfTemplate->isChecked() )
	{
		if( vcfTemplateLabel->text() != "" )
		{
			if( Vcf::exportFromTemplate(vcfTemplateLabel->text(), destLabel->text(), mVect, mW) )
				this->close();
			return;
		}
		else
		{
			QMessageBox::critical(this, tr("Erreur"), tr("Veuillez sélectionner un template VCF..."));
			return;
		}
	}
	if( radioVCard->isChecked() )
	{
		
		if( Vcf::exportFromTemplate(QString( COMMON_RESSOURCES_PATH ) + "template/VCF/export/vcf.template" , destLabel->text(), mVect, mW) )
			this->close();
		return;
	}
}

void ExportWidget::selectTemplate()
{
	QString dest = QFileDialog::getOpenFileName();
	if( dest != "" )
	{
		cvsTemplateLabel->setText(dest);
	}
}

void ExportWidget::selectVcfTemplate()
{
	QString dest = QFileDialog::getOpenFileName();
	if( dest != "" )
	{
		vcfTemplateLabel->setText(dest);
	}
}
