( French version below // version française en dessous : )

### Grettings :

Link to the application picture : findicons.com/icon/44584/address_book?id=44584
This picture is free.

Qt is distributed under a LGPL license. You will find more informations on :
qt-project.org/products/licensing

### About : 

This programm is distributed under the Creative and Commun license ( By, Share in the same
conditions, 3.0, France)
	
You will find all the informations you need at this address : 
http://creativecommons.org/licences/by-sa/3.0/fr

### INSTALL :

To install on a GTK or KDE based environnement, run :

	cd build
	qmake ..
	make
	sudo make install

It will take about 353 KB for the binary file,
and about 244 KB for shared datas,
for a total of 597 KB.

Your datas will be at ~/.kbotin
Shared datas will be at /usr/share/kbotin
Executable is at /usr/bin/kbotin

Have a good use.
Help is below.

### NON INSTALLED MODE :

You can use this programm without installing it.
To do so, you will need to modify src/paths.hpp : 

Uncomment the non installed block

And comment the installed block.

### UPDATE : 

If you want to deploy a new version of kbotin you modified,
and if you didn't touch the structure of .data folder and files,
you will simply have to run :

		qmake ..
		sudo make update
		
### For traductors :


First of all thanks for your work.
Meanwhile I want you to pay attention to this point : you should always translate the same 
way these expressions :

	- Name
	- First Name
	- Address
	- Postal Code
	- City
	- Web Site
	- Home Phone
	- e-mail
	- Cell Phone
	- <none>
	- ...

## Help :

Welcome in this programme of contact management!
		
The basic page allow you to have a global view of all your contacts. You can see their names
anf first names. While clicking on the button with a + allow you to see the contact's datzas
and to access to modification, suppression, and mailing buttons. In this case button - can be 
use to fold the contact. You can also attach an image to a contact while modifying it. To send
a mail to someone, you need to have thunderbird on your computer and a Unix system.
	
	
### Adding contacts
	
	
To do so, you just have to click on the button 'Add' in your toolbar. It open an interface 
allowing you to add several contacts at the same time. You can also give them as many 
attributes as you want. Click on submit to add them in your contact list.
	
	
### Searching bar
	
	
You can search something in the searching bar. To do so, select the desired field of research
and then enter the exact value you want for this field. The contact with this field value will 
be shown in the central widget.
	
	
### Contact transfer via network
	
	
This programm allow you to share your contacts on the internet (or also private networks...)
To do so, enable Networking in your settings. You can also there specify the address and 
ports of a distant server. Then by clicking on "send" you will send a contact to this server.

You can also send all the contact of the central widget by using the "send all" button.
You can view and moderate the contacts you received via the waiting queue. You can empty
the all waiting queue by using the appropriate button.
Moreover, you can consult your contacts from another computer : you have to allow this in 
your settings,then click on "Consult a distant kbotin". You will have to give the server's password.

By default this is plop.
You can't delete them nor modifying them, but you can export, print, and send them mails.
	
	
### Contact groups
	
	
You can form contact groups. A contact can belong to several groups. These groups can be 
found via the group menu. A managing window can be open via the file menu. Then you can
consult the contacts groups, send a mail to all its members, add contacts...
You can also include a group in an other one :If group A is included in group B, if group A is 
modified, the group B will have the exact same  behaviour. You can modify the inclusions via 
the window enabling contact adding to groups. You are also protected against circular inclusions.
	
	
### Exportation
	
	
The exportatons allow you to save contacts into a file (standard format). You can acces them into
the file menu, then exportation. By using other compatibility format, you can export your contacts
for other applications (Google+, Outlook, Thunderbird, and KAddressBook) but you might loose 
some informations. 
In all the case, choose a destination file, and the appropriate option.
	
	
### Importation
	
	
The	importations options allow you to import contacts from other applications(Google+, Outlook,<br/>
Thunderbird, and KAddressBook). You can access them in the file menu, then importation.<br/><br/>
In all the case, choose a source file, and the appropriate option
	
	
## Advanced help
	
	
### CVS Import template
	
	
These templates, using the short language I will preent you, allow you to import any data at the 
CVS format.
	
This language is composed of 4 key words, delimiting blocks. In these block, they are single values
or coupes of values. These values are separated by line return. Here are these 4 key-words :

  - value : (works by couple of value ) It allows you to definethe name of the attributes in
function of their column's position. These attributes are added before all.  Remeember they 
are a minimum of 9 attributes that should be defined that way, in this order :Name, First Name, 
Address, Postal Code, City, Web Site, e-mail, Home Phone, Cell Phone.
 - forbid Forbid the importationof values in fonction of their positions (integer) 
 - forbidvalues Forbid the importations of colums with a given title.
 - assimilate (works by couple of values) During import, columns with their title 
with the first value as title will create an attribute named with the second value.
	
This symbol : } should be located at the begining of a line for a maximal compatibility
	
