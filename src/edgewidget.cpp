#include "edgewidget.h"

EdgeWidget::EdgeWidget(QWidget *p) : QListWidget(p), selected(nullptr)
{

}

void EdgeWidget::slot_addEdges(QListWidgetItem* q)
{
    QString s("Edge ");
    s.append(QString::number(((HalfEdge*)q)->getId()));
    q->setText(s);
    addItem(q);
}
