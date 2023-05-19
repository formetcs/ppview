#include "domitem.h"
#include "urobjekt.h"

UrObjekt::UrObjekt(DomItem* item)
{
    domItem = item;
}

bool UrObjekt::isUrObjekt()
{
    if(!domItem)
    {
        return false;
    }
    return (domItem->getFirstItemAtPath("Identitaet/Wert") != NULL);
}

DomItem* UrObjekt::getDomItem() const
{
    return domItem;
}

QString UrObjekt::getName() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getName();
}

QString UrObjekt::getIdentitaet()
{
    return getIdentitaet(domItem);
}

QString UrObjekt::getIdentitaet(DomItem* item)
{
    if(!item)
    {
        return QString();
    }
    return item->getFirstValueAtPath("Identitaet/Wert");
}
