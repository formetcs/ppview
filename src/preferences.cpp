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

#include "preferences.h"

Preferences* Preferences::preferences = 0;

Preferences::Preferences()
{
    setDefaultValues();
    readSettings();
}

Preferences* Preferences::getInstance()
{
    if(preferences == 0)
    {
        preferences = new Preferences();
    }
    return preferences;
}

void Preferences::readSettings()
{
    QSettings settings;
    size = settings.value("geometry/size", QSize(500, 450)).toSize();
    pos = settings.value("geometry/pos", QPoint(200, 200)).toPoint();
    windowState = settings.value("geometry/windowState").toByteArray();
    language = settings.value("preferences/language", "en").toString();
    zoomStep = settings.value("preferences/zoomStep", 20).toInt();
    minZoom = settings.value("preferences/minZoom", 10).toInt();
    rotateStep = settings.value("preferences/rotateStep", 15).toInt();
    maxRecursionDepth = settings.value("preferences/maxRecursionDepth", 5).toInt();

    settings.beginGroup("filterSettings");
    QStringList keys = settings.childKeys();
    for(int i = 0; i < keys.count(); ++i)
    {
        QString filter = keys.at(i);
        bool enabled = settings.value(filter, true).toBool();
        filterSettings.insert(filter, enabled);
    }
    settings.endGroup();

    settings.beginGroup("colors/pens");
    keys = settings.childKeys();
    for(int i = 0; i < keys.count(); ++i)
    {
        QString type = keys.at(i);
        QPen pen = settings.value(type, QPen(Qt::black)).value<QPen>();
        graphicsViewPens.insert(type, pen);
    }
    settings.endGroup();

    settings.beginGroup("colors/brushes");
    keys = settings.childKeys();
    for(int i = 0; i < keys.count(); ++i)
    {
        QString type = keys.at(i);
        QBrush brush = settings.value(type, QBrush(Qt::white)).value<QBrush>();
        graphicsViewBrushes.insert(type, brush);
    }
    settings.endGroup();

    objectListFgStartBrush = settings.value("colors/objectListFgStartBrush", QBrush(Qt::black)).value<QBrush>();
    objectListBgStartBrush = settings.value("colors/objectListBgStartBrush", QBrush(Qt::yellow)).value<QBrush>();
    objectListFgEndBrush = settings.value("colors/objectListFgEndBrush", QBrush(Qt::black)).value<QBrush>();
    objectListBgEndBrush = settings.value("colors/objectListBgEndBrush", QBrush(Qt::red)).value<QBrush>();
    objectListFgBothBrush = settings.value("colors/objectListFgBothBrush", QBrush(Qt::black)).value<QBrush>();
    objectListBgBothBrush = settings.value("colors/objectListBgBothBrush", QBrush(Qt::white)).value<QBrush>();

    graphicsViewStartPen = settings.value("colors/graphicsViewStartPen", QPen(Qt::yellow)).value<QPen>();
    graphicsViewStartBrush = settings.value("colors/graphicsViewStartBrush", QBrush(Qt::yellow)).value<QBrush>();
    graphicsViewEndPen = settings.value("colors/graphicsViewEndPen", QPen(Qt::red)).value<QPen>();
    graphicsViewEndBrush = settings.value("colors/graphicsViewEndBrush", QBrush(Qt::red)).value<QBrush>();
    graphicsViewBothPen = settings.value("colors/graphicsViewBothPen", QPen(Qt::black)).value<QPen>();
    graphicsViewBothBrush = settings.value("colors/graphicsViewBothBrush", QBrush(Qt::black)).value<QBrush>();
}

