#include "r_draw_lit.h"
#include "r_state_utils.h"
#include "r_state.h"
#include "r_wind.h"
#include "rb_draw3d.h"
#include "r_foliage.h"
#include "r_adszscale.h"
#include "rb_fog.h"
#include "r_dvars.h"

#if 0
// local variable allocation has failed, the output may be wrong!
void    R_SetDrawLitConstants(
                GfxCmdBufSourceState *source,
                const GfxViewInfo *viewInfo,
                const GfxViewParms *viewParms)
{
    float v4; // [esp+2Ch] [ebp-B0h]
    float v5; // [esp+30h] [ebp-ACh]
    float v6; // [esp+34h] [ebp-A8h]
    float v7; // [esp+40h] [ebp-9Ch]
    float v8; // [esp+44h] [ebp-98h]
    float v9; // [esp+48h] [ebp-94h]
    int state; // [esp+54h] [ebp-88h]
    float integer; // [esp+58h] [ebp-84h]
    _BYTE x[76]; // [esp+60h] [ebp-7Ch] OVERLAPPED BYREF
    float treeScatterAmount; // [esp+ACh] [ebp-30h]
    float treeScatterIntensity; // [esp+B0h] [ebp-2Ch]
    SunLightParseParams *p_sunParse; // [esp+B4h] [ebp-28h]
    float *v16; // [esp+B8h] [ebp-24h]
    int v17; // [esp+BCh] [ebp-20h]
    SunLightParseParams *sunParse; // [esp+C0h] [ebp-1Ch]
    float v19; // [esp+C4h] [ebp-18h]
    float v20; // [esp+C8h] [ebp-14h]
    const float *CloudArea; // [esp+CCh] [ebp-10h]
    unsigned int v22[2]; // [esp+D0h] [ebp-Ch] BYREF
    _UNKNOWN *retaddr; // [esp+DCh] [ebp+0h]

    v22[0] = a1;
    v22[1] = retaddr;
    R_SetWindShaderConstants(source);
    R_SetSunConstants(&source->input);
    CloudArea = (const float *)R_GetCloudArea();
    v20 = *CloudArea - viewParms->origin[0];
    v19 = CloudArea[1] - viewParms->origin[1];
    sunParse = (SunLightParseParams *)*((unsigned int *)CloudArea + 2);
    v17 = *((unsigned int *)CloudArea + 3);
    v16 = source->input.consts[115];
    source->input.consts[115][0] = v20;
    v16[1] = v19;
    *((unsigned int *)v16 + 2) = sunParse;
    *((unsigned int *)v16 + 3) = v17;
    R_DirtyCodeConstant(source, 0x73u);
    R_SetADSZScaleConstants(viewInfo->localClientNum, source);
    R_SetFrameFog(&source->input, viewInfo);
    p_sunParse = &rgp.world->sunParse;
    treeScatterIntensity = rgp.world->sunParse.treeScatterIntensity;
    treeScatterAmount = rgp.world->sunParse.treeScatterAmount;
    *(unsigned int *)&x[72] = source->input.consts[119];
    source->input.consts[119][0] = treeScatterIntensity;
    *(float *)(*(unsigned int *)&x[72] + 4) = treeScatterAmount;
    *(unsigned int *)(*(unsigned int *)&x[72] + 8) = 0;
    *(unsigned int *)(*(unsigned int *)&x[72] + 12) = 0;
    R_DirtyCodeConstant(source, 0x77u);
    colorTempMatrix((float (*)[4])x, r_skyColorTemp->current.value);
    integer = (float)r_treeScale->current.integer;
    state = r_testScale->current.integer;
    LODWORD(source->input.consts[68][0]) = r_skyTransition->current.integer;
    source->input.consts[68][1] = 0.0f;
    source->input.consts[68][2] = integer;
    LODWORD(source->input.consts[68][3]) = state;
    R_DirtyCodeConstant(source, 0x44u);
    v9 = *(float *)&x[16];
    v8 = *(float *)&x[32];
    v7 = *(float *)&x[48];
    source->input.consts[44][0] = *(float *)x;
    source->input.consts[44][1] = v9;
    source->input.consts[44][2] = v8;
    source->input.consts[44][3] = v7;
    R_DirtyCodeConstant(source, 0x2Cu);
    v6 = *(float *)&x[20];
    v5 = *(float *)&x[36];
    v4 = *(float *)&x[52];
    source->input.consts[45][0] = *(float *)&x[4];
    source->input.consts[45][1] = v6;
    source->input.consts[45][2] = v5;
    source->input.consts[45][3] = v4;
    R_DirtyCodeConstant(source, 0x2Du);
    Vec4Set(source->input.consts[46], *(float *)&x[8], *(float *)&x[24], *(float *)&x[40], *(float *)&x[56]);
    R_DirtyCodeConstant(source, 0x2Eu);
}
#endif

