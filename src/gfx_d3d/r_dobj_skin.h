#pragma once
#include <xanim/dobj.h>
#include <xanim/xanim.h>

struct __declspec(align(2)) GfxModelSurfaceInfo // sizeof=0xC
{                                       // XREF: GfxModelSkinnedSurface/r
    //$428C0DFD0D9970E1A3749A5754AD806F ___u0;
    union //$428C0DFD0D9970E1A3749A5754AD806F // sizeof=0x4
    {                                       // XREF: GfxModelSurfaceInfo/r
        const DObjAnimMat *baseMat;
        const DObjAnimMat *baseMat_local;
    };
    unsigned __int8 boneIndex;
    unsigned __int8 boneCount;
    unsigned __int16 gfxEntIndex;
    unsigned __int16 lightingHandle;
    unsigned __int8 dobjModelIndex;
    // padding byte
};

struct GfxModelSkinnedSurface // sizeof=0x18
{                                       // XREF: GfxModelRigidSurface/r
    int skinnedCachedOffset;
    XSurface *xsurf;
    GfxModelSurfaceInfo info;
    //$BFB86953EFC7B86272823EA22BE25520 ___u3;
    union //$BFB86953EFC7B86272823EA22BE25520 // sizeof=0x4
    {                                       // XREF: GfxModelSkinnedSurface/r
        GfxPackedVertex *skinnedVert;
        int oldSkinnedCachedOffset;
    };
};

struct GfxModelRigidSurface // sizeof=0x38
{
    GfxModelSkinnedSurface surf;
    GfxScaledPlacement placement;
};

void __cdecl R_FlagXModelAsSkinned(GfxSceneEntity *sceneEnt, unsigned int surfaceCount);
void __cdecl R_SkinSceneDObj(
                GfxSceneEntity *sceneEnt,
                GfxSceneEntity *localSceneEnt,
                const DObj *obj,
                DObjAnimMat *boneMatrix,
                int waitForCullState);
unsigned int    R_SkinSceneDObjModels@<eax>(
                int a1@<ebp>,
                GfxSceneEntity *sceneEnt,
                const DObj *obj,
                DObjAnimMat *boneMatrix);
int __cdecl R_AllocSkinnedCachedVerts(int vertCount, bool highPriority);
// local variable allocation has failed, the output may be wrong!
int    R_PreSkinXSurface@<eax>(
                GfxModelRigidSurface *a1@<ebp>,
                const DObj *obj,
                XSurface *surf,
                const GfxModelSurfaceInfo *surfaceInfo,
                unsigned int *numSkinnedVerts,
                GfxModelRigidSurface *surfPos);
void __cdecl QuatMultiplyInverse(const float *in1, const float *in2, float *out);
void __cdecl R_TransformSkelMat(const float *in1, const float4 *in2, float *out);
void __cdecl R_SkinGfxEntityCmd(GfxSceneEntity **data);
