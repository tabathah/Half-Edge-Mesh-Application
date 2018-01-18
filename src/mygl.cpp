#include "mygl.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include <la.h>

#include<string.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

#include <QFileDialog>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      geom_cylinder(this), geom_sphere(this), mesh(this),
      geom_vertex(this), geom_edge(this), geom_face(this), rootJoint(nullptr),
      selectedVert(nullptr), selectedEdge(nullptr), selectedFace(nullptr), selectedJoint(nullptr),
      prog_lambert(this), prog_flat(this), prog_skeleton(this),
      gl_camera(), timeCount(0), renderMode(0), circleMode(0), spinMode(0), bound(false)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cylinder.destroy();
    geom_sphere.destroy();
    mesh.destroy();
}

//creates a mesh for a cube which spans -0.5 to 0.5 on each axis
void createCubeMesh(Mesh &m)
{
    //create a collection of vertices for the mesh, vertices range id #s from 0 to 7
    std::vector<Vertex*> vs;

    vs.push_back(new Vertex(0.5f, 0.5f, 0.5f, 0));
    vs.push_back(new Vertex(0.5f, 0.5f, -0.5f, 1));
    vs.push_back(new Vertex(-0.5f, 0.5f, -0.5f, 2));
    vs.push_back(new Vertex(-0.5f, 0.5f, 0.5f, 3));
    vs.push_back(new Vertex(0.5f, -0.5f, -0.5f, 4));
    vs.push_back(new Vertex(0.5f, -0.5f, 0.5f, 5));
    vs.push_back(new Vertex(-0.5f, -0.5f, 0.5f, 6));
    vs.push_back(new Vertex(-0.5f, -0.5f, -0.5f, 7));

    //create a collection of faces for the mesh, faces range id #s from 0 to 5
    std::vector<Face*> fs;
    //the positive and negative y faces will be red, the positive and negative x faces will be green, and the positive and negative z faces will be blue

    fs.push_back(new Face(glm::vec3(255, 0, 0), 0));
    fs.push_back(new Face(glm::vec3(0, 255, 0), 1));
    fs.push_back(new Face(glm::vec3(0, 0, 255), 2));
    fs.push_back(new Face(glm::vec3(0, 255, 0), 3));
    fs.push_back(new Face(glm::vec3(0, 0, 255), 4));
    fs.push_back(new Face(glm::vec3(255, 0, 0), 5));

    //create a collection of half-edges for the mesh, edges range id #s from 0 to 23
    std::vector<HalfEdge*> es;

    es.push_back(new HalfEdge(0));
    es.push_back(new HalfEdge(1));
    es.push_back(new HalfEdge(2));
    es.push_back(new HalfEdge(3));
    es.push_back(new HalfEdge(4));
    es.push_back(new HalfEdge(5));
    es.push_back(new HalfEdge(6));
    es.push_back(new HalfEdge(7));
    es.push_back(new HalfEdge(8));
    es.push_back(new HalfEdge(9));
    es.push_back(new HalfEdge(10));
    es.push_back(new HalfEdge(11));
    es.push_back(new HalfEdge(12));
    es.push_back(new HalfEdge(13));
    es.push_back(new HalfEdge(14));
    es.push_back(new HalfEdge(15));
    es.push_back(new HalfEdge(16));
    es.push_back(new HalfEdge(17));
    es.push_back(new HalfEdge(18));
    es.push_back(new HalfEdge(19));
    es.push_back(new HalfEdge(20));
    es.push_back(new HalfEdge(21));
    es.push_back(new HalfEdge(22));
    es.push_back(new HalfEdge(23));

    //setting the edges' pointers to vertices
    es[0]->setVert(vs[1]);
    es[1]->setVert(vs[2]);
    es[2]->setVert(vs[3]);
    es[3]->setVert(vs[0]);
    es[4]->setVert(vs[4]);
    es[5]->setVert(vs[1]);
    es[6]->setVert(vs[0]);
    es[7]->setVert(vs[5]);
    es[8]->setVert(vs[4]);
    es[9]->setVert(vs[7]);
    es[10]->setVert(vs[2]);
    es[11]->setVert(vs[1]);
    es[12]->setVert(vs[2]);
    es[13]->setVert(vs[7]);
    es[14]->setVert(vs[6]);
    es[15]->setVert(vs[3]);
    es[16]->setVert(vs[0]);
    es[17]->setVert(vs[3]);
    es[18]->setVert(vs[6]);
    es[19]->setVert(vs[5]);
    es[20]->setVert(vs[5]);
    es[21]->setVert(vs[6]);
    es[22]->setVert(vs[7]);
    es[23]->setVert(vs[4]);

    //setting edges' pointers to the next edges
    es[0]->setNext(es[1]);
    es[1]->setNext(es[2]);
    es[2]->setNext(es[3]);
    es[3]->setNext(es[0]);
    es[4]->setNext(es[5]);
    es[5]->setNext(es[6]);
    es[6]->setNext(es[7]);
    es[7]->setNext(es[4]);
    es[8]->setNext(es[9]);
    es[9]->setNext(es[10]);
    es[10]->setNext(es[11]);
    es[11]->setNext(es[8]);
    es[12]->setNext(es[13]);
    es[13]->setNext(es[14]);
    es[14]->setNext(es[15]);
    es[15]->setNext(es[12]);
    es[16]->setNext(es[17]);
    es[17]->setNext(es[18]);
    es[18]->setNext(es[19]);
    es[19]->setNext(es[16]);
    es[20]->setNext(es[21]);
    es[21]->setNext(es[22]);
    es[22]->setNext(es[23]);
    es[23]->setNext(es[20]);

    //setting edges' pointers to their symmetric edges
    es[0]->setSym(es[6]);
    es[1]->setSym(es[11]);
    es[2]->setSym(es[12]);
    es[3]->setSym(es[17]);
    es[4]->setSym(es[20]);
    es[5]->setSym(es[8]);
    es[6]->setSym(es[0]);
    es[7]->setSym(es[16]);
    es[8]->setSym(es[5]);
    es[9]->setSym(es[23]);
    es[10]->setSym(es[13]);
    es[11]->setSym(es[1]);
    es[12]->setSym(es[2]);
    es[13]->setSym(es[10]);
    es[14]->setSym(es[22]);
    es[15]->setSym(es[18]);
    es[16]->setSym(es[7]);
    es[17]->setSym(es[3]);
    es[18]->setSym(es[15]);
    es[19]->setSym(es[21]);
    es[20]->setSym(es[4]);
    es[21]->setSym(es[19]);
    es[22]->setSym(es[14]);
    es[23]->setSym(es[9]);

    //setting edges' pointers to their faces
    es[0]->setFace(fs[0]);
    es[1]->setFace(fs[0]);
    es[2]->setFace(fs[0]);
    es[3]->setFace(fs[0]);
    es[4]->setFace(fs[1]);
    es[5]->setFace(fs[1]);
    es[6]->setFace(fs[1]);
    es[7]->setFace(fs[1]);
    es[8]->setFace(fs[2]);
    es[9]->setFace(fs[2]);
    es[10]->setFace(fs[2]);
    es[11]->setFace(fs[2]);
    es[12]->setFace(fs[3]);
    es[13]->setFace(fs[3]);
    es[14]->setFace(fs[3]);
    es[15]->setFace(fs[3]);
    es[16]->setFace(fs[4]);
    es[17]->setFace(fs[4]);
    es[18]->setFace(fs[4]);
    es[19]->setFace(fs[4]);
    es[20]->setFace(fs[5]);
    es[21]->setFace(fs[5]);
    es[22]->setFace(fs[5]);
    es[23]->setFace(fs[5]);

    //note when the half edges' vertices and faces were set, the vertices' edges were set and the faces' edges were set as well

    m.setVerts(vs);
    m.setFaces(fs);
    m.setEdges(es);

    m.create();
}

