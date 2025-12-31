#pragma once
#include "rb_state.h"
#include "r_rendercmds.h"

enum LightHasShadowMap : __int32
{                                       // XREF: ?R_SetLightProperties@@YAXPAUGfxCmdBufSourceState@@PBUGfxLight@@PBUGfxLightDef@@W4LightHasShadowMap@@M@Z/r
                                        // ?R_SetShadowableLight@@YAXPAUGfxCmdBufSourceState@@IPBUGfxViewInfo@@@Z/r
    LIGHT_HAS_SHADOWMAP    = 0x0,
    LIGHT_HAS_NO_SHADOWMAP = 0x1,
};

// local variable allocation has failed, the output may be wrong!
void    R_SetLightProperties(
                GfxCmdBufSourceState *source,
                const GfxLight *light,
                const GfxLightDef *def,
                LightHasShadowMap hasShadowMap,
                float spotShadowFade);
void __cdecl R_SetShadowableLight(GfxCmdBufSourceState *source, unsigned int shadowableLightIndex);
void __cdecl R_SetDrawSurfsShadowableLight(GfxCmdBufSourceState *source, const GfxDrawSurfListInfo *info);
unsigned int __cdecl R_GetShadowableLightIndex(
                const GfxBackEndData *data,
                const GfxViewInfo *viewInfo,
                const GfxLight *light);
