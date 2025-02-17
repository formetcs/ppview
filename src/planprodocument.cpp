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
 * \file planprodocument.cpp
 *
 * \brief Implementation of the PlanProDocument class.
 */

#include "planprodocument.h"

#include <QDateTime>
#include <QRegularExpression>
#include <QUuid>

PlanProDocument::PlanProDocument(QObject *parent)
    : QObject{parent}
{
    rootItem = NULL;
    cachedDocumentType = DocumentTypeUnknown;
}

PlanProDocument::~PlanProDocument()
{
    if(rootItem != NULL)
    {
        delete rootItem;
    }
}

void PlanProDocument::clear()
{
    if(rootItem != NULL)
    {
        delete rootItem;
        rootItem = NULL;
    }
    clearCache();
}

void PlanProDocument::endDataChange()
{
    clearCache();
    emit dataChanged();
}

void PlanProDocument::clearCache()
{
    cachedDocumentType = DocumentTypeUnknown;
    cachedStartObjectList.clear();
    cachedEndObjectList.clear();
    cachedCombinedObjectList.clear();
}

DomItem* PlanProDocument::getRootItem()
{
    return rootItem;
}

const DomItem* PlanProDocument::getRootItem() const
{
    return rootItem;
}

QDateTime PlanProDocument::getTimestamp() const
{
    if(rootItem == NULL)
    {
        return QDateTime();
    }
    QString timestamp = rootItem->getFirstValueAtPath("PlanPro_Schnittstelle_Allg/Erzeugung_Zeitstempel/Wert");
    return QDateTime::fromString(timestamp, Qt::ISODate);
}

QString PlanProDocument::getToolName() const
{
    if(rootItem == NULL)
    {
        return QString();
    }
    return rootItem->getFirstValueAtPath("PlanPro_Schnittstelle_Allg/Werkzeug_Name/Wert");
}

QString PlanProDocument::getToolVersion() const
{
    if(rootItem == NULL)
    {
        return QString();
    }
    return rootItem->getFirstValueAtPath("PlanPro_Schnittstelle_Allg/Werkzeug_Version/Wert");
}

QString PlanProDocument::getRemark() const
{
    if(rootItem == NULL)
    {
        return QString();
    }
    return rootItem->getFirstValueAtPath("PlanPro_Schnittstelle_Allg/Bemerkung/Wert");
}

void PlanProDocument::setRemark(const QString& rem)
{
    if(rootItem == NULL)
    {
        return;
    }
    DomItem* remarkItem = rootItem->getFirstItemAtPath("PlanPro_Schnittstelle_Allg/Bemerkung");
    DomItem* remarkValueItem = NULL;
    if(remarkItem == NULL)
    {
        DomItem* parentItem = rootItem->getFirstChildItem("PlanPro_Schnittstelle_Allg");
        remarkItem = new DomItem("Bemerkung", parentItem);
        parentItem->addChild(remarkItem);
        remarkValueItem = new DomItem("Wert", remarkItem);
        remarkItem->addChild(remarkValueItem);
    }
    else
    {
        remarkValueItem = remarkItem->getFirstChildItem("Wert");
    }
    remarkValueItem->setValue(rem);
}

void PlanProDocument::updateHeader(const QString& toolname, const QString& toolversion)
{
    if(rootItem == NULL)
    {
        return;
    }
    DomItem* idValueItem = rootItem->getFirstItemAtPath("Identitaet/Wert");
    DomItem* timestampValueItem = rootItem->getFirstItemAtPath("PlanPro_Schnittstelle_Allg/Erzeugung_Zeitstempel/Wert");
    DomItem* toolnameValueItem = rootItem->getFirstItemAtPath("PlanPro_Schnittstelle_Allg/Werkzeug_Name/Wert");
    DomItem* toolversionValueItem = rootItem->getFirstItemAtPath("PlanPro_Schnittstelle_Allg/Werkzeug_Version/Wert");
    idValueItem->setValue(QUuid::createUuid().toString(QUuid::WithoutBraces).toUpper());
    timestampValueItem->setValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    toolnameValueItem->setValue(toolname);
    toolversionValueItem->setValue(toolversion);
}

PlanProDocument::DocumentType PlanProDocument::getDocumentType() const
{
    if(cachedDocumentType != DocumentTypeUnknown)
    {
        return cachedDocumentType;
    }

    if(rootItem == NULL)
    {
        cachedDocumentType = DocumentTypeInvalid;
    }
    else if(rootItem->getFirstItemAtPath("LST_Zustand") != NULL)
    {
        cachedDocumentType = DocumentTypeState;
    }
    else if(rootItem->getFirstItemAtPath("LST_Planung") != NULL)
    {
        cachedDocumentType = DocumentTypePlanning;
    }
    else
    {
        cachedDocumentType = DocumentTypeInvalid;
    }
    return cachedDocumentType;
}