glm::vec3 getRandomColor()
{
    float r = (float)(rand()) / (float)(RAND_MAX/5.0);
    float g = (float)(rand()) / (float)(RAND_MAX/5.0);
    float b = (float)(rand()) / (float)(RAND_MAX/5.0);
    return glm::vec3(r, g, b);
}

void MyGL::createMesh(QString fileName)
{
    //collections of vertices, faces, and edges which will be input into our mesh instance
    std::vector<Vertex*> vs;
    std::vector<Face*> fs;
    std::vector<HalfEdge*> es;

    //counts on the number of vertices, faces, and edges so far encountered so that we can easily give them ids
    int vertCount = 0;
    int edgeCount = 0;
    int faceCount = 0;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line.startsWith('v', Qt::CaseSensitive) && !line.startsWith("vt", Qt::CaseSensitive) && !line.startsWith("vn", Qt::CaseSensitive))
          {
              //read vertex positions and populate vs
              float x, y, z, w;
              sscanf(line.toLatin1().data(), "v %f %f %f %f", &x, &y, &z, &w);
              vs.push_back(new Vertex(x, y, z, vertCount));
              vertCount++;
          }
          if(line.startsWith('f', Qt::CaseSensitive))
          {
              //read faces one by one
              Face* f = new Face(getRandomColor(), faceCount);
              faceCount++;
              fs.push_back(f);

              //populate vertexInds with first numbers in face // things
              std::vector<int> vertexInds;
              char* token;
              char* string = strdup(line.toStdString().c_str());
              token = strtok (string," ,.-");
              while (token != nullptr)
              {
                  if(token[0] != 'f')
                  {
                      int pos, uv, norm;
                      sscanf(token, "%d/%d/%d", &pos, &uv, &norm);
                      vertexInds.push_back(pos);
                  }
                  token = strtok (nullptr, " ,.-");
              }

              //create all the edges on this face, setting their face, vertex, and next pointers as we go
              Vertex* startVert = vs[vertexInds[0]-1];
              HalfEdge* startEdge = new HalfEdge(edgeCount);
              edgeCount++;
              es.push_back(startEdge);
              startEdge->setFace(f);
              startEdge->setVert(vs[vertexInds[1]-1]);
              HalfEdge* currEdge = startEdge;
              for(unsigned int i = 2; i < vertexInds.size(); i++)
              {
                  HalfEdge* newEdge = new HalfEdge(edgeCount);
                  edgeCount++;
                  es.push_back(newEdge);
                  newEdge->setFace(f);
                  newEdge->setVert(vs[vertexInds[i]-1]);
                  currEdge->setNext(newEdge);
                  currEdge = newEdge;
              }
              HalfEdge* lastEdge = new HalfEdge(edgeCount);
              edgeCount++;
              es.push_back(lastEdge);
              lastEdge->setFace(f);
              lastEdge->setVert(startVert);
              currEdge->setNext(lastEdge);
              lastEdge->setNext(startEdge);
          }
       }
       file.close();
    }


    //set the sym pointers for each edge
    for(unsigned int j = 0; j < es.size(); j++)
    {
        HalfEdge* e = es[j];
        if(e->getSym() == nullptr)
        {
            Vertex* v1 = e->getVert();
            HalfEdge* curr = e->getNext();
            while(curr->getNext() != e)
            {
                curr = curr->getNext();
            }
            Vertex* v2 = curr->getVert();
            for(unsigned int k = 0; k < es.size(); k++)
            {
                HalfEdge* checkEdge = es[k];
                if(checkEdge->getSym() == nullptr && checkEdge->getVert() == v2)
                {
                    curr = checkEdge->getNext();
                    while(curr->getNext() != checkEdge)
                    {
                        curr = curr->getNext();
                    }
                    if(curr->getVert() == v1)
                    {
                        e->setSym(checkEdge);
                        break;
                    }
                }
            }
        }
    }

    //setting mesh's collections to the one's we populated and creating the mesh
    mesh.setVerts(vs);
    mesh.setFaces(fs);
    mesh.setEdges(es);

    mesh.create();

    //clearing lists, sending the vertices, edges, and faces to the list widgets
    emit sig_ClearLists();
    geom_vertex.setVertex(nullptr);
    geom_edge.setEdge(nullptr);
    geom_face.setFace(nullptr);
    for(unsigned int i = 0; i < mesh.getVs().size(); i++)
    {
        emit sig_SendVerts(mesh.getVs()[i]);
    }
    for(unsigned int i = 0; i < mesh.getEs().size(); i++)
    {
        emit sig_SendEdges(mesh.getEs()[i]);
    }
    for(unsigned int i = 0; i < mesh.getFs().size(); i++)
    {
        emit sig_SendFaces(mesh.getFs()[i]);
    }

    update();
}

void MyGL::parsePieces(Joint* parent, QJsonArray children)
{
    for(int i = 0; i < children.size(); i++)
    {
        QJsonObject j = children[i].toObject();
        QString n = j["name"].toString();
        QJsonArray posit = j["pos"].toArray();
        glm::vec4 p = glm::vec4(posit[0].toDouble(), posit[1].toDouble(), posit[2].toDouble(), 1.0);
        QJsonArray rotat = j["rot"].toArray();
        glm::vec4 rvec = glm::vec4(
            rotat[0].toDouble(),
            rotat[1].toDouble(),
            rotat[2].toDouble(),
            rotat[3].toDouble()
        );
        glm::quat r = glm::angleAxis(rvec[0], glm::vec3(rvec[1], rvec[2], rvec[3]));
        Joint* newJ = new Joint(this, n, parent, p, r);

        QJsonArray childs = j["children"].toArray();
        parsePieces(newJ, childs);
    }
}

