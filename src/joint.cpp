#include "joint.h"

Joint::Joint(GLWidget277* context, QString n, Joint* p, glm::vec4 pos, glm::quat rot) :
    Drawable(context), name(n), parent(p), children(), position(pos), rotation(rot), bindMatrix(glm::mat4(1.0f)), selected(false),
    xAngle(0.0f), yAngle(0.0f), zAngle(0.0f)
{
    this->setText(0, name);
    if(parent != nullptr)
    {
        parent->addChild(this);
    }
}

glm::mat4 Joint::getLocalTransformation()
{

    return glm::translate(glm::mat4(1), glm::vec3(position)) * glm::mat4_cast(rotation);
}

glm::mat4 Joint::getOverallTransformation()
{
    if(parent == nullptr)
    {
        return getLocalTransformation();
    }
    else
    {
        return parent->getOverallTransformation()*getLocalTransformation();
    }
}

void Joint::addChild(QTreeWidgetItem *c)
{
    QTreeWidgetItem::addChild(c);
    children.push_back((Joint*)c);
}

QString Joint::getName()
{
    return name;
}

Joint* Joint::getParent()
{
    return parent;
}

std::vector<Joint*> Joint::getChildren()
{
    return children;
}

glm::vec3 Joint::getPos()
{
    return glm::vec3(position);
}

glm::mat4 Joint::getBindMatrix()
{
    return bindMatrix;
}

float Joint::getXAngle()
{
    return xAngle;
}

float Joint::getYAngle()
{
    return yAngle;
}

float Joint::getZAngle()
{
    return zAngle;
}

glm::dualquat Joint::getDualQuat()
{
    return glm::dualquat(rotation, glm::vec3(position));
}

void Joint::setBindMatrix()
{
    bindMatrix = glm::inverse(getOverallTransformation());
}

void Joint::setSelected(bool sel)
{
    selected = sel;
}

void Joint::transX(double d)
{
    position[0] = d;
}

void Joint::transY(double d)
{
    position[1] = d;
}

void Joint::transZ(double d)
{
    position[2] = d;
}

void Joint::rotX(double d)
{
    xAngle = d;
    rotation = glm::rotate(rotation, xAngle, glm::vec3(1, 0, 0));
}

void Joint::rotY(double d)
{
    yAngle = d;
    rotation = glm::rotate(rotation, yAngle, glm::vec3(0, 1, 0));
}

void Joint::rotZ(double d)
{
    zAngle = d;
    rotation = glm::rotate(rotation, zAngle, glm::vec3(0, 0, 1));
}

GLenum Joint::drawMode()
{
    return GL_LINES;
}

void Joint::create()
{
    //the collections of vertex positions, vertex colors, and indices, which will be used to create our VBO
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> colors;
    std::vector<GLuint> indices;

    //put in positions for the three circles that make up our joint's sphere
    for(int i = 0; i < 12; i++)
    {
        positions.push_back(getOverallTransformation()*(glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(1, 0, 0)) * glm::vec4(0.0f, 0.5f, 0.0f, 1.0f)));
    }
    for(int i = 0; i < 12; i++)
    {
        positions.push_back(getOverallTransformation()*(glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, 0.0f, 0.0f, 1.0f)));
    }
    for(int i = 0; i < 12; i++)
    {
        positions.push_back(getOverallTransformation()*(glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 0, 1)) * glm::vec4(0.5f, 0.0f, 0.0f, 1.0f)));
    }

    //put in the color white for these circle vertices
    glm::vec4 color;
    if(selected)
    {
        color = glm::vec4(0, 1, 1, 1);
    }
    else
    {
        color = glm::vec4(1, 1, 1, 1);
    }
    for(int i = 0; i < 36; i++)
    {

        colors.push_back(color);
    }

    //put in indices for these circles
    for(int i = 0; i < 36; i++)
    {
        indices.push_back(i);
        if(i%12 == 11)
        {
            indices.push_back(i-11);
        }
        else
        {
            indices.push_back(i+1);
        }
    }

    if(parent == nullptr)
    {
        count = indices.size();
    }
    else
    {
        //put in positions for the line from the child to the parent
        positions.push_back(getOverallTransformation()*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        positions.push_back(getOverallTransformation()*(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)-glm::vec4(glm::vec3(position), 0)));

        //color the vertices on this line, with yellow on child's end, magenta on parent's end
        colors.push_back(glm::vec4(1, 1, 0, 1));
        colors.push_back(glm::vec4(1, 0, 1, 1));

        //put in indices for the line from child to parent
        indices.push_back(36);
        indices.push_back(37);

        count = indices.size();
    }

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

    for(unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->create();
    }
}
