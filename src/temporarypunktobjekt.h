#ifndef TEMPORARYPUNKTOBJEKT_H
#define TEMPORARYPUNKTOBJEKT_H

#include "punktobjekt.h"

class TemporaryPunktObjekt : public PunktObjekt
{
public:
    TemporaryPunktObjekt(QString id, QString idtk, int abst, QString wirkr);
    bool isPunktObjekt() const;
    QString getIdentitaet() const;
    int getPunktObjektTopKanteCount() const;
    QString getIdTopKante(int i) const;
    int getAbstand(int i) const;
    QString getWirkrichtung(int i) const;
    QString getSeitlicheLage(int i) const;
    int getSeitlicherAbstand(int i) const;

private:
    QString identitaet;
    QString idTopKante;
    int abstand;
    QString wirkrichtung;
};

#endif // TEMPORARYPUNKTOBJEKT_H