void MyGL::createSkeleton(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray text = file.readAll();

    QJsonDocument doc(QJsonDocument::fromJson(text, nullptr));

    QJsonObject obj = doc.object();

    QJsonObject j = obj["root"].toObject();
    QString n = j["name"].toString();
    QJsonArray posit = j["pos"].toArray();
    glm::vec4 p = glm::vec4(posit[0].toDouble(), posit[1].toDouble(), posit[2].toDouble(), 1.0);
    QJsonArray rotat = j["rot"].toArray();
    glm::vec4 rvec = glm::vec4(
        rotat[0].toDouble(),
        rotat[1].toDouble(),
        rotat[2].toDouble(),
        rotat[3].toDouble()
    );
    glm::quat r = glm::angleAxis(rvec[0], glm::vec3(rvec[1], rvec[2], rvec[3]));
    rootJoint = new Joint(this, n, nullptr, p, r);

    QJsonArray children = j["children"].toArray();
    parsePieces(rootJoint, children);

    rootJoint->create();

    emit sig_SendRootJoint(rootJoint);

    update();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    geom_cylinder.create();

    geom_sphere.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Create and set up the skeleton shader
    prog_skeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);

    createCubeMesh(mesh);

//    glm::quat quat = glm::angleAxis(0.0f, glm::vec3(1, 0, 0));
//    rootJoint = new Joint(this, "root", nullptr, glm::vec4(0.0, 0.0, 0.0, 1.0), quat);

//    Joint* nextJoint = new Joint(this, "next", rootJoint, glm::vec4(1.0, -1.0, 0.0, 1.0), quat);

//    rootJoint->create();

//    emit sig_SendRootJoint(rootJoint);

    for(unsigned int i = 0; i < mesh.getVs().size(); i++)
    {
        emit sig_SendVerts(mesh.getVs()[i]);
    }
    for(unsigned int i = 0; i < mesh.getEs().size(); i++)
    {
        emit sig_SendEdges(mesh.getEs()[i]);
    }
    for(unsigned int i = 0; i < mesh.getFs().size(); i++)
    {
        emit sig_SendFaces(mesh.getFs()[i]);
    }
}

void MyGL::slot_SelectVertex(QListWidgetItem *clicked)
{
    selectedVert = (Vertex*)clicked;
    geom_vertex.setVertex((Vertex*)clicked);
    geom_vertex.create();
    update();

    glm::vec3 pos = selectedVert->getPos();
    emit sig_SendXPos(pos[0]);
    emit sig_SendYPos(pos[1]);
    emit sig_SendZPos(pos[2]);
}

void MyGL::slot_SelectEdge(QListWidgetItem *clicked)
{
    selectedEdge = (HalfEdge*)clicked;
    geom_edge.setEdge((HalfEdge*)clicked);
    geom_edge.create();
    update();
}

void MyGL::slot_SelectFace(QListWidgetItem *clicked)
{
    selectedFace = (Face*)clicked;
    geom_face.setFace((Face*)clicked);
    geom_face.create();
    update();

    glm::vec3 col = selectedFace->getCol();
    emit sig_SendRed(col[0]);
    emit sig_SendGreen(col[1]);
    emit sig_SendBlue(col[2]);
}

void MyGL::slot_SelectJoint(QTreeWidgetItem *clicked)
{
    if(selectedJoint != nullptr)
    {
        selectedJoint->setSelected(false);
    }
    selectedJoint = (Joint*)clicked;
    selectedJoint->setSelected(true);
    rootJoint->create();
    update();

    glm::mat4 trans = selectedJoint->getLocalTransformation();
    emit sig_SendTransX(trans[3][0]);
    emit sig_SendTransY(trans[3][1]);
    emit sig_SendTransZ(trans[3][2]);
    emit sig_SendRotX(selectedJoint->getXAngle());
    emit sig_SendRotY(selectedJoint->getYAngle());
    emit sig_SendRotZ(selectedJoint->getZAngle());
}

void MyGL::slot_ChangeXPos(double d)
{
    if(selectedVert != nullptr)
    {
        selectedVert->setX(d);
        checkPlanarity(selectedVert);
        mesh.create();
        update();
    }
}

void MyGL::slot_ChangeYPos(double d)
{
    if(selectedVert != nullptr)
    {
        selectedVert->setY(d);
        checkPlanarity(selectedVert);
        mesh.create();
        update();
    }
}

void MyGL::slot_ChangeZPos(double d)
{
    if(selectedVert != nullptr)
    {
        selectedVert->setZ(d);
        checkPlanarity(selectedVert);
        mesh.create();
        update();
    }
}

void MyGL::slot_ChangeRed(double d)
{
    if(selectedFace != nullptr)
    {
        selectedFace->setR(d);
        mesh.create();
        update();
    }
}

void MyGL::slot_ChangeGreen(double d)
{
    if(selectedFace != nullptr)
    {
        selectedFace->setG(d);
        mesh.create();
        update();
    }
}

void MyGL::slot_ChangeBlue(double d)
{
    if(selectedFace != nullptr)
    {
        selectedFace->setB(d);
        mesh.create();
        update();
    }
}

void MyGL::slot_TranslateX(double d)
{
    if(selectedJoint != nullptr)
    {
        selectedJoint->transX(d);
        rootJoint->create();
        update();
    }
}

void MyGL::slot_TranslateY(double d)
{
    if(selectedJoint != nullptr)
    {
        selectedJoint->transY(d);
        rootJoint->create();
        update();
    }
}

void MyGL::slot_TranslateZ(double d)
{
    if(selectedJoint != nullptr)
    {
        selectedJoint->transZ(d);
        rootJoint->create();
        update();
    }
}

void MyGL::slot_RotateX(double d)
{
    if(selectedJoint != nullptr)
    {
        selectedJoint->rotX(d);
        rootJoint->create();
        update();
    }
}

void MyGL::slot_RotateY(double d)
{
    if(selectedJoint != nullptr)
    {
        selectedJoint->rotY(d);
        rootJoint->create();
        update();
    }
}

void MyGL::slot_RotateZ(double d)
{
    if(selectedJoint != nullptr)
    {
        selectedJoint->rotZ(d);
        rootJoint->create();
        update();
    }
}

void MyGL::goToNext()
{
    if(selectedEdge != nullptr)
    {
        selectedEdge = selectedEdge->getNext();
        geom_edge.setEdge(selectedEdge);
        geom_edge.create();
        update();
    }
}

void MyGL::goToSym()
{
    if(selectedEdge != nullptr)
    {
        selectedEdge = selectedEdge->getSym();
        geom_edge.setEdge(selectedEdge);
        geom_edge.create();
        update();
    }
}

void MyGL::goToFace()
{
    if(selectedEdge != nullptr)
    {
        selectedFace = selectedEdge->getFace();
        geom_face.setFace(selectedFace);
        geom_face.create();
        update();
    }
}

void MyGL::goToVert()
{
    if(selectedEdge != nullptr)
    {
        selectedVert = selectedEdge->getVert();
        geom_vertex.setVertex(selectedVert);
        geom_vertex.create();
        update();
    }
}

void MyGL::vertToEdge()
{
    if(selectedVert != nullptr)
    {
        selectedEdge = selectedVert->getEdge();
        geom_edge.setEdge(selectedEdge);
        geom_edge.create();
        update();
    }
}

void MyGL::faceToEdge()
{
    if(selectedFace != nullptr)
    {
        selectedEdge = selectedFace->getEdge();
        geom_edge.setEdge(selectedEdge);
        geom_edge.create();
        update();
    }
}

