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

#ifndef DOMITEM_H
#define DOMITEM_H

#include <QString>
#include <QHash>
#include <QList>


/*!
 * \brief Class to store DOM items in a hierarchical tree structure.
 *
 * A DomItem has a name and a value, which corresponds to the element name and text of
 * standard DOM elements. It can also have attributes as name/value pairs
 * and a ordered list of child items. Every DomItem except the root item has a pointer to
 * the parent item (for the root item, the pointer to parent is NULL).
 *
 * In contrast to Qt's own XML classes, DomItem is compatible to Qt's Model/View framework.
 */
class DomItem
{
public:
    /*!
     * \brief Constructor.
     *
     * Constructs a DOM item with a name, value and a pointer to the parent item.
     * For the root item, parent must be set to NULL.
     *
     * \param name the name of the DOM item
     * \param value the value of the DOM item
     * \param parent the parent item of this DOM item or NULL if this is the root item
     */
    DomItem(const QString& name, const QString& value, DomItem* parent = NULL);

    /*!
     * \brief Constructor.
     *
     * Constructs a DOM item with a name and a pointer to the parent item.
     * The value will be an empty string.
     * For the root item, parent must be set to NULL.
     *
     * \param name the name of the DOM item
     * \param parent the parent item of this DOM item or NULL if this is the root item
     */
    DomItem(const QString& name, DomItem* parent = NULL);

    /*!
     * \brief Destructor.
     */
    ~DomItem();

    /*!
     * \brief Get the name of the DOM item.
     *
     * \return the name of the DOM item
     */
    QString getName() const;

    /*!
     * \brief Get the value of the DOM item.
     *
     * \return the value of the DOM item
     */
    QString getValue() const;

    /*!
     * \brief Set the value of the DOM item.
     *
     * \param v the value of the DOM item
     */
    void setValue(const QString& v);

    /*!
     * \brief Get the child item at a specific position.
     *
     * \param i the position of the child
     * \return the child item or NULL if the position is out of bounds
     */
    DomItem* getChild(int i);

    /*!
     * \brief Get the child item at a specific position.
     *
     * \param i the position of the child
     * \return the child item or NULL if the position is out of bounds
     */
    const DomItem* getChild(int i) const;

    /*!
     * \brief Get the first child item with a given name.
     *
     * \param c the item name to look for
     * \return the first child item with the given name or NULL if no item with such a name exists
     */
    DomItem* getFirstChildItem(const QString& c);

    /*!
     * \brief Get the first child item with a given name.
     *
     * \param c the item name to look for
     * \return the first child item with the given name or NULL if no item with such a name exists
     */
    const DomItem* getFirstChildItem(const QString& c) const;

    /*!
     * \brief Get a list of all child item which ave a given name.
     *
     * \param c the item name to look for
     * \return the list of child items with the given name (empty list if no such name exists)
     */
    QList<DomItem*> getChildItems(const QString& c);

    /*!
     * \brief Get a list of all child item which ave a given name.
     *
     * \param c the item name to look for
     * \return the list of child items with the given name (empty list if no such name exists)
     */
    QList<const DomItem*> getChildItems(const QString& c) const;

    /*!
     * \brief Get the first item at a given path.
     *
     * A path is a sequence of item names, separated by a / character,
     * which leads from the current item to the target item.
     * The first name in the sequence is an immediate child of this item,
     * and the last name is the name of the target item.
     *
     * \param p the path to the target item
     * \return the target item or NULL if no item exists at the given path
     */
    DomItem* getFirstItemAtPath(const QString& p);

    /*!
     * \brief Get the first item at a given path.
     *
     * A path is a sequence of item names, separated by a / character,
     * which leads from the current item to the target item.
     * The first name in the sequence is an immediate child of this item,
     * and the last name is the name of the target item.
     *
     * \param p the path to the target item
     * \return the target item or NULL if no item exists at the given path
     */
    const DomItem* getFirstItemAtPath(const QString& p) const;

    /*!
     * \brief Get the value from the first item at a given path.
     *
     * A path is a sequence of item names, separated by a / character,
     * which leads from the current item to the target item.
     * The first name in the sequence is an immediate child of this item,
     * and the last name is the name of the target item.
     *
     * \param p the path to the target item
     * \return the value of the target item or an empty string if no item exists at the given path
     */
    QString getFirstValueAtPath(const QString& p) const;

    /*!
     * \brief Append a new child item to end of the child item list.
     *
     * \param c the new child item which should be added
     */
    void addChild(DomItem* c);

    /*!
     * \brief Get the number of child items.
     *
     * \return the child item count
     */
    int childCount() const;

    /*!
     * \brief Get the own position in the parent's child item list.
     *
     * For the root item and items without siblings, 0 will be returned.
     *
     * \return the position of the item
     */
    int getOwnPosition() const;

    /*!
     * \brief Get the attribute value for a given attribute name.
     *
     * \param name the name of the attribute
     * \return the value of the attribute or an empty string if the attribute name does not exist
     */
    QString getAttribute(const QString& name) const;

    /*!
     * \brief Add a new attribute as name/value pair.
     *
     * \param name the name of the new attribute
     * \param value the value of the new attribute
     */
    void addAttribute(const QString& name, const QString& value);

    /*!
     * \brief Get the map containing the attributes.
     *
     * \return the attribute map
     */
    QHash<QString, QString> getAttributeMap() const;

    /*!
     * \brief Get the parent item of this item.
     *
     * \return the parent item or NULL if this is the root item
     */
    DomItem* parent();

    /*!
     * \brief Get the parent item of this item.
     *
     * \return the parent item or NULL if this is the root item
     */
    const DomItem* parent() const;

private:
    QString name; /*!< \brief The name of the DOM item. */
    QString value; /*!< \brief The value of the DOM item. */
    QList<DomItem*> childItems; /*!< \brief The list of child items of the DOM item. */
    QHash<QString, QString> attributes; /*!< \brief The attributes of the DOM item as name/value pair. */
    DomItem* parentItem; /*!< \brief The parent item of the DOM item or NULL if this is the root item. */
};

#endif // DOMITEM_H
