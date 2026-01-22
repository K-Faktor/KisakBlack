#include "r_draw_shadowablelight.h"
#include "r_state_utils.h"
#include "r_foliage.h"
#include "rb_draw3d.h"
#include <cgame_mp/cg_ents_mp.h>
#include "rb_backend.h"

// local variable allocation has failed, the output may be wrong!
void    R_SetLightProperties(
                GfxCmdBufSourceState *source,
                GfxLight *light,
                const GfxLightDef *def,
                LightHasShadowMap hasShadowMap,
                float spotShadowFade)
{
    float v6; // xmm0_4
    float v7; // xmm0_4
    long double v8; // [esp+10h] [ebp-1CCh]
    long double v9; // [esp+10h] [ebp-1CCh]
    float v10; // [esp+14h] [ebp-1C8h]
    float v11; // [esp+18h] [ebp-1C4h]
    float spotDotBias; // [esp+20h] [ebp-1BCh]
    float spotDotScale; // [esp+24h] [ebp-1B8h]
    float v14; // [esp+30h] [ebp-1ACh]
    float v15; // [esp+34h] [ebp-1A8h]
    float v16; // [esp+40h] [ebp-19Ch]
    float v17; // [esp+44h] [ebp-198h]
    float v18; // [esp+50h] [ebp-18Ch]
    float v19; // [esp+54h] [ebp-188h]
    float v20; // [esp+58h] [ebp-184h]
    float v21; // [esp+64h] [ebp-178h]
    float v22; // [esp+68h] [ebp-174h]
    float v23; // [esp+6Ch] [ebp-170h]
    float v24; // [esp+78h] [ebp-164h]
    float v25; // [esp+7Ch] [ebp-160h]
    float v26; // [esp+80h] [ebp-15Ch]
    float v27; // [esp+8Ch] [ebp-150h]
    float v28; // [esp+90h] [ebp-14Ch]
    float v29; // [esp+94h] [ebp-148h]
    float v30; // [esp+A0h] [ebp-13Ch]
    float v31; // [esp+A4h] [ebp-138h]
    float v32; // [esp+A8h] [ebp-134h]
    float v33[4][4]; // [esp+B0h] [ebp-12Ch] OVERLAPPED BYREF
    float v34; // [esp+FCh] [ebp-E0h]
    float v35; // [esp+100h] [ebp-DCh]
    float v36; // [esp+104h] [ebp-D8h]
    float v43; // [esp+120h] [ebp-BCh]
    float v44; // [esp+124h] [ebp-B8h]
    float v45; // [esp+128h] [ebp-B4h]
    float re; // [esp+12Ch] [ebp-B0h]
    float v47; // [esp+130h] [ebp-ACh]
    float bBAdd; // [esp+134h] [ebp-A8h]
    float *bBMul; // [esp+138h] [ebp-A4h]
    float *aAAdd; // [esp+13Ch] [ebp-A0h]
    float aAMul; // [esp+140h] [ebp-9Ch]
    float v52; // [esp+144h] [ebp-98h]
    float v53; // [esp+148h] [ebp-94h]
    float mx21; // [esp+14Ch] [ebp-90h]
    float mx11; // [esp+150h] [ebp-8Ch]
    float mx01; // [esp+154h] [ebp-88h]
    float mx20; // [esp+158h] [ebp-84h]
    float mx10; // [esp+15Ch] [ebp-80h]
    float mx00; // [esp+160h] [ebp-7Ch]
    float sy; // [esp+164h] [ebp-78h]
    float sx; // [esp+168h] [ebp-74h]
    float cy; // [esp+16Ch] [ebp-70h]
    float v63; // [esp+170h] [ebp-6Ch]
    float rc; // [esp+174h] [ebp-68h]
    float w; // [esp+178h] [ebp-64h]
    float rs; // [esp+17Ch] [ebp-60h]
    float y; // [esp+180h] [ebp-5Ch]
    float eAdd; // [esp+184h] [ebp-58h]
    float z; // [esp+188h] [ebp-54h]
    float eMul; // [esp+18Ch] [ebp-50h]
    float x; // [esp+190h] [ebp-4Ch]
    float sAdd; // [esp+194h] [ebp-48h]
    float v73; // [esp+198h] [ebp-44h]
    float sMul; // [esp+19Ch] [ebp-40h]
    float v75; // [esp+1A0h] [ebp-3Ch]
    float farEdge; // [esp+1A4h] [ebp-38h]
    //float *nearEdge; // [esp+1A8h] [ebp-34h]
    //float v81; // [esp+1B8h] [ebp-24h]
    float v82; // [esp+1BCh] [ebp-20h]
    float v83; // [esp+1C0h] [ebp-1Ch]
    float v84; // [esp+1C4h] [ebp-18h]
    //float lightOrigin[5]; // [esp+1C8h] [ebp-14h]
    //float retaddr; // [esp+1DCh] [ebp+0h]
    //
    //lightOrigin[2] = a1;
    //lightOrigin[3] = retaddr;

    iassert(source->viewMode == VIEW_MODE_3D);
    iassert(light);
    iassert(light->type == GFX_LIGHT_TYPE_OMNI || light->type == GFX_LIGHT_TYPE_SPOT);
    iassert(light->radius > 0.0f);

    R_SetCodeImageTexture(source, 0x10u, def->attenuation.image);
    R_SetCodeImageSamplerState(source, 0x10u, 0x14u);
    //LODWORD(lightOrigin[1]) = source->skinnedPlacement.base.origin;
    //LODWORD(lightOrigin[0]) = light->origin;

    v82 = light->origin[0] - source->skinnedPlacement.base.origin[0];
    v83 = light->origin[1] - source->skinnedPlacement.base.origin[1];
    v84 = light->origin[2] - source->skinnedPlacement.base.origin[2];

    source->input.consts[5][0] = light->attenuation[0] + 0.000015287891;
    source->input.consts[5][1] = light->attenuation[1];
    source->input.consts[5][2] = light->attenuation[2];
    source->input.consts[5][3] = light->attenuation[3];
    R_DirtyCodeConstant(source, 5u);

    farEdge = light->falloff[0];
    v75 = light->falloff[1];
    sMul = light->falloff[2];
    v73 = light->falloff[3];

    if ( farEdge == sMul )
        sAdd = 1.0f;
    else
        sAdd = 1.0 / (float)(sMul - farEdge);
    x = sAdd;
    if ( farEdge == sMul )
        v6 = -farEdge;
    else
        v6 = (-(farEdge)) * (float)(1.0 / (float)(sMul - farEdge));
    eMul = v6;
    z = v6;
    if ( v73 == v75 )
        eAdd = -1.0f;
    else
        eAdd = 1.0 / (float)(v73 - v75);
    y = eAdd;
    if ( v73 == v75 )
        v7 = v75;
    else
        v7 = (-(v75)) * (float)(1.0 / (float)(v73 - v75));
    rs = v7;
    w = v7;

    if ( light->type == 3 )
    {
        R_UpdateCodeConstant(source, 6u, x, y, z, w);
    }
    else if ( light->type == 2 )
    {

        float angle =
            (light->cookieControl2[3] * source->materialTime) +
            light->cookieControl2[2];

        float s = sinf(angle);
        float c = cosf(angle);

        //__libm_sse2_sin(v8);
        //__libm_sse2_cos(v9);

        rc = (float)(light->cookieControl2[3] * source->materialTime) + light->cookieControl2[2];
        cy = (float)(light->cookieControl2[3] * source->materialTime) + light->cookieControl2[2];
        v63 = rc;

        sx = cy;
        sy = (light->cookieControl2[0] - c * light->cookieControl2[0])
            + (s * light->cookieControl2[1]);

        mx00 = (light->cookieControl2[1] - s * light->cookieControl2[0])
            - (c * light->cookieControl2[1]);
        mx10 = (float)(light->cookieControl1[2] * source->materialTime) + light->cookieControl1[0];
        mx20 = (float)(light->cookieControl1[3] * source->materialTime) + light->cookieControl1[0];

        mx01 = c * light->cookieControl0[2];
        mx11 = -s * light->cookieControl0[2];
        mx21 = (float)(sy * light->cookieControl0[2]) + mx10;
        v53 = rc * light->cookieControl0[3];
        v52 = cy * light->cookieControl0[3];
        aAMul = (float)(mx00 * light->cookieControl0[3]) + mx20;
        v53 = (float)(mx01 * light->cookieControl0[1]) + v53;
        v52 = (float)(mx11 * light->cookieControl0[1]) + v52;
        aAMul = (float)(mx21 * light->cookieControl0[1]) + aAMul;
        mx01 = (float)(v53 * light->cookieControl0[0]) + mx01;
        mx11 = (float)(v52 * light->cookieControl0[0]) + mx11;
        mx21 = (float)(aAMul * light->cookieControl0[0]) + mx21;
        aAAdd = source->input.consts[4];
        source->input.consts[4][0] = mx21 + 0.5;
        aAAdd[1] = 0.0f;
        aAAdd[2] = aAMul + 0.5;
        aAAdd[3] = spotShadowFade;
        R_DirtyCodeConstant(source, 4u);
        bBMul = source->input.consts[15];
        source->input.consts[15][0] = mx01 * 0.5;
        bBMul[1] = mx11 * 0.5;
        bBMul[2] = v53 * 0.5;
        bBMul[3] = v52 * 0.5;
        R_DirtyCodeConstant(source, 0xFu);
        bBAdd = 1.0 / (float)(light->aAbB[0] - light->aAbB[1]);
        v47 = (-(light->aAbB[1])) * (float)(1.0 / (float)(light->aAbB[0] - light->aAbB[1]));
        re = 1.0 / (float)(light->aAbB[2] - light->aAbB[3]);
        v45 = (-(light->aAbB[3])) * (float)(1.0 / (float)(light->aAbB[2] - light->aAbB[3]));
        R_UpdateCodeConstant(source, 6u, x, re, bBAdd, y);
        R_UpdateCodeConstant(source, 7u, z, v45, v47, w);
        if ( light->attenuation[3] == 0.0 )
            v44 = 1.0f;
        else
            v44 = 2.0 / light->attenuation[3];
        v43 = v44;
        R_UpdateCodeConstant(source, 0xDu, v44, -1.0 / v44, 1.0, light->attenuation[3]);
        R_UpdateCodeConstant(source, 0xEu, light->aAbB[0] * light->aAbB[2], light->aAbB[1] * light->aAbB[3], -2.0, 3.0);

        source->input.consts[12][0] = light->aAbB[0];
        source->input.consts[12][1] = light->aAbB[1];
        source->input.consts[12][2] = light->aAbB[2];
        source->input.consts[12][3] = light->aAbB[3];
        R_DirtyCodeConstant(source, 12);

        v36 = v82;
        v35 = v83;
        v34 = v84;
        light->viewMatrix.m[3][0] = -((float)((float)(v82 * light->viewMatrix.m[0][0]) + (float)(v83 * light->viewMatrix.m[1][0])) + (float)(v84 * light->viewMatrix.m[2][0]));
        light->viewMatrix.m[3][1] = -((float)((float)(v36 * light->viewMatrix.m[0][1]) + (float)(v35 * light->viewMatrix.m[1][1])) + (float)(v34 * light->viewMatrix.m[2][1]));
        light->viewMatrix.m[3][2] = -((float)((float)(v36 * light->viewMatrix.m[0][2]) + (float)(v35 * light->viewMatrix.m[1][2])) + (float)(v34 * light->viewMatrix.m[2][2]));

        MatrixMultiply44(light->viewMatrix.m, light->projMatrix.m, v33);
        v32 = v33[1][0];
        v31 = v33[2][0];
        v30 = v33[3][0];
        source->input.consts[8][0] = v33[0][0];
        source->input.consts[8][1] = v32;
        source->input.consts[8][2] = v31;
        source->input.consts[8][3] = v30;
        R_DirtyCodeConstant(source, 8u);
        v29 = v33[1][1];
        v28 = v33[2][1];
        v27 = v33[3][1];
        source->input.consts[9][0] = v33[0][1];
        source->input.consts[9][1] = v29;
        source->input.consts[9][2] = v28;
        source->input.consts[9][3] = v27;
        R_DirtyCodeConstant(source, 9u);
        v26 = v33[1][2];
        v25 = v33[2][2];
        v24 = v33[3][2];
        source->input.consts[10][0] = v33[0][2];
        source->input.consts[10][1] = v26;
        source->input.consts[10][2] = v25;
        source->input.consts[10][3] = v24;
        R_DirtyCodeConstant(source, 0xAu);
        v23 = v33[1][3];
        v22 = v33[2][3];
        v21 = v33[3][3];
        source->input.consts[11][0] = v33[0][3];
        source->input.consts[11][1] = v23;
        source->input.consts[11][2] = v22;
        source->input.consts[11][3] = v21;
        R_DirtyCodeConstant(source, 0xBu);
    }
    v20 = v83;
    v19 = v84;
    v18 = 1.0 / (float)(light->radius * light->radius);
    source->input.consts[0][0] = v82;
    source->input.consts[0][1] = v20;
    source->input.consts[0][2] = v19;
    source->input.consts[0][3] = v18;
    R_DirtyCodeConstant(source, 0);

    v17 = light->diffuseColor[1];
    v16 = light->diffuseColor[2];
    source->input.consts[1][0] = light->diffuseColor[0];
    source->input.consts[1][1] = v17;
    source->input.consts[1][2] = v16;
    source->input.consts[1][3] = 1.0f;
    R_DirtyCodeConstant(source, 1u);

    v15 = light->specularColor[1];
    v14 = light->specularColor[2];
    source->input.consts[2][0] = light->specularColor[0];
    source->input.consts[2][1] = v15;
    source->input.consts[2][2] = v14;
    source->input.consts[2][3] = 1.0f;
    R_DirtyCodeConstant(source, 2u);

    spotDotScale = light->dir[1];
    spotDotBias = light->dir[2];
    source->input.consts[3][0] = light->dir[0];
    source->input.consts[3][1] = spotDotScale;
    source->input.consts[3][2] = spotDotBias;
    source->input.consts[3][3] = 0.0f;
    R_DirtyCodeConstant(source, 3u);

    if ( light->type == 3 && hasShadowMap == LIGHT_HAS_SHADOWMAP )
    {
        v11 = 1.0 / (float)(light->cosHalfFovInner - light->cosHalfFovOuter);
        v10 = (-(v11)) * light->cosHalfFovOuter;
        source->input.consts[4][0] = v11;
        source->input.consts[4][1] = v10;
        source->input.consts[4][2] = 0.0f;
        source->input.consts[4][3] = spotShadowFade;
        R_DirtyCodeConstant(source, 4u);
    }
}