// aislop
void R_SetDrawLitConstants(
    GfxCmdBufSourceState *source,
    const GfxViewInfo *viewInfo,
    const GfxViewParms *viewParms)
{
    // --------------------------------------------------
    // Wind + sun
    // --------------------------------------------------
    R_SetWindShaderConstants(source);
    R_SetSunConstants(&source->input);

    // --------------------------------------------------
    // Cloud area (CONST 0x73)
    // --------------------------------------------------
    const float (*cloudArea)[4] = R_GetCloudArea();

    float cloudX = (*cloudArea)[0] - viewParms->origin[0];
    float cloudY = (*cloudArea)[1] - viewParms->origin[1];
    float cloudZ = (*cloudArea)[2];
    float cloudW = (*cloudArea)[3];

    float *cloudConsts = source->input.consts[0x73];
    cloudConsts[0] = cloudX;
    cloudConsts[1] = cloudY;
    cloudConsts[2] = cloudZ;
    cloudConsts[3] = cloudW;

    R_DirtyCodeConstant(source, 0x73);

    // --------------------------------------------------
    // ADSZ scale
    // --------------------------------------------------
    R_SetADSZScaleConstants(viewInfo->localClientNum, source);

    // --------------------------------------------------
    // Fog
    // --------------------------------------------------
    R_SetFrameFog(&source->input, viewInfo);

    // --------------------------------------------------
    // Tree scatter (CONST 0x77)
    // --------------------------------------------------
    const SunLightParseParams &sunParse = rgp.world->sunParse;

    float *treeConsts = source->input.consts[0x77];
    treeConsts[0] = sunParse.treeScatterIntensity;
    treeConsts[1] = sunParse.treeScatterAmount;
    treeConsts[2] = 0.0f;
    treeConsts[3] = 0.0f;

    R_DirtyCodeConstant(source, 0x77);

    // --------------------------------------------------
    // Sky color temperature matrix
    // --------------------------------------------------
    GfxMatrix skyColorMatrix;
    colorTempMatrix(skyColorMatrix.m, r_skyColorTemp->current.value);

    // --------------------------------------------------
    // Sky transition / tree scale / test scale (CONST 0x44)
    // --------------------------------------------------
    float *skyParams = source->input.consts[0x44];
    skyParams[0] = r_skyTransition->current.value;
    skyParams[1] = 0.0f;
    skyParams[2] = (float)r_treeScale->current.integer;
    skyParams[3] = r_testScale->current.value;

    R_DirtyCodeConstant(source, 0x44);

    // --------------------------------------------------
    // Sky color matrix rows
    // --------------------------------------------------

    // CONST 0x2C (row 1)
    float *row1 = source->input.consts[0x2C];
    row1[0] = skyColorMatrix.m[0][0];
    row1[1] = skyColorMatrix.m[0][1];
    row1[2] = skyColorMatrix.m[0][2];
    row1[3] = skyColorMatrix.m[0][3];
    R_DirtyCodeConstant(source, 0x2C);

    // CONST 0x2D (row 2)
    float *row2 = source->input.consts[0x2D];
    row2[0] = skyColorMatrix.m[1][0];
    row2[1] = skyColorMatrix.m[1][1];
    row2[2] = skyColorMatrix.m[1][2];
    row2[3] = skyColorMatrix.m[1][3];
    R_DirtyCodeConstant(source, 0x2D);

    // CONST 0x2E (row 3)
    Vec4Set(
        source->input.consts[0x2E],
        skyColorMatrix.m[2][0],
        skyColorMatrix.m[2][1],
        skyColorMatrix.m[2][2],
        skyColorMatrix.m[2][3]);

    R_DirtyCodeConstant(source, 0x2E);
}


