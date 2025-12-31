#pragma once
#include "r_material.h"

enum GfxViewMode : __int32
{                                       // XREF: GfxCmdBufSourceState/r
    VIEW_MODE_NONE     = 0x0,
    VIEW_MODE_3D       = 0x1,
    VIEW_MODE_2D       = 0x2,
    VIEW_MODE_IDENTITY = 0x3,
};

enum GfxViewportBehavior : __int32
{                                       // XREF: GfxCmdBufSourceState/r
    GFX_USE_VIEWPORT_FOR_VIEW = 0x0,    // XREF: .rdata:s_viewportBehaviorForRenderTarget/s
    GFX_USE_VIEWPORT_FULL     = 0x1,    // XREF: .rdata:s_viewportBehaviorForRenderTarget/s
};

struct GfxPrimStats // sizeof=0x18
{                                       // XREF: GfxViewStats/r
    int primCount;                      // XREF: RB_CopyBackendStats(void)+3D/o
                                        // RB_Stats_Summarize+52/w ...
    int triCount;                       // XREF: RB_Stats_Summarize+55/w
    int staticIndexCount;               // XREF: RB_Stats_Summarize+58/w
    int staticVertexCount;              // XREF: RB_Stats_Summarize+5B/w
    int dynamicIndexCount;              // XREF: RB_Stats_Summarize+5E/w
    int dynamicVertexCount;             // XREF: RB_Stats_Summarize+61/w
};

struct GfxViewStats // sizeof=0x138
{                                       // XREF: GfxFrameStats/r
    GfxPrimStats primStats[10];         // XREF: RB_CopyBackendStats(void)+3D/o
    int drawSurfCount;
    int drawMatCount;
    int drawPrimHistogram[16];          // XREF: RB_DrawPrimHistogramOverlay(void)+59/r
                                        // RB_DrawPrimHistogramOverlay(void)+6E/r ...
};

struct GfxFrameStats // sizeof=0x27C
{                                       // XREF: .data:GfxFrameStats g_frameStatsCur/r
                                        // GfxCmdBufPrimState/r ...
    GfxViewStats viewStats[2];          // XREF: RB_CopyBackendStats(void)+3D/o
                                        // RB_DrawPrimHistogramOverlay(void)+59/r ...
    int gfxEntCount;
    int geoIndexCount;                  // XREF: RB_CopyBackendStats(void)+11/r
    int fxIndexCount;                   // XREF: RB_CopyBackendStats(void)+1F/r
};

struct GfxCmdBufPrimState // sizeof=0x2BC
{                                       // XREF: GfxCmdBufState/r
    //$0548B6ED447A4690AAB8EE4D4B1790A5 ___u0;
    union //$0548B6ED447A4690AAB8EE4D4B1790A5 // sizeof=0x4
    {                                       // XREF: R_SetAndClearSceneTarget+6D/r
        IDirect3DDevice9 *device;
        IDirect3DDevice9 *localDevice;
    };
                                        // XREF: R_SetAndClearSceneTarget+6D/r
                                        // R_SetAndClearSceneTarget+7F/r ...
    IDirect3DIndexBuffer9 *indexBuffer; // XREF: RB_CallExecuteRenderCommands(void)+372/r
                                        // RB_SetInitialState(void)+1C5/r
    MaterialVertexDeclType vertDeclType;
                                        // XREF: RB_BeginSurface(Material const *,uchar)+126/w
                                        // RB_DrawTessSurface+C3/r ...
    struct //GfxCmdBufPrimState::<unnamed_type_streams> // sizeof=0xC
    {                                       // XREF: GfxCmdBufPrimState/r
        unsigned int stride;                // XREF: RB_SetInitialState(void):loc_AC7AC9/r
        // RB_SetInitialState(void):loc_AC7B4D/r
        IDirect3DVertexBuffer9 *vb;         // XREF: RB_SetInitialState(void):loc_AC7A71/r
        // RB_SetInitialState(void):loc_AC7AF5/r
        unsigned int offset;                // XREF: RB_SetInitialState(void):loc_AC7A9D/r
        // RB_SetInitialState(void):loc_AC7B21/r
    } streams[3];
    //GfxCmdBufPrimState::<unnamed_type_streams> streams[3];
                                        // XREF: RB_SetInitialState(void):loc_AC7A71/r
                                        // RB_SetInitialState(void):loc_AC7A9D/r ...
    IDirect3DVertexDeclaration9 *vertexDecl;
                                        // XREF: RB_ClearVertexDecl(void)+6/r
                                        // RB_ClearVertexDecl(void):loc_AC6A5E/r
    GfxFrameStats frameStats;
    GfxPrimStats *primStats;            // XREF: RB_ExecuteRenderCommandsLoop(void const *,int *)+2A5/r
                                        // RB_BeginSurface(Material const *,uchar):loc_AC706C/r ...
    GfxPrimStats *backupPrimStats;
    GfxViewStats *viewStats;
};

