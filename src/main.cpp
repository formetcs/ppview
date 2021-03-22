#include "mainwindow.h"
#include "makro.h"
#include "version.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setOrganizationDomain(ORGANIZATION_DOMAIN);
    app.setApplicationName(APPLICATION_NAME);
    app.setApplicationVersion(QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH));

    QSettings settings;
    QString language = settings.value("preferences/language", "en").toString();
    QTranslator qtTranslator;
#ifdef _WINDOWS
    if(qtTranslator.load(QApplication::applicationDirPath() + "/translations/qt_" + language))
        app.installTranslator(&qtTranslator);
#else
    if(qtTranslator.load(QApplication::applicationDirPath() + "/qt_" + language))
        app.installTranslator(&qtTranslator);
#endif
    QTranslator appTranslator;
    if(appTranslator.load(QApplication::applicationDirPath() + "/ppview_" + language))
        app.installTranslator(&appTranslator);

    QString datafile = QString();
    if(argc > 1)
        datafile = argv[1];
    MainWindow w(datafile);
    w.show();

    return app.exec();
}