void __cdecl R_SetShadowableLight(GfxCmdBufSourceState *source, unsigned int shadowableLightIndex)
{
    float spotShadowFade; // [esp+28h] [ebp-20h]
    const GfxLightDef *def; // [esp+2Ch] [ebp-1Ch]
    const GfxBackEndData *data; // [esp+30h] [ebp-18h]
    const GfxSpotShadow *spotShadow; // [esp+3Ch] [ebp-Ch]
    LightHasShadowMap hasShadowMap; // [esp+40h] [ebp-8h]
    int savedregs; // [esp+48h] [ebp+0h] BYREF

    if ( shadowableLightIndex >= 0xFF
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_draw_shadowablelight.cpp",
                    228,
                    0,
                    "shadowableLightIndex doesn't index GFX_MAX_PRIMARY_LIGHTS\n\t%i not in [0, %i)",
                    shadowableLightIndex,
                    255) )
    {
        __debugbreak();
    }
    if ( LODWORD(source[1].matrices.matrix[0].m[2][3]) != shadowableLightIndex )
    {
        LODWORD(source[1].matrices.matrix[0].m[2][3]) = shadowableLightIndex;
        if ( shadowableLightIndex )
        {
            data = source->input.data;
            if ( shadowableLightIndex >= data->shadowableLightCount
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_draw_shadowablelight.cpp",
                            241,
                            0,
                            "shadowableLightIndex doesn't index data->shadowableLightCount\n\t%i not in [0, %i)",
                            shadowableLightIndex,
                            data->shadowableLightCount) )
            {
                __debugbreak();
            }
            if ( data->shadowableLights[shadowableLightIndex].type == 1 )
            {
                if ( source->cameraView != shadowableLightIndex )
                {
                    source->cameraView = shadowableLightIndex;
                    R_SetShadowLookupMatrix(source, &data->sunShadow.lookupMatrix);
                }
            }
            else
            {
                def = data->shadowableLights[shadowableLightIndex].def;
                R_UpdateCodeConstant(
                    source,
                    0x19u,
                    (float)def->attenuation.image->width * 0.001953125,
                    0.0,
                    (float)def->lmapLookupStart * 0.001953125,
                    0.0);
                hasShadowMap = LIGHT_HAS_NO_SHADOWMAP;
                spotShadowFade = 0.0f;
                if ( Com_BitCheckAssert(source->input.data->shadowableLightHasShadowMap, shadowableLightIndex, 32) )
                {
                    if ( data->shadowableLights[shadowableLightIndex].spotShadowIndex >= 4
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_draw_shadowablelight.cpp",
                                    271,
                                    0,
                                    "light->spotShadowIndex doesn't index R_SPOTSHADOW_TILE_COUNT\n\t%i not in [0, %i)",
                                    data->shadowableLights[shadowableLightIndex].spotShadowIndex,
                                    4) )
                    {
                        __debugbreak();
                    }
                    spotShadow = &data->spotShadows[data->shadowableLights[shadowableLightIndex].spotShadowIndex];
                    if ( source->cameraView != shadowableLightIndex )
                    {
                        source->cameraView = shadowableLightIndex;
                        R_SetShadowLookupMatrix(source, &spotShadow->lookupMatrix);
                    }
                    hasShadowMap = LIGHT_HAS_SHADOWMAP;
                    spotShadowFade = spotShadow->fade;
                    R_SetCodeImageTexture(source, 7u, spotShadow->image);
                    R_UpdateCodeConstantFromVec4(source, 0x46u, (float*)spotShadow->pixelAdjust);
                }
                R_SetLightProperties(
                    source,
                    &data->shadowableLights[shadowableLightIndex],
                    def,
                    hasShadowMap,
                    spotShadowFade);
            }
        }
    }
}