An example file is given :../exemples/templateCVS/import/exemple.template
	
	
### CVS Export template
	
	
These templates, using the short language I will preent you, allow you to import any data at the 
CVS you want.
This language is using a single key-word : combinaison
Then comes pairs of values in { }. The first value is giving the title of the column in the CVS file, and
the second is for the attribute that correspond to it. Concerning this second value, two special
exist :

  - ""  fis the statement for a nul column.
  - -- force values of the column to be equal to the value comming next.

Moreover, you can decide to quote values using quote; outside { }. You can also
add a coma at the end of each line using the coma; directive, and at the end of each
contact, using the comafields; one.
	
An example file is given :../exemples/templateCVS/export/exemple.template<br/><br/>
	
### VCF Export template
	
	
These templates, using the short language I will preent you, allow you to export any data at the 
VCF format.
You can find in the doc folder an HTML describing VCF 3.0 format (source : wikipédia).
This (short) reading is required to well understand what is coming next.
Each newline block describes a politic to adopt for the next line of VCF file. The 
first line of this block is the VCF attribute name (before : ) and the following lines caracterises
the values of this attribute.
-- force the value coming next.
Add a line with a = on is own to skip ;
A line without these two valueswill be replaced by the value of the attribute corresponding to 
it in the contact. "" is the nul value.
You will also need to specify the VCF version.
An example file is given :../exemples/templateVCF/export/exemple.template
	
	
### VCF Import template
	
	
These templates, using the short language I will preent you, allow you to import any data at the 
VCF format.
You can find in the doc folder an HTML describing VCF 3.0 format (source : wikipédia).
This (short) reading is required to well understand what is coming next.
Each ref block describe a politic to adopt. The first line caracterises the VCF 
attribute name (before : ). The politic will be applyed when the attribute will be seen.
Next lines describes this policy. They works by pair : We first indicate the separator 
delimiting tne reading filed with a line starting by sep: and the famous separator.
and then the attributes name this field should correspond to.Note : nul value is "" for second field.
An example file is given :../exemples/templateVCF/import/exemple.template

============================================
French version // Version française
============================================

### Remerciements :

Lien vers l'image de l'application : findicons.com/icon/44584/address_book?id=44584
Elle est libre de droit.

Qt est quant à lui distribué sous une license LGPL. Vous trouverez des informations sur :
qt-project.org/products/licensing

### A propos

Le programme suivant est distribué sous la license Creative Commun (Attribution - Partage
dans les Mêmes conditions 3.0 France)

Ceci signifie que vous êtes libres de :

  - partager (reproduire, distribuer et communiquer le présent contenu )
  - remixer ( adapter ce contenu )

Selon les conditions suivantes :
  - attribution : Vous devez attribuer l'oeuvre de la manière indiquée par l'auteur
(mais pas d'une manière qui suggère qu'il vous approuve)
Ici, il s'agit d'ajoutéer la mention : "D'après le travail de Mr Tellier Benoit"
partout ou la présente license est mentionnée. Vous pouvez bien entendu y ajouter
vos commentaires.
  - partage selon les mêmes conditions.

Comprenant bien que :

  - Chacunes des conditions suivantes peut être levée si vous obtenez l'autorisation
