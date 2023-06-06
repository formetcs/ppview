#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QRegularExpression>
#include <QTextBrowser>

#include "domitem.h"

class ObjectInfoWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit ObjectInfoWidget(QWidget* parent = nullptr);


public slots:
    void setInfoText(QList<DomItem*> objectlist);

signals:
    void uuidClicked(const QString& id);

private slots:
    void handleHyperlinkSelection(const QUrl& link);

private:
    int getMaxDomDepth(const DomItem* o) const;
    int getLeafCount(const DomItem* o) const;
    QString generateAttributeString(const DomItem* o) const;
    QString generateInfoText(const DomItem* o) const;
    QString generateInfoTextRec(const DomItem* o, int currentDepth, int maxDepth, QString originalId) const;

    QRegularExpression uuidRegExp;
};

#endif // OBJECTINFOWIDGET_H