void R_DrawCloakHDR(const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf, CloakPhaseID phase)
{
    const float *CloudArea; // [esp-1AA4h] [ebp-1AB0h]
    GfxCmdBufSourceState state; // [esp-1AA0h] [ebp-1AACh] BYREF

    //PIXBeginNamedEvent(-1, "R_DrawCloak");
    R_InitCmdBufSourceState(&state, &viewInfo->input, 1);
    R_SetRenderTargetSize(&state, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetViewportStruct(&state, &viewInfo->cullViewInfo.sceneViewport);
    R_SetWindShaderConstants(&state);

    CloudArea = (const float *)R_GetCloudArea();
    state.input.consts[115][0] = *CloudArea - viewInfo->cullViewInfo.viewParms.origin[0];
    state.input.consts[115][1] = CloudArea[1] - viewInfo->cullViewInfo.viewParms.origin[1];
    state.input.consts[115][2] = CloudArea[2];
    state.input.consts[115][3] = CloudArea[3];
    R_DirtyCodeConstant(&state, 115);

    state.input.consts[119][0] = rgp.world->sunParse.treeScatterIntensity;
    state.input.consts[119][1] = rgp.world->sunParse.treeScatterAmount;
    state.input.consts[119][2] = 0.0f;
    state.input.consts[119][3] = 0.0f;
    R_DirtyCodeConstant(&state, 119);

    R_SetADSZScaleConstants(viewInfo->localClientNum, &state);
    if (phase)
    {
        if (phase == CLOAK_PHASE_CLOAKED)
            R_DrawCall(
                (void(__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawCloakPostEmissiveCallbackHDR,
                viewInfo,
                &state,
                viewInfo,
                &viewInfo->drawList[6],
                &viewInfo->cullViewInfo.viewParms,
                cmdBuf,
                0);
    }
    else
    {
        R_DrawCall(
            (void(__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawCloakPrePassCallbackHDR,
            viewInfo,
            &state,
            viewInfo,
            &viewInfo->drawList[6],
            &viewInfo->cullViewInfo.viewParms,
            cmdBuf,
            0);
    }
    //if (g_DXDeviceThread == GetCurrentThreadId())
    //    D3DPERF_EndEvent();
}

void __cdecl R_DrawCloakPrePassCallbackHDR(
                const GfxViewInfo *userData,
                GfxCmdBufContext context,
                GfxCmdBufContext prepassContext)
{
    int v3; // [esp+30h] [ebp-60h]
    jqBatchGroup *i; // [esp+34h] [ebp-5Ch]
    GfxDrawSurfListInfo info; // [esp+38h] [ebp-58h] BYREF
    const GfxViewInfo *viewInfo; // [esp+8Ch] [ebp-4h]

    v3 = 3;
    for ( i = info.group; --v3 >= 0; ++i )
    {
        i->QueuedBatchCount = 0;
        i->ExecutingBatchCount = 0;
    }
    viewInfo = userData;
    R_SetRenderTarget(context, userData->sceneComposition.mainSceneMSAA);
    if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
        R_HW_EnableScissor(
            context.state->prim.device,
            viewInfo->cullViewInfo.scissorViewport.x,
            viewInfo->cullViewInfo.scissorViewport.y,
            viewInfo->cullViewInfo.scissorViewport.width,
            viewInfo->cullViewInfo.scissorViewport.height);
    memcpy(&info, &viewInfo->drawList[6], sizeof(info));
    info.baseTechType = 10;
    R_DrawSurfs(context, prepassContext.state, &viewInfo->drawList[6]);
    if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
        R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_DrawCloakPostEmissiveCallbackHDR(
                const GfxViewInfo *userData,
                GfxCmdBufContext context,
                GfxCmdBufContext prepassContext)
{
    int v3; // [esp+30h] [ebp-60h]
    jqBatchGroup *i; // [esp+34h] [ebp-5Ch]
    GfxDrawSurfListInfo info; // [esp+38h] [ebp-58h] BYREF
    const GfxViewInfo *viewInfo; // [esp+8Ch] [ebp-4h]

    v3 = 3;
    for ( i = info.group; --v3 >= 0; ++i )
    {
        i->QueuedBatchCount = 0;
        i->ExecutingBatchCount = 0;
    }
    viewInfo = userData;
    R_SetRenderTarget(context, userData->sceneComposition.mainSceneMSAA);
    if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
        R_HW_EnableScissor(
            context.state->prim.device,
            viewInfo->cullViewInfo.scissorViewport.x,
            viewInfo->cullViewInfo.scissorViewport.y,
            viewInfo->cullViewInfo.scissorViewport.width,
            viewInfo->cullViewInfo.scissorViewport.height);
    memcpy(&info, &viewInfo->drawList[6], sizeof(info));
    info.baseTechType = 4;
    R_DrawSurfs(context, prepassContext.state, &info);
    if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
        R_HW_DisableScissor(context.state->prim.device);
}

void    R_DrawLit(
                const GfxViewInfo *viewInfo,
                GfxCmdBuf *cmdBuf,
                GfxCmdBuf *prepassCmdBuf,
                LitPhaseID phase)
{
    void *v5; // esp
    GfxCmdBufSourceState v6; // [esp-1AA0h] [ebp-1AACh] BYREF

    R_InitCmdBufSourceState(&v6, &viewInfo->input, 1);
    R_SetRenderTargetSize(&v6, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetViewportStruct(&v6, &viewInfo->cullViewInfo.sceneViewport);
    R_SetDrawLitConstants(&v6, viewInfo, &viewInfo->cullViewInfo.viewParms);
    if ( phase )
    {
        if ( phase == LIT_PHASE_POST_RESOLVE )
        {
            R_DrawCall(
                (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawLitPostResolveCallback,
                viewInfo,
                &v6,
                viewInfo,
                &viewInfo->drawList[1],
                &viewInfo->cullViewInfo.viewParms,
                cmdBuf,
                prepassCmdBuf);
        }
        else if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_draw_lit.cpp", 636, 0, "invalid phase") )
        {
            __debugbreak();
        }
    }
    else
    {
        R_DrawCall(
            (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawLitCallback,
            viewInfo,
            &v6,
            viewInfo,
            viewInfo->drawList,
            &viewInfo->cullViewInfo.viewParms,
            cmdBuf,
            prepassCmdBuf);
    }
}

void __cdecl R_DrawLitCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext)
{
    const GfxViewInfo *viewInfo = (const GfxViewInfo *)userData;

    R_SetRenderTarget(context, viewInfo->sceneComposition.mainSceneMSAA);
    if (prepassContext.state)
        R_SetRenderTarget(prepassContext, viewInfo->sceneComposition.mainSceneMSAA);
    if ((viewInfo->sceneComposition.renderingMode & 7) == 0)
        R_HW_EnableScissor(
            context.state->prim.device,
            viewInfo->cullViewInfo.scissorViewport.x,
            viewInfo->cullViewInfo.scissorViewport.y,
            viewInfo->cullViewInfo.scissorViewport.width,
            viewInfo->cullViewInfo.scissorViewport.height);
    R_DrawSurfs(context, prepassContext.state, viewInfo->drawList);
    if ((viewInfo->sceneComposition.renderingMode & 7) == 0)
        R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_DrawLitPostResolveCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext)
{
    const GfxViewInfo *viewInfo = (const GfxViewInfo *)userData;

    R_SetRenderTarget(context, viewInfo->sceneComposition.mainSceneMSAA);
    if (prepassContext.state)
        R_SetRenderTarget(prepassContext, viewInfo->sceneComposition.mainSceneMSAA);
    if ((viewInfo->sceneComposition.renderingMode & 7) == 0)
        R_HW_EnableScissor(
            context.state->prim.device,
            viewInfo->cullViewInfo.scissorViewport.x,
            viewInfo->cullViewInfo.scissorViewport.y,
            viewInfo->cullViewInfo.scissorViewport.width,
            viewInfo->cullViewInfo.scissorViewport.height);
    R_DrawSurfs(context, prepassContext.state, &viewInfo->drawList[1]);
    if ((viewInfo->sceneComposition.renderingMode & 7) == 0)
        R_HW_DisableScissor(context.state->prim.device);
}

void R_DrawDecal(const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf, GfxCmdBuf *prepassCmdBuf)
{
    const float *CloudArea; // [esp-1AA4h] [ebp-1AB0h]
    GfxCmdBufSourceState v10; // [esp-1AA0h] [ebp-1AACh] BYREF

    //PIXBeginNamedEvent(-1, "R_DrawDecal");
    R_InitCmdBufSourceState(&v10, &viewInfo->input, 1);
    R_SetRenderTargetSize(&v10, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetViewportStruct(&v10, &viewInfo->cullViewInfo.sceneViewport);
    R_SetWindShaderConstants(&v10);

    CloudArea = (const float *)R_GetCloudArea();
    v10.input.consts[115][0] = CloudArea[0] - viewInfo->cullViewInfo.viewParms.origin[0];
    v10.input.consts[115][1] = CloudArea[1] - viewInfo->cullViewInfo.viewParms.origin[1];
    v10.input.consts[115][2] = CloudArea[2];
    v10.input.consts[115][3] = CloudArea[3];
    R_DirtyCodeConstant(&v10, 115);

    v10.input.consts[119][0] = rgp.world->sunParse.treeScatterIntensity;
    v10.input.consts[119][1] = rgp.world->sunParse.treeScatterAmount;
    v10.input.consts[119][2] = 0.0f;
    v10.input.consts[119][3] = 0.0f;
    R_DirtyCodeConstant(&v10, 119);

    R_SetADSZScaleConstants(viewInfo->localClientNum, &v10);
    R_DrawCall(
        (void(__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawDecalCallback,
        viewInfo,
        &v10,
        viewInfo,
        &viewInfo->drawList[3],
        &viewInfo->cullViewInfo.viewParms,
        cmdBuf,
        prepassCmdBuf);
    //if (g_DXDeviceThread == GetCurrentThreadId())
    //    D3DPERF_EndEvent();
}

void __cdecl R_DrawDecalCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext)
{
    const GfxViewInfo *viewInfo = (const GfxViewInfo *)userData;

    R_SetRenderTarget(context, viewInfo->sceneComposition.mainSceneMSAA);
    if (prepassContext.state)
        R_SetRenderTarget(prepassContext, viewInfo->sceneComposition.mainSceneMSAA);
    if ((viewInfo->sceneComposition.renderingMode & 7) == 0)
        R_HW_EnableScissor(
            context.state->prim.device,
            viewInfo->cullViewInfo.scissorViewport.x,
            viewInfo->cullViewInfo.scissorViewport.y,
            viewInfo->cullViewInfo.scissorViewport.width,
            viewInfo->cullViewInfo.scissorViewport.height);
    R_DrawSurfs(context, prepassContext.state, &viewInfo->drawList[3]);
    if ((viewInfo->sceneComposition.renderingMode & 7) == 0)
        R_HW_DisableScissor(context.state->prim.device);
}