void MyGL::switchRenderMode()
{
    if(renderMode == 0)
    {
        renderMode = 1;
    }
    else
    {
        renderMode = 0;
    }
    prog_lambert.useMe();
    glUniform1i(prog_lambert.unifRenderMode, renderMode);
    update();
}

void MyGL::switchCircleMode()
{
    if(circleMode == 0)
    {
        circleMode = 1;
    }
    else
    {
        circleMode = 0;
    }
    prog_lambert.useMe();
    glUniform1i(prog_lambert.unifCircleMode, circleMode);
    update();
}

void MyGL::switchSpinMode()
{
    if(spinMode == 0)
    {
        spinMode = 1;
    }
    else
    {
        spinMode = 0;
    }
    update();
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    gl_camera = Camera(w, h);
    glm::mat4 viewproj = gl_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);
    prog_skeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

void MyGL::drawSkeleton(Joint* root)
{
    prog_flat.draw(*root);
    for(unsigned int i = 0; i < root->getChildren().size(); i++)
    {
        drawSkeleton(root->getChildren()[i]);
    }
}

void findAllJoints(Joint* j, std::vector<Joint*> &js)
{
    js.push_back(j);
    for(unsigned int i = 0; i < j->getChildren().size(); i++)
    {
        findAllJoints(j->getChildren()[i], js);
    }
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog_flat.setViewProjMatrix(gl_camera.getViewProj());
    prog_lambert.setViewProjMatrix(gl_camera.getViewProj());
    prog_skeleton.setViewProjMatrix(gl_camera.getViewProj());

#define NOPE
#ifdef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::mat4(1.0);
            //glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
    //Send the geometry's transformation matrix to the shader
//    prog_lambert.setModelMatrix(model);
//    //Draw the example sphere using our lambert shader
//    prog_lambert.draw(geom_sphere);

//    //Now do the same to render the cylinder
//    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    prog_lambert.setModelMatrix(model);
//    prog_lambert.draw(geom_cylinder);

    if(spinMode == 1)
    {
        float rotAngle = float(timeCount)/40.0f;
        model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    prog_lambert.setModelMatrix(model);
    prog_flat.setModelMatrix(model);
    prog_skeleton.setModelMatrix(model);

    if(bound)
    {
        std::vector<Joint*> allJoints;
        findAllJoints(rootJoint, allJoints);

        std::vector<glm::mat4> bindMats;
        std::vector<glm::mat4> transMats;

        for(unsigned int i = 0; i < allJoints.size(); i++)
        {
            bindMats.push_back(allJoints[i]->getBindMatrix());
            transMats.push_back(allJoints[i]->getOverallTransformation());
        }

        prog_skeleton.useMe();
        glUniformMatrix4fv(prog_skeleton.unifBindMats, allJoints.size(), false, &bindMats.data()[0][0][0]);
        glUniformMatrix4fv(prog_skeleton.unifTransMats, allJoints.size(), false, &transMats.data()[0][0][0]);

        prog_skeleton.draw(mesh);
    }
    else
    {
        prog_lambert.draw(mesh);
    }

    glDisable(GL_DEPTH_TEST);
    if(geom_vertex.getVertex() != nullptr)
    {
        prog_flat.draw(geom_vertex);
    }
    if(geom_edge.getEdge() != nullptr)
    {
        prog_flat.draw(geom_edge);
    }
    if(geom_face.getFace() != nullptr)
    {
        prog_flat.draw(geom_face);
    }
    if(rootJoint != nullptr)
    {
        drawSkeleton(rootJoint);
    }
    glEnable(GL_DEPTH_TEST);

#endif
}


void MyGL::keyPressEvent(QKeyEvent *e)
{

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
        if (e->key() & Qt::Key_H) {
           faceToEdge();
        }
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        gl_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        gl_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        gl_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        gl_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        gl_camera = Camera(this->width(), this->height());
        switchRenderMode();
    } else if (e->key() == Qt::Key_N) {
        goToNext();
    } else if (e->key() == Qt::Key_M) {
        goToSym();
    } else if (e->key() == Qt::Key_F) {
        goToFace();
    } else if (e->key() == Qt::Key_V) {
        goToVert();
    } else if (e->key() == Qt::Key_H) {
        vertToEdge();
    } else if (e->key() == Qt::Key_Shift) {
        faceToEdge();
    } else if (e->key() == Qt::Key_C) {
        switchCircleMode();
    } else if (e->key() == Qt::Key_Z) {
        switchSpinMode();
    }
    gl_camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::timerUpdate()
{
    timeCount++;
    prog_lambert.useMe();
    glUniform1i(prog_lambert.unifTime, timeCount);
    update();
}

//returns the index at which check is in vec if check is in vec, otherwise, returns -1
int contains(std::vector<glm::vec3> vec, glm::vec3 check)
{
    for(unsigned int i = 0; i < vec.size(); i++)
    {
        if(glm::abs(vec[i][0] - check[0]) < 0.0000000000001f && glm::abs(vec[i][1] - check[1]) < 0.0000000000001f && glm::abs(vec[i][2] - check[2]) < 0.0000000000001f)
        {
            return i;
        }
    }
    return -1;
}

void MyGL::exportOBJ()
{
    std::map<Vertex*, int> posMap = std::map<Vertex*, int>(); //map of a vertex to the index of its position in the vector positions
    std::map<Vertex*, int> normMap = std::map<Vertex*, int>(); //map of a vertex to the index of its normal in the vector normals
    std::vector<glm::vec3> positions; //collection of distinct positions of vertices
    std::vector<glm::vec3> normals; //collection of distinct normals of vertices

    for(unsigned int m = 0; m < mesh.getVs().size(); m++)
    {
        Vertex* v = mesh.getVs()[m];
        int check1 = contains(positions, v->getPos());
        if(check1 == -1)
        {
            posMap.insert(std::pair<Vertex*,int>(v, positions.size()));
            positions.push_back(v->getPos());
        }
        else
        {
            posMap.insert(std::pair<Vertex*,int>(v, check1));
        }
        for(unsigned int n = 0; n < v->getNorms().size(); n++)
        {
            int check2 = contains(normals, v->getNorms()[n]);
            if(check2 == -1)
            {
                normMap.insert(std::pair<Vertex*,int>(v, normals.size()));
                normals.push_back(v->getNorms()[n]);
            }
            else
            {
                normMap.insert(std::pair<Vertex*,int>(v, check2));
            }
        }
    }

    QString filename = QFileDialog::getSaveFileName(this, "NewObj", ":/", "*.obj");
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for(unsigned int i = 0; i < positions.size(); i++)
        {
            stream << "v " << positions[i][0] << " " << positions[i][1] << " " << positions[i][2] << " 1.0" << endl;
        }
        for(unsigned int j = 0; j < normals.size(); j++)
        {
            stream << "vn " << normals[j][0] << " " << normals[j][1] << " " << normals[j][2] << endl;
        }
        for(unsigned int k = 0; k < mesh.getFs().size(); k++)
        {
            stream << "f ";
            Face* f = mesh.getFs()[k];
            HalfEdge* startEdge = f->getEdge();
            stream << posMap.at(startEdge->getVert()) << "//" << normMap.at(startEdge->getVert()) << " ";
            HalfEdge* currEdge = startEdge->getNext();
            while(currEdge != startEdge)
            {
                stream << posMap.at(currEdge->getVert()) << "//" << normMap.at(currEdge->getVert()) << " ";
                currEdge = currEdge->getNext();
            }
            stream << endl;
        }
    }
}



