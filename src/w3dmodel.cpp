
/**
 * @todo Optimize Model Rendering
 */

#include "w3dmodel.h"
#include <gltexture.h>
#include <glvertexbuffer.h>
#include <glshader.h>
#include <w3d_model.h>
#include <engine_gl.h>

template<typename T, typename ...Args>
void initVector(std::vector<T>& v, size_t s, Args ...args)
{
    for(size_t i = 0; i < s; ++i)
    {
        v.push_back(T(args...));
    }
}


glm::vec3 w3dRGBToVec3(const w3d::W3D_RGB& rgb)
{
    return glm::vec3(rgb.r() / 255.0f, rgb.b() / 255.0f, rgb.b() / 255.0f);
}


NAMESPACE(sage3d)



GLW3DModel::GLW3DModel(const std::string& filename)
{
    w3d::W3D_Model mdl(filename);
    if (!mdl.loaded())
        return;
    for(int i = 0; i < mdl.meshs().size(); ++i)
    {
        const w3d::W3D_Mesh& mesh = mdl.meshs()[i];
        //std::vector<uint32_t> indices;
        std::vector<glm::vec3> verts;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        int passIdx = mesh.materialPass().size() - 1;
        const w3d::W3D_MaterialPass& mp = mesh.materialPass()[passIdx];

        uint32_t triIdx = 0;
        for(const w3d::W3D_Triangle& tri : mesh.triangles())
        {
            for(int i = 0; i < 3; ++i)
            {
                uint32_t d = tri.vIndex()[i];
                verts.push_back(mesh.vertices()[d]);
                normals.push_back(mesh.normals()[d]);
                if (mp.textureStage().size() > 0)
                    texCoords.push_back(mp.textureStage()[0].texCoords()[d]);
                else
                    texCoords.push_back(glm::vec2(0.f));
            }
        }

        std::vector<DrawData> dd;
        if (mp.textureStage().size() > 0)
        {
            for(int tIdx = 0; tIdx < mp.textureStage().size(); ++tIdx)
            {
                const w3d::W3D_TextureStage& tStage = mp.textureStage()[tIdx];
                for(int k = 0; k < verts.size(); k += 3)
                {
                    uint32_t ki = k / 3;

                    uint32_t tid = 0;

                    if (ki >= tStage.textureIDs().size())
                        tid = tStage.textureIDs()[tStage.textureIDs().size() - 1];
                    else
                        tid = tStage.textureIDs()[ki];

                    uint32_t sid = 0;
                    if (ki >= mp.shaderIDs().size())
                        sid = mp.shaderIDs()[mp.shaderIDs().size() - 1];
                    else
                        sid = mp.shaderIDs()[ki];

                    uint32_t mid = 0;
                    if (ki >= mp.vertexMaterialIDs().size())
                        mid = mp.vertexMaterialIDs()[mp.vertexMaterialIDs().size() - 1];
                    else
                        mid = mp.vertexMaterialIDs()[ki];

                    //bool fnd = false;
                    DrawData* ddp = nullptr;
                    for(auto& ddi : dd)
                    {
                        if (ddi.texId == tid && ddi.shaderId == sid && ddi.matId == mid)
                        {
                            ddp = &ddi;
                            break;
                        }
                    }
                    if (!ddp)
                    {
                        DrawData ndd;
                        ndd.texId = tid;
                        ndd.shaderId = sid;
                        ndd.matId = mid;
                        dd.push_back(ndd);
                        ddp = &dd[dd.size() - 1];
                    }
                    for(int vv = 0; vv < 3; ++vv)
                    {
                        ddp->pos.push_back(verts[k + vv]);
                        ddp->normals.push_back(normals[k + vv]);
                        ddp->texCoord.push_back(texCoords[k + vv]);
                    }
                }
            }
        }


        pMesh pmesh;
        pmesh.boneIndex = -1;
        pmesh.name = std::string(mesh.meshHeader().containerName()) + "." + mesh.meshHeader().meshName();
        pmesh.shaders = std::vector<w3d::W3D_Shader>(mesh.shaders());
        pmesh.materials = std::vector<w3d::W3D_Material>(mesh.materials());
        for(const auto& ddi : dd)
        {
            opengl::GLVertexBuffer* vb = new opengl::GLVertexBuffer(opengl::Triangles);
            vb->add(ddi.pos);
            vb->add(ddi.normals);
            vb->add(ddi.texCoord);
            vb->init();
            pmesh.glBuffers.push_back(vb);
            pmesh.texIds.push_back(ddi.texId);
            pmesh.shaderIds.push_back(ddi.shaderId);
            pmesh.materialIds.push_back(ddi.matId);
        }


        for(const auto& tex : mesh.textures())
        {
            std::string n = tex.name();
            n = n.substr(0, n.find_last_of('.')) + ".dds";
            for(int i = 0; i < n.size(); ++i)
            {
                n[i] = std::tolower(n[i]);
            }
            pmesh.textures.push_back(new opengl::GLTexture("textures/" + n));
        }

        if (mdl.hlod().size() > 0 && mdl.hlod()[0].lods().size() > 0)
        {
            std::string n = std::string(mesh.meshHeader().containerName()) + "." + mesh.meshHeader().meshName();
            for(const auto& so : mdl.hlod()[0].lods()[0].subObjects())
            {
                if (std::string(so.name()) == n)
                {
                    pmesh.boneIndex = so.boneIndex();
                    break;
                }
            }
        }
        _meshs.push_back(pmesh);
    }
    prepareBones(&mdl);
    prepareAnimationFrames(&mdl);

    uint32_t pIdx = 0;

    for(const w3d::W3D_Pivot* pv : mdl.hierarchy().pivots())
    {
        Pivot p;
        p.pos = pv->translation();
        p.id = pIdx++;
        p.parent = pv->parentIdx();
        p.rot = glm::mat4_cast(pv->rotation());
        _pivots.push_back(p);
    }
}

