#include "rb_depthprepass.h"
#include "r_dvars.h"
#include "r_state_utils.h"
#include "r_state.h"
#include "r_adszscale.h"
#include "rb_draw3d.h"
#include "rb_backend.h"
#include "r_foliage.h"
#include "r_wind.h"

void    R_DepthPrepass(
                unsigned __int8 renderTargetId,
                const GfxViewInfo *viewInfo,
                GfxCmdBuf *cmdBuf)
{
    GfxCmdBufSourceState state; // [sp+50h] [-F00h] BYREF

    if ( !r_reflectionProbeGenerate->current.enabled )
    {
        R_InitCmdBufSourceState(&state, &viewInfo->input, 1);
        R_SetRenderTargetSize(&state, renderTargetId);
        R_SetViewportStruct(&state, &viewInfo->cullViewInfo.sceneViewport);
        R_SetADSZScaleConstants(viewInfo->localClientNum, &state);
        R_DrawCall(
            R_DepthPrepassCallback,
            viewInfo,
            &state,
            viewInfo,
            (GfxDrawSurfListInfo *)0,
            &viewInfo->cullViewInfo.viewParms,
            cmdBuf,
            0);
    }
}

void __cdecl R_DepthPrepassCallback(const void *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext)
{
    int v2; // [esp+44h] [ebp-64h]
    jqBatchGroup *i; // [esp+48h] [ebp-60h]
    GfxDrawSurfListInfo info; // [esp+50h] [ebp-58h] BYREF
    unsigned __int8 baseTechType; // [esp+A3h] [ebp-5h]
    const GfxViewInfo *viewInfo; // [esp+A4h] [ebp-4h]

    viewInfo = (const GfxViewInfo *)userData;

    if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
        R_HW_EnableScissor(
            context.state->prim.device,
            viewInfo->cullViewInfo.scissorViewport.x,
            viewInfo->cullViewInfo.scissorViewport.y,
            viewInfo->cullViewInfo.scissorViewport.width,
            viewInfo->cullViewInfo.scissorViewport.height);
    if ( viewInfo->renderingFloatZ )
    {
        if ( viewInfo->isMissileCamera )
        {
            R_SetRenderTarget(context, 0x17u);
        }
        else
        {
            if (!gfxRenderTargets[viewInfo->isMissileCamera ? 23 : 7].surface.color
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_depthprepass.cpp",
                    213,
                    0,
                    "%s",
                    "R_HaveFloatZ(viewInfo->isMissileCamera)"))
            {
                __debugbreak();
            }
            R_SetRenderTarget(context, 7u);
        }
        baseTechType = 1;
        R_DrawQuadMesh(context, rgp.shadowClearMaterial, &viewInfo->fullSceneViewMesh->meshData);
        context.source->input.consts[75][0] = 0.0f;
        context.source->input.consts[75][1] = 0.0f;
        context.source->input.consts[75][2] = 0.0f;
        context.source->input.consts[75][3] = 1.0f;
        R_DirtyCodeConstant(context.source, CONST_SRC_CODE_DEPTH_FROM_CLIP);
    }
    else
    {
        R_SetRenderTarget(context, dx.supportsSceneNullRenderTarget + 3);
        baseTechType = 0;
    }
    R_SetWindShaderConstants(context.source);
    memcpy(&info, &viewInfo->drawList[3], sizeof(info));
    info.baseTechType = baseTechType;

    {
        PROF_SCOPED("zprepass decalInfo");
        R_DrawSurfs(context, 0, &info);
    }
    
    memcpy(&info, viewInfo->drawList, sizeof(info));
    info.baseTechType = baseTechType;

    {
        PROF_SCOPED("zprepass litInfo");
        R_DrawSurfs(context, 0, &info);
    }
    
    if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
        R_HW_DisableScissor(context.state->prim.device);
}

