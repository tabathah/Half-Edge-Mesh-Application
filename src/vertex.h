#ifndef VERTEX_H
#define VERTEX_H

#include <la.h>
#include <QListWidgetItem>
#include "halfedge.h"
#include "joint.h"

class HalfEdge;

class Vertex : public QListWidgetItem
{
  private:
    glm::vec3 pos;
    std::vector<glm::vec3> norms; //for OBJ exporting purposes
    HalfEdge* edge;
    int id;
    int j1; //index of joint that influences this vertex
    int j2;
    float j1Infl;
    float j2Infl;

  public:
    Vertex(float x, float y, float z, int iden);
    glm::vec3 getPos();
    std::vector<glm::vec3> getNorms(); //for OBJ exporting purposes
    int getId();
    HalfEdge* getEdge();
    int getJoint1();
    int getJoint2();
    float getInfl1();
    float getInfl2();
    void setX(double d);
    void setY(double d);
    void setZ(double d);
    void setEdge(HalfEdge* edg);
    void addNorm(glm::vec3 n);
    void setJoint1(int j, float infl);
    void setJoint2(int j, float infl);

};
#endif // VERTEX_H
