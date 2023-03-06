#ifndef PLANPRODOCUMENT_H
#define PLANPRODOCUMENT_H

#include "domitem.h"

#include <QObject>

class PlanProDocument : public QObject
{
    Q_OBJECT
public:
    enum DocumentType
    {
        Invalid = 0,
        State = 1,
        Planning = 2
    };

    enum PlanningState
    {
        Start = 0,
        End = 1,
        Both = 2
    };

    struct ObjectListItem
    {
        DomItem* item;
        PlanningState state;
    };

    explicit PlanProDocument(QObject *parent = nullptr);
    ~PlanProDocument();
    DomItem* getRootItem();
    QDateTime getTimestamp() const;
    QString getToolName() const;
    QString getToolVersion() const;
    QString getRemark() const;
    void setRemark(const QString& rem);
    void updateHeader(const QString& toolname, const QString& toolversion);
    DocumentType getDocumentType() const;
    QStringList getCategoryList() const;
    QList<DomItem*> getObjectList(PlanningState state, const QString& category = QString());
    QList<ObjectListItem> getCombinedObjectList(const QString& category = QString());
    DomItem* getObjectById(const QString& id, PlanningState state = End);

signals:

protected:
    DomItem* rootItem;

};

#endif // PLANPRODOCUMENT_H
