#include "drawedge.h"
#include<la.h>
#include <iostream>

DrawHalfEdge::DrawHalfEdge(GLWidget277 *context) : Drawable(context), edge(nullptr)
{}

void DrawHalfEdge::setEdge(HalfEdge* e)
{
    edge = e;
}

HalfEdge* DrawHalfEdge::getEdge()
{
    return edge;
}

GLenum DrawHalfEdge::drawMode()
{
    return GL_LINES;
}

void DrawHalfEdge::create()
{
    //we don't want to create an edge if we haven't clicked one
    if(edge != nullptr)
    {
        //the collections of vertex positions, vertex normals, and indices, which will be used to create our VBO
        std::vector<glm::vec4> positions;
        std::vector<glm::vec4> colors;
        std::vector<GLuint> indices;

        //put in the positions and color (yellow) of the vertex to which the half edge points
        glm::vec3 pos1 = edge->getVert()->getPos();
        positions.push_back(glm::vec4(pos1[0], pos1[1], pos1[2], 1));
        colors.push_back(glm::vec4(1, 1, 0, 1));

        //put in the positions and color (red) of the vertex from which the half edge points
        //we find the vertex by getting the vertex of the symmetric edge
        glm::vec3 pos2 = edge->getSym()->getVert()->getPos();
        positions.push_back(glm::vec4(pos2[0], pos2[1], pos2[2], 1));
        colors.push_back(glm::vec4(1, 0, 0, 1));

        //we populate the index buffer with the two points on the line
        indices.push_back(0);
        indices.push_back(1);

        count = 2;

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

        generateCol();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
        context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
    }
}
