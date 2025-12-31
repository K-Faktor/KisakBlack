#pragma once
#include "r_rendercmds.h"
#include "rb_state.h"

void    R_DepthPrepass(
                unsigned __int8 renderTargetId,
                const GfxViewInfo *viewInfo,
                GfxCmdBuf *cmdBuf);
void __cdecl R_DepthPrepassCallback(const GfxViewInfo *userData, GfxCmdBufContext context);
