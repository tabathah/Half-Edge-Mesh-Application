#include "vertwidget.h"

VertWidget::VertWidget(QWidget *p) : QListWidget(p), selected(nullptr)
{

}

void VertWidget::slot_addVertices(QListWidgetItem* q)
{
    QString s("Vertex ");
    s.append(QString::number(((Vertex*)q)->getId()));
    q->setText(s);
    addItem(q);
}