float fanAngle = 1.0f;

void GLW3DModel::draw(Engine_GL* gl, opengl::GLShader* shader, const glm::mat4& model)
{

    for(const auto& m : _meshs)
    {
        glm::mat4 mmat = model;
        auto x = m.textures[0]->filename().find("lightbeam");
        if (x < m.textures[0]->filename().size())
        {
            shader->setInt(1, "light_tex");
        }
        else
        {
            shader->setInt(0, "light_tex");
        }

        if (m.boneIndex != -1)
        {
            mmat = model * _framebones[m.boneIndex];
        }
        shader->setMatrix4(mmat, "model");

        for(size_t i = 0; i < m.glBuffers.size(); ++i)
        {
            // bind material
            {
                const w3d::W3D_Material& mat = m.materials[m.materialIds[i]];
                shader->setVec3(w3dRGBToVec3(mat.vertexMaterialInfo().ambient()), "mat_light_info.ambient");
                shader->setVec3(w3dRGBToVec3(mat.vertexMaterialInfo().diffuse()), "mat_light_info.diffuse");
                shader->setVec3(w3dRGBToVec3(mat.vertexMaterialInfo().specular()), "mat_light_info.specular");
                shader->setFloat(mat.vertexMaterialInfo().shininess(), "mat_light_info.shininess");
            }


            m.textures[m.texIds[i]]->bindForSampler(0);
            const w3d::W3D_Shader& s = m.shaders[m.shaderIds[i]];
            shader->setInt(s.texturing(), "texturing_enabled");

            SourceBlendFunc sf = SourceBlendFunc_One;
            DestBlendFunc df = DestBlendFunc_Zero;

            switch (s.srcBlend())
            {
            case w3d::W3DSHADER_SRCBLENDFUNC_ZERO: sf = SourceBlendFunc_Zero; break;
            case w3d::W3DSHADER_SRCBLENDFUNC_ONE: sf = SourceBlendFunc_One; break;
            case w3d::W3DSHADER_SRCBLENDFUNC_SRC_ALPHA: sf = SourceBlendFunc_SrcAlpha; break;
            case w3d::W3DSHADER_SRCBLENDFUNC_ONE_MINUS_SRC_ALPHA: sf = SourceBlendFunc_OneMinusSrcAlpha; break;
            }

            switch (s.destBlend())
            {
            case w3d::W3DSHADER_DESTBLENDFUNC_ZERO: df = DestBlendFunc_Zero; break;
            case w3d::W3DSHADER_DESTBLENDFUNC_ONE: df = DestBlendFunc_One; break;
            case w3d::W3DSHADER_DESTBLENDFUNC_SRC_COLOR: df = DestBlendFunc_SrcColor; break;
            case w3d::W3DSHADER_DESTBLENDFUNC_ONE_MINUS_SRC_COLOR: df = DestBlendFunc_OneMinusSrcColor; break;
            case w3d::W3DSHADER_DESTBLENDFUNC_SRC_ALPHA: df = DestBlendFunc_SrcAlpha; break;
            case w3d::W3DSHADER_DESTBLENDFUNC_ONE_MINUS_SRC_ALPHA: df = DestBlendFunc_OneMinusSrcAlpha; break;
            }


            gl->setEnabled(Blend, sf != SourceBlendFunc_One || df != DestBlendFunc_Zero);
            gl->setBlendFunc(sf, df);

            switch (s.depthCompare())
            {
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_NEVER: gl->setDepthTestFunc(DepthFunc_Never); break;
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_LESS: gl->setDepthTestFunc(DepthFunc_Less); break;
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_EQUAL: gl->setDepthTestFunc(DepthFunc_Equal); break;
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_LEQUAL: gl->setDepthTestFunc(DepthFunc_LessOrEqual); break;
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_GREATER: gl->setDepthTestFunc(DepthFunc_Greater); break;
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_NOTEQUAL: gl->setDepthTestFunc(DepthFunc_NotEqual); break;
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_GEQUAL: gl->setDepthTestFunc(DepthFunc_GreaterOrEqual); break;
            case w3d::W3DSHADER_DEPTHCOMPARE_PASS_ALWAYS: gl->setDepthTestFunc(DepthFunc_Always); break;
            }

            shader->setInt(s.alphaTest(), "alpha_test_enabled");

            gl->setDepthMask(s.depthMask() == w3d::W3DSHADER_DEPTHMASK_WRITE_ENABLE);

            gl->drawBuffer(m.glBuffers[i]);
        }
    }
}

