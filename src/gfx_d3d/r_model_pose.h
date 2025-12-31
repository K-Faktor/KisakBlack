#pragma once
#include <xanim/dobj.h>
#include "r_scene.h"

// local variable allocation has failed, the output may be wrong!
DObjAnimMat * R_UpdateSceneEntBounds(
                GfxSceneEntity *sceneEnt,
                GfxSceneEntity **pLocalSceneEnt,
                const DObj **pObj,
                int waitForCullState);
DObjAnimMat *__cdecl R_DObjCalcPose(const GfxSceneEntity *sceneEnt, DObj *obj, int *partBits);
void __cdecl R_SetNoDraw(GfxSceneEntity *sceneEnt);
void __cdecl R_UpdateGfxEntityBoundsCmd(GfxSceneEntity **data);
