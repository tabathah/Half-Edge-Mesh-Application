#include "skelwidget.h"

SkelWidget::SkelWidget(QWidget *p) : QTreeWidget(p), JointName("asdf")
{}

void SkelWidget::slot_addJoint(QTreeWidgetItem *j)
{
    addTopLevelItem(j);
}
