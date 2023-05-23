#include "temporarypunktobjekt.h"

TemporaryPunktObjekt::TemporaryPunktObjekt(QString id, QString idtk, int abst, QString wirkr) : PunktObjekt(nullptr)
{
    identitaet = id;
    idTopKante = idtk;
    abstand = abst;
    wirkrichtung = wirkr;
}

bool TemporaryPunktObjekt::isPunktObjekt() const
{
    return true;
}

QString TemporaryPunktObjekt::getIdentitaet() const
{
    return identitaet;
}

int TemporaryPunktObjekt::getPunktObjektTopKanteCount() const
{
    return 1;
}

QString TemporaryPunktObjekt::getIdTopKante(int i) const
{
    if(i != 0)
    {
        return QString();
    }
    return idTopKante;
}

int TemporaryPunktObjekt::getAbstand(int i) const
{
    if(i != 0)
    {
        return 0;
    }
    return abstand;
}

QString TemporaryPunktObjekt::getWirkrichtung(int i) const
{
    if(i != 0)
    {
        return QString();
    }
    return wirkrichtung;
}

QString TemporaryPunktObjekt::getSeitlicheLage(int i) const
{
    Q_UNUSED(i);
    return QString();
}

int TemporaryPunktObjekt::getSeitlicherAbstand(int i) const
{
    Q_UNUSED(i);
    return 0;
}
