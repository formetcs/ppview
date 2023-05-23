#ifndef BASISOBJEKT_H
#define BASISOBJEKT_H

#include "urobjekt.h"

class BasisObjekt : public UrObjekt
{
public:
    virtual bool isBasisObjekt() const;

    virtual int getIdBearbeitungsvermerkCount() const;
    virtual QString getIdBearbeitungsvermerk(int i) const;
    virtual QString getDbGdiReferenz() const;
    virtual QString getTechnischerPlatz() const;
    virtual QString getObjektzustandBesonders() const;
    virtual bool getBestandsschutz() const;
    virtual QString getIdAnhangRegelwerkBesonders() const;
    virtual QString getDatumRegelwerk() const;

protected:
    BasisObjekt(DomItem* item = nullptr);
};

#endif // BASISOBJEKT_H
