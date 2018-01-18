#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <la.h>
#include <QListWidgetItem>
#include "face.h"
#include "vertex.h"

class Face;
class Vertex;

class HalfEdge : public QListWidgetItem
{
  private:
    Face* face;
    Vertex* vert;
    HalfEdge* next;
    HalfEdge* sym;
    int id;

  public:
    HalfEdge(int iden);
    int getId();
    Face* getFace();
    Vertex* getVert();
    HalfEdge* getNext();
    HalfEdge* getSym();
    void setFace(Face* f);
    void setVert(Vertex* v);
    void setNext(HalfEdge* e);
    void setSym(HalfEdge* e);

};

#endif // HALFEDGE_H
