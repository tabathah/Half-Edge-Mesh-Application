#include "face.h"

Face::Face(glm::vec3 col, int iden) : start_edge(nullptr), color(col), id(iden)
{}

glm::vec3 Face::getCol()
{
    return color;
}

int Face::getId()
{
    return id;
}

HalfEdge* Face::getEdge()
{
    return start_edge;
}

void Face::setR(double d)
{
    color[0] = d;
}

void Face::setG(double d)
{
    color[1] = d;
}

void Face::setB(double d)
{
    color[2] = d;
}

void Face::setEdge(HalfEdge* edg)
{
    start_edge = edg;
}
