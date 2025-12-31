#pragma once
#include "r_rendercmds.h"
#include "rb_state.h"

void __cdecl R_DrawSpotShadowMapCallback(const GfxSpotShadow **userData, GfxCmdBufContext context);
void __cdecl RB_SpotShadowMaps(const GfxBackEndData *data, const GfxViewInfo *viewInfo);
void    R_DrawSpotShadowMapArray(const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf);
void __cdecl RB_DrawSpotShadowOverlay();
void __cdecl RB_SetSpotShadowOverlayScaleAndBias(const GfxSpotShadow *spotShadow);
