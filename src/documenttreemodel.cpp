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

#include "documenttreemodel.h"
#include "domitem.h"
#include "planprodocument.h"

DocumentTreeModel::DocumentTreeModel(QObject* parent)
    :QAbstractItemModel(parent)
{
    doc = NULL;
}

DocumentTreeModel::~DocumentTreeModel()
{
}

void DocumentTreeModel::setDocument(PlanProDocument* d)
{
    doc = d;
    endResetModel();
}

void DocumentTreeModel::modelAboutToBeChanged()
{
    beginResetModel();
}

void DocumentTreeModel::modelChanged()
{
    endResetModel();
}

QModelIndex DocumentTreeModel::getModelIndex(DomItem* item)
{
    if(!item)
    {
        return QModelIndex();
    }

    DomItem* parentItem = item->parent();
    QModelIndex parentIndex = getModelIndex(parentItem);
    int row = item->getOwnPosition();

    return index(row, 0, parentIndex);
}

QVariant DocumentTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DomItem* item = static_cast<DomItem*>(index.internalPointer());

    QStringList attributes;
    QHash<QString, QString> attributeMap = item->getAttributeMap();
    QHashIterator<QString, QString> i(attributeMap);
    while(i.hasNext())
    {
        i.next();
        attributes << i.key() + "=\"" + i.value() + '"';
    }

    switch (index.column()) {
    case 0:
        return item->getName();
    case 1:
        return item->getValue().split("\n").join(' ');
    case 2:
        return attributes.join(' ');

    default:
        return QVariant();
    }
}

Qt::ItemFlags DocumentTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant DocumentTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("Name");
        case 1:
            return tr("Value");
        case 2:
            return tr("Attributes");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QModelIndex DocumentTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem* childItem;

    if (!parent.isValid())
    {
        childItem = doc->getRootItem();
    }
    else
    {
        DomItem* parentItem = static_cast<DomItem*>(parent.internalPointer());
        childItem = parentItem->getChild(row);
    }

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DocumentTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem* childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem* parentItem = childItem->parent();

    if (!parentItem)
        return QModelIndex();

    return createIndex(parentItem->getOwnPosition(), 0, parentItem);
}

int DocumentTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem* parentItem;

    if (!parent.isValid())
    {
        parentItem = NULL;
    }
    else
    {
        parentItem = static_cast<DomItem*>(parent.internalPointer());
    }

    if(!parentItem)
    {
        return 1;
    }

    return parentItem->childCount();
}

int DocumentTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}