//HW6////////////////////////////////////////////////////////

void MyGL::splitEdge(const glm::vec3 &vertPos, HalfEdge* selected)
{
    Vertex* vMid = new Vertex(vertPos[0], vertPos[1], vertPos[2], mesh.getVs().size());
    mesh.addVert(vMid);
    HalfEdge* e1 = selected;
    HalfEdge* e2 = selected->getSym();
    HalfEdge* e3 = new HalfEdge(mesh.getEs().size());
    mesh.addEdge(e3);
    HalfEdge* e4 = new HalfEdge(mesh.getEs().size());
    mesh.addEdge(e4);
    e3->setFace(e1->getFace());
    e4->setFace(e2->getFace());
    e3->setVert(e1->getVert());
    e4->setVert(e2->getVert());
    e3->setNext(e1->getNext());
    e4->setNext(e2->getNext());
    e1->setVert(vMid);
    e2->setVert(vMid);
    e1->setNext(e3);
    e2->setNext(e4);
    e1->setSym(e4);
    e2->setSym(e3);

    //send signals for new vertex and edges to be sent to the list widgets, leave new vertex selected
    emit sig_SendVerts(vMid);
    emit sig_SendEdges(e3);
    emit sig_SendEdges(e4);
    selectedVert = vMid;
}

void MyGL::slot_SplitEdge()
{
    glm::vec3 pos1 = selectedEdge->getVert()->getPos();
    glm::vec3 pos2 = selectedEdge->getSym()->getVert()->getPos();
    splitEdge(glm::vec3((pos1[0]+pos2[0])/2.0f, (pos1[1]+pos2[1])/2.0f, (pos1[2]+pos2[2])/2.0f), selectedEdge);
    mesh.create();
    geom_vertex.setVertex(selectedVert);
    geom_vertex.create();
    geom_edge.create();
    update();
}

void MyGL::triangulate()
{
    Face* triFace = selectedFace; //face we are currently triangulating
    HalfEdge* ei = triFace->getEdge(); //first edge coming out of v0 (the vertex from which the fan will be created)
    Vertex* v0 = ei->getSym()->getVert(); //vertex from which fan will be created
    HalfEdge* curr = ei;
    while(curr->getVert() != v0)
    {
        curr = curr->getNext();
    }
    HalfEdge* ef = curr; //edge going into v0 on triFace
    std::vector<HalfEdge*> edgesToSend; //collection of new edges we will need to send to the list widget
    std::vector<Face*> facesToSend; //collection of new faces we will need to send to the list widget

    while(ei->getNext()->getNext() != ef)
    {
        //create new edges and new face for spliting triFace
        HalfEdge* eA = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(eA);
        HalfEdge* eB = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(eB);
        edgesToSend.push_back(eA); edgesToSend.push_back(eB);
        Face* fNew = new Face(getRandomColor(), mesh.getFs().size());
        mesh.addFace(fNew);
        facesToSend.push_back(fNew);

        eA->setFace(triFace);
        eB->setFace(fNew);

        HalfEdge* e2 = ei->getNext();

        eA->setVert(v0);
        eB->setVert(e2->getVert());
        eA->setNext(ei);
        eB->setNext(e2->getNext());
        eA->setSym(eB);
        e2->setNext(eA);
        ef->setNext(eB);

        //reset ei and triFace for triangulating the remainder of the face
        ei = eB;
        triFace = fNew;
    }

    //send signals for new edges and faces to be sent to the list widgets
    for(unsigned int i = 0; i < edgesToSend.size(); i++)
    {
        emit sig_SendEdges(edgesToSend[i]);
    }
    for(unsigned int i = 0; i < facesToSend.size(); i++)
    {
        emit sig_SendFaces(facesToSend[i]);
    }
}

void MyGL::slot_TriangulateFace()
{
    triangulate();
    mesh.create();
    geom_face.create();
    update();
}

