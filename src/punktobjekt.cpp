#include "punktobjekt.h"

PunktObjekt::PunktObjekt()
{
    identitaet = QString();
    idTopKante = QString();
    abstand = 0;
    wirkrichtung = QString();
}

PunktObjekt::PunktObjekt(QString idtk, double abst, QString wirkr)
{
    identitaet = QString();
    idTopKante = idtk;
    abstand = abst;
    wirkrichtung = wirkr;
}

PunktObjekt::PunktObjekt(QString guid, QString idtk, double abst, QString wirkr)
{
    identitaet = guid;
    idTopKante = idtk;
    abstand = abst;
    wirkrichtung = wirkr;
}

void PunktObjekt::setIdentitaet(QString i)
{
    identitaet = i;
}

QString PunktObjekt::getIdentitaet()
{
    return identitaet;
}

void PunktObjekt::setIdTopKante(QString i)
{
    idTopKante = i;
}

QString PunktObjekt::getIdTopKante()
{
    return idTopKante;
}

void PunktObjekt::setAbstand(double a)
{
    abstand = a;
}

double PunktObjekt::getAbstand()
{
    return abstand;
}

void PunktObjekt::setWirkrichtung(QString w)
{
    wirkrichtung = w;
}

QString PunktObjekt::getWirkrichtung()
{
    return wirkrichtung;
}
