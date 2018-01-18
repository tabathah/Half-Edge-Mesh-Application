#ifndef DRAWFACE_H
#define DRAWFACE_H

#include "drawable.h"
#include "face.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class DrawFace : public Drawable
{
  private:
    Face* face;

  public:
    DrawFace(GLWidget277* context);
    void setFace(Face* f);
    Face* getFace();
    virtual void create();
    GLenum drawMode();
};

#endif // DRAWFACE_H
