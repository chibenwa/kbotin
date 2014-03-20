/*!
* \file distantWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe permettant de rentrer le mot de passe pour acceder au kbotin distant
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

#include "consultConfig.hpp"

#include <QLabel>
#include <QLineEdit> 
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include "mainWindow.hpp"

#include "socket.hpp"

#include "divers.hpp"

ConsultWidget::ConsultWidget(MainWindow* w)
{
	mW = w;
	QVBoxLayout* mainLayout = new QVBoxLayout;
	QLabel* title = new QLabel;
	title->setText(tr("<h2>Rentrez les informations du serveur distant :</h2>"));
	mainLayout->addWidget( title );
	QFormLayout* qfo = new QFormLayout;
	ed3 = new QLineEdit;
	ed3->setEchoMode( QLineEdit::Password );
	qfo->addRow( tr( "Mot de passe : "), ed3);
	mainLayout->addLayout( qfo );
	QPushButton* mSubmit = new QPushButton(tr("Soumettre"));
	mSubmit->setMaximumSize( mSubmit->sizeHint() );
	mainLayout->addWidget( mSubmit );
	this->setLayout( mainLayout );
	connect( mSubmit, SIGNAL( clicked() ), this, SLOT( submit() ) );
};

void ConsultWidget::submit()
{
	debug("submitted",mW);
	Socket* soc = new Socket( mW->getPref(), mW);
	soc->askDistantContactList(ed3->text());
}
