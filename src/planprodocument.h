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
        Planning = 2,
        Unknown = 99
    };

    enum PlanningState
    {
        Start = 0,
        End = 1,
        Both = 2
    };

    struct ObjectListItem
    {
        DomItem* itemStart;
        DomItem* itemEnd;
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
    void clearCache();

public slots:
    void documentChanged();

signals:
    void dataChanged();

protected:
    DomItem* rootItem;
    mutable DocumentType cachedDocumentType;
    QHash<QString, QList<DomItem*>> cachedStartObjectList;
    QHash<QString, QList<DomItem*>> cachedEndObjectList;
    QHash<QString, QList<ObjectListItem>> cachedCombinedObjectList;

};

#endif // PLANPRODOCUMENT_H
