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
 * \file planprodocument.h
 *
 * \brief Declaration of the PlanProDocument class.
 */

#ifndef PLANPRODOCUMENT_H
#define PLANPRODOCUMENT_H

#include "domitem.h"

#include <QObject>


/*!
 * \brief Class representing the contents of a PlanPro file as tree of DomItem objects.
 *
 * A valid PlanProDocument can be one of two different types:
 * DocumentType::DocumentTypeState contains a single state describing an existing installation, and
 * DocumentType::DocumentTypePlanning contains a planning process with a start and end state.
 * Each state consists of a list of PlanPro objects which exist in that particular state.
 * A planning document also contains categories, which divide the objects into smaller sublists based
 * on technical criteria like Geo, ESTW or ETCS.
 *
 * The object lists are not directly available from the file because the items are stored inside the tree
 * at different branches. Use the methods getObjectList() and getCombinedObjectList() to create linear lists
 * containing the objects. To speed up access, the lists are stored in a cache once they have been created
 * for the first time.
 */
class PlanProDocument : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Enum describing the document type.
     */
    enum DocumentType
    {
        DocumentTypeInvalid = 0, /*!< \brief The document is invalid. */
        DocumentTypeState = 1, /*!< \brief The document describes a single existing state. */
        DocumentTypePlanning = 2, /*!< \brief The document describes a planning process with start and end state. */
        DocumentTypeUnknown = 99 /*!< \brief The document type is unknown. */
    };

    /*!
     * \brief Enum describing the planning state of an item.
     */
    enum PlanningState
    {
        PlanningStateStart = 0, /*!< \brief The item exists in the start state. */
        PlanningStateEnd = 1, /*!< \brief The item exists in the end state. */
        PlanningStateBoth = 2 /*!< \brief The item exists in both start and end state. */
    };

    /*!
     * \brief Enum describing the match type for search operations.
     */
    enum FindMatch
    {
        FindMatchContains = 0, /*!< \brief The searched string contains the given pattern. */
        FindMatchStartsWith = 1, /*!< \brief The searched string starts with the given pattern. */
        FindMatchEndsWith = 2, /*!< \brief The searched string ends with the given pattern. */
        FindMatchExactMatch = 3, /*!< \brief The seached string matches the given pattern exactly. */
        FindMatchRegExp = 4 /*!< \brief The searched string matches a given Regular Expression. */
    };

    /*!
     * \brief Enum describing which planning states should be considered for search operations.
     */
    enum FindState
    {
        FindStateStartOrEnd = 0, /*!< \brief The item containing the searched string exists in at least one state (start, end, or both). */
        FindStateStartAndEnd = 1, /*!< \brief The item containing the searched string exists in both start and end state. */
        FindStateStartOnly = 2, /*!< \brief The item containing the searched string exists in start state, but not in end state. */
        FindStateEndOnly = 3 /*!< \brief The item containing the searched string exists in end state, but not in start state. */
    };

    /*!
     * \brief Structure containing DomItem objects of both start and end state for the same PlanPro object (with identical ID).
     *
     * If a PlanPro object exists in only one state, the DomItem for the other state has to be set to NULL.
     * If the document is of type DocumentType::DocumentTypeState (which contains only one state),
     * both fields itemStart and itemEnd have to be set to this single item, and state
     * has to be set to PlanningState::PlanningStateBoth.
     */
    struct ObjectListItem
    {
        DomItem* itemStart; /*!< \brief DomItem pointing to the start state. */
        DomItem* itemEnd; /*!< \brief DomItem pointing to the start state. */
        QString id; /*!< \brief Identity of the object. */
        PlanningState state; /*!< \brief The planning state(s) in which the item exists. */

        /*!
         * \brief Constructor, creating an empty item.
         */
        ObjectListItem()
        {
            itemStart = NULL;
            itemEnd = NULL;
            id = "00000000-0000-0000-0000-000000000000";
            state = PlanningStateBoth;
        }

        /*!
         * \brief Checks if the item is valid, which is the case if at least one state exists.
         *
         * \return true if the item is valid, otherwise false
         */
        bool isValid() const
        {
            return (itemStart || itemEnd);
        }
    };

    /*!
     * \brief Creates an empty PlanPro document, where the root item is set to NULL.
     *
     * \param parent the parent object
     */
    explicit PlanProDocument(QObject *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~PlanProDocument();

    /*!
     * \brief Get the root item.
     *
     * \return the root item
     */
    DomItem* getRootItem();

    /*!
     * \brief Get the root item.
     *
     * \return the root item
     */
    const DomItem* getRootItem() const;

    /*!
     * \brief Get the timestamp of the last save time.
     *
     * \return the timestamp, or an empty QDateTime object if the document is invalid
     */
    QDateTime getTimestamp() const;

    /*!
     * \brief Get the name of the tool which performed the last write operation.
     *
     * \return the tool name, or an empty string if the document is invalid
     */
    QString getToolName() const;

    /*!
     * \brief Get the version of the tool which performed the last write operation.
     *
     * \return the tool version, or an empty string if the document is invalid
     */
    QString getToolVersion() const;

    /*!
     * \brief Get the optional remark of the file.
     *
     * \return the remark, or an empty string if the document is invalid or does not contain any remark
     */
    QString getRemark() const;

    /*!
     * \brief Set a new remark.
     *
     * \param rem the new remark
     */
    void setRemark(const QString& rem);

    /*!
     * \brief Update the header information of the PlanPro file.
     *
     * This method has to be called after each write operation to mark the document as modified.
     * It updates the tool name, tool version and time stamp and sets a new UUID.
     *
     * \param toolname the name of the tool which performed the last write operation
     * \param toolversion the version of the tool which performed the last write operation
     */
    void updateHeader(const QString& toolname, const QString& toolversion);

    /*!
     * \brief Get the document type.
     *
     * This information is calculated at the first method call and stored in a cache to speed up any following access.
     * The cache has to be cleared when the document data changes in a way that affects the document type.
     *
     * \return the document type
     */
    DocumentType getDocumentType() const;

    /*!
     * \brief Get the list of available planning categories.
     *
     * Categories exist only in documents of type DocumentType::DocumentTypePlanning and divide the plan
     * in technical sections like Geo, ESTW, and ETCS.
     *
     * \return the category list, or an empty list if the plan does not have categories
     */
    QStringList getCategoryList() const;

    /*!
     * \brief Get the list of PlanPro objects for a given planning state and category.
     *
     * For documents of type DocumentType::DocumentTypeState, the values for planning state and category do not have any effect.
     *
     * The list is calculated at the first method call and stored in a cache to speed up any following access.
     * The cache has to be cleared when the document data changes in a way that affects the list items.
     *
     * \param state the planning state (allowed values are PlanningState::PlanningStateStart and PlanningState::PlanningStateEnd)
     * \param category the category string, or an empty string to get a complete list with all categories
     * \return the list of PlanPro objects, or an empty list if the document is invalid
     */
    QList<DomItem*> getObjectList(PlanningState state, const QString& category = QString());

    /*!
     * \brief Get a list of ObjectListItem objects containing both start and end state of an item.
     *
     * For documents of type DocumentType::DocumentTypeState, the value for category does not have any effect.
     *
     * The list is calculated at the first method call and stored in a cache to speed up any following access.
     * The cache has to be cleared when the document data changes in a way that affects the list items.
     *
     * \param category the category string, or an empty string to get a complete list with all categories
     * \return the list of PlanPro objects, or an empty list if the document is invalid
     */
    QList<ObjectListItem> getCombinedObjectList(const QString& category = QString());

    /*!
     * \brief Get the PlanPro object with a given ID from a given planning state.
     *
     * \param id the UUID of the object
     * \param state the planning state (allowed values are PlanningState::PlanningStateStart and PlanningState::PlanningStateEnd)
     * \return the searched object, or NULL if it does not exist
     */
    DomItem* getObjectById(const QString& id, PlanningState state = PlanningStateEnd);

    /*!
     * \brief Get an ObjectListItem with a given ID.
     *
     * \param id the UUID of the object
     * \return the searched object, or an empty ObjectListItem if it does not exist
     */
    ObjectListItem getObjectListItemById(const QString& id);

    /*!
     * \brief Get a list of objects which have references to the ID of a given object.
     *
     * This method searches for dependent objects only in the same planning state as the given object.
     *
     * \param item the DomItem for which the references should be found
     * \return the list of all objects pointing to the given object
     */
    QList<DomItem*> findDependentObjects(DomItem* item);

    /*!
     * \brief Check if a given object has references to a given UUID string.
     *
     * This method searches recursively the value of all child items for the given id.
     *
     * \param item the DomItem for which the ID reference should be found
     * \param id the UUID to search for
     * \return true if a dependency could be found, otherwise false
     */
    bool hasDependency(const DomItem* item, const QString& id) const;

    /*!
     * \brief Get a list of objects containing a given search pattern.
     *
     * \param pattern the pattern to search for
     * \param match the match type
     * \param state the states which should be considered for search
     * \param category the category which should be considered for search (or an empty string to search in all categories)
     * \param caseSensitive true if the search should be case-sensitive (has no effect if match is FindMatch::FindMatchRegExp)
     * \param attrName true to search for the pattern in the name of a DomItem
     * \param attrValue true to search for the pattern in the value of a DomItem
     * \return the list of object matching the pattern
     */
    QList<DomItem*> findObjects(QString pattern, FindMatch match = FindMatchContains, FindState state = FindStateStartOrEnd, QString category = QString(), bool caseSensitive = false, bool attrName = true, bool attrValue = true);

    /*!
     * \brief Checks if a given object contains a given search pattern.
     *
     * \param item the item which should be checked
     * \param pattern the pattern to search for
     * \param match the match type
     * \param caseSensitive true if the search should be case-sensitive (has no effect if match is FindMatch::FindMatchRegExp)
     * \param attrName true to search for the pattern in the name of a DomItem
     * \param attrValue true to search for the pattern in the value of a DomItem
     * \param recursive true to check recursively all child items, false to check only the given item
     * \return true if the item matches the pattern, otherwise false
     */
    bool hasPatternMatch(const DomItem* item, QString pattern, FindMatch match = FindMatchContains, bool caseSensitive = false, bool attrName = true, bool attrValue = true, bool recursive = false);

    /*!
     * \brief Clear all data caches.
     *
     * This method has to be called after data changes that affect the object lists or the document type.
     */
    void clearCache();


public slots:
    /*!
     * \brief Finalize an external data change.
     *
     * This method has to be called when the data has been changed from outside of the class.
     * It clears all caches and emits the dataChanged() signal.
     */
    void endDataChange();

    /*!
     * \brief Clear the document.
     *
     * This method deletes the root item (and all child items) and clears the data caches.
     */
    void clear();

signals:
    /*!
     * \brief Signal to indicate data changes.
     *
     * This signal is emitted when the internal data changes. Connect it to views to update them.
     */
    void dataChanged();

protected:
    DomItem* rootItem; /*!< \brief The root item. */
    mutable DocumentType cachedDocumentType; /*!< \brief Cached value of the calculated document type to speed up access. */
    QHash<QString, QList<DomItem*>> cachedStartObjectList; /*!< \brief Cached value of the calculated list of start state objects to speed up access. */
    QHash<QString, QList<DomItem*>> cachedEndObjectList; /*!< \brief Cached value of the calculated list of end state objects to speed up access. */
    QHash<QString, QList<ObjectListItem>> cachedCombinedObjectList; /*!< \brief Cached value of the calculated list of combined start/end state objects to speed up access. */

};

#endif // PLANPRODOCUMENT_H
