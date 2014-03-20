/*!
* \file VCF.cpp
* \author Benoit Tellier
* \version 0.1
* \date 21/06/2013
*
* \brief Classe de travail sur le format VCF...
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

#include "VCF.hpp"

#include "kontact.hpp"

#include <fstream>
#include <string>
#include "divers.hpp"
#include <utility>
#include <map>
#include <QTranslator>

#include "mainWindow.hpp"

enum VcfExportState
{
	None,
	Wait,
	Newline
};

Vcf::Vcf()
{
	
}

bool Vcf::importFromTemplate(QString templateFile, QString source, std::vector<Kontact*>* dest, MainWindow* w)
{
	// Nom de la référence, contenu de la référence
	std::map< QString, std::vector< std::pair<QString, QString> >* > aRefs;
	//Lecture et parçage du template
	//
	// Chargement du template
	{
		//Politique suivie: Nom de l'attribut, séparateur attendu ensuite...
		debug("start import : reading template",w);
		std::vector< std::pair<QString, QString> >* aRef;
		VcfExportState state = None;
		std::string read;
		QString Read;
		std::ifstream ifs( templateFile.toStdString().c_str() );
		int line = 0;
		QString l;
		QString vcfName;
		QString sep;
		bool first = true;
		bool binary = true;
		while( getline(ifs, read ) )
		{
			++line;
			l.setNum(line);
			debug("Reading line "+l,w);
			Read = read.c_str();
			if( Read.startsWith("#") )
			{
				l.setNum(line);
				Read = "";
				debug("Commentairy on line "+l,w);
			}
			while( Read != "" )
			{
				debug("Continue reading",w);
				if( state == None )
				{
					removingSpaces( Read );
					if( Read.startsWith( "ref" ) )
					{
						state = Wait;
						Read.remove(0,3);
						l.setNum(line);
						debug("Switching Mode to Wait on line "+l, w);
						continue;
					}
					if( Read != "" )
					{
						l.setNum(line);
						errorMessage(QTranslator::tr("Motif inconnu : ")+Read + QTranslator::tr(" on line ") +l);
						return false;
					}
				}
				if( state == Wait )
				{
					removingSpaces( Read );
					if( Read.startsWith( "{" ) )
					{
						state = Newline;
						Read.remove(0,1);
						l.setNum(line);
						debug("Switching Mode to Ref on line "+l, w);
						first = true;
						continue;
					}
					else
					{
						l.setNum(line);
						errorMessage(QTranslator::tr(" { attendu mais aucun fournit à la ligne ")+l);
						std::map< QString, std::vector< std::pair<QString, QString> >* >::iterator it;
						for(it = aRefs.begin(); it != aRefs.end(); it++)
						{
							delete (*it).second;
						}
						return false;
					}
				}
				if( state == Newline )
				{
					if( first )
					{
						if( Read == "" )
						{
							continue;
						}
						else
						{
							l.setNum(line);
							debug("Adding name "+Read+" on line "+l, w);
							vcfName = Read;
							aRef = new std::vector< std::pair<QString, QString> >;
							Read = "";
							first = false;
							continue;
						}
					}
					else
					{
						if( isThereSomethingAfterSpaces( Read ) )
						{
							l.setNum(line);
							debug("Switching to mode none on line "+l, w);
							aRefs[vcfName] = aRef;
							removingSpaces( Read );
							Read.remove(0,1);
							state = None;
							first = true;
							binary = true;
							continue;
						}
						else
						{
							if(binary)
							{
								binary = false;
								if( ! Read.startsWith("sep:") )
								{
									l.setNum(line);
									errorMessage(QTranslator::tr("attente de la descrition d'un séparateur non stasfaite à la ligne ")+l);
									delete aRef;
									std::map< QString, std::vector< std::pair<QString, QString> >* >::iterator it;
									for(it = aRefs.begin(); it != aRefs.end(); it++)
									{
										delete (*it).second;
									}
									return false;
								}
								else
								{
									Read.remove(0,4);
									sep = Read;
									debug("providing separator "+sep+"on line "+l, w);
									Read = "";
									continue;
								}
							}
							else
							{
								binary = true;
								aRef->push_back( std::pair<QString, QString>(Read, sep) );
								l.setNum( line );
								debug("Adding policy "+Read+" with separator "+sep+" on line "+l, w);
								Read = "";
								continue;
							}
						}
					}
				}
			}
		}
	}
	// Importation
	{
		std::ifstream ifs( source.toStdString().c_str() );
		std::string read;
		QString Read;
		QString plop;
		QString policy;
		std::vector< std::pair<QString, QString> >* aRef;
		int state = 0;
		int line = 0;
		int fio = 0;
		int n;
		int i;
		Kontact* kon;
		QString l;
		while( getline(ifs, read) )
		{
			debug("Reading",w);
			++line;
			Read = read.c_str();
			while( Read != "" )
			{
				debug("new turn",w);
				if( state == 0 )
				{
					if(Read.startsWith("BEGIN:VCARD") )
					{
						debug("Starting reading contact",w);
						state = 1;
						Read = "";
						continue;
					}
					else
					{
						l.setNum(line);
						errorMessage(QTranslator::tr("Attendait BEGIN:VCARD mais ce n'est pas le cas à la ligne ")+l);
						std::map< QString, std::vector< std::pair<QString, QString> >* >::iterator it;
						for(it = aRefs.begin(); it != aRefs.end(); it++)
						{
							delete (*it).second;
						}
						return false;
					}
				}
				if( state == 1 )
				{
					if( Read.startsWith("VERSION:") )
					{
						debug("Version specification", w);
						state = 2;
						Read = "";
						kon = new Kontact;
						continue;
					}
					else
					{
						l.setNum(line);
						errorMessage(QTranslator::tr("Pas de version VCF détecté ligne ")+l);
						std::map< QString, std::vector< std::pair<QString, QString> >* >::iterator it;
						for(it = aRefs.begin(); it != aRefs.end(); it++)
						{
							delete (*it).second;
						}
						return false;
					}
				}
				if( state == 2 )
				{
					if( Read.startsWith("END:VCARD") )
					{
						dest->push_back(kon);
						state = 0;
						Read = "";
						continue;
					}
					else
					{
						//Traitement d'une ligne !!!
						if( Read.contains( ":" ) )
						{
							plop = Read;
							fio = plop.indexOf(":");
							plop.remove(fio, plop.size()-fio );
							if( aRefs.find( plop ) != aRefs.end() )
							{
								debug("Entry found : "+plop, w);
								aRef = aRefs[plop];
								Read.remove(0, fio +1);
								debug("Trainting :"+Read,w);
								//Bon là c'est la partie technique...
								//Essais foireux
								debug("New interpretation",w);
								n = aRef->size();
								for(i=0; i<n; ++i)
								{
									if( (*aRef)[i].first != "\"\"" )
									{
										if( Read.contains( (*aRef)[i].second ) )
										{
											debug("Séparation detected",w);
											fio = Read.indexOf( (*aRef)[i].second );
											policy = Read;
											policy.remove(fio, Read.size()-fio);
											Read.remove(0, fio+(*aRef)[i].second.size());
											debug("Read value ::--::==::--::"+Read,w);
										}
										else
										{
											policy = Read;
											Read = "";
										}
										debug("Value of "+(*aRef)[i].first+" is "+policy,w);
										kon->setAttribute((*aRef)[i].first, policy);
									}
									else
									{
										if( Read.contains( (*aRef)[i].second ) )
										{
										fio = Read.indexOf( (*aRef)[i].second );
											Read.remove(0, (*aRef)[i].second.size()+fio -1);
										}
										else
										{
											Read = "";
										}
										Read.remove(0, (*aRef)[i].second.size() );
										debug("Read state ::--::==::--:: " + Read,w);
									}
									if( Read == "" )
									{
										i = n;
									}
								}
								Read = "";
							}
							else
							{
								debug("Unknown entry : "+plop,w);
								Read = "";
								continue;
							}
						}
						else
						{
							l.setNum(line);
							errorMessage(QTranslator::tr("Pas de symbole \":\" à la ligne ")+l);
							std::map< QString, std::vector< std::pair<QString, QString> >* >::iterator it;
							for(it = aRefs.begin(); it != aRefs.end(); it++)
							{
								delete (*it).second;
							}
							return false;
						}
					}
				}
			}
		}
		if( state != 0 )
		{
			l.setNum(line);
			errorMessage(QTranslator::tr("Fin prématurée du fichier à la ligne ")+l);
			std::map< QString, std::vector< std::pair<QString, QString> >* >::iterator it;
			for(it = aRefs.begin(); it != aRefs.end(); it++)
			{
				delete (*it).second;
			}
			return false;
		}
	}
	// Désalocution
	std::map< QString, std::vector< std::pair<QString, QString> >* >::iterator it;
	for(it = aRefs.begin(); it != aRefs.end(); it++)
	{
		delete (*it).second;
	}
	return true;
}

bool Vcf::exportFromTemplate(QString templateFile, QString dest, std::vector<Kontact*>* source, MainWindow* w)
{
	//définitions des données internes
	std::vector< std::vector< QString >* > exportRules;
	std::vector< QString > names;
	{
		//Chargement du template
		VcfExportState state = None;
		std::string read;
		QString Read;
		std::vector< QString >* exportRule;
		std::ifstream ifs( templateFile.toStdString().c_str() );
		int line = 0;
		QString l;
		QString cvfName;
		bool first;
		while( getline(ifs, read ) )
		{
			++line;
			Read = read.c_str();
			if( Read.startsWith("#") )
				Read = "";
			while( Read != "" )
			{
				if( state == None )
				{
					removingSpaces( Read );
					if( Read.startsWith( "newline" ) )
					{
						state = Wait;
						Read.remove(0,7);
						l.setNum(line);
						debug("Switching Mode to Wait on line "+l,w);
						continue;
					}
					if( Read != "" )
					{
						l.setNum(line);
						errorMessage(QTranslator::tr("Motif inconnu : ")+Read + QTranslator::tr(" on line ")+l);
						int i;
						int n = exportRules.size();
						for(i=0; i<n; ++i)
							delete exportRules[i];
						return false;
					}
				}
				if( state == Wait )
				{
					removingSpaces( Read );
					if( Read.startsWith( "{" ) )
					{
						state = Newline;
						Read.remove(0,1);
						l.setNum(line);
						debug("Switching Mode to Newline on line "+l,w);
						exportRule = new std::vector< QString >;
						exportRules.push_back( exportRule );
						first = true;
						continue;
					}
					else
					{
						l.setNum(line);
						errorMessage(QTranslator::tr(" { attendu mais non fournit à la ligne ")+l);
						int i;
						int n = exportRules.size();
						for(i=0; i<n; ++i)
							delete exportRules[i];
						return false;
					}
				}
				if( state == Newline )
				{
					if( first )
					{
						if( Read == "" )
						{
							continue;
						}
						else
						{
							l.setNum(line);
							debug("Adding name "+Read+" on line "+l,w);
							names.push_back(Read);
							Read = "";
							first = false;
							continue;
						}
					}
					else
					{
						if( isThereSomethingAfterSpaces( Read ) )
						{
							l.setNum(line);
							debug("Switching to mode none on line "+l,w);
							removingSpaces( Read );
							Read.remove(0,1);
							state = None;
							continue;
						}
						else
						{
							l.setNum(line);
							debug("Adding policy "+Read+" on line "+l,w);
							exportRule->push_back( Read );
							Read = "";
							continue;
						}
					}
				}
			}
		}
	}
	//Exportation
	{
		std::ofstream ofs( dest.toStdString().c_str() );
		int i,j,k,m;
		int l = exportRules.size();
		int n = source->size();
		QString plop;
		bool flop = true;
		for(i=0; i<n; ++i)
		{
			ofs << "BEGIN:VCARD" << std::endl;
//			ofs << "VERSION:3.0" << std::endl;
			l = exportRules.size();
			for(j=0; j<l; ++j)
			{
				ofs << names[j].toStdString() << ":";
				m = exportRules[j]->size();
				for(k=0; k<m; ++k)
				{
					if( (*(exportRules[j]))[k] != "\"\""  && !((*(exportRules[j]))[k].startsWith("=") ) )
					{
						if( (*(exportRules[j]))[k].startsWith("<--") )
						{
							plop = (*(exportRules[j]))[k];
							plop.remove(0,3);
							debug("Injecting fixed value "+plop,w);
							ofs << plop.toStdString();
						}
						else
						{
							plop = (*source)[i]->findValue( (*(exportRules[j]))[k] );
							ofs << plop.toStdString();
							debug("Find value "+plop+" --of-- "+(*(exportRules[j]))[k],w);
						}
					}
					if( ( k < m-1 ) && (*(exportRules[j]))[k+1].startsWith("=") )
					{
						debug("Detecting equal",w);
						flop = false;
					}
					if( (*(exportRules[j]))[k].startsWith("=") )
						flop = false;
					if( ( k < m-1 ) && flop)
						ofs << ";";
					flop = true;
				}
				ofs << std::endl;
			}
			ofs << "END:VCARD" << std::endl;
		}
	}
	//Désalocution
	int i;
	int n = exportRules.size();
	for(i=0; i<n; ++i)
		delete exportRules[i];
	return true;
}
