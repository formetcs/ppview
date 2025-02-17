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
 * \file objectlistmodel.cpp
 *
 * \brief Implementation of the ObjectListModel class.
 */

#include "objectlistmodel.h"
#include "planprodocument.h"
#include "preferences.h"

ObjectListModel::ObjectListModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    doc = NULL;
    viewMode = MainWindow::ViewModeStateEnd;
    category = QString();
}

void ObjectListModel::setDocument(PlanProDocument* d)
{
    doc = d;
    endResetModel();
}

void ObjectListModel::changeViewMode(MainWindow::ViewMode mode)
{
    viewMode = mode;
    endResetModel();
}

void ObjectListModel::changeCategory(const QString& cat)
{
    category = cat;
    endResetModel();
}

void ObjectListModel::modelAboutToBeChanged()
{
    beginResetModel();
}

void ObjectListModel::modelChanged()
{
    endResetModel();
}

DomItem* ObjectListModel::getItem(const QModelIndex& index) const
{
    int row = index.row();
    if(viewMode == MainWindow::ViewModeStateComparison)
    {
        QList<PlanProDocument::ObjectListItem> objectlist = doc->getCombinedObjectList(category);
        if (row >= objectlist.count())
        {
            return NULL;
        }
        PlanProDocument::ObjectListItem item = objectlist.at(row);
        DomItem* returnitem = (item.itemEnd) ? item.itemEnd : item.itemStart;
        return returnitem;
    }

    QList<DomItem*> objectlist;
    if(viewMode == MainWindow::ViewModeStateStart)
    {
        objectlist = doc->getObjectList(PlanProDocument::PlanningStateStart, category);
    }
    else
    {
        objectlist = doc->getObjectList(PlanProDocument::PlanningStateEnd, category);
    }

    if (row >= objectlist.count())
    {
        return NULL;
    }
    return objectlist.at(row);
}

QModelIndex ObjectListModel::getModelIndexById(QString id) const
{
    if(viewMode == MainWindow::ViewModeStateComparison)
    {
        QList<PlanProDocument::ObjectListItem> objectlist = doc->getCombinedObjectList(category);
        for(int i = 0; i < objectlist.count(); ++i)
        {
            PlanProDocument::ObjectListItem item = objectlist.at(i);
            if(item.id == id)
            {
                return index(i, 0, QModelIndex());
            }
        }
        return QModelIndex();
    }

    QList<DomItem*> objectlist;
    if(viewMode == MainWindow::ViewModeStateStart)
    {
        objectlist = doc->getObjectList(PlanProDocument::PlanningStateStart, category);
    }
    else
    {
        objectlist = doc->getObjectList(PlanProDocument::PlanningStateEnd, category);
    }

    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* item = objectlist.at(i);
        QString itemId = item->getFirstValueAtPath("Identitaet/Wert");
        if(itemId == id)
        {
            return index(i, 0, QModelIndex());
        }
    }

    return QModelIndex();
}

QString ObjectListModel::createSectionText(const DomItem* item, int section) const
{
    if(section == 0)
    {
        return item->getName();
    }
    if(section == 1)
    {
        return item->getFirstValueAtPath("Identitaet/Wert");
    }
    if(section == 2)
    {
        return item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
    }
    return QString();
}


QVariant ObjectListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    if(viewMode == MainWindow::ViewModeStateComparison)
    {
        QList<PlanProDocument::ObjectListItem> objectlist = doc->getCombinedObjectList(category);

        if (index.row() >= objectlist.count())
            return QVariant();

        PlanProDocument::ObjectListItem item = objectlist.at(index.row());
        
        if(role == Qt::ForegroundRole && item.state == PlanProDocument::PlanningStateBoth)
        {
            return Preferences::getInstance()->getObjectListFgBothBrush();
        }
        if(role == Qt::ForegroundRole && item.state == PlanProDocument::PlanningStateStart)
        {
            return Preferences::getInstance()->getObjectListFgStartBrush();
        }
        if(role == Qt::ForegroundRole && item.state == PlanProDocument::PlanningStateEnd)
        {
            return Preferences::getInstance()->getObjectListFgEndBrush();
        }
        if(role == Qt::BackgroundRole && item.state == PlanProDocument::PlanningStateBoth)
        {
            return Preferences::getInstance()->getObjectListBgBothBrush();
        }
        if(role == Qt::BackgroundRole && item.state == PlanProDocument::PlanningStateStart)
        {
            return Preferences::getInstance()->getObjectListBgStartBrush();
        }
        if(role == Qt::BackgroundRole && item.state == PlanProDocument::PlanningStateEnd)
        {
            return Preferences::getInstance()->getObjectListBgEndBrush();
        }
        if(role == Qt::DisplayRole)
        {
            DomItem* resultitem = (item.itemEnd) ? item.itemEnd : item.itemStart;
            return createSectionText(resultitem, index.column());
        }
        return QVariant();
    }

    QList<DomItem*> objectlist;
    if(viewMode == MainWindow::ViewModeStateStart)
    {
        objectlist = doc->getObjectList(PlanProDocument::PlanningStateStart, category);
    }
    else
    {
        objectlist = doc->getObjectList(PlanProDocument::PlanningStateEnd, category);
    }

    if (index.row() >= objectlist.count())
        return QVariant();

    DomItem* item = objectlist.at(index.row());

    if(role == Qt::DisplayRole)
    {
        return createSectionText(item, index.column());
    }

    return QVariant();
}

Qt::ItemFlags ObjectListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractTableModel::flags(index);
}

QVariant ObjectListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("Type");
        case 1:
            return tr("ID");
        case 2:
            return tr("Name");
        case 3:
            return tr("Description");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

int ObjectListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    
    if(viewMode == MainWindow::ViewModeStateComparison)
    {
        return doc->getCombinedObjectList(category).count();
    }
    else if(viewMode == MainWindow::ViewModeStateStart)
    {
        return doc->getObjectList(PlanProDocument::PlanningStateStart, category).count();
    }

    return doc->getObjectList(PlanProDocument::PlanningStateEnd, category).count();
}

int ObjectListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return 4;
}
