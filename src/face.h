#ifndef FACE_H
#define FACE_H

#include <la.h>
#include <QListWidgetItem>
#include "halfedge.h"

class HalfEdge;

class Face : public QListWidgetItem
{
  private:
    HalfEdge* start_edge;
    glm::vec3 color;
    int id;

  public:
    Face(glm::vec3 col, int iden);
    glm::vec3 getCol();
    int getId();
    HalfEdge* getEdge();
    void setR(double d);
    void setG(double d);
    void setB(double d);
    void setEdge(HalfEdge* edg);

};

#endif // FACE_H