struct GfxCmdBufState // sizeof=0x13D0
{                                       // XREF: .data:GfxCmdBufState gfxCmdBufState/r
    unsigned __int8 refSamplerState[16];
    unsigned int samplerState[16];
    const GfxTexture *samplerTexture[16];
    GfxCmdBufPrimState prim;            // XREF: R_ShutdownStreams(void):loc_A515AE/o
                                        // R_SetAndClearSceneTarget+6D/r ...
    const Material *material;           // XREF: RB_BeginSurface(Material const *,uchar)+118/w
                                        // RB_BeginSurface(Material const *,uchar)+161/w ...
    unsigned __int8 techType;           // XREF: RB_BeginSurface(Material const *,uchar)+121/w
                                        // RB_BeginSurface(Material const *,uchar)+166/w ...
    // padding byte
    // padding byte
    // padding byte
    const MaterialTechnique *technique; // XREF: RB_BeginSurface(Material const *,uchar)+184/w
                                        // RB_BeginSurface(Material const *,uchar)+189/r ...
    const MaterialPass *pass;
    unsigned int passIndex;
    GfxDepthRangeType depthRangeType;
    float depthRangeNear;
    float depthRangeFar;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    unsigned __int64 vertexShaderConstState[256];
    unsigned __int64 pixelShaderConstState[256];
    unsigned __int8 alphaRef;           // XREF: RB_SetInitialState(void):loc_AC7BFD/r
    // padding byte
    // padding byte
    // padding byte
    unsigned int refStateBits[2];
    unsigned int activeStateBits[2];    // XREF: RB_UpdateBackEndDvarOptions+FC/r
    const MaterialPixelShader *pixelShader;
                                        // XREF: RB_ClearPixelShader(void)+6/r
                                        // RB_ClearPixelShader(void):loc_AC67FF/w
    const MaterialVertexShader *vertexShader;
                                        // XREF: RB_ClearVertexShader(void)+6/r
                                        // RB_ClearVertexShader(void):loc_AC692F/w ...
    unsigned int pixPrimarySortKey;
    const Material *pixMaterial;
    const MaterialTechnique *pixTechnique;
    int pixCombine;
    GfxViewport viewport;
    GfxViewport scissor;
    int scissorEnabled;
    unsigned __int8 renderTargetId;     // XREF: RB_GetResolvedScene+3/r
                                        // RB_GetResolvedScene+11/r
    // padding byte
    // padding byte
    // padding byte
    const Material *origMaterial;       // XREF: RB_SetTessTechnique(Material const *,uchar):loc_AC6F8F/r
                                        // RB_BeginSurface(Material const *,uchar)+133/w ...
    unsigned __int8 origTechType;       // XREF: RB_SetTessTechnique(Material const *,uchar)+3A/r
                                        // RB_BeginSurface(Material const *,uchar)+13C/w ...
    // padding byte
    // padding byte
    // padding byte
    int stateOverride;
};

struct GfxCodeMatrices // sizeof=0x800
{                                       // XREF: GfxCmdBufSourceState/r
    GfxMatrix matrix[32];
};

