#ifndef JOINT_H
#define JOINT_H

#include <la.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include "drawable.h"
#include <QTreeWidgetItem>

class Joint : public Drawable, public QTreeWidgetItem
{
  private:
    QString name;
    Joint* parent;
    std::vector<Joint*> children;
    glm::vec4 position;
    glm::quat rotation;
    glm::mat4 bindMatrix;
    bool selected;
    float xAngle;
    float yAngle;
    float zAngle;

  public:
    Joint(GLWidget277* context, QString n, Joint* p, glm::vec4 pos, glm::quat rot);
    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();
    void addChild(QTreeWidgetItem*c);
    QString getName();
    Joint* getParent();
    std::vector<Joint*> getChildren();
    glm::vec3 getPos();
    glm::mat4 getBindMatrix();
    float getXAngle();
    float getYAngle();
    float getZAngle();
    glm::dualquat getDualQuat();
    void setBindMatrix();
    void setSelected(bool sel);
    void transX(double d);
    void transY(double d);
    void transZ(double d);
    void rotX(double d);
    void rotY(double d);
    void rotZ(double d);

    GLenum drawMode();
    virtual void create();
};

#endif // JOINT_H
