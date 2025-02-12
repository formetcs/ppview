/*
 * PlanPro Viewer
 *
 * Copyright (c) 2017-2025, The FormETCS Project. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mainwindow.h"
#include "preferences.h"
#include "global.h"
#include "version.h"
#include <QApplication>
#include <QTranslator>


/*!
 * \brief Entry point of the program.
 *
 * Initializes the application, loads the translation files and shows the MainWindow.
 * If the name of a PlanPro file is provided as first (and only) argument, this file will be loaded.
 *
 * \param argc the argument count
 * \param argv array of pointers to the argument strings
 * \return 0 if the program terminated without error, otherwise a nonzero number
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setOrganizationDomain(ORGANIZATION_DOMAIN);
    app.setApplicationName(APPLICATION_NAME);
    app.setApplicationVersion(QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH));

    QString language = Preferences::getInstance()->getLanguage();
    QTranslator qtTranslator;
    QTranslator appTranslator;
#if defined Q_OS_WIN
    if(qtTranslator.load(QApplication::applicationDirPath() + "/translations/qt_" + language))
        app.installTranslator(&qtTranslator);
    if(appTranslator.load(QApplication::applicationDirPath() + "/translations/ppview_" + language))
        app.installTranslator(&appTranslator);
#elif defined Q_OS_MACOS
    if(qtTranslator.load(QApplication::applicationDirPath() + "/../Resources/translations/qt_" + language))
        app.installTranslator(&qtTranslator);
    if(appTranslator.load(QApplication::applicationDirPath() + "/../Resources/translations/ppview_" + language))
        app.installTranslator(&appTranslator);
#else // Q_OS_LINUX and others
    if(qtTranslator.load("/usr/share/qt6/translations/qt_" + language))
        app.installTranslator(&qtTranslator);
    if(appTranslator.load(QApplication::applicationDirPath() + "/translations/ppview_" + language))
        app.installTranslator(&appTranslator);
#endif
    QString planprofile = QString();
    if(argc > 1)
        planprofile = argv[1];
    MainWindow mainwindow(planprofile);
    mainwindow.show();

    return app.exec();
}
