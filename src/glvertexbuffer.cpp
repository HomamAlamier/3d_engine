#include "glvertexbuffer.h"
#include <memory.h>
#include <GL/glew.h>

NAMESPACE(opengl)

GLVertexBuffer::GLVertexBuffer(GLVertexBufferDrawType dtype)
    : _vao(0)
    , _vbo(0)
    , _ebo(0)
    , _inited(false)
    , _vertexCount(0)
    , _vertexAttribs(0)
    , _drawType(dtype)
{
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void GLVertexBuffer::add(const std::vector<glm::vec2> &v)
{
    Element e;
    e.type = Vec2;
    e.ptr = new std::vector<glm::vec2>(v);
    e.elementCount = 1;
    e.count = v.size();
    _vertexCount = v.size();
    _elements.push_back(e);
}


void GLVertexBuffer::add(const std::vector<glm::vec3> &v)
{
    Element e;
    e.type = Vec3;
    e.ptr = new std::vector<glm::vec3>(v);
    e.elementCount = 1;
    e.count = v.size();
    _vertexCount = v.size();
    _elements.push_back(e);
}

void GLVertexBuffer::add(const std::vector<glm::vec4> &v)
{
    Element e;
    e.type = Vec4;
    e.ptr = new std::vector<glm::vec4>(v);
    e.elementCount = 1;
    e.count = v.size();
    _vertexCount = v.size();
    _elements.push_back(e);
}

void GLVertexBuffer::add(const float *buffer, size_t count, int elementCount)
{
    Element e;
    e.type = Float;
    e.ptr = new float[count];
    memcpy(e.ptr, buffer, sizeof(float) * count);
    e.elementCount = elementCount;
    e.count = count;
    _vertexCount = count / elementCount;
    _elements.push_back(e);
}

void GLVertexBuffer::init()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);

    std::vector<float> v;
    std::vector<int> ve;
    int fullVertexSize = 0;
    for(const Element& e : _elements)
    {
        switch (e.type)
        {
        case Vec2:
        {
            ve.push_back(2);
            fullVertexSize += 2;
            break;
        }
        case Vec3:
        {
            ve.push_back(3);
            fullVertexSize += 3;
            break;
        }
        case Vec4:
        {
            ve.push_back(4);
            fullVertexSize += 4;
            break;
        }
        case Float:
            ve.push_back(e.elementCount);
            fullVertexSize += e.elementCount;
            break;
        }
    }
    for(size_t vi = 0; vi < _vertexCount; ++vi)
    {
        for(const Element& e : _elements)
        {
            switch (e.type)
            {
            case Vec2:
            {
                const glm::vec2& vec = (static_cast<std::vector<glm::vec2>*>(e.ptr))->at(vi);
                v.push_back(vec.x);
                v.push_back(vec.y);
                break;
            }
            case Vec3:
            {
                const glm::vec3& vec = (static_cast<std::vector<glm::vec3>*>(e.ptr))->at(vi);
                v.push_back(vec.x);
                v.push_back(vec.y);
                v.push_back(vec.z);
                break;
            }
            case Vec4:
            {
                const glm::vec4& vec = (static_cast<std::vector<glm::vec4>*>(e.ptr))->at(vi);
                v.push_back(vec.x);
                v.push_back(vec.y);
                v.push_back(vec.z);
                v.push_back(vec.w);
                break;
            }
            case Float:
                for(int i = 0; i < e.elementCount; ++i)
                {
                    v.push_back(static_cast<float*>(e.ptr)[ (vi * e.elementCount) + i ]);
                }
                break;
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), v.data(), GL_STATIC_DRAW);

    int ptr = 0;
    for(int i : ve)
    {
        glVertexAttribPointer(_vertexAttribs, i, GL_FLOAT, GL_FALSE, fullVertexSize * sizeof(float), (void*)(ptr * sizeof(float)));
        glEnableVertexAttribArray(_vertexAttribs);
        ptr += i;
        ++_vertexAttribs;
    }

    for(const Element& e : _elements)
    {
        switch (e.type)
        {
        case Vec2:
            delete (std::vector<glm::vec2>*)e.ptr;
            break;
        case Vec3:
            delete (std::vector<glm::vec3>*)e.ptr;
            break;
        case Vec4:
            delete (std::vector<glm::vec4>*)e.ptr;
            break;
        case Float:
            delete[] (float*)e.ptr;
            break;
        }
    }
    _elements.clear();
    _inited = true;
    glBindVertexArray(0);
}

void GLVertexBuffer::setDrawIndices(const std::vector<uint32_t> &v)
{
    if (!_inited)
        return;
    glBindVertexArray(_vao);
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * v.size(), v.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    _indicesCount = v.size();
}

void GLVertexBuffer::bind()
{
    if (!_inited)
        init();
    glBindVertexArray(_vao);
}

NAMESPACE_END
