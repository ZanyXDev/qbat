#include <qtsingleapplication.h> //alex
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>

#include <cstdio>

#include "powermanager.h"
#include "constants.h"

#ifndef ASTRA_VER
#include <fly/flyhelp.h> //alex
#endif


using namespace qbat;

int main(int argc, char * argv[])
{
	//QApplication app(argc, argv); alex: use single app ---------
	QtSingleApplication app(argc, argv);
	
	if (app.isRunning()) {
	    fprintf(stderr,"qbat: Another instance is running, so I will exit.");
	    app.sendMessage(QString("show")); //alex
	    return 0;
        }
	//-----------------------
	QString locale = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	QTranslator translator;
	translator.load("qbat_" + locale, UI_PATH_TRANSLATIONS);
	app.installTranslator(&translator);

	app.setQuitOnLastWindowClosed(false);
	
	{
		QDir workdir(UI_PATH_WORK);
		if (!workdir.exists()) {
			workdir.cdUp();
			workdir.mkdir(UI_DIR_WORK);
		}
	}

	//flyHelpInstall("qbat"); //alex
	
	CPowerManager mainobject;
        QObject::connect(&app,SIGNAL(messageReceived(const QString &)),&mainobject,SLOT(messageReceived(const QString &))); //alex
	//QObject::connect(&app,SIGNAL(commitDataRequest(QSessionManager&)),&mainobject,SLOT(sessSlot(QSessionManager&))); //alex
	//QObject::connect(&app,SIGNAL(saveStateRequest(QSessionManager&)), &mainobject,SLOT(sessSlot(QSessionManager&))); //alex
	return app.exec();
}
