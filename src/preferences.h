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
 * \file preferences.h
 *
 * \brief Declaration of the Preferences class.
 */

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QBrush>
#include <QPen>
#include <QtCore>


/*!
 * \brief Class to provide global access to the application settings.
 *
 * This class is realized using the Singleton Design Pattern and cannot be instantiated directly.
 * Use the static getInstance() method to get global access to the settings.
 *
 * All settings are stored persistently using the QSettings class.
 */
class Preferences
{
public:
    /*!
     * \brief Get the Preferences instance.
     *
     * \return the Preferences instance
     */
    static Preferences* getInstance();

    /*!
     * \brief Read the settings from disk.
     */
    void readSettings();

    /*!
     * \brief Write the settings to disk.
     */
    void writeSettings();

    /*!
     * \brief Set the default values.
     */
    void setDefaultValues();

    /*!
     * \brief Get the position of the main window.
     *
     * \return the position of the main window
     */
    QPoint getPos();

    /*!
     * \brief Set the position of the main window.
     *
     * \param p the position of the main window
     */
    void setPos(QPoint p);

    /*!
     * \brief Get the size of the main window.
     *
     * \return the size of the main window
     */
    QSize getSize();

    /*!
     * \brief Set the size of the main window.
     *
     * \param s the size of the main window
     */
    void setSize(QSize s);

    /*!
     * \brief Get the window state of the main window.
     *
     * \return the window state of the main window
     */
    QByteArray getWindowState();

    /*!
     * \brief Set the window state of the main window.
     *
     * \param ws the window state of the main window
     */
    void setWindowState(QByteArray ws);

    /*!
     * \brief Get the active language.
     *
     * \return the active language
     */
    QString getLanguage();

    /*!
     * \brief Set the active language.
     *
     * \param lang the active language
     */
    void setLanguage(QString lang);

    /*!
     * \brief Get the zoom step.
     *
     * This is the zoom step that is used when one of the zoom actions has been clicked.
     *
     * \return the zoom step
     */
    int getZoomStep();

    /*!
     * \brief Set the zoom step.
     *
     * This is the zoom step that is used when one of the zoom actions has been clicked.
     *
     * \param step the zoom step
     */
    void setZoomStep(int step);

    /*!
     * \brief Get the minimum zoom value.
     *
     * \return the minimum zoom value
     */
    int getMinZoom();

    /*!
     * \brief Set the minimum zoom value.
     *
     * \param min the minimum zoom value
     */
    void setMinZoom(int min);

    /*!
     * \brief Get the rotate step.
     *
     * This is the rotate step that is used when one of the rotate actions has been clicked.
     *
     * \return the rotate step
     */
    int getRotateStep();

    /*!
     * \brief Set the rotate step.
     *
     * This is the rotate step that is used when one of the rotate actions has been clicked.
     *
     * \param step the rotate step
     */
    void setRotateStep(int step);

    /*!
     * \brief Get the maximum recursion depth.
     *
     * This is the recursion depth used for the search for referencing objects.
     *
     * \return the maximum recursion depth
     */
    int getMaxRecursionDepth();

    /*!
     * \brief Set the maximum recursion depth.
     *
     * This is the recursion depth used for the search for referencing objects.
     *
     * \param depth the maximum recursion depth
     */
    void setMaxRecursionDepth(int depth);

    /*!
     * \brief Get the list of all registered SMT solvers.
     *
     * \return the list of SMT solvers (as their identifier string)
     */
    QStringList getSmtList();

    /*!
     * \brief Clear the list of SMT solvers.
     */
    void clearSmtList();

    /*!
     * \brief Get the command for a given SMT id.
     *
     * \param smtId the smt id
     * \return the command
     */
    QString getSmtCommand(QString smtId);

    /*!
     * \brief Set the command for a given SMT id.
     *
     * \param smtId the smt id
     * \param command the command
     */
    void setSmtCommand(QString smtId, QString command);

    /*!
     * \brief Get the arguments for a given SMT id.
     *
     * \param smtId the smt id
     * \return the arguments
     */
    QString getSmtArguments(QString smtId);

