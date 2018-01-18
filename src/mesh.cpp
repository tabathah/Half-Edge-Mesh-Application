#include "mesh.h"
#include<la.h>
#include <iostream>

Mesh::Mesh(GLWidget277 *context) : Drawable(context)
{}

void Mesh::setVerts(std::vector<Vertex*> vs)
{
    verts = vs;
}

void Mesh::setEdges(std::vector<HalfEdge*> es)
{
    edges = es;
}

void Mesh::setFaces(std::vector<Face*> fs)
{
    faces = fs;
}

void Mesh::addVert(Vertex* v)
{
    verts.push_back(v);
}

void Mesh::addEdge(HalfEdge* e)
{
    edges.push_back(e);
}

void Mesh::addFace(Face* f)
{
    faces.push_back(f);
}

std::vector<Vertex*> Mesh::getVs()
{
    return verts;
}

std::vector<HalfEdge*> Mesh::getEs()
{
    return edges;
}

std::vector<Face*> Mesh::getFs()
{
    return faces;
}

void Mesh::create()
{
    //the collections of vertex positions, vertex normals, and indices, which will be used to create our VBO
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec4> colors;
    std::vector<GLuint> indices;
    std::vector<glm::ivec2> ids;
    std::vector<glm::vec2> weights;

    //We are setting up our VBO data on a per-face basis
    for(unsigned int k = 0; k < faces.size(); k++)
    {
        Face* f = faces[k];

        //the index at which we begin populating the position and normal collections when we get to this face
        //used for indexing purposes
        int startIndex = positions.size();

        //each vertex on the face will have the same color, held in the Face class
        glm::vec3 col = f->getCol();
        glm::vec4 color = glm::vec4(col[0], col[1], col[2], 1);

        //we organize data for the initial edge the face points to
        HalfEdge* start = f->getEdge();
        glm::vec3 currPos = start->getVert()->getPos();
        positions.push_back(glm::vec4(currPos[0], currPos[1], currPos[2], 1));

        //we find the normal of this vertex for this face by taking the cross product of two vectors coming out of this vertex on this face
        Vertex* v1 = start->getSym()->getVert();
        Vertex* v2 = start->getVert();
        Vertex* v3 = start->getNext()->getVert();
        glm::vec3 vec1 = v1->getPos() - v2->getPos();
        glm::vec3 vec2 = v3->getPos() - v2->getPos();
        glm::vec3 cross = glm::cross(vec2, vec1);
        glm::vec4 normal = glm::vec4(cross[0], cross[1], cross[2], 0);
        v2->addNorm(cross); //for OBJ exporting purposes
        normals.push_back(normal);

        colors.push_back(color);

        ids.push_back(glm::vec2(start->getVert()->getJoint1(), start->getVert()->getJoint2()));
        weights.push_back(glm::vec2(start->getVert()->getInfl1(), start->getVert()->getInfl2()));

        //we organize data for all the other edges, using a while loop so we go around until we get back to the start
        HalfEdge* curr = start->getNext();
        while(curr != start)
        {
            currPos = curr->getVert()->getPos();
            positions.push_back(glm::vec4(currPos[0], currPos[1], currPos[2], 1));

            v1 = curr->getSym()->getVert();
            v2 = curr->getVert();
            v3 = curr->getNext()->getVert();
            vec1 = v1->getPos() - v2->getPos();
            vec2 = v3->getPos() - v2->getPos();
            cross = glm::cross(vec2, vec1);
            normal = glm::vec4(cross[0], cross[1], cross[2], 0);
            v2->addNorm(cross);;//for OBJ exporting purposes
            normals.push_back(normal);

            colors.push_back(color);

            ids.push_back(glm::vec2(curr->getVert()->getJoint1(), curr->getVert()->getJoint2()));
            weights.push_back(glm::vec2(curr->getVert()->getInfl1(), curr->getVert()->getInfl2()));

            curr = curr->getNext();
        }

        //number of vertices we added while traversing this face, useful for indexing purposes
        int vertsAdded = positions.size()-startIndex;

        //indexing each of the triangles in the face
        for(int i = 0; i <= vertsAdded - 3; i++){
            indices.push_back(startIndex);
            indices.push_back(startIndex + i + 1);
            indices.push_back(startIndex + i + 2);
        }

    }

    count = indices.size();

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), normals.data(), GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);

    generateID();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufID);
    context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::ivec2), ids.data(), GL_STATIC_DRAW);

    generateWeight();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufWeight);
    context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), weights.data(), GL_STATIC_DRAW);
}
