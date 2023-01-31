#include "planprodocument.h"

PlanProDocument::PlanProDocument(QObject *parent)
    : QObject{parent}
{
    rootItem = NULL;
}

PlanProDocument::~PlanProDocument()
{
    if(rootItem != NULL)
    {
        delete rootItem;
    }
}

DomItem* PlanProDocument::getRootItem()
{
    return rootItem;
}