    /*!
     * \brief Set the arguments for a given SMT id.
     *
     * \param smtId the smt id
     * \param arguments the arguments
     */
    void setSmtArguments(QString smtId, QString arguments);

    /*!
     * \brief Get the filter state for a given PlanPro object type.
     *
     * \param filter the PlanPro object type
     * \return true if the object should be shown, false if it should be hidden
     */
    bool getFilterSetting(QString filter);

    /*!
     * \brief Set the filter state for a given PlanPro object type.
     *
     * \param filter the PlanPro object type
     * \param enabled true if the object should be shown, false if it should be hidden
     */
    void setFilterSetting(QString filter, bool enabled);

    /*!
     * \brief Get the graphics view pen for a given PlanPro object type.
     *
     * \param type the PlanPro object type
     * \return the pen
     */
    QPen getGraphicsViewPen(QString type);

    /*!
     * \brief Set the graphics view pen for a given PlanPro object type.
     *
     * \param type the PlanPro object type
     * \param pen the pen
     */
    void setGraphicsViewPen(QString type, QPen pen);

    /*!
     * \brief Get the graphics view brush for a given PlanPro object type.
     *
     * \param type the PlanPro object type
     * \return the brush
     */
    QBrush getGraphicsViewBrush(QString type);

    /*!
     * \brief Set the graphics view brush for a given PlanPro object type.
     *
     * \param type the PlanPro object type
     * \param brush the brush
     */
    void setGraphicsViewBrush(QString type, QBrush brush);

    /*!
     * \brief Get the foreground brush for object list items in the start state (combined view only).
     *
     * \return the brush
     */
    QBrush getObjectListFgStartBrush();

    /*!
     * \brief Set the foreground brush for object list items in the start state (combined view only).
     *
     * \param brush the brush
     */
    void setObjectListFgStartBrush(QBrush brush);

    /*!
     * \brief Get the background brush for object list items in the start state (combined view only).
     *
     * \return the brush
     */
    QBrush getObjectListBgStartBrush();

    /*!
     * \brief Set the background brush for object list items in the start state (combined view only).
     *
     * \param brush the brush
     */
    void setObjectListBgStartBrush(QBrush brush);

    /*!
     * \brief Get the foreground brush for object list items in the end state (combined view only).
     *
     * \return the brush
     */
    QBrush getObjectListFgEndBrush();

    /*!
     * \brief Set the foreground brush for object list items in the end state (combined view only).
     *
     * \param brush the brush
     */
    void setObjectListFgEndBrush(QBrush brush);

    /*!
     * \brief Get the background brush for object list items in the end state (combined view only).
     *
     * \return the brush
     */
    QBrush getObjectListBgEndBrush();

    /*!
     * \brief Set the background brush for object list items in the end state (combined view only).
     *
     * \param brush the brush
     */
    void setObjectListBgEndBrush(QBrush brush);

    /*!
     * \brief Get the foreground brush for object list items in both states (combined view only).
     *
     * \return the brush
     */
    QBrush getObjectListFgBothBrush();

    /*!
     * \brief Set the foreground brush for object list items in both states (combined view only).
     *
     * \param brush the brush
     */
    void setObjectListFgBothBrush(QBrush brush);

    /*!
     * \brief Get the background brush for object list items in both states (combined view only).
     *
     * \return the brush
     */
    QBrush getObjectListBgBothBrush();

    /*!
     * \brief Set the background brush for object list items in both states (combined view only).
     *
     * \param brush the brush
     */
    void setObjectListBgBothBrush(QBrush brush);

    /*!
     * \brief Get the pen for graphics view items in the start state (combined view only).
     *
     * \return the pen
     */
    QPen getGraphicsViewStartPen();

    /*!
     * \brief Set the pen for graphics view items in the start state (combined view only).
     *
     * \param pen the pen
     */
    void setGraphicsViewStartPen(QPen pen);

    /*!
     * \brief Get the brush for graphics view items in the start state (combined view only).
     *
     * \return the brush
     */
    QBrush getGraphicsViewStartBrush();

    /*!
     * \brief Set the brush for graphics view items in the start state (combined view only).
     *
     * \param brush the brush
     */
    void setGraphicsViewStartBrush(QBrush brush);

