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

/*!
 * \file graphicsscene.h
 *
 * \brief Declaration of the GraphicsScene class.
 */

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QList>
#include <QGraphicsScene>
#include <QHash>

#include "filterwidget.h"
#include "mainwindow.h"


const int GRAPHICSITEM_TYPE = 1; /*!< \brief Identifies the object type in the graphics item data. */
const int GRAPHICSITEM_ID = 2; /*!< \brief Identifies the object ID in the graphics item data. */
const int GRAPHICSITEM_VIEWMODE = 3; /*!< \brief Identifies the view mode in which this item should be visible in the graphics item data. */
const int GRAPHICSITEM_COMPARISON_STATE = 4; /*!< \brief Identifies the planning state for this object in the graphics item data. */


/*!
 * \brief Subclass of QGraphicsScene containing the data for the track layout view.
 *
 * The scene contains all visible PlanPro objects (Punkt_Objekt subtypes, Bereich_Objekt subtypes and all graph elements for the GEO and TOP layer).
 * The track layout view can be in 3 different view modes: Showing the start state only, showing the end state only, or showing a combined start/end comparison
 * using a black-red-yellow drawing. For each view mode, the scene contains an individual graphics item for each PlanPro object.
 * The items for the currently not active view modes are hidden, and the items for the active view mode are shown only if they are also enabled by the filter settings.
 */
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /*!
     * \brief Creates a new GraphicsScene.
     *
     * \param parent the parent object
     */
    explicit GraphicsScene(QObject* parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~GraphicsScene();

    /*!
     * \brief Get the graphics item with a given id.
     *
     * Only the item for the currently active view mode is returned.
     * It is also returned if it is hidden by the filter settings.
     *
     * \param id the UUID of the object
     *
     * \return the item for the active view mode with the searched id or NULL if no such item exists
     */
    QGraphicsItem* getItemById(const QString& id);

signals:

public slots:
    /*!
     * \brief Change the filter setting for a given object type.
     *
     * The visibility of this type is updated immediately.
     *
     * \param key the object type
     * \param state true to show the item, false to hide it
     */
    void changeFilterSettings(const QString& key, bool state);

    /*!
     * \brief Change the filter settings for a list of object types.
     *
     * The visibility of these types is updated immediately.
     *
     * \param statelist a list of FilterState entries containing type and state
     */
    void changeFilterSettings(const QList<FilterState>& statelist);

    /*!
     * \brief Change the currenty active view mode.
     *
     * \param mode the view mode to activate
     */
    void changeViewMode(MainWindow::ViewMode mode);

    /*!
     * \brief Update the color settings after they have been changed in the preferences.
     */
    void updateColorSettings();

private slots:

private:
    MainWindow::ViewMode viewMode; /*!< \brief The currently active view mode. */
    QHash<QString,bool> filterSettings; /*!< \brief The hash map containing the filter settings. */

};

#endif // GRAPHICSSCENE_H
