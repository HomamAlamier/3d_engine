#ifndef W3DMODEL_H
#define W3DMODEL_H
#include <macros.h>
#include <logger.h>
#include <glm/mat4x4.hpp>

NAMESPACE(w3d)
class W3D_Shader;
class W3D_Material;
NAMESPACE_END

NAMESPACE(opengl)
class GLVertexBuffer;
class GLTexture;
class GLShader;
NAMESPACE_END

NAMESPACE(sage3d)
class Engine_GL;
class GLW3DModel
{
    LOGGER(Sage3D, GLW3DModel)
public:
    GLW3DModel(const std::string& filename);
    void draw(Engine_GL*, opengl::GLShader*, const glm::mat4&);
    void nextFrame();
private:
    void prepareBones(void*);
    void prepareAnimationFrames(void*);
    struct DrawData
    {
        std::vector<glm::vec3> pos;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoord;
        uint32_t texId;
        uint32_t shaderId;
        uint32_t matId;
    };
    struct pMesh
    {
        std::string name;
        std::vector<opengl::GLTexture*> textures;
        std::vector<opengl::GLVertexBuffer*> glBuffers;
        std::vector<int> texIds;
        std::vector<w3d::W3D_Shader> shaders;
        std::vector<uint32_t> shaderIds;
        std::vector<uint32_t> materialIds;
        std::vector<w3d::W3D_Material> materials;
        glm::mat4 model;
        int boneIndex;
    };
    struct AnimationFrame
    {
        std::vector<glm::vec3> translations;
        std::vector<glm::mat4> rotations;
    };
    struct AnimationInfo
    {
        std::vector<uint32_t> frameCount;
        std::vector<uint32_t> currentFrame;
    };
    struct Animation
    {
        AnimationInfo info;
        std::vector<AnimationFrame> frames;
    };
    struct Pivot
    {
        glm::vec3 pos;
        uint32_t id;
        uint32_t parent;
        glm::mat4 rot;
    };


    std::vector<Animation> _anims;
    std::vector<pMesh> _meshs;
    std::vector<glm::mat4> _bones;
    std::vector<glm::mat4> _framebones;
    std::vector<Pivot> _pivots;
};

NAMESPACE_END

#endif // W3DMODEL_H
