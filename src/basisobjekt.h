#ifndef BASISOBJEKT_H
#define BASISOBJEKT_H

#include "urobjekt.h"

class BasisObjekt : public UrObjekt
{
public:
    bool isBasisObjekt();

    virtual int getIdBearbeitungsvermerkCount();
    virtual QString getIdBearbeitungsvermerk(int i);
    virtual QString getDbGdiReferenz();
    virtual QString getTechnischerPlatz();
    virtual QString getObjektzustandBesonders();
    virtual bool getBestandsschutz();
    virtual QString getIdAnhangRegelwerkBesonders();
    virtual QString getDatumRegelwerk();

protected:
    BasisObjekt(DomItem* item = nullptr);
};

#endif // BASISOBJEKT_H
