#include "drawvertex.h"
#include<la.h>
#include <iostream>

DrawVertex::DrawVertex(GLWidget277 *context) : Drawable(context), vert(nullptr)
{}

void DrawVertex::setVertex(Vertex* v)
{
    vert = v;
}

Vertex* DrawVertex::getVertex()
{
    return vert;
}

GLenum DrawVertex::drawMode()
{
    return GL_POINTS;
}

void DrawVertex::create()
{
    //we don't want to create a vertex if we haven't clicked one
    if(vert != nullptr)
    {
        //the collections of vertex positions, vertex normals, and indices, which will be used to create our VBO
        std::vector<glm::vec4> positions;
        std::vector<glm::vec4> colors;
        std::vector<GLuint> indices;

        //put in the vertex's position
        glm::vec3 pos = vert->getPos();
        positions.push_back(glm::vec4(pos[0], pos[1], pos[2], 1));

        //we highlight it with a white color
        colors.push_back(glm::vec4(255, 255, 255, 1));

        //we populate the index buffer with the single point
        indices.push_back(0);

        count = 1;

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
        context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
    }
}
