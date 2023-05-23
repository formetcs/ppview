#include "domitem.h"
#include "urobjekt.h"

UrObjekt::UrObjekt(DomItem* item)
{
    domItem = item;
}

bool UrObjekt::isUrObjekt() const
{
    if(!domItem)
    {
        return false;
    }
    return (domItem->getFirstItemAtPath("Identitaet/Wert") != NULL);
}

DomItem* UrObjekt::getDomItem()
{
    return domItem;
}

const DomItem* UrObjekt::getDomItem() const
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

QString UrObjekt::getIdentitaet() const
{
    return getIdentitaet(domItem);
}

QString UrObjekt::getIdentitaet(const DomItem *item)
{
    if(!item)
    {
        return QString();
    }
    return item->getFirstValueAtPath("Identitaet/Wert");
}