QStringList PlanProDocument::getCategoryList() const
{
    QStringList returnlist;
    if(rootItem == NULL || getDocumentType() != DocumentTypePlanning)
    {
        return returnlist;
    }
    DomItem* fachdatenItem = rootItem->getFirstItemAtPath("LST_Planung/Fachdaten");
    QList<DomItem*> ausgabeFachdatenList = fachdatenItem->getChildItems("Ausgabe_Fachdaten");
    for(int i = 0; i < ausgabeFachdatenList.count(); ++i)
    {
        returnlist.append(ausgabeFachdatenList.at(i)->getFirstValueAtPath("Untergewerk_Art/Wert"));
    }
    return returnlist;
}

QList<DomItem*> PlanProDocument::getObjectList(PlanProDocument::PlanningState state, const QString& category)
{
    QString actualCategory = getDocumentType() == DocumentTypeState ? QString() : category;
    if(state == PlanningStateStart && cachedStartObjectList.contains(actualCategory))
    {
        return cachedStartObjectList.value(actualCategory);
    }
    if(state != PlanningStateStart && cachedEndObjectList.contains(actualCategory))
    {
        return cachedEndObjectList.value(actualCategory);
    }

    QList<DomItem*> returnlist;
    if(rootItem == NULL || getDocumentType() == DocumentTypeInvalid)
    {
        return returnlist;
    }

    if(getDocumentType() == DocumentTypeState)
    {
        DomItem* containerItem = rootItem->getFirstItemAtPath("LST_Zustand/Container");
        for(int i = 0; i < containerItem->childCount(); ++i)
        {
            returnlist.append(containerItem->getChild(i));
        }
        cachedStartObjectList.insert(QString(), returnlist);
        cachedEndObjectList.insert(QString(), returnlist);
    }
    else if(getDocumentType() == DocumentTypePlanning)
    {
        DomItem* fachdatenItem = rootItem->getFirstItemAtPath("LST_Planung/Fachdaten");
        QString statestring = "LST_Zustand_Ziel";
        if(state == PlanningStateStart)
        {
            statestring = "LST_Zustand_Start";
        }
        QList<DomItem*> ausgabeFachdatenList = fachdatenItem->getChildItems("Ausgabe_Fachdaten");
        for(int i = 0; i < ausgabeFachdatenList.count(); ++i)
        {
            QString cat = ausgabeFachdatenList.at(i)->getFirstValueAtPath("Untergewerk_Art/Wert");
            if(category.isEmpty() || category == cat)
            {
                DomItem* containerItem = ausgabeFachdatenList.at(i)->getFirstItemAtPath(statestring + "/Container");
                for(int j = 0; j < containerItem->childCount(); ++j)
                {
                    returnlist.append(containerItem->getChild(j));
                }
            }
        }
        if(state == PlanningStateStart)
        {
            cachedStartObjectList.insert(category, returnlist);
        }
        else
        {
            cachedEndObjectList.insert(category, returnlist);
        }
    }

    return returnlist;
}

