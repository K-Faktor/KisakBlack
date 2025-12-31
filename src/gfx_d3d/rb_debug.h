#pragma once
#include "rb_backend.h"

struct __declspec(align(4)) materialCommands_t // sizeof=0x22A95C
{                                       // XREF: .data:materialCommands_t tess/r
    GfxVertex verts[5450];              // XREF: RB_SetPolyVert(float const * const,GfxColor,int)+44/w
                                        // RB_SetPolyVert(float const * const,GfxColor,int)+52/w ...
    unsigned __int16 indices[1048576];  // XREF: RB_SetTessQuad+75/w
                                        // RB_SetTessQuad+87/w ...
    MaterialVertexDeclType vertDeclType;
    unsigned int vertexSize;
    int indexCount;                     // XREF: RB_CheckTessOverflow(int,int)+78/r
                                        // RB_SetFrameBufferAlpha+20/r ...
    int vertexCount;                    // XREF: RB_CheckTessOverflow(int,int):loc_857149/r
                                        // RB_SetTessQuad+6/r ...
    int firstVertex;                    // XREF: RB_BeginSurface(Material const *,uchar):loc_AC70D1/w
    int lastVertex;                     // XREF: RB_BeginSurface(Material const *,uchar)+10B/w
    bool finishedFilling;               // XREF: RB_EndSurfacePrologue:loc_AC71E2/w
                                        // RB_EndSurfaceEpilogue+6B/w
    // padding byte
    // padding byte
    // padding byte
};

int __cdecl RB_AddDebugLine(
                const float *start,
                const float *end,
                const float *color,
                bool depthTest,
                int vertCount,
                int vertLimit,
                GfxPointVertex *verts);
int __cdecl RB_EndDebugLines(int vertCount, const GfxPointVertex *verts);
void __cdecl RB_SetPolyVert(float *xyz, GfxColor color, int tessVertIndex);
void __cdecl RB_SetDebugBrushesAndPatchesCallback(void (__cdecl *callback)());
void __cdecl RB_DrawDebug(const GfxViewParms *viewParms);
const GfxBackEndData *RB_DrawDebugPolys();
void __cdecl RB_DrawPolyInteriors(PolySet *polySet);
const Material *__cdecl RB_DebugPolyGetMaterialByDepthTest(bool depthTest, bool blend);
void __cdecl RB_DrawPolyOutlines(PolySet *polySet);
void __cdecl RB_DrawDebugSpheres(trDebugSphere_t *spheres, int sphereCount);
void __cdecl RB_DrawDebugSphere(trDebugSphere_t *sphere);
void __cdecl RB_DrawDebugLines(trDebugLine_t *lines, int lineCount, GfxPointVertex *verts);
void __cdecl RB_DrawDebugStrings(trDebugString_t *strings, int stringCount);
void __cdecl RB_AddPlumeStrings(const GfxViewParms *viewParms);
void RB_DrawDebugBrushesAndPatches();
void __cdecl RB_ApplySunLight(const float (*verts)[3], const float *color, float *out_color);
void __cdecl RB_BeginCollisionPolygons(bool faceDepthTest, bool faceBlend);
void __cdecl RB_DrawCollisionPolygon(
                int vertCount,
                const float (*verts)[3],
                const float *faceColor,
                bool faceDepthTest,
                int debug_partition);
materialCommands_t *__cdecl RB_GetCmd();
void __cdecl RB_EndCollisionPolygons();
