#ifndef FACEWIDGET_H
#define FACEWIDGET_H

#include <QListWidget>
#include <QListWidgetItem>
#include "face.h"

class FaceWidget : public QListWidget
{
    Q_OBJECT

private:
    Face* selected;

public:
    FaceWidget(QWidget* p);

public slots:
    void slot_addFaces(QListWidgetItem *q);

};

#endif // FACEWIDGET_H