void Preferences::writeSettings()
{
    QSettings settings;
    settings.setValue("geometry/size", size);
    settings.setValue("geometry/pos", pos);
    settings.setValue("geometry/windowState", windowState);
    settings.setValue("preferences/language", language);
    settings.setValue("preferences/zoomStep", zoomStep);
    settings.setValue("preferences/minZoom", minZoom);
    settings.setValue("preferences/rotateStep", rotateStep);
    settings.setValue("preferences/maxRecursionDepth", maxRecursionDepth);

    QHash<QString, bool>::const_iterator i = filterSettings.constBegin();
    while(i != filterSettings.constEnd())
    {
        settings.setValue("filterSettings/" + i.key(), i.value());
        ++i;
    }

    QHash<QString, QPen>::const_iterator i2 = graphicsViewPens.constBegin();
    while(i2 != graphicsViewPens.constEnd())
    {
        settings.setValue("colors/pens/" + i2.key(), i2.value());
        ++i2;
    }

    QHash<QString, QBrush>::const_iterator i3 = graphicsViewBrushes.constBegin();
    while(i3 != graphicsViewBrushes.constEnd())
    {
        settings.setValue("colors/brushes/" + i3.key(), i3.value());
        ++i3;
    }

    settings.setValue("colors/objectListFgStartBrush", objectListFgStartBrush);
    settings.setValue("colors/objectListBgStartBrush", objectListBgStartBrush);
    settings.setValue("colors/objectListFgEndBrush", objectListFgEndBrush);
    settings.setValue("colors/objectListBgEndBrush", objectListBgEndBrush);
    settings.setValue("colors/objectListFgBothBrush", objectListFgBothBrush);
    settings.setValue("colors/objectListBgBothBrush", objectListBgBothBrush);

    settings.setValue("colors/graphicsViewStartPen", graphicsViewStartPen);
    settings.setValue("colors/graphicsViewStartBrush", graphicsViewStartBrush);
    settings.setValue("colors/graphicsViewEndPen", graphicsViewEndPen);
    settings.setValue("colors/graphicsViewEndBrush", graphicsViewEndBrush);
    settings.setValue("colors/graphicsViewBothPen", graphicsViewBothPen);
    settings.setValue("colors/graphicsViewBothBrush", graphicsViewBothBrush);
}

