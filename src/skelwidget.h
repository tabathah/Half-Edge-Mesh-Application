#ifndef SKELWIDGET_H
#define SKELWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "joint.h"

class Joint;

class SkelWidget : public QTreeWidget
{
    Q_OBJECT

private:
    QString JointName;

public:
    SkelWidget(QWidget* p);

public slots:
    void slot_addJoint(QTreeWidgetItem* j);

};
#endif // SKELWIDGET_H
