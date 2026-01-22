#include "r_workercmds.h"
#include <universal/com_workercmds.h>
#include "r_dpvs_entity.h"
#include "r_model_skin.h"
#include "r_rendercmds.h"
#include "r_dpvs_static.h"
#include "r_dpvs_dynmodel.h"
#include "r_dpvs_sceneent.h"
#include "r_water.h"
#include "r_water_sim.h"
#include "r_spotshadow.h"
#include "r_workercmds_common.h"
#include <EffectsCore/fx_update.h>
#include <qcommon/threads.h>
#include <EffectsCore/fx_draw.h>


volatile unsigned int g_waterLock;

jqModule fx_update_spotModule;
jqModule fx_updateModule;
jqModule fx_update_remainingModule;
jqModule r_dpvs_staticModule;
jqModule r_dpvs_sceneentModule;
jqModule r_dpvs_dynmodelModule;
jqModule r_dpvs_dynbrushModule;
jqModule r_dpvs_entityModule;
jqModule r_add_sceneentModule;
jqModule r_spot_shadow_entModule;
jqModule dobj_skelModule;
jqModule dobj_skinModule;
jqModule fx_drawModule;
jqModule fx_marks_drawModule;
jqModule r_model_skinModule;
jqModule nuge_physicsModule;
jqModule r_skin_cached_staticmodelModule;

jqModule r_water_simModule;
volatile unsigned int r_water_simLimit;
jqWorkerCmd r_water_simWorkerCmd = { &r_water_simModule, 84u, 0, 0, &r_water_simLimit, NULL, 0u };

// probably decl'd with a macro here, but ya'know

volatile unsigned int fx_update_spotLimit = 1;
jqWorkerCmd fx_update_spotWorkerCmd = { &fx_update_spotModule, 52u, 0, 0, &fx_update_spotLimit, NULL, 0u };

volatile unsigned int fx_updateLimit = 1;
jqWorkerCmd fx_updateWorkerCmd = { &fx_updateModule, 52u, 0, 0, &fx_updateLimit, NULL, 0u };

volatile unsigned int fx_update_remainingLimit = 1;
jqWorkerCmd fx_update_remainingWorkerCmd = { &fx_update_remainingModule, 52u, 0, 0, &fx_update_remainingLimit, NULL, 0u };

volatile unsigned int r_dpvs_staticLimit = 1;
jqWorkerCmd r_dpvs_staticWorkerCmd = { &r_dpvs_staticModule, 12u, 0, 0, &r_dpvs_staticLimit, NULL, 0u };

volatile unsigned int r_dpvs_sceneentLimit = 1;
jqWorkerCmd r_dpvs_sceneentWorkerCmd = { &r_dpvs_sceneentModule, 12u, 0, 0, &r_dpvs_sceneentLimit, NULL, 0u };

volatile unsigned int r_dpvs_dynmodelLimit = 1;
jqWorkerCmd r_dpvs_dynmodelWorkerCmd = { &r_dpvs_dynmodelModule, 12u, 0, 0, &r_dpvs_dynmodelLimit, NULL, 0u };

volatile unsigned int r_dpvs_dynbrushLimit = 1;
jqWorkerCmd r_dpvs_dynbrushWorkerCmd = { &r_dpvs_dynbrushModule, 12u, 0, 0, &r_dpvs_dynbrushLimit, NULL, 0u };

volatile unsigned int r_dpvs_entityLimit = 1;
jqWorkerCmd r_dpvs_entityWorkerCmd = { &r_dpvs_entityModule, 16u, 0, 0, &r_dpvs_entityLimit, NULL, 0u };

volatile unsigned int r_add_sceneentLimit = 1;
jqWorkerCmd r_add_sceneentWorkerCmd = { &r_add_sceneentModule, 4u, 0, 0, &r_add_sceneentLimit, NULL, 0u };

volatile unsigned int r_spot_shadow_entLimit = 1;
jqWorkerCmd r_spot_shadow_entWorkerCmd = { &r_spot_shadow_entModule, 8u, 0, 0, &r_spot_shadow_entLimit, NULL, 0u };

volatile unsigned int dobj_skelLimit = 1;
jqWorkerCmd dobj_skelWorkerCmd = { &dobj_skelModule, 4u, 0, 0, &dobj_skelLimit, NULL, 0u };

volatile unsigned int dobj_skinLimit = 1;
jqWorkerCmd dobj_skinWorkerCmd = { &dobj_skinModule, 4u, 0, 0, &dobj_skinLimit, NULL, 0u }; 

volatile unsigned int fx_drawLimit = 1;
jqWorkerCmd fx_drawWorkerCmd = { &fx_drawModule, 80u, 0, 0, &fx_drawLimit, NULL, 0u };

volatile unsigned int fx_marks_drawLimit = 1;
jqWorkerCmd fx_marks_drawWorkerCmd = { &fx_marks_drawModule, 52u, 0, 0, &fx_marks_drawLimit, NULL, 0u };

volatile unsigned int r_model_skinLimit;
jqWorkerCmd r_model_skinWorkerCmd = { &r_model_skinModule, 48u, 0, 0, &r_model_skinLimit, NULL, 0u };

