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

#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractTableModel>

#include "mainwindow.h"

class PlanProDocument;


/*!
 * \brief Class providing the model for the object list view.
 *
 * It makes the information from the PlanPro document accessible to Qt's model/view framework
 * and provides a multi-column list view of a specific part of the PlanPro objects.
 * It depends on the category and view mode settings which objects are contained in the provided list.
 */
class ObjectListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief Creates a new ObjectListModel.
     *
     * \param parent the parent object
     */
    explicit ObjectListModel(QObject *parent = nullptr);

    /*!
     * \brief Set the document for the model.
     *
     * \param d the PlanPro document
     */
    void setDocument(PlanProDocument* d);

    /*!
     * \brief Get the DomItem referenced by a given model index.
     *
     * \param index the model index
     * \return the DomItem for the model index
     */
    DomItem* getItem(const QModelIndex& index) const;

    /*!
     * \brief Get the model index of the item with the given UUID.
     *
     * \param id the UUID of the item
     * \return the model index with the given id
     */
    QModelIndex getModelIndexById(QString id) const;

    /*!
     * \brief Return the data for a given model index.
     *
     * \param index the model index
     * \param role the item data role
     * \return the data for the index and role
     */
    QVariant data(const QModelIndex& index, int role) const;

    /*!
     * \brief Return the item flags for a given model index.
     *
     * \param index the model index
     * \return the flags for the index
     */
    Qt::ItemFlags flags(const QModelIndex& index) const;

    /*!
     * \brief Return the header data for a given section.
     *
     * \param section the header section
     * \param orientation the orientation of the header
     * \param role the item data role
     * \return the header data
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /*!
     * \brief Return the number of rows under the given parent index.
     *
     * \param parent the parent index
     * \return the row count
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    /*!
     * \brief Return the number of columns under the given parent index.
     *
     * \param parent the parent index
     * \return the column count
     */
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

public slots:
    /*!
     * \brief Change the active view mode.
     *
     * \param mode the new view mode
     */
    void changeViewMode(MainWindow::ViewMode mode);

    /*!
     * \brief Change the category that should be displayed.
     *
     * \param cat the new category
     */
    void changeCategory(const QString& cat);

    /*!
     * \brief Indicate beginning changes on the model.
     *
     * This method has to be called before any changes on the underlying PlanPro document are performed.
     * When the changes are finished, modelChanged() has to be called.
     */
    void modelAboutToBeChanged();

    /*!
     * \brief Indicate that changes on the model have been finished.
     *
     * This method has to be called after the completion of any changes on the underlying PlanPro document.
     */
    void modelChanged();

private:
    /*!
     * \brief Create the text that should be displayed for a given item at a specific section.
     *
     * \param item the item
     * \param section the header section
     * \return the text that should be displayed
     */
    QString createSectionText(const DomItem* item, int section) const;

    PlanProDocument* doc; /*!< \brief The PlanPro document. */
    QString category; /*!< \brief The category that is currently shown. */
    MainWindow::ViewMode viewMode; /*!< \brief The currently active view mode. */
};

#endif // OBJECTLISTMODEL_H
