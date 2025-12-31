#pragma once
#include "r_rendercmds.h"
#include "rb_state.h"

void    R_DrawSunShadowMap(
                const GfxViewInfo *viewInfo,
                unsigned int partitionIndex,
                GfxCmdBuf *cmdBuf);
void __cdecl R_DrawSunShadowMapCallback(const GfxSunShadowPartition **userData, GfxCmdBufContext context);