QList<PlanProDocument::ObjectListItem> PlanProDocument::getCombinedObjectList(const QString& category)
{
    QString actualCategory = getDocumentType() == DocumentTypeState ? QString() : category;
    if(cachedCombinedObjectList.contains(actualCategory))
    {
        return cachedCombinedObjectList.value(actualCategory);
    }

    QList<ObjectListItem> returnlist;
    if(rootItem == NULL || getDocumentType() == DocumentTypeInvalid)
    {
        return returnlist;
    }

    if(getDocumentType() == DocumentTypeState)
    {
        DomItem* containerItem = rootItem->getFirstItemAtPath("LST_Zustand/Container");
        for(int i = 0; i < containerItem->childCount(); ++i)
        {
            DomItem* currentItem = containerItem->getChild(i);
            ObjectListItem oli;
            oli.itemStart = currentItem;
            oli.itemEnd = currentItem;
            oli.id = currentItem->getFirstValueAtPath("Identitaet/Wert");
            oli.state = PlanningStateBoth;
            returnlist.append(oli);
        }
        cachedCombinedObjectList.insert(QString(), returnlist);
    }
    else if(getDocumentType() == DocumentTypePlanning)
    {
        DomItem* fachdatenItem = rootItem->getFirstItemAtPath("LST_Planung/Fachdaten");
        QList<DomItem*> ausgabeFachdatenList = fachdatenItem->getChildItems("Ausgabe_Fachdaten");
        for(int i = 0; i < ausgabeFachdatenList.count(); ++i)
        {
            QString cat = ausgabeFachdatenList.at(i)->getFirstValueAtPath("Untergewerk_Art/Wert");
            if(category.isEmpty() || category == cat)
            {
                DomItem* startContainerItem = ausgabeFachdatenList.at(i)->getFirstItemAtPath("LST_Zustand_Start/Container");
                DomItem* endContainerItem = ausgabeFachdatenList.at(i)->getFirstItemAtPath("LST_Zustand_Ziel/Container");
                for(int j = 0; j < startContainerItem->childCount(); ++j)
                {
                    DomItem* currentItem = startContainerItem->getChild(j);
                    bool found = false;
                    for(int k = 0; k < endContainerItem->childCount(); ++k)
                    {
                        DomItem* compareItem = endContainerItem->getChild(k);
                        if(currentItem->getFirstValueAtPath("Identitaet/Wert") == compareItem->getFirstValueAtPath("Identitaet/Wert"))
                        {
                            found = true;
                            ObjectListItem oli;
                            oli.itemStart = currentItem;
                            oli.itemEnd = compareItem;
                            oli.id = currentItem->getFirstValueAtPath("Identitaet/Wert");
                            oli.state = PlanningStateBoth;
                            returnlist.append(oli);
                            break;
                        }
                    }
                    if(!found)
                    {
                        ObjectListItem oli;
                        oli.itemStart = currentItem;
                        oli.itemEnd = NULL;
                        oli.id = currentItem->getFirstValueAtPath("Identitaet/Wert");
                        oli.state = PlanningStateStart;
                        returnlist.append(oli);
                    }
                }

                for(int j = 0; j < endContainerItem->childCount(); ++j)
                {
                    DomItem* currentItem = endContainerItem->getChild(j);
                    bool found = false;
                    for(int k = 0; k < startContainerItem->childCount(); ++k)
                    {
                        DomItem* compareItem = startContainerItem->getChild(k);
                        if(currentItem->getFirstValueAtPath("Identitaet/Wert") == compareItem->getFirstValueAtPath("Identitaet/Wert"))
                        {
                            found = true;
                            // case was already handeled in first loop -> nothing to do here
                            break;
                        }
                    }
                    if(!found)
                    {
                        ObjectListItem oli;
                        oli.itemStart = NULL;
                        oli.itemEnd = currentItem;
                        oli.id = currentItem->getFirstValueAtPath("Identitaet/Wert");
                        oli.state = PlanningStateEnd;
                        returnlist.append(oli);
                    }
                }
            }
        }
        cachedCombinedObjectList.insert(category, returnlist);
    }
    return returnlist;
}

DomItem* PlanProDocument::getObjectById(const QString& id, PlanningState state)
{
    if(rootItem == NULL || getDocumentType() == DocumentTypeInvalid)
    {
        return NULL;
    }
    QList<DomItem*> objectlist = getObjectList(state, QString());
    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* currentItem = objectlist.at(i);
        QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
        if(currentId == id)
        {
            return currentItem;
        }
    }
    return NULL;
}

PlanProDocument::ObjectListItem PlanProDocument::getObjectListItemById(const QString& id)
{
    ObjectListItem returnval = ObjectListItem();
    if(rootItem == NULL || getDocumentType() == DocumentTypeInvalid)
    {
        return returnval;
    }
    QList<ObjectListItem> objectlist = getCombinedObjectList(QString());
    for(int i = 0; i < objectlist.count(); ++i)
    {
        ObjectListItem currentItem = objectlist.at(i);
        if(currentItem.id == id)
        {
            return currentItem;
        }
    }
    return returnval;
}

QList<DomItem*> PlanProDocument::findDependentObjects(DomItem* item)
{
    QList<DomItem*> returnlist;
    if(!item)
    {
        return returnlist;
    }
    DomItem* lstZustandItem = item->parent()->parent();
    QString lstZustandItemName = lstZustandItem->getName();
    PlanningState currentItemState = PlanningStateEnd;
    if(lstZustandItemName == "LST_Zustand_Start")
    {
        currentItemState = PlanningStateStart;
    }
    QString id = item->getFirstValueAtPath("Identitaet/Wert");
    QList<DomItem*> objectlist = getObjectList(currentItemState);
    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* currentItem = objectlist.at(i);
        if(currentItem != item && hasDependency(currentItem, id))
        {
            returnlist.append(currentItem);
        }
    }
    return returnlist;
}

