#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include "camera.h"
#include "mesh.h"
#include "drawvertex.h"
#include "drawedge.h"
#include "drawface.h"
#include "joint.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QListWidgetItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    Cylinder geom_cylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere geom_sphere;// The instance of a unit sphere we can use to render any sphere
    Mesh mesh;
    DrawVertex geom_vertex;
    DrawHalfEdge geom_edge;
    DrawFace geom_face;
    Joint* rootJoint; //pointer to instance of root joint to the joint structure

    Vertex* selectedVert;
    HalfEdge* selectedEdge;
    Face* selectedFace;
    Joint* selectedJoint;

    ShaderProgram prog_lambert;// A shader program that uses lambertian reflection
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram prog_skeleton;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera gl_camera;

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;
    int timeCount;
    int renderMode;
    int circleMode;
    int spinMode;

    bool bound;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void createMesh(QString fileName);

    void goToNext();
    void goToSym();
    void goToFace();
    void goToVert();
    void vertToEdge();
    void faceToEdge();

    void switchRenderMode();
    void switchCircleMode();
    void switchSpinMode();

    void exportOBJ();

    //HW6
    void splitEdge(const glm::vec3 &vertPos, HalfEdge* selected);
    void triangulate();
    void extrude();
    void computeSmoothedMidpoints(const std::map<int, Vertex *> &centroids);
    void checkPlanarity(Vertex* v);
    void insertEdgeLoop();

    //HW7
    void drawSkeleton(Joint* root);
    void createSkeleton(QString fileName);
    void parsePieces(Joint* parent, QJsonArray children);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sig_SendVerts(QListWidgetItem*);
    void sig_SendEdges(QListWidgetItem*);
    void sig_SendFaces(QListWidgetItem*);
    void sig_SendXPos(double);
    void sig_SendYPos(double);
    void sig_SendZPos(double);
    void sig_SendRed(double);
    void sig_SendGreen(double);
    void sig_SendBlue(double);
    void sig_ClearLists();
    void sig_ClearFaceList();
    //HW7
    void sig_SendRootJoint(QTreeWidgetItem*);
    void sig_SendTransX(double);
    void sig_SendTransY(double);
    void sig_SendTransZ(double);
    void sig_SendRotX(double);
    void sig_SendRotY(double);
    void sig_SendRotZ(double);

private slots:
    //slots for highlighting and indicating newly selected vertices, edges, or faces
    void slot_SelectVertex(QListWidgetItem*);
    void slot_SelectEdge(QListWidgetItem*);
    void slot_SelectFace(QListWidgetItem*);
    //slots for changing the position of the selected vertex
    void slot_ChangeXPos(double d);
    void slot_ChangeYPos(double d);
    void slot_ChangeZPos(double d);
    //slots for changing the color of the selected face
    void slot_ChangeRed(double d);
    void slot_ChangeGreen(double d);
    void slot_ChangeBlue(double d);
    /// Slot that gets called ~60 times per second
    void timerUpdate();
    //HW6
    void slot_SplitEdge();
    void slot_TriangulateFace();
    void slot_SmoothMesh();
    void slot_ExtrudeFace();
    void slot_InsertEdgeLoop();
    //HW7
    void slot_SelectJoint(QTreeWidgetItem*);
    void slot_Skin();
    void slot_TranslateX(double);
    void slot_TranslateY(double);
    void slot_TranslateZ(double);
    void slot_RotateX(double);
    void slot_RotateY(double);
    void slot_RotateZ(double);
};


#endif // MYGL_H
