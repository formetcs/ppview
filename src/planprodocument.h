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

#ifndef PLANPRODOCUMENT_H
#define PLANPRODOCUMENT_H

#include "domitem.h"

#include <QObject>

class PlanProDocument : public QObject
{
    Q_OBJECT
public:
    enum DocumentType
    {
        DocumentTypeInvalid = 0,
        DocumentTypeState = 1,
        DocumentTypePlanning = 2,
        DocumentTypeUnknown = 99
    };

    enum PlanningState
    {
        PlanningStateStart = 0,
        PlanningStateEnd = 1,
        PlanningStateBoth = 2
    };

    struct ObjectListItem
    {
        DomItem* itemStart;
        DomItem* itemEnd;
        QString id;
        PlanningState state;

        ObjectListItem()
        {
            itemStart = NULL;
            itemEnd = NULL;
            id = "00000000-0000-0000-0000-000000000000";
            state = PlanningStateBoth;
        }

        bool isValid() const
        {
            return (itemStart || itemEnd);
        }
    };

    explicit PlanProDocument(QObject *parent = nullptr);
    ~PlanProDocument();
    DomItem* getRootItem();
    const DomItem* getRootItem() const;
    QDateTime getTimestamp() const;
    QString getToolName() const;
    QString getToolVersion() const;
    QString getRemark() const;
    void setRemark(const QString& rem);
    void updateHeader(const QString& toolname, const QString& toolversion);
    DocumentType getDocumentType() const;
    QStringList getCategoryList() const;
    QList<DomItem*> getObjectList(PlanningState state, const QString& category = QString());
    QList<ObjectListItem> getCombinedObjectList(const QString& category = QString());
    DomItem* getObjectById(const QString& id, PlanningState state = PlanningStateEnd);
    ObjectListItem getObjectListItemById(const QString& id);
    QList<DomItem*> findDependentObjects(DomItem* item);
    bool hasDependency(const DomItem* item, const QString& id) const;
    void clearCache();


public slots:
    void endDataChange();
    void clear();

signals:
    void dataChanged();

protected:
    DomItem* rootItem;
    mutable DocumentType cachedDocumentType;
    QHash<QString, QList<DomItem*>> cachedStartObjectList;
    QHash<QString, QList<DomItem*>> cachedEndObjectList;
    QHash<QString, QList<ObjectListItem>> cachedCombinedObjectList;

};

#endif // PLANPRODOCUMENT_H