void Preferences::setDefaultValues()
{
    graphicsViewPens.insert("GEO_Punkt", QPen(Qt::black));
    graphicsViewPens.insert("GEO_Knoten", QPen(Qt::black));
    graphicsViewPens.insert("GEO_Kante", QPen(Qt::black));
    graphicsViewPens.insert("TOP_Knoten", QPen(Qt::red));
    graphicsViewPens.insert("TOP_Kante", QPen(Qt::red));
    graphicsViewPens.insert("Datenpunkt", QPen(Qt::black));
    graphicsViewPens.insert("ZUB_Bereichsgrenze", QPen(Qt::red));
    graphicsViewPens.insert("Bahnsteig_Zugang", QPen(Qt::cyan));
    graphicsViewPens.insert("BUE_Anlage", QPen(Qt::blue));
    graphicsViewPens.insert("BUE_Gefahrraum_Eckpunkt", QPen(Qt::blue));
    graphicsViewPens.insert("BUE_Kante", QPen(Qt::blue));
    graphicsViewPens.insert("GFR_Tripelspiegel", QPen(Qt::blue));
    graphicsViewPens.insert("Schrankenantrieb", QPen(Qt::blue));
    graphicsViewPens.insert("Sonstiger_Punkt", QPen(Qt::magenta));
    graphicsViewPens.insert("Hoehenpunkt", QPen(Qt::black));
    graphicsViewPens.insert("Technischer_Punkt", QPen(Qt::black));
    graphicsViewPens.insert("Ueberhoehung", QPen(Qt::black));
    graphicsViewPens.insert("FMA_Element", QPen("brown"));
    graphicsViewPens.insert("FMA_Komponente", QPen("brown"));
    graphicsViewPens.insert("Zugeinwirkung", QPen("brown"));
    graphicsViewPens.insert("PZB_Element", QPen(Qt::gray));
    graphicsViewPens.insert("Signal", QPen(Qt::green));
    graphicsViewPens.insert("Signal_Befestigung", QPen(Qt::black));
    graphicsViewPens.insert("Gleis_Abschluss", QPen("coral"));
    graphicsViewPens.insert("W_Kr_Gsp_Komponente", QPen("coral"));

    graphicsViewPens.insert("ZUB_Streckeneigenschaft", QPen(Qt::yellow));
    graphicsViewPens.insert("Stell_Bereich", QPen("orange"));
    graphicsViewPens.insert("Bahnsteig_Dach", QPen(Qt::cyan));
    graphicsViewPens.insert("Bahnsteig_Kante", QPen(Qt::cyan));
    graphicsViewPens.insert("BUE_Gleisbezogener_Gefahrraum", QPen(Qt::blue));
    graphicsViewPens.insert("Fstr_Fahrweg", QPen(Qt::magenta));
    graphicsViewPens.insert("Geschwindigkeitsprofil", QPen(Qt::red));
    graphicsViewPens.insert("Strecke", QPen(Qt::red));
    graphicsViewPens.insert("Technischer_Bereich", QPen(Qt::red));
    graphicsViewPens.insert("Gleis_Abschnitt", QPen("olive"));
    graphicsViewPens.insert("Gleis_Art", QPen("olive"));
    graphicsViewPens.insert("Gleis_Baubereich", QPen("olive"));
    graphicsViewPens.insert("Gleis_Bezeichnung", QPen("olive"));
    graphicsViewPens.insert("Gleis_Fahrbahn", QPen("olive"));
    graphicsViewPens.insert("Gleis_Lichtraum", QPen("olive"));
    graphicsViewPens.insert("Gleis_Schaltgruppe", QPen("olive"));

    graphicsViewBrushes.insert("GEO_Punkt", QBrush(Qt::white));
    graphicsViewBrushes.insert("GEO_Knoten", QBrush(Qt::black));
    graphicsViewBrushes.insert("TOP_Knoten", QBrush(Qt::red));
    graphicsViewBrushes.insert("Datenpunkt", QBrush(Qt::yellow));
    graphicsViewBrushes.insert("ZUB_Bereichsgrenze", QBrush(Qt::yellow));
    graphicsViewBrushes.insert("Bahnsteig_Zugang", QBrush(Qt::cyan));
    graphicsViewBrushes.insert("BUE_Anlage", QBrush(Qt::blue));
    graphicsViewBrushes.insert("BUE_Gefahrraum_Eckpunkt", QBrush(Qt::blue));
    graphicsViewBrushes.insert("BUE_Kante", QBrush(Qt::blue));
    graphicsViewBrushes.insert("GFR_Tripelspiegel", QBrush(Qt::blue));
    graphicsViewBrushes.insert("Schrankenantrieb", QBrush(Qt::blue));
    graphicsViewBrushes.insert("Sonstiger_Punkt", QBrush(Qt::magenta));
    graphicsViewBrushes.insert("Hoehenpunkt", QBrush(Qt::red));
    graphicsViewBrushes.insert("Technischer_Punkt", QBrush(Qt::red));
    graphicsViewBrushes.insert("Ueberhoehung", QBrush(Qt::red));
    graphicsViewBrushes.insert("FMA_Element", QBrush("brown"));
    graphicsViewBrushes.insert("FMA_Komponente", QBrush("brown"));
    graphicsViewBrushes.insert("Zugeinwirkung", QBrush("brown"));
    graphicsViewBrushes.insert("PZB_Element", QBrush(Qt::gray));
    graphicsViewBrushes.insert("Signal", QBrush(Qt::green));
    graphicsViewBrushes.insert("Signal_Befestigung", QBrush("greenyellow"));
    graphicsViewBrushes.insert("Gleis_Abschluss", QBrush("coral"));
    graphicsViewBrushes.insert("W_Kr_Gsp_Komponente", QBrush("coral"));
}

bool Preferences::getFilterSetting(QString filter)
{
    if(!filterSettings.contains(filter))
    {
        return true;
    }
    return filterSettings.value(filter);
}

void Preferences::setFilterSetting(QString filter, bool enabled)
{
    filterSettings.insert(filter, enabled);
}

QPen Preferences::getGraphicsViewPen(QString type)
{
    if(!graphicsViewPens.contains(type))
    {
        return QPen(Qt::black);
    }
    return graphicsViewPens.value(type);
}