volatile unsigned int nuge_physicsLimit= 1;
jqWorkerCmd nuge_physicsWorkerCmd = { &nuge_physicsModule, 124u, 0, 0, &nuge_physicsLimit, NULL, 0u };

volatile unsigned int r_skin_cached_staticmodelLimit = 1;
jqWorkerCmd r_skin_cached_staticmodelWorkerCmd =
{
  &r_skin_cached_staticmodelModule,
  4u,
  0,
  0,
  &r_skin_cached_staticmodelLimit,
  NULL,
  0u
};

volatile unsigned int fx_update_remaining_ppuLimit = 1;
jqModule fx_update_remaining_ppuModule;
jqWorkerCmd fx_update_remaining_ppuWorkerCmd =
{
  &fx_update_remaining_ppuModule,
  52u,
  0,
  0,
  &fx_update_remaining_ppuLimit,
  NULL,
  0u
};


volatile unsigned int r_model_lightingLimit = 1;
jqModule r_model_lightingModule;
jqWorkerCmd r_model_lightingWorkerCmd =
{ &r_model_lightingModule, 44u, 0, 0, &r_model_lightingLimit, NULL, 0u };




int __cdecl r_dpvs_entityCallback(jqBatch *batch)
{
    unsigned __int16 *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "r_dpvs_entity");
    data = (unsigned __int16 *)jqLockData(batch);
    R_AddEntitySurfacesInFrustumCmd(data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

int __cdecl r_model_skinCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]
    int savedregs; // [esp+14h] [ebp+0h] BYREF

    data = jqLockData(batch);
    if ( R_DynamicFenceBusy() )
        return 1;
    *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
    //PIXBeginNamedEvent(-1, "r_model_skin");
    R_SkinXModelCmd((SkinXModelCmd *)data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

bool __cdecl R_DynamicFenceBusy()
{
    return R_DynamicFencePending();
}

int __cdecl r_dpvs_staticCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "r_dpvs_static");
    data = jqLockData(batch);
    R_AddCellStaticSurfacesInFrustumCmd((DpvsStaticCellCmd *)data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

int __cdecl r_dpvs_dynmodelCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "r_dpvs_dynmodel");
    data = jqLockData(batch);
    R_AddCellDynModelSurfacesInFrustumCmd((const DpvsPlane **)data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

int __cdecl r_dpvs_sceneentCallback(jqBatch *batch)
{
    GfxWorldDpvsPlanes *data; // [esp+10h] [ebp-4h]
    int savedregs; // [esp+14h] [ebp+0h] BYREF

    //PIXBeginNamedEvent(-1, "r_dpvs_sceneent");
    data = (GfxWorldDpvsPlanes *)jqLockData(batch);
    R_AddCellSceneEntSurfacesInFrustumCmd(data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

int __cdecl r_dpvs_dynbrushCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "r_dpvs_dynbrush");
    data = jqLockData(batch);
    R_AddCellDynBrushSurfacesInFrustumCmd((const DpvsPlane **)data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

int __cdecl r_waterCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    data = jqLockData(batch);
    if ( R_UploadWaterPending() )
        return 1;
    //PIXBeginNamedEvent(-1, "r_water");
    R_UploadWaterTextureInternal((water_t **)data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

int __cdecl R_UploadWaterPending()
{
    if ( !_InterlockedExchangeAdd(&g_waterLock, 1u) )
        return 0;
    R_ExecWaterFailed();
    return 1;
}

signed __int32 R_ExecWaterFailed()
{
    return _InterlockedExchangeAdd(&g_waterLock, 0xFFFFFFFF);
}

int __cdecl r_water_simCallback(jqBatch *batch)
{
    const WaterSimulationCmd *data; // [esp+10h] [ebp-4h]

    data = (const WaterSimulationCmd *)jqLockData(batch);
    if ( R_EndFenceBusy() )
        return 1;
    //PIXBeginNamedEvent(-1, "r_water_sim");
    R_WaterSimulationCmd(data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

bool __cdecl R_EndFenceBusy()
{
    return R_EndFencePending();
}

int __cdecl r_spot_shadow_entCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "r_spot_shadow_ent");
    data = jqLockData(batch);
    R_AddSpotShadowEntCmd((const GfxSpotShadowEntCmd *)data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

int __cdecl fx_updateCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "fx_update");
    data = jqLockData(batch);
    R_ProcessCmd_UpdateFxNonDependent((FxCmd *)data);
    jqUnlockData(batch);
    //if ( GetCurrentThreadId() == g_DXDeviceThread )
        //D3DPERF_EndEvent();
    return 0;
}

int __cdecl fx_update_remainingCallback(jqBatch *batch)
{
    char *Name; // [esp+8h] [ebp-10h]
    FxCmd *data; // [esp+14h] [ebp-4h]

    data = (FxCmd *)jqLockData(batch);
    if ( R_FXUpdateRemainingWait() )
        return 1;
    Name = va("fx_update_remaining cl=%d", data->localClientNum);
    //PIXBeginNamedEvent(-1, Name);
    FX_UpdateRemaining(data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

bool __cdecl R_FXUpdateRemainingWait()
{
    return R_FXSpotLightPending();
}

bool __cdecl R_FXSpotLightPending()
{
    return jqPoll(&fx_update_spotModule.Group);
}

int __cdecl fx_update_remaining_ppuCallback(jqBatch *batch)
{
    char *Name; // [esp+8h] [ebp-10h]
    FxCmd *data; // [esp+14h] [ebp-4h]

    data = (FxCmd *)jqLockData(batch);
    if ( R_FXUpdateRemainingPPUWait() )
        return 1;
    Name = va("fx_update_remaining_ppu cl=%d", data->localClientNum);
    //PIXBeginNamedEvent(-1, Name);
    R_ProcessCmd_UpdateFxRemaining(data);
    jqUnlockData(batch);
    ////if ( GetCurrentThreadId() == g_DXDeviceThread )
    //    //D3DPERF_EndEvent();
    return 0;
}

bool __cdecl R_FXUpdateRemainingPPUWait()
{
    return R_FXDependentPending();
}

bool __cdecl R_FXDependentPending()
{
    return jqPoll(&fx_update_remainingModule.Group);
}

int __cdecl fx_drawCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-8h]

    data = jqLockData(batch);
    if ( *frontEndDataOut->dynamicBufferCurrentFrame
        && *frontEndDataOut->dynamicBufferCurrentFrame < frontEndDataOut->frameCount )
    {
        return 1;
    }
    *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
    //PIXBeginNamedEvent(-1, "fx_draw");
    if ( Sys_QueryD3DDeviceOKEvent() )
        FX_GenerateVerts((FxGenerateVertsCmd *)data);
    jqUnlockData(batch);
    //if ( GetCurrentThreadId() == g_DXDeviceThread )
        //D3DPERF_EndEvent();
    return 0;
}

int __cdecl dobj_skelCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "dobj_skel");
    data = jqLockData(batch);
    R_UpdateGfxEntityBoundsCmd((GfxSceneEntity **)data);
    jqUnlockData(batch);
    //if ( GetCurrentThreadId() == g_DXDeviceThread )
        //D3DPERF_EndEvent();
    return 0;
}

int __cdecl dobj_skinCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "dobj_skin");
    data = jqLockData(batch);
    R_SkinGfxEntityCmd((GfxSceneEntity **)data);
    jqUnlockData(batch);
    //if ( GetCurrentThreadId() == g_DXDeviceThread )
        //D3DPERF_EndEvent();
    return 0;
}

