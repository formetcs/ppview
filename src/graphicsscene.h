#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QList>
#include <QGraphicsScene>

#include "filterwidget.h"

//class QGraphicsItem;


const int GRAPHICSITEM_TYPE = 1;
const int GRAPHICSITEM_ID = 2;
const int GRAPHICSITEM_INFOTEXT = 3;


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject* parent = nullptr);
    ~GraphicsScene();

    QGraphicsItem* getItemById(const QString& id);

signals:

public slots:
    void changeFilterSettings(const QString& key, bool state);
    void changeFilterSettings(const QList<FilterState>& statelist);
    void unselectAllItems();

private slots:

private:

};

#endif // GRAPHICSSCENE_H
