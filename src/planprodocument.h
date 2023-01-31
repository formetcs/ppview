#ifndef PLANPRODOCUMENT_H
#define PLANPRODOCUMENT_H

#include "domitem.h"

#include <QObject>

class PlanProDocument : public QObject
{
    Q_OBJECT
public:
    explicit PlanProDocument(QObject *parent = nullptr);
    ~PlanProDocument();
    DomItem* getRootItem();

signals:

protected:
    DomItem* rootItem;

};

#endif // PLANPRODOCUMENT_H
