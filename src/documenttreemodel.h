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

#ifndef DOCUMENTTREEMODEL_H
#define DOCUMENTTREEMODEL_H

#include <QAbstractItemModel>

class DomItem;
class PlanProDocument;


/*!
 * \brief Class providing the model for the document tree view.
 *
 * It makes the information from the PlanPro document accessible to Qt's model/view framework
 * and provides a tree view of the complete PlanPro document.
 */
class DocumentTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    /*!
     * \brief Creates a new DocumentTreeModel.
     *
     * \param parent the parent object
     */
    DocumentTreeModel(QObject* parent = 0);

    /*!
     * \brief Destructor.
     */
    ~DocumentTreeModel();

    /*!
     * \brief Set the document for the model.
     *
     * \param d the PlanPro document
     */
    void setDocument(PlanProDocument* d);

    /*!
     * \brief Get the model index for a given DomItem.
     *
     * \param item the DomItem
     * \return the model index for the item, or an empty model index if the item was NULL
     */
    QModelIndex getModelIndex(DomItem* item);

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
     * \brief Calculate the model index of an item for a given row, column and parent index.
     *
     * \param row the row of the item
     * \param column the column of the item
     * \param parent the parent model index of the item
     * \return the model index for the item
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

    /*!
     * \brief Return the parent of the item with a given model index.
     *
     * \param child model index of the child item
     * \return model index of the parent item
     */
    QModelIndex parent(const QModelIndex& child) const;

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
    PlanProDocument* doc; /*!< \brief The PlanPro document. */
};

#endif // DOCUMENTTREEMODEL_H