void __cdecl R_SetDrawSurfsShadowableLight(GfxCmdBufSourceState *source, const GfxDrawSurfListInfo *info)
{
    unsigned int shadowableLightIndex; // [esp+4h] [ebp-14h]
    const GfxLight *light; // [esp+14h] [ebp-4h]
    int savedregs; // [esp+18h] [ebp+0h] BYREF

    if ( info->light )
    {
        light = info->light;
        shadowableLightIndex = R_GetShadowableLightIndex(source->input.data, info->viewInfo, light);
        if ( shadowableLightIndex )
            R_SetShadowableLight(source, shadowableLightIndex);
        else
            R_SetLightProperties(source, light, light->def, LIGHT_HAS_NO_SHADOWMAP, 0.0);
    }
}

unsigned int __cdecl R_GetShadowableLightIndex(
                const GfxBackEndData *data,
                const GfxViewInfo *viewInfo,
                const GfxLight *light)
{
    int PrimaryLightCount; // eax

    if ( !light
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_draw_shadowablelight.cpp", 29, 0, "%s", "light") )
    {
        __debugbreak();
    }
    if ( !light->canUseShadowMap )
        return 0;
    PrimaryLightCount = Com_GetPrimaryLightCount();
    if ( !Com_BitCheckAssert(data->shadowableLightHasShadowMap, PrimaryLightCount, 32) )
        return 0;
    if ( data->shadowableLightCount != Com_GetPrimaryLightCount() + 1
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_draw_shadowablelight.cpp",
                    39,
                    0,
                    "%s",
                    "data->shadowableLightCount == Com_GetPrimaryLightCount() + GFX_MAX_EMISSIVE_SPOT_LIGHTS") )
    {
        __debugbreak();
    }
    return Com_GetPrimaryLightCount();
}