void MyGL::extrude()
{
    Face* f = selectedFace; //face we are currently extruding
    std::vector<Vertex*> origVerts; //collection of original vertices to help keep track of what vertices we have already extruded
    //populating origVerts
    HalfEdge* start = f->getEdge();
    origVerts.push_back(start->getVert());
    HalfEdge* curr = start->getNext();
    while(curr != start)
    {
        origVerts.push_back(curr->getVert());
        curr = curr->getNext();
    }

    std::vector<Vertex*> vertsToSend; //collection of new vertices we will need to send to the list widget
    std::vector<HalfEdge*> edgesToSend; //collection of new edges we will need to send to the list widget
    std::vector<Face*> facesToSend; //collection of new faces we will need to send to the list widget

    //calculating normal of face, will use to get positions of extruded vertices
    Vertex* vA = origVerts[0];
    Vertex* vB = origVerts[1];
    Vertex* vC = origVerts[2];
    glm::vec3 vec1 = vA->getPos() - vB->getPos();
    glm::vec3 vec2 = vC->getPos() - vB->getPos();
    glm::vec3 normal = glm::cross(vec2, vec1);

    HalfEdge* startEdge = start->getNext(); //first edge we will start extruding
    Vertex* v1 = origVerts[0];
    Vertex* v2 = origVerts[1];
    Vertex* v3 = new Vertex((v1->getPos()[0]+normal[0]), (v1->getPos()[1]+normal[1]), (v1->getPos()[2]+normal[2]), mesh.getVs().size());
    mesh.addVert(v3); //extruded vertex of v1
    vertsToSend.push_back(v3);
    Vertex* startVert = v3; //first extruded vertex, we will need it for extruding the last edge
    Vertex* v4 = new Vertex((v2->getPos()[0]+normal[0]), (v2->getPos()[1]+normal[1]), (v2->getPos()[2]+normal[2]), mesh.getVs().size());
    mesh.addVert(v4); //extruded vertex of v2
    vertsToSend.push_back(v4);
    origVerts.erase(origVerts.begin()); origVerts.erase(origVerts.begin()); //popping the first two vertices from our collection

    HalfEdge* e1 = startEdge;
    e1->setVert(v4);
    HalfEdge* e2 = e1->getSym();
    HalfEdge* e3 = new HalfEdge(mesh.getEs().size());
    mesh.addEdge(e3);
    edgesToSend.push_back(e3);
    HalfEdge* e4 = new HalfEdge(mesh.getEs().size());
    mesh.addEdge(e4);
    edgesToSend.push_back(e4);
    e3->setSym(e1); e1->setSym(e3);
    e4->setSym(e2); e2->setSym(e4);
    e3->setVert(v3);
    e4->setVert(v2);
    HalfEdge* e5 = new HalfEdge(mesh.getEs().size());
    mesh.addEdge(e5);
    edgesToSend.push_back(e5);
    HalfEdge* e6 = new HalfEdge(mesh.getEs().size());
    mesh.addEdge(e6);
    edgesToSend.push_back(e6);
    e5->setVert(v1);
    e6->setVert(v4);
    e3->setNext(e5);
    e5->setNext(e4);
    e4->setNext(e6);
    e6->setNext(e3);
    Face* fNew = new Face(getRandomColor(), mesh.getFs().size());
    mesh.addFace(fNew);
    facesToSend.push_back(fNew);
    e3->setFace(fNew);
    e4->setFace(fNew);
    e5->setFace(fNew);
    e6->setFace(fNew);

    HalfEdge* currEdge = startEdge->getNext();
    while(currEdge != startEdge)
    {
        //moving on to the vertices of the next edge
        v1 = v2;
        v3 = new Vertex((v1->getPos()[0]+normal[0]), (v1->getPos()[1]+normal[1]), (v1->getPos()[2]+normal[2]), mesh.getVs().size());
        mesh.addVert(v3);
        vertsToSend.push_back(v3);
        if(!origVerts.empty())
        {
            v2 = origVerts[0];
            v4 = new Vertex((v2->getPos()[0]+normal[0]), (v2->getPos()[1]+normal[1]), (v2->getPos()[2]+normal[2]), mesh.getVs().size());
            mesh.addVert(v4);
            vertsToSend.push_back(v4);
            origVerts.erase(origVerts.begin());
        }
        else
        {
            //if origVerts is empty, we have extruded every vertex, and we need to connect to the first vertex and its extruded vertex
            v2 = start->getVert();
            v4 = startVert;
        }

        //moving on to the next edge
        e1 = currEdge;
        e1->setVert(v4);
        e2 = e1->getSym();
        e3 = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(e3);
        edgesToSend.push_back(e3);
        e4 = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(e4);
        edgesToSend.push_back(e4);
        e3->setSym(e1); e1->setSym(e3);
        e4->setSym(e2); e2->setSym(e4);
        e3->setVert(v3);
        e4->setVert(v2);
        e5 = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(e5);
        edgesToSend.push_back(e5);
        e6 = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(e6);
        edgesToSend.push_back(e6);
        e5->setVert(v1);
        e6->setVert(v4);
        e3->setNext(e5);
        e5->setNext(e4);
        e4->setNext(e6);
        e6->setNext(e3);
        fNew = new Face(getRandomColor(), mesh.getFs().size());
        mesh.addFace(fNew);
        facesToSend.push_back(fNew);
        e3->setFace(fNew);
        e4->setFace(fNew);
        e5->setFace(fNew);
        e6->setFace(fNew);

        currEdge = currEdge->getNext();
    }

    //have to set the syms for the edges on the sides of the newly created faces
    HalfEdge* eA = start;
    HalfEdge* eB = start->getNext();
    eB->getSym()->getNext()->setSym(eA->getSym()->getNext()->getNext()->getNext());
    eA = eB;
    eB = eA->getNext();
    while(eA != start)
    {
        eB->getSym()->getNext()->setSym(eA->getSym()->getNext()->getNext()->getNext());
        eA = eB;
        eB = eA->getNext();
    }
}

void MyGL::slot_ExtrudeFace()
{
    extrude();
    mesh.create();
    geom_face.create();
    update();
}

std::map<int, Vertex*> computeCentroids(Mesh& m)
{
    std::map<int, Vertex*> centroids; //map from face id #s to the position of the face's centroid

    std::vector<glm::vec3> verts; //collection of positions of the vertices on this face
    for(unsigned int i = 0; i < m.getFs().size(); i++)
    {
        verts.clear();
        HalfEdge* startEdge = m.getFs()[i]->getEdge();
        verts.push_back(startEdge->getVert()->getPos());
        HalfEdge* curr = startEdge->getNext();
        while(curr != startEdge)
        {
            verts.push_back(curr->getVert()->getPos());
            curr = curr->getNext();
        }

        glm::vec3 centrPos = glm::vec3(0.0f, 0.0f, 0.0f);
        for(unsigned int j = 0; j < verts.size(); j++)
        {
            centrPos += verts[j];
        }
        centrPos /= verts.size();
        Vertex* newCentr = new Vertex(centrPos[0], centrPos[1], centrPos[2], m.getVs().size());
        m.addVert(newCentr);
        centroids.insert(std::pair<int, Vertex*>(i, newCentr));
    }

    return centroids;
}

void MyGL::computeSmoothedMidpoints(const std::map<int, Vertex*> &centroids)
{
    std::vector<HalfEdge*> origEdges;
    //we popualte origEdges with only one edge from each sym pair so that we don't duplicate midpoints
    for(unsigned int i = 0; i < mesh.getEs().size(); i++)
    {
        HalfEdge* e = mesh.getEs()[i];
        if(e->getId() < e->getSym()->getId())
        {
            origEdges.push_back(e);
        }
    }

    for(unsigned int i = 0; i < origEdges.size(); i++)
    {
        HalfEdge* e = origEdges[i];
        Vertex* v1 = e->getVert();
        Vertex* v2 = e->getSym()->getVert();
        Vertex* c1 = centroids.at(e->getFace()->getId());
        Vertex* c2 = centroids.at(e->getSym()->getFace()->getId());

        glm::vec3 midPos = (v1->getPos() + v2->getPos() + c1->getPos() + c2->getPos())/4.0f;
        splitEdge(midPos, e);
    }
}

glm::vec3 adjustSmoothPosition(Vertex* v, const std::map<int, Vertex*> &centroids)
{
    int n = 0;
    glm::vec3 sumMidpts = glm::vec3(0.0f);
    glm::vec3 sumCentrs = glm::vec3(0.0f);

    HalfEdge* startEdge = v->getEdge();
    sumCentrs += centroids.at(startEdge->getFace()->getId())->getPos();
    sumMidpts += startEdge->getNext()->getVert()->getPos();
    n++;

    HalfEdge* currEdge = startEdge->getNext()->getSym();
    while(currEdge != startEdge)
    {
        sumCentrs += centroids.at(currEdge->getFace()->getId())->getPos();
        sumMidpts += currEdge->getNext()->getVert()->getPos();
        n++;
        currEdge = currEdge->getNext()->getSym();
    }

    glm::vec3 vPos = v->getPos();
    vPos *= (float)(n-2)/(float)n;
    sumMidpts /= pow(n, 2);
    sumCentrs /= pow(n, 2);

    return vPos + sumMidpts + sumCentrs;
}