int __cdecl r_model_lightingCallback(jqBatch *batch)
{
    CalcLightingCmd *data; // [esp+8h] [ebp-4h]

    data = (CalcLightingCmd *)jqLockData(batch);
    //PIXBeginNamedEvent(-1, "r_model_lighting");
    R_CalcModelLightingCmd(data);
    //if ( GetCurrentThreadId() == g_DXDeviceThread )
        //D3DPERF_EndEvent();
    jqUnlockData(batch);
    return 0;
}

int __cdecl r_add_sceneentCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+8h] [ebp-4h]

    data = jqLockData(batch);
    //PIXBeginNamedEvent(-1, "r_add_sceneent");
    R_AddAllSceneEntSurfacesCamera((const GfxViewInfo *)*data);
    //if ( GetCurrentThreadId() == g_DXDeviceThread )
        //D3DPERF_EndEvent();
    jqUnlockData(batch);
    return 0;
}

int __cdecl fx_marks_drawCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+10h] [ebp-4h]

    data = jqLockData(batch);
    if ( *frontEndDataOut->dynamicBufferCurrentFrame
        && *frontEndDataOut->dynamicBufferCurrentFrame < frontEndDataOut->frameCount )
    {
        return 1;
    }
    *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
    if ( Sys_QueryD3DDeviceOKEvent() )
    {
        //PIXBeginNamedEvent(-1, "fx_marks_draw");
        FX_GenerateMarkVertsForWorld(data[1], (const GfxLight *)data[9], data[10]);
        //if ( GetCurrentThreadId() == g_DXDeviceThread )
            //D3DPERF_EndEvent();
    }
    jqUnlockData(batch);
    return 0;
}

int __cdecl fx_update_spotCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+8h] [ebp-4h]

    data = jqLockData(batch);
    //PIXBeginNamedEvent(-1, "fx_update_spot");
    R_ProcessCmd_UpdateFxSpotLight((FxCmd *)data);
    //if ( GetCurrentThreadId() == g_DXDeviceThread )
        //D3DPERF_EndEvent();
    jqUnlockData(batch);
    return 0;
}

int __cdecl r_skin_cached_staticmodelCallback(jqBatch *batch)
{
    unsigned int *data; // [esp+0h] [ebp-4h]

    data = jqLockData(batch);
    if ( *frontEndDataOut->dynamicBufferCurrentFrame
        && *frontEndDataOut->dynamicBufferCurrentFrame < frontEndDataOut->frameCount )
    {
        return 1;
    }
    *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
    R_SkinCachedStaticModelCmd((SkinCachedStaticModelCmd *)data);
    jqUnlockData(batch);
    return 0;
}

