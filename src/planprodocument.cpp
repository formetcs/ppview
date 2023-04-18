#include "planprodocument.h"

#include <QDateTime>
#include <QUuid>

PlanProDocument::PlanProDocument(QObject *parent)
    : QObject{parent}
{
    rootItem = NULL;
    cachedDocumentType = Unknown;
}

PlanProDocument::~PlanProDocument()
{
    if(rootItem != NULL)
    {
        delete rootItem;
    }
}

void PlanProDocument::documentChanged()
{
    cachedDocumentType = Unknown;
    cachedStartObjectList.clear();
    cachedEndObjectList.clear();
    cachedCombinedObjectList.clear();

    emit dataChanged();
}

DomItem* PlanProDocument::getRootItem()
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
    if(cachedDocumentType != Unknown)
    {
        return cachedDocumentType;
    }

    if(rootItem == NULL)
    {
        cachedDocumentType = Invalid;
    }
    else if(rootItem->getFirstItemAtPath("LST_Zustand") != NULL)
    {
        cachedDocumentType = State;
    }
    else if(rootItem->getFirstItemAtPath("LST_Planung") != NULL)
    {
        cachedDocumentType = Planning;
    }
    else
    {
        cachedDocumentType = Invalid;
    }
    return cachedDocumentType;
}

QStringList PlanProDocument::getCategoryList() const
{
    QStringList returnlist;
    if(rootItem == NULL || getDocumentType() != Planning)
    {
        return returnlist;
    }
    DomItem* fachdatenItem = rootItem->getFirstItemAtPath("LST_Planung/Fachdaten");
    QList<DomItem*> ausgabeFachdatenList = fachdatenItem->getChildItems("Ausgabe_Fachdaten");
    for(int i = 0; i < ausgabeFachdatenList.count(); ++i)
    {
        returnlist.append(ausgabeFachdatenList[i]->getFirstValueAtPath("Untergewerk_Art/Wert"));
    }
    return returnlist;
}

QList<DomItem*> PlanProDocument::getObjectList(PlanProDocument::PlanningState state, const QString& category)
{
    if(state == Start && cachedStartObjectList.contains(category))
    {
        return cachedStartObjectList.value(category);
    }
    if(state != Start && cachedEndObjectList.contains(category))
    {
        return cachedEndObjectList.value(category);
    }

    QList<DomItem*> returnlist;
    if(rootItem == NULL || getDocumentType() == Invalid)
    {
        return returnlist;
    }

    if(getDocumentType() == State)
    {
        DomItem* containerItem = rootItem->getFirstItemAtPath("LST_Zustand/Container");
        for(int i = 0; i < containerItem->childCount(); ++i)
        {
            returnlist.append(containerItem->getChild(i));
        }
        cachedStartObjectList.insert(category, returnlist);
        cachedEndObjectList.insert(category, returnlist);
    }
    else if(getDocumentType() == Planning)
    {
        DomItem* fachdatenItem = rootItem->getFirstItemAtPath("LST_Planung/Fachdaten");
        QString statestring = "LST_Zustand_Ziel";
        if(state == Start)
        {
            statestring = "LST_Zustand_Start";
        }
        QList<DomItem*> ausgabeFachdatenList = fachdatenItem->getChildItems("Ausgabe_Fachdaten");
        for(int i = 0; i < ausgabeFachdatenList.count(); ++i)
        {
            QString cat = ausgabeFachdatenList[i]->getFirstValueAtPath("Untergewerk_Art/Wert");
            if(category.isEmpty() || category == cat)
            {
                DomItem* containerItem = ausgabeFachdatenList[i]->getFirstItemAtPath(statestring + "/Container");
                for(int j = 0; j < containerItem->childCount(); ++j)
                {
                    returnlist.append(containerItem->getChild(j));
                }
            }
        }
        if(state == Start)
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
    if(cachedCombinedObjectList.contains(category))
    {
        return cachedCombinedObjectList.value(category);
    }

    QList<ObjectListItem> returnlist;
    if(rootItem == NULL || getDocumentType() != Planning)
    {
        return returnlist;
    }
    DomItem* fachdatenItem = rootItem->getFirstItemAtPath("LST_Planung/Fachdaten");
    QList<DomItem*> ausgabeFachdatenList = fachdatenItem->getChildItems("Ausgabe_Fachdaten");
    for(int i = 0; i < ausgabeFachdatenList.count(); ++i)
    {
        QString cat = ausgabeFachdatenList[i]->getFirstValueAtPath("Untergewerk_Art/Wert");
        if(category.isEmpty() || category == cat)
        {
            DomItem* startContainerItem = ausgabeFachdatenList[i]->getFirstItemAtPath("LST_Zustand_Start/Container");
            DomItem* endContainerItem = ausgabeFachdatenList[i]->getFirstItemAtPath("LST_Zustand_Ziel/Container");
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
                        oli.item = currentItem;
                        oli.state = Both;
                        returnlist.append(oli);
                        break;
                    }
                }
                if(!found)
                {
                    ObjectListItem oli;
                    oli.item = currentItem;
                    oli.state = Start;
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
                    oli.item = currentItem;
                    oli.state = End;
                    returnlist.append(oli);
                }
            }
        }
    }

    cachedCombinedObjectList.insert(category, returnlist);
    return returnlist;
}

DomItem* PlanProDocument::getObjectById(const QString& id, PlanningState state)
{
    if(rootItem == NULL || getDocumentType() == Invalid)
    {
        return NULL;
    }

    if(getDocumentType() == State)
    {
        DomItem* containerItem = rootItem->getFirstItemAtPath("LST_Zustand/Container");
        for(int i = 0; i < containerItem->childCount(); ++i)
        {
            DomItem* currentItem = containerItem->getChild(i);
            QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
            if(currentId == id)
            {
                return currentItem;
            }
        }
    }
    else if(getDocumentType() == Planning)
    {
        DomItem* fachdatenItem = rootItem->getFirstItemAtPath("LST_Planung/Fachdaten");
        QString statestring = "LST_Zustand_Ziel";
        if(state == Start)
        {
            statestring = "LST_Zustand_Start";
        }
        QList<DomItem*> ausgabeFachdatenList = fachdatenItem->getChildItems("Ausgabe_Fachdaten");
        for(int i = 0; i < ausgabeFachdatenList.count(); ++i)
        {
            DomItem* containerItem = ausgabeFachdatenList[i]->getFirstItemAtPath(statestring + "/Container");
            for(int j = 0; j < containerItem->childCount(); ++j)
            {
                DomItem* currentItem = containerItem->getChild(j);
                QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
                if(currentId == id)
                {
                    return currentItem;
                }
            }
        }
    }

    return NULL;
}
