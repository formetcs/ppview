#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include "planprodocument.h"

class ObjectListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ObjectListModel(QObject *parent = nullptr);
    void setDocument(PlanProDocument* d);
    void changePlanningState(PlanProDocument::PlanningState st);
    void changeCategory(const QString& cat);
    void modelChanged();

    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

private:
    QString createSectionText(DomItem* item, int section) const;

    PlanProDocument* doc;
    QString category;
    PlanProDocument::PlanningState state;
};

#endif // OBJECTLISTMODEL_H
