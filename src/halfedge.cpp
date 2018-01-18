#include "halfedge.h"

HalfEdge::HalfEdge(int iden) : face(nullptr), vert(nullptr), next(nullptr), sym(nullptr), id(iden)
{}

int HalfEdge::getId()
{
    return id;
}

Face* HalfEdge::getFace()
{
    return face;
}

Vertex* HalfEdge::getVert()
{
    return vert;
}

HalfEdge* HalfEdge::getNext()
{
    return next;
}

HalfEdge* HalfEdge::getSym()
{
    return sym;
}

void HalfEdge::setFace(Face* f)
{
    face = f;
    f->setEdge(this);
}

void HalfEdge::setVert(Vertex* v)
{
    vert = v;
    v->setEdge(this);
}

void HalfEdge::setNext(HalfEdge* e)
{
    next = e;
}

void HalfEdge::setSym(HalfEdge* e)
{
    sym = e;
    if(e->getSym() == nullptr)
    {
        e->setSym(this);
    }
}
