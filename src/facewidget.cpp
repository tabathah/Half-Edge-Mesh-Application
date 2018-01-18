#include "facewidget.h"

FaceWidget::FaceWidget(QWidget *p) : QListWidget(p), selected(nullptr)
{

}

void FaceWidget::slot_addFaces(QListWidgetItem* q)
{
    QString s("Face ");
    s.append(QString::number(((Face*)q)->getId()));
    q->setText(s);
    addItem(q);
}
