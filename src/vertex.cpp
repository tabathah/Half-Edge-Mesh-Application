#include "vertex.h"

Vertex::Vertex(float x, float y, float z, int iden) : pos(glm::vec3(x, y, z)), edge(nullptr), id(iden),
  j1(-1), j2(-1), j1Infl(1.0f), j2Infl(0.0f)
{}

glm::vec3 Vertex::getPos()
{
    return pos;
}

std::vector<glm::vec3> Vertex::getNorms()
{
    return norms;
}

int Vertex::getId()
{
    return id;
}

HalfEdge* Vertex::getEdge()
{
    return edge;
}

int Vertex::getJoint1()
{
    return j1;
}

int Vertex::getJoint2()
{
    return j2;
}

float Vertex::getInfl1()
{
    return j1Infl;
}

float Vertex::getInfl2()
{
    return j2Infl;
}

void Vertex::setX(double d)
{
    pos[0] = d;
}

void Vertex::setY(double d)
{
    pos[1] = d;
}

void Vertex::setZ(double d)
{
    pos[2] = d;
}

void Vertex::setEdge(HalfEdge* edg)
{
    edge = edg;
}

void Vertex::addNorm(glm::vec3 n)
{
    norms.push_back(n);
}

void Vertex::setJoint1(int j, float infl)
{
    j1 = j;
    j1Infl = infl;
}

void Vertex::setJoint2(int j, float infl)
{
    j2 = j;
    j2Infl = infl;
}
