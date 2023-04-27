#ifndef PLANPROMODEL_H
#define PLANPROMODEL_H

#include <QAbstractItemModel>
#include <QString>
#include <QByteArray>
#include <QtXml>

//#include "domitem.h"
#include "planprodocument.h"
#include "nexttopkanteresult.h"
#include "punktobjekt.h"

class PlanProModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    PlanProModel(QObject* parent = 0);
    ~PlanProModel();
    void setDocument(PlanProDocument* d);

    QDomElement getContainerElement();
    QDomElement getObjectById(QString id);
    QModelIndex getModelIndexById(QString id);
    QString getBinaryFileName(QModelIndex index);
    QByteArray extractBinaryFile(QModelIndex index);
    QList<NextTopKanteResult> getNextTopKante(QDomNode topKante, bool forward);
    double calculateDistance(QModelIndexList selectedIndexes);
    double calculateDistance(PunktObjekt startpos, PunktObjekt endpos, bool forward);
    QStringList findReferencingObjects(QModelIndex index);

    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& child) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

public slots:
    void modelChanged();

private:
    QStringList findReferencingObjectsRec(QDomNode node, QString searchId, QString objectName, QString objectId);
    //QDomDocument domDocument;
    //DomItem* rootItem;
    PlanProDocument* doc;
};

#endif // PLANPROMODEL_H
