#ifndef EDGEWIDGET_H
#define EDGEWIDGET_H

#include <QListWidget>
#include <QListWidgetItem>
#include "halfedge.h"

class EdgeWidget : public QListWidget
{
    Q_OBJECT

private:
    HalfEdge* selected;

public:
    EdgeWidget(QWidget* p);

public slots:
    void slot_addEdges(QListWidgetItem *q);

};

#endif // EDGEWIDGET_H
