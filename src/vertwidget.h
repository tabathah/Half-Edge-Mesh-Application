#ifndef VERTWIDGET_H
#define VERTWIDGET_H

#include <QListWidget>
#include <QListWidgetItem>
#include "vertex.h"

class VertWidget : public QListWidget
{
    Q_OBJECT

private:
    Vertex* selected;

public:
    VertWidget(QWidget* p);

public slots:
    void slot_addVertices(QListWidgetItem *q);

};

#endif // VERTWIDGET_H
