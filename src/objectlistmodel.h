#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include "mainwindow.h"
#include "planprodocument.h"

class ObjectListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ObjectListModel(QObject *parent = nullptr);
    void setDocument(PlanProDocument* d);
    DomItem* getItem(const QModelIndex& index) const;
    QModelIndex getModelIndexById(QString id) const;

    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

public slots:
    void changeViewMode(MainWindow::ViewMode mode);
    void changeCategory(const QString& cat);
    void modelChanged();

private:
    QString createSectionText(DomItem* item, int section) const;

    PlanProDocument* doc;
    QString category;
    MainWindow::ViewMode viewMode;
};

#endif // OBJECTLISTMODEL_H