void GLW3DModel::nextFrame()
{
    for(Animation& ani : _anims)
    {
        AnimationInfo& info = ani.info;
        for(int i = 0; i < _framebones.size(); ++i)
        {
            if (info.frameCount[i] > 0 && info.currentFrame[i] >= info.frameCount[i])
            {
                _framebones[i] = _bones[i];
                info.currentFrame[i] = 0;
                continue;
            }

            Pivot* p = &_pivots[i];
            std::vector<glm::mat4> mats;
            /*glm::mat4 smat(1.0f);
            smat = glm::translate(smat, p->pos);
            smat *= p->rot;
            mats.push_back(smat);*/
            while (true)
            {
                glm::mat4 mat(1.0f);
                mat = glm::translate(mat, p->pos);
                mat *= p->rot;
                mat = glm::translate(mat, ani.frames[info.currentFrame[p->id]].translations[p->id]);
                mat *= ani.frames[info.currentFrame[p->id]].rotations[p->id];
                mats.push_back(mat);
                if (p->parent < _pivots.size())
                    p = &_pivots[p->parent];
                else
                    break;
            }
            glm::mat4 cpy(1.0f);// = _framebones[i];
            for(int j = 0; j < mats.size(); ++j)
            {
                cpy = mats[j] * cpy;
            }
            _framebones[i] = cpy;

            if (info.frameCount[i] > 0)
                ++info.currentFrame[i];
        }

    }
}

void GLW3DModel::prepareBones(void* ptr)
{
    w3d::W3D_Model* mdl = (w3d::W3D_Model*)ptr;
    for(int i = 0; i < mdl->hierarchy().pivots().size(); ++i)
    {
        w3d::W3D_Pivot* pivot = mdl->hierarchy().pivots()[i];
        std::vector<glm::mat4> mats;
        while (true)
        {
            glm::mat4 mat(1.0f);
            mat = glm::translate(mat, pivot->translation());
            mat *= glm::mat4_cast(pivot->rotation());
            mats.push_back(mat);
            if (pivot->parentIdx() < mdl->hierarchy().pivots().size())
                pivot = mdl->hierarchy().pivots()[pivot->parentIdx()];
            else
                break;
        }

        glm::mat4 mat(1.0f);
        for(const auto& m : mats)
            mat = m * mat;
        _bones.push_back(mat);
        _framebones.push_back(mat);
    }
}

void GLW3DModel::prepareAnimationFrames(void* ptr)
{
    w3d::W3D_Model* mdl = (w3d::W3D_Model*)ptr;
    for(const w3d::W3D_Animation& ani : mdl->animations())
    {
        Animation a;
        initVector(a.frames, ani.animationHeader().numFrames() - 1);
        initVector(a.info.currentFrame, _bones.size(), 0);
        initVector(a.info.frameCount, _bones.size(), 0);
        uint16_t count = 0;

        for(uint32_t i = 0; i < ani.animationChannelCount(); ++i)
        {
            const w3d::W3D_AnimationChannel& ch = ani.animationChannels()[i];
            a.info.frameCount[ch.animationChannelheader().pivot()] = ch.animationChannelheader().lastFrame()
                    - ch.animationChannelheader().firstFrame();
        }

        for(int fi = 0; fi < ani.animationHeader().numFrames() - 1; ++fi)
        {
            initVector(a.frames[fi].translations, _bones.size(), 0.0f);
            initVector(a.frames[fi].rotations, _bones.size(), 1.0f);
            AnimationFrame& fr = a.frames[fi];
            for(const w3d::W3D_AnimationChannel& ch : ani.animationChannels())
            {
                uint32_t offset = fi * ch.animationChannelheader().vectorLen();
                uint32_t pivotIdx = ch.animationChannelheader().pivot();
                switch (ch.animationChannelheader().flags())
                {
                case w3d::W3DANIM_CHANNEL_X:
                    fr.translations[pivotIdx] += glm::vec3(ch.data()[offset], 0.0f, 0.0f);
                    break;
                case w3d::W3DANIM_CHANNEL_Y:
                    fr.translations[pivotIdx] += glm::vec3(0.0f, ch.data()[offset],0.0f);
                    break;
                case w3d::W3DANIM_CHANNEL_Z:
                    fr.translations[pivotIdx] += glm::vec3(0.0f, 0.0f, ch.data()[offset]);
                    break;
                case w3d::W3DANIM_CHANNEL_Q:
                    glm::quat qu = glm::make_quat(&ch.data()[offset]);
                    glm::mat4 mat = glm::mat4_cast(qu);
                    fr.rotations[pivotIdx] = mat;
                    break;
                }
            }
        }
        _anims.push_back(a);
    }
}

NAMESPACE_END
