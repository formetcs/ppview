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

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QList>
#include <QGraphicsScene>
#include <QHash>

#include "filterwidget.h"
#include "mainwindow.h"


const int GRAPHICSITEM_TYPE = 1;
const int GRAPHICSITEM_ID = 2;
const int GRAPHICSITEM_VIEWMODE = 3;
const int GRAPHICSITEM_COMPARISON_STATE = 4;


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject* parent = nullptr);
    ~GraphicsScene();
    QGraphicsItem* getItemById(const QString& id);

signals:

public slots:
    void changeFilterSettings(const QString& key, bool state);
    void changeFilterSettings(const QList<FilterState>& statelist);
    void changeViewMode(MainWindow::ViewMode mode);
    void updateColorSettings();

private slots:

private:
    MainWindow::ViewMode viewMode;
    QHash<QString,bool> filterSettings;

};

#endif // GRAPHICSSCENE_H
