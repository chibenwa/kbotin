/*!
* \file cvsTemplate.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Interpreteur du template d'import CVS
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

#include "cvsTemplate.hpp" 

#include "kontact.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <QStringList>
#include "divers.hpp"
#include <QTranslator>
#include "mainWindow.hpp"

enum ReadingState
{
	None,
	Wait,
	Value,
	Forbid,
	Assimilate,
	ForbidValue,
	Combinaison
};

CvsTemplate::CvsTemplate(QString filePath, MainWindow* w, bool isImport)
{
	mW = w;
	ok = true;
	coma = false;
	quote = false;
	comafields=false;
	std::ifstream ifs(filePath.toStdString().c_str());
	std::string read;
	QString Read;
	ReadingState readingState = None;
	ReadingState next = None;
	bool errors = false;
	int line = 0;
	QString l;
	QString stock;
	QString aff;
	bool binary = true;
	if(isImport)
	{
		bool* intIndicator = new bool;
		(*intIndicator) = true;
		int a;
		while( getline(ifs, read) && !errors)
		{
			l.setNum(line);
			Read = read.c_str();
			debug( QString("Reading line") + l + " ::" + Read, mW );
			++line;
			if( !Read.startsWith("#"))
			{
				while( Read != "" )
				{
					debug("Continue Reading", mW);
					if( readingState == None )
					{
						removingSpaces( Read );
						if( Read.startsWith("value") )
						{
							l.setNum(line);
							debug( QString("switching to mode wait with next mode value in line ")+l , mW);
							Read.remove(0,5);
							readingState = Wait;
							next = Value;
							binary = true;
							continue;
						}
						if( Read.startsWith("assimilate") )
						{
							l.setNum(line);
							debug( QString("switching to mode wait with next mode assimilate in line ")+l , mW);
							Read.remove(0,10);
							readingState = Wait;
							next = Assimilate;
							binary = true;
							continue;
						}
						if( Read.startsWith("forbidvalues") )
						{
							l.setNum(line);
							debug( QString("switching to mode wait with next mode forbidvalues in line ")+l , mW);
							Read.remove(0,12);
							readingState = Wait;
							next = ForbidValue;
							continue;
						}
						if( Read.startsWith("forbid") )
						{
							l.setNum(line);
							debug( QString("switching to mode wait with next mode forbid in line ")+l , mW);
							Read.remove(0,6);
							readingState = Wait;
							next = Forbid;
							continue;
						}
						if( Read != "" )
						{
							l.setNum(line);
							ok = false;
							errorMessage(QTranslator::tr("Attente de la spécification d'un mode mais aucun fournit à la ligne ")+l );
							Read = "";
							errors = true;
						}
						continue;
					}
					if( readingState == Wait )
					{
						removingSpaces( Read );
						if( Read == "" )
							continue;
						if( Read[0] == '{' )
						{
							debug("Switching to next mode completed..." , mW);
							Read.remove(0,1);
							readingState = next;
						}
						else
						{
							l.setNum(line);
							ok = false;
							errorMessage(QTranslator::tr("attente de { à la ligne ")+ l + QTranslator::tr(" mais ") + Read + QTranslator::tr(" fournit à la place") );
							Read = "";
							errors = true;
						}
						continue;
					}
					if( readingState == Forbid )
					{
						removingSpaces( Read );
						if( isThereSomethingAfterSpaces( Read ) )
						{
							l.setNum(line);
							debug(QString("Switching to mode None on line ")+ l , mW);
							Read.remove(0,1);
							readingState = None;
						}
						else
						{
							a = Read.toInt( intIndicator );
							if( !(*intIndicator ) )
							{
								l.setNum(line);
								ok = false;
								errorMessage(QTranslator::tr("entier désiré à la ligne ")+ l);
								Read = "";
								errors = true;
								continue;
							}
							forbid.push_back(a);
							Read = "";
							l.setNum(line);
							aff.setNum(a);
							debug("Reading int "+aff+QString(" on line ")+ l , mW);
						}
						continue;
					}
					if( readingState == Value )
					{
						if( isThereSomethingAfterSpaces( Read ) )
						{
							if( binary )
							{
								removingSpaces(Read);
								l.setNum(line);
								debug(QString("Switching to mode None on line ")+ l , mW);
								Read.remove(0,1);
								readingState = None;
							}
							else
							{
								l.setNum(line);
								ok = false;
								errorMessage(QTranslator::tr("Fin prématurée du fichier ")+ l );
								Read = "";
								errors = true;
							}
						}
						else
						{
							if( binary )
							{
								removingSpaces( Read );
								a = Read.toInt( intIndicator );
								if( !(*intIndicator ) )
								{
									l.setNum(line);
									ok = false;
									errorMessage(QTranslator::tr("Un entier est désiré ligne ")+ l );
									Read = "";
									errors = true;
								}
								binary = false;
								Read = "";
								l.setNum(line);
								aff.setNum(a);
								debug(QString("Reading int ")+aff+QString(" on line ")+ l  , mW);
								binary = false;
							}
							else
							{
								l.setNum(line);
								debug( QString("Reading ")+Read+QString(" on line ")+ l  , mW);
								values.push_back( std::pair<int,QString>(a,Read) );
								Read = "";
								binary = true;
							}
						}
						continue;
					}
					if( readingState == ForbidValue )
					{
						if( isThereSomethingAfterSpaces( Read ) )
						{
							removingSpaces(Read);
							l.setNum(line);
							debug(QString("Switching to mode None on line ")+l , mW);
							Read.remove(0,1);
							readingState = None;
						}
						else
						{
							l.setNum(line);
							debug( QString("Reading ")+Read+QString(" on line ")+l  , mW);
							forbidValues.push_back( Read );
							Read = "";
						}
						continue;
					}
					if( readingState == Assimilate )
					{
						if( isThereSomethingAfterSpaces( Read ) )
						{
							if( binary )
							{
								removingSpaces(Read);
								l.setNum(line);
								debug(QString("Switching to mode None on line ")+l , mW);
								Read.remove(0,1);
								readingState = None;
							}
							else
							{
								l.setNum(line);
								ok = false;
								errorMessage(QTranslator::tr("fin prématurée du fichier ligne ")+QString(l));
								Read = "";
								errors = true;
							}
						}
						else
						{
							if( binary )
							{
								stock = Read;
								Read = "";
								l.setNum(line);
								debug( QString("Reading ")+Read+QString(" on line ")+l  , mW);
								binary = false;
							}
							else
							{
								assimilate.push_back( std::pair<QString,QString>(stock, Read) );
								l.setNum(line);
								debug( "Pushing " + Read + " standing for " + stock + " on line " + l  , mW);
								binary = true;
								Read = "";
							}
						}
						continue;
					}
				}
			}
			else
			{
				Read = "";
				l.setNum(line);
				debug(QString("Comment on line ")+l , mW);
			}
		}
		if( !errors )
		{
			if( readingState != None )
			{
				wellSetted = false;
				ok = false;
				errorMessage(QTranslator::tr("Fin prématurée du fichier. Vous devez avoir oublié un } ..."));
			}
			else
			{
				debug("Succesfull file reading !!! " , mW);
				wellSetted = true;
			}
		}
		delete intIndicator;
	}
	else
	{
		debug("export mode setted" , mW);
		//On lit ici un template d'export...
		while( getline(ifs, read) && !errors)
		{
			Read = read.c_str();
			++line;
			l.setNum(line);
			debug("Reading "+Read+" on line : "+l , mW);
			if( !Read.startsWith("#"))
			{
				while( Read != "" )
				{
					if( readingState == None )
					{
						removingSpaces(Read);
						if(Read.startsWith("quote;"))
						{
							debug("detecting quotes" , mW);
							quote = true;
							Read.remove(0,6);
							continue;
						}
						if(Read.startsWith("coma;"))
						{
							debug("adding coma" , mW);
							coma = true;
							Read.remove(0,5);
							continue;
						}
						if(Read.startsWith("comafields;"))
						{
							debug("adding coma to fields" , mW);
							comafields = true;
							Read.remove(0,11);
							continue;
						}
						if(Read.startsWith("combinaisons"))
						{
							debug("Will switch to mode combinaisons" , mW);
							readingState = Wait;
							next = Combinaison;
							Read.remove(0,12);
							continue;
						}
						if( Read != "" )
						{
							l.setNum(line);
							ok = false;
							errorMessage(QTranslator::tr("En attente de spécification d'un mode mais aucun fournit ligne ")+l );
							Read = "";
							errors = true;
						}
						continue;
					}
					if(readingState == Wait)
					{
						removingSpaces( Read );
						if( Read == "" )
							continue;
						if( Read[0] == '{' )
						{
							debug("Switching to next mode completed..." , mW);
							Read.remove(0,1);
							readingState = next;
						}
						else
						{
							l.setNum(line);
							ok = false;
							errorMessage(QTranslator::tr("Attente d'un { ligne ")+ l + QTranslator::tr(" mais ") + Read + QTranslator::tr(" est fournit à la place") );
							Read = "";
							errors = true;
						}
						continue;
					}
					if(readingState == Combinaison)
					{
						if( isThereSomethingAfterSpaces( Read ) )
						{
							if( binary )
							{
								removingSpaces(Read);
								l.setNum(line);
								debug(QString("Switching to mode None on line ")+l);
								Read.remove(0,1);
								readingState = None;
							}
							else
							{
								l.setNum(line);
								ok = false;
								errorMessage(QTranslator::tr("Fin prématurée de fichier à la ligne ")+l+QTranslator::tr(" Vous devez avoir oublié un } "));
								Read = "";
								errors = true;
							}
						}
						else
						{
							if( binary )
							{
								stock = Read;
								Read = "";
								l.setNum(line);
								debug( QString("Reading ")+Read+QString(" on line ")+l  , mW);
								binary = false;
							}
							else
							{
								exporter.push_back( std::pair<QString,QString>(stock, Read) );
								l.setNum(line);
								debug( "Pushing " + Read + " standing for " + stock + " on line " + l  , mW);
								binary = true;
								Read = "";
							}
						}
						continue;
					}
				}
			}
		}
	}
}

void CvsTemplate::imports(QString fileName, std::vector<Kontact*>* dest)
{
	//On commence par charger le fichier à importer en RAM...
	debug("getting started" , mW);
	std::ifstream ifs(fileName.toStdString().c_str());
	std::string read;
	getline(ifs, read);
	QStringList headers = QString(read.c_str()).split(",");
	std::vector<QStringList> qsl;
	while( getline(ifs, read) )
	{
		std::cout << "        Reading : " << read << std::endl;
		qsl.push_back( QString(read.c_str()).split(",") );
	}
	int n = qsl.size();
	//Maintenant on traite ceci :p
	//Matching des values
	int i, j;
	int secu = headers.size();
	Kontact* kon;
	for(j=0; j< n; ++j)
	{
		kon = new Kontact;
		//Matching des valeurs...
		for(i=0; i<values.size(); ++i)
		{
			if(values[i].first >0 && values[i].first < secu)
			{
				kon->setAttribute(traitement(values[i].second), traitement(qsl[j].at(values[i].first)) );
				debug("Ajout de l'attibut "+values[i].second+" : "+qsl[j].at(values[i].first) , mW);
			}
		}
		//On parcours maintenant l'ensemble des cases en prenant garde aux valeurs et cases interdites ainsi qu'aux assimilations
		for(i=0; i< secu; ++i)
		{
			debug("Reading attibute..." , mW);
			if( i > qsl[j].size() )
			{
				QString l;
				l.setNum(j);
				errorMessage(QTranslator::tr(" Case manquante dans le fichier CVS ligne ")+j);
				delete kon;
				return;
			}
			QString aze,l;
			aze.setNum(i);
			l.setNum(qsl[j].size());
			debug(aze + " / " + l , mW);
			if( !contains(forbid,i) && !contains(values,i) && !contains(forbidValues, qsl[j].at(i)) )
			{
				if( traitement(qsl[j].at(i)) != "" )
				{
					kon->setAttribute(traitement(memberOf(assimilate, headers[i])), traitement(qsl[j].at(i)) );
					debug("Ajout de l'attibut "+headers[i]+" : "+qsl[j].at(i)  , mW);
				}
			}
		}
		debug("Ajout du contact" , mW);
		dest->push_back( kon );
	}
}

void CvsTemplate::exports(QString filePath, std::vector<Kontact*>* orig)
{
	int i, j;
	int n = orig->size();
	std::ofstream ofs(filePath.toStdString().c_str());
	QString val;
	QString res="";
	//exportation du header CVS
	int k = exporter.size();
	val.setNum(k);
	debug("vect size : "+val , mW);
	for(i=0; i<k-1; ++i)
	{
		res = quoter(exporter[i].first, quote);
		ofs << res.toStdString() << ",";
	}
	res = quoter(exporter[k-1].first, quote);
	debug("res : "+res , mW);
	ofs << res.toStdString();
	if(coma)
		ofs << ",";
	ofs << std::endl;
	//evacuation des contacts
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!  " << n << std::endl;
	for(i=0; i<n; ++i)
	{
		
		for(j=0; j<k; ++j)
		{
			if( exporter[j].second == "\"\"" )
				res = quoter( "", quote);
			else
				if(exporter[j].second.startsWith("<--"))
					res = quoter(r3(exporter[j].second) , quote);
				else
				{
					res = quoter( (*orig)[i]->findValue( exporter[j].second ), quote);
				}
			if( res != "" )
				ofs << res.toStdString();
			if(j<k-1)
				ofs << ",";
			else
				if(coma || comafields)
					ofs << "," << std::endl;
				else
					ofs << std::endl;
		}
	}
}
