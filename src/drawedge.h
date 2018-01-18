#ifndef DRAWEDGE_H
#define DRAWEDGE_H

#include "drawable.h"
#include "halfedge.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class DrawHalfEdge : public Drawable
{
  private:
    HalfEdge* edge;

  public:
    DrawHalfEdge(GLWidget277* context);
    void setEdge(HalfEdge* e);
    HalfEdge* getEdge();
    virtual void create();
    GLenum drawMode();
};

#endif // DRAWEDGE_H
