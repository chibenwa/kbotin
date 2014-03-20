/*!
* \file preferencesWidget.hpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Edition des préférences
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

#include "preferencesWidget.hpp" 

#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include "mainWindow.hpp"

PreferencesWidget::PreferencesWidget(Preference* pref, MainWindow* w):QWidget()
{
	mW = w;
	mPref = pref;
	mainLayout = new QVBoxLayout;
	formLayout = new QFormLayout;
	buttonLayout = new QHBoxLayout;
	activeNetworking = new QCheckBox;
	listeningPort = new QSpinBox;
	listeningPort->setMinimum(0);
	listeningPort->setMaximum(65000);
	maxContactLength = new QSpinBox;
	maxContactLength->setMinimum(0);
	maxContactLength->setMaximum(100000);
	nbMax = new QSpinBox;
	nbMax->setMinimum(0);
	nbMax->setMaximum(100000);
	maxContactLength->setMinimum(10);
	mApply= new QPushButton(tr("Appliquer"));
	mOk = new QPushButton(tr("Ok"));
	mApply->setMaximumSize(mApply->sizeHint());
	mOk->setMaximumSize(mOk->sizeHint());
	address = new QLineEdit;
	distPort = new QSpinBox;
	distPort->setMinimum(0);
	distPort->setMaximum(65000);
	passWord = new QLineEdit;
	passWord->setEchoMode( QLineEdit::Password );
	distantConfig = new QCheckBox;
	debugger = new QCheckBox;
	netError = new QCheckBox;
	formLayout->addRow(tr("Activer le réseau :"), activeNetworking);
	formLayout->addRow(tr("Port d'écoute :"), listeningPort);
	formLayout->addRow(tr("Taille maximale d'un contact reçu :"), maxContactLength);
	formLayout->addRow(tr("Taille maximale de la boite de réception :"), nbMax);
	formLayout->addRow(tr("Adresse d'envoi :"), address);
	formLayout->addRow(tr("Port d'écoute du serveur d'arrivée :"), distPort);
	formLayout->addRow(tr("Mot de passe pour consultation des contacts en ligne :"), passWord );
	formLayout->addRow(tr("Consultation du répertoire à distance :"), distantConfig );
	formLayout->addRow(tr("Montrer les ereurs de réseau :"), netError);
	formLayout->addRow(tr("Activer le mode debug :"), debugger);
	buttonLayout->addWidget(mApply);
	buttonLayout->addWidget(mOk);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	this->setLayout(mainLayout);
	if(mPref->networking)
		activeNetworking->setCheckState( Qt::Checked );
	else
		activeNetworking->setCheckState( Qt::Unchecked );
	listeningPort->setValue(mPref->port);
	maxContactLength->setValue(mPref->contactSize);
	nbMax->setValue( mPref->tailSize );
	address->setText( mPref->address );
	passWord->setText( mPref->passWord );
	distPort->setValue( mPref->distPort );
	if(mPref->distantConfig)
		distantConfig->setCheckState( Qt::Checked );
	else
		distantConfig->setCheckState( Qt::Unchecked );
	if(mPref->netError)
		netError->setCheckState( Qt::Checked );
	else
		netError->setCheckState( Qt::Unchecked );
	if(mPref->debugger)
		debugger->setCheckState( Qt::Checked );
	else
		debugger->setCheckState( Qt::Unchecked );
	connect(mApply, SIGNAL( clicked() ), this, SLOT( apply() ) );
	connect(mOk, SIGNAL( clicked() ), this, SLOT( ok() ) );
}

void PreferencesWidget::apply()
{
	mPref->networking = activeNetworking->isChecked();
	mPref->port = listeningPort->value();
	mPref->contactSize = maxContactLength->value();
	mPref->tailSize = nbMax->value();
	mPref->address = address->text();
	mPref->distPort = distPort->value();
	mPref->passWord = passWord->text();
	mPref->distantConfig = distantConfig->isChecked();
	mPref->debugger = debugger->isChecked();
	mPref->netError = netError->isChecked();
	mW->savePref();
	mW->setNetworking();
}

void PreferencesWidget::ok()
{
	mW-> showContact();
}
