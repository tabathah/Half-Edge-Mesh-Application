#include "drawface.h"
#include<la.h>
#include <iostream>

DrawFace::DrawFace(GLWidget277 *context) : Drawable(context), face(nullptr)
{}

void DrawFace::setFace(Face* f)
{
    face = f;
}

Face* DrawFace::getFace()
{
    return face;
}

GLenum DrawFace::drawMode()
{
    return GL_LINES;
}

void DrawFace::create()
{
    //we don't want to create a face if one hasn't been clicked
    if(face != nullptr)
    {
        //the collections of vertex positions, vertex normals, and indices, which will be used to create our VBO
        std::vector<glm::vec4> positions;
        std::vector<glm::vec4> colors;
        std::vector<GLuint> indices;

        //find the opposite color of the face
        glm::vec3 col = face->getCol();
        glm::vec4 color = glm::vec4((5-col[0])/5, (5-col[1])/5, (5-col[2])/5, 1);

        //we populate positions with all the vertices in the face

        //we organize data for the initial edge the face points to
        HalfEdge* start = face->getEdge();
        glm::vec3 currPos = start->getVert()->getPos();
        positions.push_back(glm::vec4(currPos[0], currPos[1], currPos[2], 1));
        colors.push_back(color);

        //we organize data for all the other edges, using a while loop so we go around until we get back to the start
        HalfEdge* curr = start->getNext();
        while(curr != start)
        {
            currPos = curr->getVert()->getPos();
            positions.push_back(glm::vec4(currPos[0], currPos[1], currPos[2], 1));
            colors.push_back(color);
            curr = curr->getNext();
        }

        //we populate the index buffer with the lines representing the edges on the face
        for(unsigned int i = 0; i < positions.size()-1; i++)
        {
            indices.push_back(i);
            indices.push_back(i+1);
        }
        indices.push_back(positions.size()-1);
        indices.push_back(0);

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

        generateCol();
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
        context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
    }
}