du titulaire des droits.
  - Les droits suivants ne sont en aucun cas affecté par la licence:
  - fair use (prérogatives issues des exceptions et limitations
aux droits exclusifs
  - Les droits moraux de l'auteur
  - A chaque réutilisation / distribution de cette oeuvre, la présente licence
Doit apparaitre clairement.

Plus d'informations sur http://creativecommons.org/licences/by-sa/3.0/fr

### INSTALLATION :

Pour installer ce programme vous devez simplement executez ces commandes :

        cd build
        qmake ..
        make
        sudo make install


L'éxécutable fait 353 ko
et les données partagées font 244 ko,
pour un total de 597 ko.

Vos données seront dans ~/.kbotin
Les données partagées dans /usr/share/kbotin
L'éxécutable sera à /usr/bin/kbotin  

Je vous souhaite un bon usage de ce logiciel.
L'aide se situe plus bas.

Mode non Installé :

Vous pouvez utiliser ce programme sans l'installer.
Pour ce faire, vous devrez modifier src/paths.hpp :

Commentez le bloc version installée

et décommentez le bloc version non installée.

### MISE A JOUR : 

Si vous voulez déployer une nouvelle version de kbotin que
vous avez modifié, et que vous n'avez pas touché à la 
structure du dossier ~/.kbotin et de ses fichiers, 

Vous devrez simplement éxécuter :

        qmake ..
        sudo make update

### Note aux traducteurs :

Tout d'abord, merci de traduire mon travail.
J'attire néanmoins votre attention sur le fait que vous devez systématiquement 
traduire les expressions suivantes de la même manière :

  -  Nom
  - Prénom
  - Adresse
  - Code Postal
  - Ville
  - Mobile
  - Site
  - e-mail
  - Téléphone
  - <aucun>
  - ...

### Compilation :

Ce programme utilise Qt 4.8.
Pour le compiler, vos devez posséder les includes de Qt (Gui, Network et Core)
Pour l'éxécuter, il vous faut les librairies Qt (idem)

Compilation sous UNIX:

	(Debian) : installez les paquets liqt4 libqt4-dev
	# rendez vous dans le dossier build
	tapez qmake ..
	suivi de make
	Vous obtenez enfin l'éxécutable.

## Aide

Bienvenue dans ce programme de gestion des contacts!

La page de base permet la visualisation de l'ensemble de vos contacts. Vous
pouvez voir leurs noms et prénoms. Cliquer sur le bouton + permet de voir l'ensemble
du contact et d'accéder aux boutons de suppression et de modification du contact.
Le bouton - qui apparait alors permet de replier le contact.
Vous pouvez alors modifier et supprimer le contact via les boutons prévus à cet effet.
Vous pouvez en plus lier une image à ce contact.


### Ajout de contacts

Pour celà, il suffi de cliquer sur le bouton add de la barre d'outils. Il vous ouvre une
interface permettat l'édition simultanée de plusieurs contacts. Vous pouvez de plus 
leurs ajouter autant d'attributs que vous le désirez. Cliquez ensuite sur soummettre
pour les ajouter à votre ensemble de contacts.


### Barre de recherche

Vous pouvez taper une recherche dans la barre de recherche du programme. Pour celà
selectionnez le champs souhaité, puis la valeur correspondante. Les ontacts ayant 
cette valeur pour ce champs seront alors affichés.


### Transfert de contact via internet


Ce programme vous offre la possibilité  d'échanger vos contacts entre différents ordinateurs.
Il vous faudra pour celà utiliser plusieurs outils :

	- Il vous faut déjà accepter de recevoir des contacts si tel est ce que vous voulez faire...
	Pour cela, je vous invite à vous rendre dans les préférences du menu 'fichier'.

	- Suite à cela, vous pouvez visualiser les contacts que vous avez reçu et les modérer.

	- Pour envoyer des contacts, il vous suffit quand vous les regardez, d'appuyer sur le 
	bouton 'envoyer' après avoir règlé de même la configuration d'envoi.

	-Une options d'envoi massif est disponible. Elle se situe dans le menu fichier, et est
	intitulée 'Envoyer massivement'. Elle permet d'envoyer l'ensemble des contacts affichés


### Groupes de contacts


Vous pouvez former des groupes de contacts. Un contact peut appartenir à plusieurs groupes.
Ces groupes sont directement accesiibles via le menu groupes. Une fenêtre peut être ouverte 
via le menu fichier pour ajouter ou supprimer des groupes. Vous pouvez envyer un mail à 
l'ensemble des contacts d'un groupe.
	
Les groupes de contacts peuvent être inclus les uns dans les autres. Ainsi si le groupe A
inclus dans B est modifié, le groupe B subira alors ces modifications. Les groupes sont
protégés contre les inclusions circulaires. Une visualisation des inclusions est fournie
dans la fenêtre de gestion des groupes. Vous pouvez ajouter ou enlever des inclusions de
groupe via la fenêtre d'ajout de contacts.


### Exportation


Les options d'exportation vous permettent d'enregistrer vos contact dans un fichier (format
standard). Elles sont disponible dans le menu fichier, onglet exportation.

Les autres options d'exports vous font perdre de l'informations mais sont compatibles
avec d'autres applications (Google+, ThunderBird, Outlook et KAdressBook).

Dans tous les cas sélectionnez le fichier de destination ainsi que l'option choisie.


### Importation

Les options d'importations vous permettent de charger des contacts avec des fichiers
produits par d'autres applications de gestion de contacts, à savoir Google+, ThunderBird,
Outlook et KAdressBook. Elles sont disponible dans le menu fichier, onglet importation.
Dans tous les cas sélectionnez le fichier source ainsi que l'option choisie.


## Aide avancées


### Import

### Templates d'import au format CVS
      
Ces templates permetent, via le langage que je vais vous expliquer, d'importer
potentiellement toute donnée au format CVS.

Ce langage comporte 4 mots clé. Il contiennent des valeurs séparées par des retours
à la ligne au sein d'accolades. Ces valeurs peuvent aller par paire, ou être solitaires.
Voici donc les fameux 4 mots clés :

	- value : (fonctionne par couple de valeurs) Il permet de définir le nom des attributs
	 en fonction de leur colonne. Ces attributs sont ajoutés avant tout autre, dans leur
	ordre de déclaration. Pensez de plus à définir via ce moyen les 8 attributs nécéssaire
	 au bon fonctionnement de l'application, et dans le bon ordre... A savoir Nom, Prénom,
	  Adresse, Code Postal, Ville, Site, e-mail, Téléphone, Portable. 
	- forbid : Interdit l'import de valeurs en fonction de leur position (entier)
	- forbidvalue : Interdit l'import des colonnes ayant un valeur spécifiée ici.
	- assimilate : (fonctionne par couple de valeurs) Lors de l'import, les colonnes
	ayant pour valeur associée une chaine ici mentionnée sera remplacée par la chaine
	 suivante dans la déclaration.

Autres astuces : Une accolade se referme en début de ligne (afin d'autoriser une
compatibilité maximum... 

Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire 
contenant l'éxécutable à cette position :

	../exemples/templateCVS/import/exemple.template


###  Templates d'export au format CVS

Ces templates permetent, via le langage que je vais vous expliquer, d'exporter
potentiellement toute donnée au format CVS.

Ce language comporte un seul mot clé : combinaisons
Il est suivi de paires de valeurs séparées par des retours à la ligne au sein
d'accolades. La première valeur indique le nom de la colonne CVS, la deuxième
indique le nom de l'attribut à utiliser pour remplir la colonne.
Concernant celle ci, deux valeurs sont à noter :

	- \"\" indique de laisser la colonne vide
	- -- force les valeurs contenues dans la colonne à être égales à la chaine de
	caractère la succédant directement.

Par ailleurs pour quoter les valeurs, rajouter simplement la directive quote;
en dehors des accolades. Vous pouvez de plus via coma; forcer l'ajout d'une virgule en
fin de chaque ligne, ou via comafields; en fin de chaque ligne de donnée.
	
Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire 
contenant l'éxécutable à cette position :

	../exemples/templateCVS/export/exemple.template

### Template d'export au language CVF
	  
Ces templates permetent, via le langage que je vais vous expliquer, d'exporter 
potentiellement toute donnée au format VCF.
Ce language comporte un seul mot clé : newline
Vous pouvez trouver dans le dossier media une page html explicant comment fonctionne
le format VCF 3.0
Cette lecture (courte) est requise pour bien appréhender ce qui suit.
	
Chaque block newline décrit une politique à adopter pour la ligne suivante pour un
contact. La première ligne caractérise le nom de l'attribut VCF (avant les : ) 
Les lignes d'après caractérisent la valeur de cet attribut.
-- force la valeur qui le suit
Rajouter une ligne avec un = pour ne pas ajouter des ;
Une ligne sans ces deux valeurs se verra remplacée par la valeur de l'attribut 
lui correspondant chez le contact. \"\" correspond à une valeur nulle.
Il est nécéssaire de spécifier la version du format VCARD utilisé.
	
Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire
contenant l'éxécutable à cette position : 
../exemples/templateVCF/export/exemple.template
	
	
###  Template d'import au language CVF
	  
	
Ces templates permetent, via le langage que je vais vous expliquer, d'importer
potentiellement toute donnée au format VCF.
Vous pouvez trouver dans le dossier media une page html explicant comment 
fonctionne le format VCF 3.0. Cette lecture (courte) est requise pour bien
appréhender ce qui suit.Chaque block ref décrit une politique à adopter.La
première ligne caractérise le nom de l'attribut VCF (avant les : ). La politique
sera appliquée quand l'attrbut VCF sera rencontré... Les lignes d'après décrivie
cette politique. Elles fonctionnent par paire : On indique d'abord le séparateur
indiquant le champs de lecture à l'aide de sep: suivi du dit séparateur. Ensuite
 suit l'attribut kbotin auquel on souhaite associer cette valeur. Notons ici de
  \"\" constitue la valeur nulle.

Un fichier d'exemple est fournis. Vous le retrouverez en partant du répertoire 
contenant l'éxécutable à cette position : 

	../exemples/templateVCF/import/exemple.template