    /*!
     * \brief Get the pen for graphics view items in the end state (combined view only).
     *
     * \return the pen
     */
    QPen getGraphicsViewEndPen();

    /*!
     * \brief Set the pen for graphics view items in the end state (combined view only).
     *
     * \param pen the pen
     */
    void setGraphicsViewEndPen(QPen pen);

    /*!
     * \brief Get the brush for graphics view items in the end state (combined view only).
     *
     * \return the brush
     */
    QBrush getGraphicsViewEndBrush();

    /*!
     * \brief Set the brush for graphics view items in the end state (combined view only).
     *
     * \param brush the brush
     */
    void setGraphicsViewEndBrush(QBrush brush);

    /*!
     * \brief Get the pen for graphics view items in both states (combined view only).
     *
     * \return the pen
     */
    QPen getGraphicsViewBothPen();

    /*!
     * \brief Set the pen for graphics view items in both states (combined view only).
     *
     * \param pen the pen
     */
    void setGraphicsViewBothPen(QPen pen);

    /*!
     * \brief Get the brush for graphics view items in both states (combined view only).
     *
     * \return the brush
     */
    QBrush getGraphicsViewBothBrush();

    /*!
     * \brief Set the brush for graphics view items in both states (combined view only).
     *
     * \param brush the brush
     */
    void setGraphicsViewBothBrush(QBrush brush);

protected:
    /*!
     * \brief Constructor.
     *
     * Direct object creation is disabled. Use the static getInstance() method to get global access to the settings.
     */
    Preferences();

private:
    static Preferences* preferences; /*!< \brief The Preferences instance (as singleton). */

    QPoint pos; /*!< \brief The position of the main window. */
    QSize size; /*!< \brief The size of the main window. */
    QByteArray windowState; /*!< \brief The window state of the main window. */

    QString language; /*!< \brief The active language. */
    int zoomStep; /*!< \brief The zoom step. */
    int minZoom; /*!< \brief The minimum zoom size. */
    int rotateStep; /*!< \brief The rotate step. */
    int maxRecursionDepth; /*!< \brief The maximum recursion depth. */

    QHash<QString, QString> smtCommands; /*!< \brief Hash map containing the commands for each SMT solver. */
    QHash<QString, QString> smtArguments; /*!< \brief Hash map containing the command line arguments for each SMT solver. */

    QHash<QString, bool> filterSettings; /*!< \brief Hash map containing the filter settings for each object type. */

    QHash<QString, QPen> graphicsViewPens; /*!< \brief Hash map containing the graphics view pens for each object type. */
    QHash<QString, QBrush> graphicsViewBrushes; /*!< \brief Hash map containing the graphics view brushes for each object type. */

    QBrush objectListFgStartBrush; /*!< \brief Foreground brush for object list items in the start state (combined view only). */
    QBrush objectListBgStartBrush; /*!< \brief Background brush for object list items in the start state (combined view only). */
    QBrush objectListFgEndBrush; /*!< \brief Foreground brush for object list items in the end state (combined view only). */
    QBrush objectListBgEndBrush; /*!< \brief Background brush for object list items in the end state (combined view only). */
    QBrush objectListFgBothBrush; /*!< \brief Foreground brush for object list items in both states (combined view only). */
    QBrush objectListBgBothBrush; /*!< \brief Background brush for object list items in both states (combined view only). */

    QPen graphicsViewStartPen; /*!< \brief Pen for graphics view items in the start state (combined view only). */
    QBrush graphicsViewStartBrush; /*!< \brief Brush for graphics view items in the start state (combined view only). */
    QPen graphicsViewEndPen; /*!< \brief Pen for graphics view items in the end state (combined view only). */
    QBrush graphicsViewEndBrush; /*!< \brief Brush for graphics view items in the end state (combined view only). */
    QPen graphicsViewBothPen; /*!< \brief Pen for graphics view items in both states (combined view only). */
    QBrush graphicsViewBothBrush; /*!< \brief Brush for graphics view items in both states (combined view only). */


};

#endif // PREFERENCES_H
