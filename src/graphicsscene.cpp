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

#include <QtGui>
#include <QtWidgets>
#include "graphicsscene.h"
#include "planprodocument.h"
#include "preferences.h"

GraphicsScene::GraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
    viewMode = MainWindow::ViewModeStateEnd;
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::changeViewMode(MainWindow::ViewMode mode)
{
    viewMode = mode;
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        QString itemType = item->data(GRAPHICSITEM_TYPE).toString();
        MainWindow::ViewMode itemviewMode = item->data(GRAPHICSITEM_VIEWMODE).value<MainWindow::ViewMode>();
        if(itemviewMode == viewMode)
        {
            item->setVisible(filterSettings.value(itemType,true));
        }
        else
        {
            item->setVisible(false);
        }
    }
}

void GraphicsScene::updateColorSettings()
{
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        QString itemType = item->data(GRAPHICSITEM_TYPE).toString();
        MainWindow::ViewMode itemviewMode = item->data(GRAPHICSITEM_VIEWMODE).value<MainWindow::ViewMode>();
        QGraphicsLineItem* lineitem = qgraphicsitem_cast<QGraphicsLineItem*>(item);
        if(lineitem != NULL)
        {
            if(itemviewMode != MainWindow::ViewModeStateComparison)
            {
                QPen pen = Preferences::getInstance()->getGraphicsViewPen(itemType);
                lineitem->setPen(pen);
            }
            else
            {
                PlanProDocument::PlanningState state = lineitem->data(GRAPHICSITEM_COMPARISON_STATE).value<PlanProDocument::PlanningState>();
                QPen pen;
                if(state == PlanProDocument::PlanningStateStart)
                {
                    pen = Preferences::getInstance()->getGraphicsViewStartPen();
                    lineitem->setPen(pen);
                }
                else if(state == PlanProDocument::PlanningStateEnd)
                {
                    pen = Preferences::getInstance()->getGraphicsViewEndPen();
                    lineitem->setPen(pen);
                }
                else
                {
                    pen = Preferences::getInstance()->getGraphicsViewBothPen();
                    lineitem->setPen(pen);
                }
            }
            continue;
        }
        QGraphicsEllipseItem* ellipseitem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        if(ellipseitem != NULL)
        {
            if(itemviewMode != MainWindow::ViewModeStateComparison)
            {
                QPen pen = Preferences::getInstance()->getGraphicsViewPen(itemType);
                QBrush brush = Preferences::getInstance()->getGraphicsViewBrush(itemType);
                ellipseitem->setPen(pen);
                ellipseitem->setBrush(brush);
            }
            else
            {
                PlanProDocument::PlanningState state = ellipseitem->data(GRAPHICSITEM_COMPARISON_STATE).value<PlanProDocument::PlanningState>();
                QPen pen;
                QBrush brush;
                if(state == PlanProDocument::PlanningStateStart)
                {
                    pen = Preferences::getInstance()->getGraphicsViewStartPen();
                    brush = Preferences::getInstance()->getGraphicsViewStartBrush();
                    ellipseitem->setPen(pen);
                    ellipseitem->setBrush(brush);
                }
                else if(state == PlanProDocument::PlanningStateEnd)
                {
                    pen = Preferences::getInstance()->getGraphicsViewEndPen();
                    brush = Preferences::getInstance()->getGraphicsViewEndBrush();
                    ellipseitem->setPen(pen);
                    ellipseitem->setBrush(brush);
                }
                else
                {
                    pen = Preferences::getInstance()->getGraphicsViewBothPen();
                    brush = Preferences::getInstance()->getGraphicsViewBothBrush();
                    ellipseitem->setPen(pen);
                    ellipseitem->setBrush(brush);
                }
            }
            continue;
        }
    }
}

void GraphicsScene::changeFilterSettings(const QString& key, bool state)
{
    QString mod_key(key);
    mod_key.remove(QChar('&'));
    filterSettings.insert(mod_key, state);
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        QString itemType = item->data(GRAPHICSITEM_TYPE).toString();
        MainWindow::ViewMode itemviewMode = item->data(GRAPHICSITEM_VIEWMODE).value<MainWindow::ViewMode>();
        if(itemType == mod_key)
        {
            item->setVisible(state && (itemviewMode == viewMode));
        }
    }
}

void GraphicsScene::changeFilterSettings(const QList<FilterState>& statelist)
{
    for(int i = 0; i < statelist.count(); ++i)
    {
        FilterState fs = statelist.at(i);
        changeFilterSettings(fs.name, fs.state);
    }
}

QGraphicsItem* GraphicsScene::getItemById(const QString& id)
{
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        if((item->data(GRAPHICSITEM_ID)).toString() == id && item->data(GRAPHICSITEM_VIEWMODE) == viewMode)
        {
            return item;
        }
    }
    return NULL;
}
