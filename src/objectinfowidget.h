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
    int getMaxDomDepth(DomItem* o);
    int getLeafCount(DomItem* o);
    QString generateInfoText(DomItem* o);
    QString generateInfoTextRec(DomItem* o, int currentDepth, int maxDepth);

    QRegularExpression uuidRegExp;
};

#endif // OBJECTINFOWIDGET_H
