#include "temporarypunktobjekt.h"

TemporaryPunktObjekt::TemporaryPunktObjekt(QString id, QString idtk, int abst, QString wirkr) : PunktObjekt(nullptr)
{
    identitaet = id;
    idTopKante = idtk;
    abstand = abst;
    wirkrichtung = wirkr;
}

QString TemporaryPunktObjekt::getIdentitaet()
{
    return identitaet;
}

int TemporaryPunktObjekt::getPunktObjektTopKanteCount()
{
    return 1;
}

QString TemporaryPunktObjekt::getIdTopKante(int i)
{
    Q_UNUSED(i);
    return idTopKante;
}

int TemporaryPunktObjekt::getAbstand(int i)
{
    Q_UNUSED(i);
    return abstand;
}

QString TemporaryPunktObjekt::getWirkrichtung(int i)
{
    Q_UNUSED(i);
    return wirkrichtung;
}

QString TemporaryPunktObjekt::getSeitlicheLage(int i)
{
    Q_UNUSED(i);
    return QString();
}

int TemporaryPunktObjekt::getSeitlicherAbstand(int i)
{
    Q_UNUSED(i);
    return 0;
}