void quadrangulate(Vertex* centroid, const std::vector<HalfEdge*> &toMids, std::vector<Face*> &newFaces, Mesh &m)
{
    std::map<HalfEdge*, HalfEdge*> origNexts; //map from edges that point to midpoints to their original next edges on the face
    for(unsigned int i = 0; i < toMids.size(); i++)
    {
        origNexts.insert(std::pair<HalfEdge*, HalfEdge*>(toMids[i], toMids[i]->getNext()));
    }

    for(unsigned int i = 0; i < toMids.size(); i++)
    {
        Face* f = new Face(getRandomColor(), newFaces.size());
        newFaces.push_back(f);

        HalfEdge* eC = new HalfEdge(m.getEs().size()); //the edge on face f which will point to the centroid
        m.addEdge(eC);

        HalfEdge* eM = new HalfEdge(m.getEs().size()); //the edge on face f which will point from the centroid to a midpoint
        m.addEdge(eM);

        HalfEdge* thisMdptEdge = toMids[i]; //the edge pointing to a midpoint on f

        HalfEdge* lastMdptEdge; //the next edge pointing to a midpoint going clockwise from thisMdptEdge
        if(i == 0)
        {
            lastMdptEdge = toMids[toMids.size()-1];
        }
        else
        {
            lastMdptEdge = toMids[i-1];
        }

        eC->setFace(f);
        eM->setFace(f);
        eC->setVert(centroid);
        eM->setVert(lastMdptEdge->getVert());
        thisMdptEdge->setNext(eC);
        eC->setNext(eM);
        eM->setNext(origNexts.at(lastMdptEdge));
        thisMdptEdge->setFace(f);
        origNexts.at(lastMdptEdge)->setFace(f);
    }

    //setting syms for inner, newly created edges
    for(unsigned int i = 0; i < toMids.size(); i++)
    {
        HalfEdge* thisMdptEdge = toMids[i]; //the edge pointing to a midpoint on f

        HalfEdge* nextMdptEdge; //the next edge pointing to a midpoint going counterclockwise from thisMdptEdge
        if(i == toMids.size()-1)
        {
            nextMdptEdge = toMids[0];
        }
        else
        {
            nextMdptEdge = toMids[i+1];
        }

        thisMdptEdge->getNext()->setSym(nextMdptEdge->getNext()->getNext());
    }
}

void MyGL::slot_SmoothMesh()
{
    std::vector<Vertex*> origVerts = mesh.getVs();
    int origEdgeSize = mesh.getEs().size(); //number of edges we had at beginning of function, used for sending out correct edges to list widget
    //calculate centroids
    std::map<int, Vertex*> centroids = computeCentroids(mesh);
    //calculate smoothed midpoints
    computeSmoothedMidpoints(centroids);
    //adjust positions of original vertices based on centroids and midpoints
    for(unsigned int i = 0; i < origVerts.size(); i++)
    {
        glm::vec3 smoothPos = adjustSmoothPosition(origVerts[i], centroids);
        origVerts[i]->setX(smoothPos[0]);
        origVerts[i]->setY(smoothPos[1]);
        origVerts[i]->setZ(smoothPos[2]);
    }
    //quadrangulate faces
    std::vector<Face*> newFaces;
    for(unsigned int i = 0; i < mesh.getFs().size(); i++)
    {
        Face* face = mesh.getFs()[i];
        std::vector<HalfEdge*> toMids;
        HalfEdge* startEdge = face->getEdge();
        if(startEdge->getVert()->getId() >= origVerts.size())
        {
            toMids.push_back(startEdge);
        }
        HalfEdge* currEdge = startEdge->getNext();
        while(currEdge != startEdge)
        {
            if(currEdge->getVert()->getId() >= origVerts.size())
            {
                toMids.push_back(currEdge);
            }
            currEdge = currEdge->getNext();
        }
        quadrangulate(centroids.at(face->getId()), toMids, newFaces, mesh);
    }
    mesh.setFaces(newFaces);

    mesh.create();

    //clearing lists, sending updated sets of vertices, edges, and faces to the list widgets
    emit sig_ClearFaceList();
    geom_vertex.setVertex(nullptr);
    geom_edge.setEdge(nullptr);
    geom_face.setFace(nullptr);
    for(unsigned int i = origVerts.size(); i < mesh.getVs().size(); i++)
    {
        emit sig_SendVerts(mesh.getVs()[i]);
    }
    for(unsigned int i = origEdgeSize; i < mesh.getEs().size(); i++)
    {
        emit sig_SendEdges(mesh.getEs()[i]);
    }
    for(unsigned int i = 0; i < mesh.getFs().size(); i++)
    {
        emit sig_SendFaces(mesh.getFs()[i]);
    }

    update();
}

