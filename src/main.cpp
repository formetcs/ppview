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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setOrganizationDomain(ORGANIZATION_DOMAIN);
    app.setApplicationName(APPLICATION_NAME);
    app.setApplicationVersion(QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH));

    QString language = Preferences::getInstance()->getLanguage();
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
