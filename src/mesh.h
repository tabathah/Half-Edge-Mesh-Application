#ifndef MESH_H
#define MESH_H

#include "drawable.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Mesh : public Drawable
{
  private:
    std::vector<Vertex*> verts;
    std::vector<HalfEdge*> edges;
    std::vector<Face*> faces;

  public:
    Mesh(GLWidget277* context);
    void setVerts(std::vector<Vertex*> vs);
    void setEdges(std::vector<HalfEdge*> es);
    void setFaces(std::vector<Face*> fs);
    void addVert(Vertex* v);
    void addEdge(HalfEdge* e);
    void addFace(Face* f);
    std::vector<Vertex*> getVs();
    std::vector<HalfEdge*> getEs();
    std::vector<Face*> getFs();
    virtual void create();
};

#endif // MESH_H
