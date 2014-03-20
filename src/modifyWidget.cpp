/*!
* \file modifyWidget.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Fenêtre de modification des contacts
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

#include "modifyWidget.hpp"

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
//#include <QHBoxLayout>
#include "kontact.hpp"
#include "mainWindow.hpp"

#include <iostream>
#include <string>
#include <QFile>

#include "paths.hpp"

ModifyWidget::ModifyWidget(MainWindow* w, Kontact* kon):AddWidget(w,0)
{
	imageName = kon->get_image();
	mKon = kon;
	std::vector< std::pair<QString,QString> >* lis = mKon->getAttributsList();
	values[0]->setText( (*lis)[0].second );
	values[1]->setText( (*lis)[1].second );
	values[2]->setText( (*lis)[2].second );
	values[3]->setText( (*lis)[3].second );
	values[4]->setText( (*lis)[4].second );
	values[5]->setText( (*lis)[5].second );
	values[6]->setText( (*lis)[6].second );
	values[7]->setText( (*lis)[7].second );
	values[8]->setText( (*lis)[8].second );
	int i;
	int n = lis->size();
	for(i=9; i<n; ++i)
	{
		this->AddAttribute();
		values[i]->setText( (*lis)[i].second );
		fields[i-9]->setText( (*lis)[i].first );
	}
	mDelete->setText(tr("Enregistrer"));
	if(kon->get_image() != "0")
	{
		imageLabel->setPixmap(kon->get_image());
	}
}

void ModifyWidget::dell()
{
	mKon->renew();
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
		QString old = mKon->get_image();
		QString newName = QString( PERSONNAL_RESSOURCES_PATH ) + ".data/images/"+sa+"_"+sb+"_"+sc+"_"+sd;
		QFile file_image;
		file_image.setFileName(imageName);
		if(file_image.copy(newName))
		{
			mKon->set_image(newName);
			QFile f;
			f.setFileName(old);
			f.remove();
		}
		else
		{
			mKon->set_image(imageName);
		}
	}
	mKon->setAttribute(tr("Nom"),values[0]->text());
	mKon->setAttribute(tr("Prénom"),values[1]->text());
	mKon->setAttribute(tr("Adresse"),values[2]->text());
	mKon->setAttribute(tr("Code Postal"),values[3]->text());
	mKon->setAttribute(tr("Ville"),values[4]->text());
	mKon->setAttribute(tr("Site"),values[5]->text());
	mKon->setAttribute(tr("e-mail"),values[6]->text());
	mKon->setAttribute(tr("Téléphone"),values[7]->text());
	mKon->setAttribute(tr("Portable"),values[8]->text());
	int n = values.size();
	int i;
	for(i=9; i<n; ++i)
	{
		mKon->setAttribute(fields[i-9]->text(),values[i]->text());
	}
	for(i=0; i<fields.size(); ++i)
	{
		mW->addAttribute(fields[i]->text());
	}
	mW->updateSearchEngine();
	mW->save();
	mW->showContact();
}
