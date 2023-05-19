#include "punktobjekt.h"
#include "domitem.h"

PunktObjektStrecke::PunktObjektStrecke(DomItem* item)
{
    domItem = item;
}

QString PunktObjektStrecke::getIdStrecke()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("ID_Strecke/Wert");
}

QString PunktObjektStrecke::getStreckeKm()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Strecke_Km/Wert");
}




PunktObjektTopKante::PunktObjektTopKante(DomItem* item)
{
    domItem = item;
}

QString PunktObjektTopKante::getIdTopKante()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("ID_TOP_Kante/Wert");
}

int PunktObjektTopKante::getAbstand()
{
    if(!domItem)
    {
        return 0;
    }
    double val = domItem->getFirstValueAtPath("Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}

QString PunktObjektTopKante::getWirkrichtung()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Wirkrichtung/Wert");
}

QString PunktObjektTopKante::getSeitlicheLage()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Seitliche_Lage/Wert");
}

int PunktObjektTopKante::getSeitlicherAbstand()
{
    if(!domItem)
    {
        return 0;
    }
    double val = domItem->getFirstValueAtPath("Seitlicher_Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}




PunktObjekt::PunktObjekt(DomItem* item) : BasisObjekt(item)
{
}

bool PunktObjekt::isPunktObjekt()
{
    if(!domItem)
    {
        return false;
    }
    return (isBasisObjekt() && (domItem->getFirstItemAtPath("Punkt_Objekt_TOP_Kante") != NULL));
}

int PunktObjekt::getPunktObjektStreckeCount()
{
    if(!domItem)
    {
        return 0;
    }
    return domItem->getChildItems("Punkt_Objekt_Strecke").count();
}

PunktObjektStrecke PunktObjekt::getPunktObjektStrecke(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektStreckeCount())
    {
        return PunktObjektStrecke();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_Strecke");
    return PunktObjektStrecke(childlist.at(i));
}

QString PunktObjekt::getIdStrecke(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektStreckeCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_Strecke");
    return childlist.at(i)->getFirstValueAtPath("ID_Strecke/Wert");
}

QString PunktObjekt::getStreckeKm(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektStreckeCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_Strecke");
    return childlist.at(i)->getFirstValueAtPath("Strecke_Km/Wert");
}

int PunktObjekt::getPunktObjektTopKanteCount()
{
    if(!domItem)
    {
        return 0;
    }
    return domItem->getChildItems("Punkt_Objekt_Strecke").count();
}

PunktObjektTopKante PunktObjekt::getPunktObjektTopKante(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return PunktObjektTopKante();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return PunktObjektTopKante(childlist.at(i));
}

QString PunktObjekt::getIdTopKante(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return childlist.at(i)->getFirstValueAtPath("ID_TOP_Kante/Wert");
}

int PunktObjekt::getAbstand(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return 0;
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    double val = childlist.at(i)->getFirstValueAtPath("Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}

QString PunktObjekt::getWirkrichtung(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return childlist.at(i)->getFirstValueAtPath("Wirkrichtung/Wert");
}

QString PunktObjekt::getSeitlicheLage(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return childlist.at(i)->getFirstValueAtPath("Seitliche_Lage/Wert");
}

int PunktObjekt::getSeitlicherAbstand(int i)
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return 0;
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    double val = childlist.at(i)->getFirstValueAtPath("Seitlicher_Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}