bool sameNormal(std::vector<Vertex*> col1, std::vector<Vertex*> col2)
{
    glm::vec3 vec11 = col1[0]->getPos() - col1[1]->getPos();
    glm::vec3 vec12 = col1[2]->getPos() - col1[1]->getPos();
    glm::vec3 norm1 = glm::cross(vec12, vec11);

    glm::vec3 vec21 = col2[0]->getPos() - col2[1]->getPos();
    glm::vec3 vec22 = col2[2]->getPos() - col2[1]->getPos();
    glm::vec3 norm2 = glm::cross(vec22, vec21);

    if(glm::length(norm1) < 0.0000000000001f || glm::length(norm2) < 0.0000000000001f)
    {
        return true;
    }
    else if(glm::abs(norm1[0] - norm2[0]) < 0.0000000000001f && glm::abs(norm1[1] - norm2[1]) < 0.0000000000001f && glm::abs((norm1[2] - norm2[2])) < 0.0000000000001f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MyGL::checkPlanarity(Vertex* v)
{
    std::vector<Face*> adj; //collection of all adjacent faces to the vertex input
    HalfEdge* startEdge  = v->getEdge();
    adj.push_back(startEdge->getFace());
    HalfEdge* currEdge = startEdge->getNext()->getSym();
    while(currEdge != startEdge)
    {
        adj.push_back(currEdge->getFace());
        currEdge = currEdge->getNext()->getSym();
    }

    for(unsigned int i = 0; i < adj.size(); i++)
    {
        std::vector<Vertex*> vertsOnFace;
        startEdge = adj[i]->getEdge();
        vertsOnFace.push_back(startEdge->getVert());
        currEdge = startEdge->getNext();
        while(currEdge != startEdge)
        {
            vertsOnFace.push_back(currEdge->getVert());
            currEdge = currEdge->getNext();
        }

        std::vector<Vertex*> col1 = {vertsOnFace[vertsOnFace.size()-2], vertsOnFace[vertsOnFace.size()-1], vertsOnFace[0]}; //collection of 3 vertices to calculate normal of middle vertex
        std::vector<Vertex*> col2 = {vertsOnFace[vertsOnFace.size()-1], vertsOnFace[0], vertsOnFace[1]}; //collection of 3 vertices to calculate normal of middle vertex
        if(!sameNormal(col1, col2))
        {
            selectedFace = adj[i];
            triangulate();
            break;
        }
        for(unsigned int j = 1; j < vertsOnFace.size()-1; j++)
        {
            col2.clear();
            col2 = {vertsOnFace[j-1], vertsOnFace[j], vertsOnFace[j+1]};
            if(!sameNormal(col1, col2))
            {
                selectedFace = adj[i];
                triangulate();
                break;
            }
        }
    }
}

void MyGL::insertEdgeLoop()
{
    std::vector<HalfEdge*> toSplits; //collection of half edges going in the same direction as the currently selected edge, that will eventually point to midpoints splitting edges around the mesh
    HalfEdge* startEdge = selectedEdge->getSym()->getNext()->getSym(); //we will traverse along the top of the face that selected edge is pointing away from
    toSplits.push_back(startEdge->getSym()->getNext());
    HalfEdge* currEdge = startEdge->getNext();
    while(currEdge != startEdge)
    {
        toSplits.push_back(currEdge->getSym()->getNext());
        currEdge = currEdge->getNext();
    }

    //we split each edge at the midpoint, based on splitEdge code, the edges in toSplit should be pointing to midpoints
    for(unsigned int i = 0; i < toSplits.size(); i++)
    {
        glm::vec3 pos1 = toSplits[i]->getVert()->getPos();
        glm::vec3 pos2 = toSplits[i]->getSym()->getVert()->getPos();
        splitEdge(glm::vec3((pos1[0]+pos2[0])/2.0f, (pos1[1]+pos2[1])/2.0f, (pos1[2]+pos2[2])/2.0f), toSplits[i]);
    }

    std::vector<HalfEdge*> edgesToSend; //collection of new edges we will need to send to the list widget
    std::vector<Face*> facesToSend; //collection of new faces we will need to send to the list widget

    for(unsigned int i = 0; i < toSplits.size(); i++)
    {
        HalfEdge* m1 = toSplits[i];
        HalfEdge* m2;
        if(i == toSplits.size()-1)
        {
            m2 = toSplits[0];
        }
        else
        {
            m2 = toSplits[i+1];
        }

        Face* f1 = m1->getFace();
        Face* f2 = new Face(getRandomColor(), mesh.getFs().size());
        mesh.addFace(f2);
        facesToSend.push_back(f2);

        //two new edges to split this face in half
        HalfEdge* e1 = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(e1);
        edgesToSend.push_back(e1);
        HalfEdge* e2 = new HalfEdge(mesh.getEs().size());
        mesh.addEdge(e2);
        edgesToSend.push_back(e2);

        e1->setFace(f1);
        e2->setFace(f2);
        e1->setVert(m2->getVert());
        e2->setVert(m1->getVert());
        e1->setNext(m2->getSym());
        e2->setNext(m1->getNext());
        e1->setSym(e2);
        m1->setNext(e1);
        if(i == toSplits.size()-1)
        {
            m2->getNext()->getSym()->getNext()->getSym()->setNext(e2);
        }
        else
        {
            m2->getNext()->getSym()->setNext(e2);
        }

        currEdge = e2->getNext();
        while(currEdge != e2)
        {
            currEdge->setFace(f2);
            currEdge = currEdge->getNext();
        }
    }

    //send signals for new edges and faces to be sent to the list widgets
    for(unsigned int i = 0; i < edgesToSend.size(); i++)
    {
        emit sig_SendEdges(edgesToSend[i]);
    }
    for(unsigned int i = 0; i < facesToSend.size(); i++)
    {
        emit sig_SendFaces(facesToSend[i]);
    }

    selectedFace = selectedEdge->getFace();
    selectedEdge = selectedFace->getEdge();
}

void MyGL::slot_InsertEdgeLoop()
{
    insertEdgeLoop();
    mesh.create();
    geom_edge.create();
    geom_face.create();
    update();
}

std::map<Joint*, glm::vec3> getJoints(Joint* j, glm::vec3 rootPos)
{
    std::map<Joint*, glm::vec3> joints;
    joints.insert(std::pair<Joint*, glm::vec3>(j, rootPos + j->getPos()));
    for(unsigned int i = 0; i < j->getChildren().size(); i++)
    {
        auto search = joints.find(j); glm::vec3 v = search->second;
        std::map<Joint*, glm::vec3> childJoints = getJoints(j->getChildren()[i], v);
        joints.insert(childJoints.begin(), childJoints.end());
    }
    return joints;
}

void bindJoints(Joint* j)
{
    j->setBindMatrix();
    for(unsigned int i = 0; i < j->getChildren().size(); i++)
    {
        bindJoints(j->getChildren()[i]);
    }
}

void MyGL::slot_Skin()
{
    if(rootJoint != nullptr)
    {
        //so that we switch over to drawing the mesh with prog_skeleton
        bound = true;

        //set bind matrices for all the joints
        bindJoints(rootJoint);

        std::map<Joint*, glm::vec3> jointPos = getJoints(rootJoint, glm::vec3(0.0, 0.0, 0.0));
        for(unsigned int i = 0; i < mesh.getVs().size(); i++)
        {
            //find the joint which is closest in distance to this vertex
            Vertex* v = mesh.getVs()[i];
            Joint* closest1;
            float closestLength1 = -1.0f;
            for(auto const& iter : jointPos)
            {
                float currLength = glm::length(v->getPos() - iter.second);
                if(closestLength1 < 0 || currLength < closestLength1)
                {
                    closestLength1 = currLength;
                    closest1 = iter.first;
                }
            }

            //make a new map with only the parent and children of the closest joint to find the second
            std::map<Joint*, glm::vec3> closeJoints;
            auto search1 = jointPos.find(closest1->getParent()); glm::vec3 pos = search1->second;
            if(closest1->getParent() != nullptr)
            {
                closeJoints.insert(std::pair<Joint*, glm::vec3>(closest1->getParent(), pos));
            }
            for(unsigned int c = 0; c < closest1->getChildren().size(); c++)
            {
                auto search2 = jointPos.find(closest1->getChildren()[c]); pos = search2->second;
                closeJoints.insert(std::pair<Joint*, glm::vec3>(closest1->getChildren()[c], pos));
            }

            Joint* closest2;
            float closestLength2 = -1.0f;
            for(auto const& iter : closeJoints)
            {
                float currLength = glm::length(v->getPos() - iter.second);
                if(closestLength2 < 0 || currLength < closestLength2)
                {
                    closestLength2 = currLength;
                    closest2 = iter.first;
                }
            }

            //find influence of joints based on distance from vertex
            float infl1 = closestLength1/(closestLength1+closestLength2);
            float infl2 = closestLength2/(closestLength1+closestLength2);

            int idx1 = -1;
            int idx2 = -1;

            std::vector<Joint*> allJoints;
            findAllJoints(rootJoint, allJoints);

            for(unsigned int j = 0; j < allJoints.size(); j++)
            {
                if(allJoints[j] == closest1)
                {
                    idx1 = j;
                }
                if(allJoints[j] == closest2)
                {
                    idx2 = j;
                }
            }

            v->setJoint1(idx1, infl1);
            v->setJoint2(idx2, infl2);
        }
    }

    mesh.create();
    update();
}

