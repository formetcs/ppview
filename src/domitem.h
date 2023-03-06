#ifndef DOMITEM_H
#define DOMITEM_H

#include <QString>
#include <QHash>
#include <QList>


class DomItem
{
public:
    DomItem(const QString& name, const QString& value, DomItem* parent = 0);
    DomItem(const QString& name, DomItem* parent = 0);
    ~DomItem();
    QString getName() const;
    QString getValue() const;
    void setValue(const QString& v);
    DomItem* getChild(int i);
    DomItem* getFirstChildItem(const QString& c);
    QList<DomItem*> getChildItems(const QString& c);
    DomItem* getFirstItemAtPath(const QString& p);
    QString getFirstValueAtPath(const QString& p);
    void addChild(DomItem* c);
    int childCount() const;
    int getOwnPosition() const;
    QString getAttribute(const QString& name) const;
    void addAttribute(const QString& name, const QString& value);
    QHash<QString, QString> getAttributeMap();
    DomItem* parent();

private:
    QString name;
    QString value;
    QList<DomItem*> childItems;
    QHash<QString, QString> attributes;
    DomItem* parentItem;
};

#endif // DOMITEM_H