void Preferences::setGraphicsViewPen(QString type, QPen pen)
{
    graphicsViewPens.insert(type, pen);
}

QBrush Preferences::getGraphicsViewBrush(QString type)
{
    if(!graphicsViewBrushes.contains(type))
    {
        return QBrush(Qt::white);
    }
    return graphicsViewBrushes.value(type);
}

void Preferences::setGraphicsViewBrush(QString type, QBrush brush)
{
    graphicsViewBrushes.insert(type, brush);
}

QPoint Preferences::getPos() {return pos;}
void Preferences::setPos(QPoint p) {pos = p;}
QSize Preferences::getSize() {return size;}
void Preferences::setSize(QSize s) {size = s;}
QByteArray Preferences::getWindowState() {return windowState;}
void Preferences::setWindowState(QByteArray ws) {windowState = ws;}

QString Preferences::getLanguage() {return language;}
void Preferences::setLanguage(QString lang) {language = lang;}
int Preferences::getZoomStep() {return zoomStep;}
void Preferences::setZoomStep(int step) {zoomStep = step;}
int Preferences::getMinZoom() {return minZoom;}
void Preferences::setMinZoom(int min) {minZoom = min;}
int Preferences::getRotateStep() {return rotateStep;}
void Preferences::setRotateStep(int step) {rotateStep = step;}
int Preferences::getMaxRecursionDepth() {return maxRecursionDepth;}
void Preferences::setMaxRecursionDepth(int depth) {maxRecursionDepth = depth;}

QBrush Preferences::getObjectListFgStartBrush() {return objectListFgStartBrush;}
void Preferences::setObjectListFgStartBrush(QBrush brush) {objectListFgStartBrush = brush;}
QBrush Preferences::getObjectListBgStartBrush() {return objectListBgStartBrush;}
void Preferences::setObjectListBgStartBrush(QBrush brush) {objectListBgStartBrush = brush;}
QBrush Preferences::getObjectListFgEndBrush() {return objectListFgEndBrush;}
void Preferences::setObjectListFgEndBrush(QBrush brush) {objectListFgEndBrush = brush;}
QBrush Preferences::getObjectListBgEndBrush() {return objectListBgEndBrush;}
void Preferences::setObjectListBgEndBrush(QBrush brush) {objectListBgEndBrush = brush;}
QBrush Preferences::getObjectListFgBothBrush() {return objectListFgBothBrush;}
void Preferences::setObjectListFgBothBrush(QBrush brush) {objectListFgBothBrush = brush;}
QBrush Preferences::getObjectListBgBothBrush() {return objectListBgBothBrush;}
void Preferences::setObjectListBgBothBrush(QBrush brush) {objectListBgBothBrush = brush;}

QPen Preferences::getGraphicsViewStartPen() {return graphicsViewStartPen;}
void Preferences::setGraphicsViewStartPen(QPen pen) {graphicsViewStartPen = pen;}
QBrush Preferences::getGraphicsViewStartBrush() {return graphicsViewStartBrush;}
void Preferences::setGraphicsViewStartBrush(QBrush brush) {graphicsViewStartBrush = brush;}
QPen Preferences::getGraphicsViewEndPen() {return graphicsViewEndPen;}
void Preferences::setGraphicsViewEndPen(QPen pen) {graphicsViewEndPen = pen;}
QBrush Preferences::getGraphicsViewEndBrush() {return graphicsViewEndBrush;}
void Preferences::setGraphicsViewEndBrush(QBrush brush) {graphicsViewEndBrush = brush;}
QPen Preferences::getGraphicsViewBothPen() {return graphicsViewBothPen;}
void Preferences::setGraphicsViewBothPen(QPen pen) {graphicsViewBothPen = pen;}
QBrush Preferences::getGraphicsViewBothBrush() {return graphicsViewBothBrush;}
void Preferences::setGraphicsViewBothBrush(QBrush brush) {graphicsViewBothBrush = brush;}
