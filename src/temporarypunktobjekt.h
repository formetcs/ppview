#ifndef TEMPORARYPUNKTOBJEKT_H
#define TEMPORARYPUNKTOBJEKT_H

#include "punktobjekt.h"

class TemporaryPunktObjekt : public PunktObjekt
{
public:
    TemporaryPunktObjekt(QString id, QString idtk, int abst, QString wirkr);
    QString getIdentitaet();
    int getPunktObjektTopKanteCount();
    QString getIdTopKante(int i);
    int getAbstand(int i);
    QString getWirkrichtung(int i);
    QString getSeitlicheLage(int i);
    int getSeitlicherAbstand(int i);

private:
    QString identitaet;
    QString idTopKante;
    int abstand;
    QString wirkrichtung;
};

#endif // TEMPORARYPUNKTOBJEKT_H