bool PlanProDocument::hasDependency(const DomItem *item, const QString& id) const
{
    if(item->getValue() == id)
    {
        return true;
    }
    for(int i = 0; i < item->childCount(); ++i)
    {
        const DomItem* child = item->getChild(i);
        if(hasDependency(child, id))
        {
            return true;
        }
    }
    return false;
}

QList<DomItem*> PlanProDocument::findObjects(QString pattern, FindMatch match, FindState state, QString category, bool caseSensitive, bool attrName, bool attrValue)
{
    QList<DomItem*> returnlist;

    QList<ObjectListItem> objectlist = getCombinedObjectList(category);
    for(int i = 0; i < objectlist.count(); ++i)
    {
        ObjectListItem currentItem = objectlist.at(i);

        if(state == FindStateStartOnly)
        {
            if(currentItem.state == PlanningStateEnd)
            {
                continue;
            }
            if(hasPatternMatch(currentItem.itemStart, pattern, match, caseSensitive, attrName, attrValue, true))
            {
                returnlist.append(currentItem.itemStart);
            }

        }
        else if(state == FindStateEndOnly)
        {
            if(currentItem.state == PlanningStateStart)
            {
                continue;
            }
            if(hasPatternMatch(currentItem.itemEnd, pattern, match, caseSensitive, attrName, attrValue, true))
            {
                returnlist.append(currentItem.itemEnd);
            }
        }
        else if(state == FindStateStartAndEnd)
        {
            if(currentItem.state != PlanningStateBoth)
            {
                continue;
            }
            if(hasPatternMatch(currentItem.itemEnd, pattern, match, caseSensitive, attrName, attrValue, true))
            {
                returnlist.append(currentItem.itemEnd);
            }
        }
        else // state == FindStateStartOrEnd
        {
            if(currentItem.itemEnd && hasPatternMatch(currentItem.itemEnd, pattern, match, caseSensitive, attrName, attrValue, true))
            {
                returnlist.append(currentItem.itemEnd);
                continue; // append only the end item if it is available in both states
            }
            if(currentItem.itemStart && hasPatternMatch(currentItem.itemStart, pattern, match, caseSensitive, attrName, attrValue, true))
            {
                returnlist.append(currentItem.itemStart);
            }
        }
    }

    return returnlist;
}

bool PlanProDocument::hasPatternMatch(const DomItem* item, QString pattern, FindMatch match, bool caseSensitive, bool attrName, bool attrValue, bool recursive)
{
    if(item == NULL)
    {
        return false;
    }

    Qt::CaseSensitivity cs = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if(match == FindMatchContains)
    {
        if(attrName && item->getName().contains(pattern, cs))
        {
            return true;
        }
        if(attrValue && item->getValue().contains(pattern, cs))
        {
            return true;
        }
    }
    else if(match == FindMatchStartsWith)
    {
        if(attrName && item->getName().startsWith(pattern, cs))
        {
            return true;
        }
        if(attrValue && item->getValue().startsWith(pattern, cs))
        {
            return true;
        }
    }
    else if(match == FindMatchEndsWith)
    {
        if(attrName && item->getName().endsWith(pattern, cs))
        {
            return true;
        }
        if(attrValue && item->getValue().endsWith(pattern, cs))
        {
            return true;
        }
    }
    else if(match == FindMatchExactMatch)
    {
        if(attrName && QString::compare(item->getName(), pattern, cs) == 0)
        {
            return true;
        }
        if(attrValue && QString::compare(item->getValue(), pattern, cs) == 0)
        {
            return true;
        }
    }
    else // match == FindMatchRegExp
    {
        QRegularExpression regExp(pattern);
        if(!regExp.isValid())
        {
            return false;
        }
        QRegularExpressionMatch matchName = regExp.match(item->getName());
        QRegularExpressionMatch matchValue = regExp.match(item->getValue());
        if(attrName && matchName.hasMatch())
        {
            return true;
        }
        if(attrValue && matchValue.hasMatch())
        {
            return true;
        }
    }

    if(recursive)
    {
        for(int i = 0; i < item->childCount(); ++i)
        {
            const DomItem* child = item->getChild(i);
            if(hasPatternMatch(child, pattern, match, caseSensitive, attrName, attrValue, true))
            {
                return true;
            }
        }
    }

    return false;
}
