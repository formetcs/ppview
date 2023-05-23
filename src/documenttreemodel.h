#ifndef DOCUMENTTREEMODEL_H
#define DOCUMENTTREEMODEL_H

#include <QAbstractItemModel>
#include <QString>
#include <QByteArray>
#include <QtXml>

//#include "domitem.h"
#include "planprodocument.h"
//#include "nexttopkanteresult.h"
//#include "punktobjekt.h"

class DocumentTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    DocumentTreeModel(QObject* parent = 0);
    ~DocumentTreeModel();
    void setDocument(PlanProDocument* d);
    QModelIndex getModelIndex(DomItem* item);

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
    PlanProDocument* doc;
};

#endif // DOCUMENTTREEMODEL_H