struct GfxCmdBufSourceState // sizeof=0x1A60
{                                       // XREF: .data:GfxCmdBufSourceState gfxCmdBufSourceState/r
    GfxCodeMatrices matrices;
    //$26AC422158757CD6FC73CEC8E4188A45 ___u1;
    union //$26AC422158757CD6FC73CEC8E4188A45 // sizeof=0xFA0
    {                                       // XREF: RB_EndSceneRendering:loc_A72FBF/o
        GfxCmdBufInput input;
        //$63D949703416AACBAFF1A0E6428E7228 __s1;
        struct
        {
            GfxViewParms viewParms;
        };
    };
                                        // XREF: RB_EndSceneRendering:loc_A72FBF/o
                                        // RB_EndSceneRendering+10C/o ...
    GfxMatrix shadowLookupMatrix;       // XREF: RB_DrawDebugStrings+7F/o
                                        // RB_DrawDebugStrings+D6/o
    unsigned __int16 constVersions[226];
    unsigned __int16 matrixVersions[8];
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    float eyeOffset[4];
    unsigned int shadowableLightForShadowLookupMatrix;
    const GfxScaledPlacement *objectPlacement;
    const GfxViewParms *viewParms3D;
    unsigned int depthHackFlags;
    GfxScaledPlacement skinnedPlacement;
                                        // XREF: RB_SetIdentity(void)+41/w
                                        // RB_SetIdentity(void)+51/w ...
    int cameraView;
    GfxViewMode viewMode;
    GfxSceneDef sceneDef;               // XREF: RB_TessCoronaBillboard+9/r
                                        // RB_TessCoronaBillboard+432/r ...
    GfxViewport sceneViewport;
    GfxViewport scissorViewport;        // XREF: RB_EndSceneRendering+C9/r
                                        // RB_EndSceneRendering+D2/r ...
    float materialTime;                 // XREF: RB_WaterSheetingFX(GfxViewInfo const *)+4C/r
    float destructibleBurnAmount;
    float destructibleFadeAmount;
    float wetness;
    GfxViewportBehavior viewportBehavior;
                                        // XREF: RB_InitSceneViewport(void)+3/w
    int renderTargetWidth;              // XREF: RB_InitSceneViewport(void)+D/w
    int renderTargetHeight;             // XREF: RB_InitSceneViewport(void)+1C/w
    bool viewportIsDirty;               // XREF: RB_InitSceneViewport(void)+27/w
                                        // RB_BlurScreen:loc_B28109/r
    bool scissorEnabled;
    // padding byte
    // padding byte
    unsigned int shadowableLightIndex;
};

struct GfxCmdBuf // sizeof=0x4
{                                       // XREF: RB_FullbrightDrawCommands/r
                                        // RB_DebugShaderDrawCommands/r ...
    IDirect3DDevice9 *device;
};

struct GfxCmdBufContext // sizeof=0x8
{                                       // XREF: .rdata:GfxCmdBufContext const gfxCmdBufContext/r
    //$DE07EA87C257CE593A1DC869AD493ECB ___u0; // XREF: R_ResetDevice+18C/r
    union// $DE07EA87C257CE593A1DC869AD493ECB // sizeof=0x8
    {                                       // XREF: R_ResetDevice+18C/r
        //$3EA432785AEAE4553212959C4AFFD2B6 __s0;
        struct //$3EA432785AEAE4553212959C4AFFD2B6 // sizeof=0x8
        {                                       // XREF: $DE07EA87C257CE593A1DC869AD493ECB/r
            GfxCmdBufSourceState *source;
            GfxCmdBufState *state;
        };
        //GfxCmdBufContext::<unnamed_tag>::<unnamed_type_local> local;
        struct //GfxCmdBufContext//::<unnamed_tag>::<unnamed_type_local> // sizeof=0x8
        {                                       // XREF: $DE07EA87C257CE593A1DC869AD493ECB/r
            GfxCmdBufSourceState *source;
            GfxCmdBufState *state;
        } local;
    };
};

void __cdecl RB_SetInitialState();
void __cdecl RB_UnbindAllImages();
void __cdecl RB_InitSceneViewport();
void __cdecl RB_BindDefaultImages();
void __cdecl RB_InitImages();

extern GfxCmdBufState gfxCmdBufState;