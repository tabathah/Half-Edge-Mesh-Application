#ifndef DRAWVERTEX_H
#define DRAWVERTEX_H

#include "drawable.h"
#include "vertex.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class DrawVertex : public Drawable
{
private:
    Vertex* vert;

public:
    DrawVertex(GLWidget277* context);
    void setVertex(Vertex* v);
    Vertex* getVertex();
    virtual void create();
    GLenum drawMode();
};

#endif // DRAWVERTEX_H
