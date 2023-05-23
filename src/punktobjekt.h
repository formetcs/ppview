#ifndef PUNKTOBJEKT_H
#define PUNKTOBJEKT_H

#include "basisobjekt.h"

#include <QString>

class PunktObjektStrecke
{
public:
    PunktObjektStrecke(DomItem* item = nullptr);

    virtual QString getIdStrecke() const;
    virtual QString getStreckeKm() const;

protected:
    DomItem* domItem;
};

class PunktObjektTopKante
{
public:
    PunktObjektTopKante(DomItem* item = nullptr);

    virtual QString getIdTopKante() const;
    virtual int getAbstand() const;
    virtual QString getWirkrichtung() const;
    virtual QString getSeitlicheLage() const;
    virtual int getSeitlicherAbstand() const;

protected:
    DomItem* domItem;
};

class PunktObjekt : public BasisObjekt
{
public:
    PunktObjekt(DomItem* item = nullptr);
    virtual bool isPunktObjekt() const;

    virtual int getPunktObjektStreckeCount() const;
    virtual PunktObjektStrecke getPunktObjektStrecke(int i) const;
    virtual QString getIdStrecke(int i) const;
    virtual QString getStreckeKm(int i) const;
    virtual int getPunktObjektTopKanteCount() const;
    virtual PunktObjektTopKante getPunktObjektTopKante(int i) const;
    virtual QString getIdTopKante(int i) const;
    virtual int getAbstand(int i) const;
    virtual QString getWirkrichtung(int i) const;
    virtual QString getSeitlicheLage(int i) const;
    virtual int getSeitlicherAbstand(int i) const;

protected:

};

#endif // PUNKTOBJEKT_H
