#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H
#include <macros.h>
#include <logger.h>

NAMESPACE(opengl)

enum GLVertexBufferDrawType
{
    Lines, Triangles, Quads
};

class GLVertexBuffer
{
public:
    READ_PROP(vao, uint32_t)
    READ_PROP(vbo, uint32_t)
    READ_PROP(ebo, uint32_t)
    READ_PROP(inited, bool)
    READ_PROP(drawType, GLVertexBufferDrawType)
    READ_PROP(vertexCount, size_t)
public:
    GLVertexBuffer(GLVertexBufferDrawType type);
    void init();
    void add(const std::vector<glm::vec2>& v);
    void add(const std::vector<glm::vec3>& v);
    void add(const std::vector<glm::vec4>& v);
    void add(const float* buffer, size_t count, int elementCount);
    void setDrawIndexes(const std::vector<uint32_t>& v);
    void bind();
private:
    enum VecType
    {
        Vec2, Vec3, Vec4, Float
    };
    struct Element
    {
        void* ptr;
        VecType type;
        int elementCount;
        int count;
    };
    int _vertexAttribs;
    std::vector<Element> _elements;
};

NAMESPACE_END

#endif // GLVERTEXBUFFER_H
