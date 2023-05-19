#ifndef PUNKTOBJEKT_H
#define PUNKTOBJEKT_H

#include "basisobjekt.h"

#include <QString>

class PunktObjektStrecke
{
public:
    PunktObjektStrecke(DomItem* item = nullptr);

    virtual QString getIdStrecke();
    virtual QString getStreckeKm();

protected:
    DomItem* domItem;
};

class PunktObjektTopKante
{
public:
    PunktObjektTopKante(DomItem* item = nullptr);

    virtual QString getIdTopKante();
    virtual int getAbstand();
    virtual QString getWirkrichtung();
    virtual QString getSeitlicheLage();
    virtual int getSeitlicherAbstand();

protected:
    DomItem* domItem;
};

class PunktObjekt : public BasisObjekt
{
public:
    PunktObjekt(DomItem* item = nullptr);
    bool isPunktObjekt();

    virtual int getPunktObjektStreckeCount();
    virtual PunktObjektStrecke getPunktObjektStrecke(int i);
    virtual QString getIdStrecke(int i);
    virtual QString getStreckeKm(int i);
    virtual int getPunktObjektTopKanteCount();
    virtual PunktObjektTopKante getPunktObjektTopKante(int i);
    virtual QString getIdTopKante(int i);
    virtual int getAbstand(int i);
    virtual QString getWirkrichtung(int i);
    virtual QString getSeitlicheLage(int i);
    virtual int getSeitlicherAbstand(int i);

protected:

};

#endif // PUNKTOBJEKT_H
