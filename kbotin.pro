TEMPLATE = app
TARGET = kbotin
DEPENDPATH += .
INCLUDEPATH += .

SOURCES += ../src/*.cpp

HEADERS += ../src/*.hpp

QT += network
QT += widgets
QT += core
QT += printsupport

#CONFIG +=  console debug

TRANSLATIONS = 	trad/kbotin_en.ts\
		trad/kbotin_es.ts

target.path = /usr/bin

desktop.path = /usr/share/applications
desktop.files += ../kbotin.desktop

trad.path = /usr/share/kbotin/
trad.files += ../trad

media.path = /usr/share/kbotin/
media.files += ../media

templates.path = /usr/share/kbotin/
templates.files += ../templates

INSTALLS += target desktop trad media templates 

dox.target = update

dox.commands = cp kbotin /usr/bin/kbotin; \
strip /usr/bin/kbotin;\
cp -r ../media /usr/share/kbotin/media;\
cp -r ../templates /usr/share/kbotin/templates;\
cp -r ../trad /usr/share/kbotin/trad;

QMAKE_EXTRA_TARGETS += dox
