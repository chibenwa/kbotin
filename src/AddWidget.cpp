/*!
* \file AddWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre permettant la saisie d'un contact
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

#include "AddWidget.hpp"

#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include "mainWindow.hpp"
#include "QPUWithInt.hpp"

#include <QHBoxLayout>
#include "kontact.hpp"
#include <QSize>
#include <QFileDialog>
#include <QFile>
#include "paths.hpp"

///Debug
#include <iostream>

AddWidget::AddWidget(MainWindow* w,int n):QWidget()
{
	imageName="0";
	mN = n;
	mW = w;
	buttonLayout = new QHBoxLayout;
	mAddAttribute = new QPushButton(tr("Ajouter un attribut"));
	mAddAttribute->setMaximumSize(mAddAttribute->sizeHint());
	mDelete = new QPushButton(tr("Supprimer le contact"));
	mDelete->setMaximumSize(mDelete->sizeHint());
	mainLayout = new QVBoxLayout;
	formLayout = new QFormLayout;
	editLayout = new QVBoxLayout;
	imageLayout = new QHBoxLayout;
	this->setLayout(mainLayout);
	buttonLayout->addWidget(mAddAttribute);
	buttonLayout->addWidget(mDelete);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addLayout(imageLayout);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(editLayout);
	imageInstr = new QLabel;
	imageInstr->setText(tr("Choisissez une image : "));
	imageLabel = new QLabel;
	imageLabel->setMaximumSize(50,50);
	imageLabel->setScaledContents(true);
	mChoose = new QPushButton("Choisir");
	mChoose->setMaximumSize(mChoose->sizeHint());
	imageLayout->addWidget( imageInstr );
	imageLayout->addWidget( imageLabel );
	imageLayout->addWidget( mChoose );
	//le formulaire
	QLineEdit* ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Nom"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Prénom"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Adresse"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Code Postal"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Ville"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Site"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("e-mail"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Téléphone"),ed);
	ed = new QLineEdit;
	values.push_back(ed);
	formLayout->addRow(tr("Portable"),ed);
	connect(mAddAttribute, SIGNAL( clicked() ), this, SLOT( AddAttribute() ) );
	connect(mDelete, SIGNAL( clicked() ), this, SLOT( dell() ) );
	connect(mChoose, SIGNAL( clicked() ), this, SLOT( chooseImage() ) );
}

void AddWidget::AddAttribute()
{
	QPUWithInt* qpu = new QPUWithInt(QPUV.size());
	QPUV.push_back(qpu);
	QLineEdit* ed1 = new QLineEdit;
	QLineEdit* ed2 = new QLineEdit;
	QHBoxLayout* temp = new QHBoxLayout;
	temp->addWidget(ed1);
	temp->addWidget(ed2);
	temp->addWidget(qpu);
	values.push_back(ed2);
	fields.push_back(ed1);
	editLayout->addLayout(temp);
	tempV.push_back(temp);
	connect(qpu, SIGNAL( deleteMe(int) ), this, SLOT( dellAtribute(int) ));
}

void AddWidget::addKontact()
{
	Kontact* kon = new Kontact;
	int i;
	{
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
		QString old = kon->get_image();
		QString newName = QString ( PERSONNAL_RESSOURCES_PATH ) + ".data/images/"+sa+"_"+sb+"_"+sc+"_"+sd;
		QFile file_image;
		file_image.setFileName(imageName);
		if(file_image.copy(newName))
		{
			kon->set_image(newName);
			QFile f;
			f.setFileName(old);
			f.remove();
		}
		else
		{
			kon->set_image(imageName);
		}
	}
	kon->setAttribute(tr("Nom"),values[0]->text());
	kon->setAttribute(tr("Prénom"),values[1]->text());
	kon->setAttribute(tr("Adresse"),values[2]->text());
	kon->setAttribute(tr("Code Postal"),values[3]->text());
	kon->setAttribute(tr("Ville"),values[4]->text());
	kon->setAttribute(tr("Site"),values[5]->text());
	kon->setAttribute(tr("e-mail"),values[6]->text());
	kon->setAttribute(tr("Téléphone"),values[7]->text());
	kon->setAttribute(tr("Portable"),values[8]->text());
	for(i=0; i<fields.size(); ++i)
	{
		kon->setAttribute(fields[i]->text(), values[9+i]->text() );
		mW->addAttribute(fields[i]->text());
	}
	kon->updateId();
	mW->addKontact(kon);
	mW->updateSearchEngine();
}

void AddWidget::dell()
{
	emit dellDemande(mN);
}

void AddWidget::dellAtribute(int n)
{
	//suppr du layout concerné et des élémens qui le composent...
	editLayout->removeItem(editLayout->itemAt(n));
	delete fields[n];
	delete tempV[n];
	delete QPUV[n];
	delete values[n+9];
	fields.erase(fields.begin()+n);
	tempV.erase(tempV.begin()+n);
	QPUV.erase(QPUV.begin()+n);
	values.erase(values.begin()+n+9);
	int i;
	for(i=n; i < QPUV.size(); ++i)
	{
		QPUV[i]->decrease();
	}
}

void AddWidget::chooseImage()
{
	QString dest = QFileDialog::getOpenFileName(this, tr("Choisissez une image..."), QString( COMMON_RESSOURCES_PATH ) + "media", "Images (*.png *.xpm *.jpg *.jpeg)");
	if( dest != "" )
	{
		imageName = dest;
		imageLabel->setPixmap(dest);
	}
}
