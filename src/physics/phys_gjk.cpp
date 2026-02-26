#include "phys_gjk.h"
#include <bgame/bg_slidemove.h>
#include "phys_assert.h"
#include <tl/tl_system.h>
#include "phys_gjk_sep_dir.h"

const float SEP_CONV_THRESH = 0.001f;
const float CONV_THRESH = 0.001f;
int MAX_CC_RESET_ITERS = 5;


const int BIT_COUNT[16] =
{ 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

phys_assert_info pai_gjk_initial_support_dir = { 0, 128, true };


int    backup1(phys_gjk_info *gjk_info, int new_index, bool seed_simplex)
{
    int v4; // esi
    double v5; // st7
    float *v6; // esi
    double v7; // st5
    float v8; // edx
    double v9; // st6
    float v10; // ecx
    float *v11; // edi
    int v12; // eax
    float *v13; // ecx
    double v14; // st5
    double v15; // st4
    double v16; // rt2
    double v17; // st4
    double v18; // st5
    double v19; // st5
    double v20; // st4
    phys_gjk_info::phys_gjk_set_info *v21; // eax
    double v22; // st7
    phys_gjk_info *v23; // ecx
    double x; // st6
    double y; // st4
    double z; // st5
    double v27; // st3
    double v28; // st3
    double v29; // st2
    double v30; // st6
    float v31; // ecx
    int *v32; // eax
    double v33; // st6
    float *v34; // edi
    double v35; // st6
    double v36; // st5
    double v37; // st4
    double v38; // st3
    double v39; // st2
    double v40; // st4
    double v41; // st7
    double v42; // st5
    double v43; // st4
    int v44; // edi
    int v45; // eax
    phys_gjk_info::phys_gjk_set_info *v46; // eax
    double v47; // st5
    double v48; // rt0
    bool v49; // zf
    double v50; // st3
    double v51; // st2
    double v52; // st7
    int w_inds[3]; // [esp+Ch] [ebp-ECh] BYREF
    phys_vec3 difs[3]; // [esp+18h] [ebp-E0h]
    int v56; // [esp+58h] [ebp-A0h]
    phys_vec3 crw2; // [esp+5Ch] [ebp-9Ch]
    float *v58; // [esp+78h] [ebp-80h]
    float v59; // [esp+7Ch] [ebp-7Ch]
    float v60; // [esp+80h] [ebp-78h]
    float v61; // [esp+84h] [ebp-74h]
    int j; // [esp+98h] [ebp-60h]
    phys_vec3 ww; // [esp+9Ch] [ebp-5Ch]
    phys_vec3 dif; // [esp+ACh] [ebp-4Ch]
    float c1; // [esp+C8h] [ebp-30h]
    int w_count; // [esp+CCh] [ebp-2Ch]
    float new_dist_sq; // [esp+D0h] [ebp-28h]
    int i; // [esp+D4h] [ebp-24h]
    float *p_z; // [esp+D8h] [ebp-20h]
    float c2; // [esp+DCh] [ebp-1Ch]
    float nww_sq; // [esp+E0h] [ebp-18h]
    float c0; // [esp+E4h] [ebp-14h]
    float denom; // [esp+E8h] [ebp-10h]
    float v74; // [esp+ECh] [ebp-Ch] BYREF
    //phys_gjk_info *gjk_infoa; // [esp+F0h] [ebp-8h]
    //phys_gjk_info *seed_simplexa; // [esp+F8h] [ebp+0h]
    //
    //v74 = a1;
    //gjk_infoa = seed_simplexa;
    v4 = 16 * (new_index + 10);
    v5 = *(float *)((char *)&gjk_info->cg2_to_cg1_xform.x.y + v4);
    v6 = (float *)((char *)&gjk_info->cg2_to_cg1_xform.x.x + v4);
    v7 = v6[2];
    LODWORD(v8) = 1 << new_index;
    v9 = *v6 * *v6;
    LODWORD(c2) = 1;
    v56 = 1 << new_index;
    new_dist_sq = v5 * v5 + v9 + v7 * v7;
    gjk_info->m_set_list[0].m_lamda[4 * (1 << new_index) + (1 << new_index) + new_index] = 1.0;
    LODWORD(v10) = ~(1 << new_index) & gjk_info->m_w_set;
    w_count = 0;
    i = 0;
    c0 = v10;
    v11 = (float *)&w_inds[2];
    p_z = &gjk_info->m_w_verts[0].z;
    while (1)
    {
        if ((LODWORD(v10) & LODWORD(c2)) != 0)
        {
            v12 = w_count;
            *((_DWORD *)&v74 + w_count - 59) = i;
            v13 = p_z;
            v59 = *(p_z - 2) - *v6;
            v60 = *(p_z - 1) - v6[1];
            v61 = *p_z - v6[2];
            *(v11 - 2) = v59;
            *(v11 - 1) = v60;
            *v11 = v61;
            nww_sq = *(v11 - 1) * *(v11 - 1) + *(v11 - 2) * *(v11 - 2) + *v11 * *v11;
            v14 = nww_sq;
            *(&v74 + v12 - 62) = nww_sq;
            nww_sq = *(v11 - 1) * v6[1] + *v6 * *(v11 - 2) + *v11 * v6[2];
            v15 = nww_sq;
            *(&difs[2].z + v12) = nww_sq;
            v16 = v15;
            v17 = v14;
            v18 = v16;
            if (v17 > 9.999999439624929e-11)
            {
                nww_sq = -v18 / v17;
                v19 = nww_sq;
                nww_sq = 1.0 - nww_sq;
                if (v19 > 0.0)
                {
                    v20 = nww_sq;
                    if (nww_sq > 0.0)
                    {
                        ww.x = *(v11 - 2) * v19;
                        ww.y = *(v11 - 1) * v19;
                        ww.z = v19 * *v11;
                        dif.x = *v6 + ww.x;
                        dif.y = v6[1] + ww.y;
                        dif.z = ww.z + v6[2];
                        nww_sq = dif.x * dif.x + dif.y * dif.y + dif.z * dif.z;
                        if (new_dist_sq > (double)nww_sq)
                        {
                            new_dist_sq = nww_sq;
                            LODWORD(v8) = v56 | LODWORD(c2);
                            v21 = &gjk_info->m_set_list[v56 | LODWORD(c2)];
                            v21->m_lamda[i] = v19;
                            v21->m_lamda[new_index] = v20;
                            v13 = p_z;
                        }
                    }
                }
            }
            if (seed_simplex
                && (nww_sq = *v13 * *v13 + *(v13 - 2) * *(v13 - 2) + *(v13 - 1) * *(v13 - 1), new_dist_sq > (double)nww_sq))
            {
                v8 = c2;
                new_dist_sq = nww_sq;
                ++w_count;
                gjk_info->m_set_list[0].m_lamda[4 * LODWORD(c2) + LODWORD(c2) + i] = 1.0;
                v13 = p_z;
                v11 += 4;
            }
            else
            {
                ++w_count;
                v11 += 4;
            }
        }
        else
        {
            v13 = p_z;
        }
        p_z = v13 + 4;
        ++i;
        c2 = COERCE_FLOAT(__ROL4__(LODWORD(c2), 1));
        if (i >= 4)
            break;
        v10 = c0;
    }
    v22 = 0.0;
    v23 = gjk_info;
    if (seed_simplex && w_count == 2)
    {
        dif.x = gjk_info->m_w_verts[2].x - gjk_info->m_w_verts[1].x;
        dif.y = gjk_info->m_w_verts[2].y - gjk_info->m_w_verts[1].y;
        dif.z = gjk_info->m_w_verts[2].z - gjk_info->m_w_verts[1].z;
        x = dif.x;
        y = dif.y;
        z = dif.z;
        c0 = dif.z * dif.z + dif.x * dif.x + dif.y * dif.y;
        v27 = c0;
        if (c0 > 9.999999439624929e-11)
        {
            c0 = gjk_info->m_w_verts[1].y * y + gjk_info->m_w_verts[1].x * x + gjk_info->m_w_verts[1].z * z;
            c0 = -c0 / v27;
            v28 = c0;
            c0 = 1.0 - c0;
            if (v28 > 0.0 && c0 > 0.0)
            {
                v29 = x * v28;
                v30 = c0;
                dif.x = v29;
                dif.y = y * v28;
                dif.z = z * v28;
                ww.x = gjk_info->m_w_verts[1].x + dif.x;
                ww.y = gjk_info->m_w_verts[1].y + dif.y;
                ww.z = gjk_info->m_w_verts[1].z + dif.z;
                c0 = ww.x * ww.x + ww.y * ww.y + ww.z * ww.z;
                if (new_dist_sq > (double)c0)
                {
                    new_dist_sq = c0;
                    LODWORD(v8) = 6;
                    gjk_info->m_set_list[6].m_lamda[1] = v30;
                    gjk_info->m_set_list[6].m_lamda[2] = v28;
                }
            }
        }
    }
    if (w_count - 1 > 0)
    {
        LODWORD(v31) = 1;
        v32 = w_inds;
        p_z = 0;
        LODWORD(nww_sq) = 1;
        i = (int)w_inds;
        LODWORD(c0) = w_count - 1;
        do
        {
            j = LODWORD(v31);
            if (SLODWORD(v31) < w_count)
            {
                c2 = *(float *)((char *)&v74 + (_DWORD)p_z - 248);
                v33 = *(float *)((char *)&difs[2].z + (_DWORD)p_z);
                v34 = (float *)(v32 + 6);
                v58 = (float *)(v32 + 6);
                c1 = -v33;
                v35 = c2;
                v36 = c1;
                do
                {
                    c2 = *(float *)v32 * *(v34 - 2) + *(v34 - 1) * *((float *)v32 + 1) + *v34 * *((float *)v32 + 2);
                    c1 = *(&v74 + LODWORD(v31) - 62);
                    denom = -*(&difs[2].z + LODWORD(v31));
                    v37 = c1;
                    v38 = c2;
                    c1 = c1 * v35 - c2 * c2;
                    if (c1 > 0.0000099999997)
                    {
                        v39 = (v37 * v36 - denom * v38) / c1;
                        v40 = denom;
                        denom = v39;
                        v41 = v36;
                        c1 = (v40 * v35 - v38 * v36) / c1;
                        v42 = denom;
                        v43 = c1;
                        c2 = 1.0 - denom - c1;
                        if (denom <= 0.0 || v43 <= 0.0 || c2 <= 0.0)
                        {
                            v47 = 0.0;
                        }
                        else
                        {
                            v59 = *(v34 - 2) * v43;
                            v60 = *(v34 - 1) * v43;
                            v61 = *v34 * v43;
                            dif.x = *(float *)i * v42;
                            dif.y = *(float *)(i + 4) * v42;
                            dif.z = *(float *)(i + 8) * v42;
                            ww.x = *v6 + dif.x;
                            ww.y = v6[1] + dif.y;
                            ww.z = dif.z + v6[2];
                            crw2.x = ww.x + v59;
                            crw2.y = ww.y + v60;
                            crw2.z = ww.z + v61;
                            denom = crw2.y * crw2.y + crw2.x * crw2.x + crw2.z * crw2.z;
                            if (new_dist_sq > (double)denom)
                            {
                                v44 = *((_DWORD *)&v74 + LODWORD(v31) - 59);
                                new_dist_sq = denom;
                                v45 = *(_DWORD *)((char *)&v74 + (_DWORD)p_z - 236);
                                LODWORD(denom) = 1;
                                LODWORD(v8) = v56 | (1 << v45) | (1 << v44);
                                v46 = &gjk_info->m_set_list[LODWORD(v8)];
                                v46->m_lamda[*(_DWORD *)((char *)&v74 + (_DWORD)p_z - 236)] = v42;
                                v46->m_lamda[v44] = v43;
                                v34 = v58;
                                v46->m_lamda[new_index] = c2;
                                v31 = *(float *)&j;
                            }
                            v47 = 0.0;
                        }
                        v48 = v47;
                        v36 = v41;
                        v22 = v48;
                    }
                    v32 = (int *)i;
                    ++LODWORD(v31);
                    v34 += 4;
                    j = LODWORD(v31);
                    v58 = v34;
                } while (SLODWORD(v31) < w_count);
                v31 = nww_sq;
            }
            ++p_z;
            ++LODWORD(v31);
            v32 += 4;
            v49 = LODWORD(c0)-- == 1;
            nww_sq = v31;
            i = (int)v32;
        } while (!v49);
        v23 = gjk_info;
    }
    if (w_count == 3 && v22 >= v23->m_lower_dist_sq)
    {
        difs[2].y = difs[0].w * *(float *)&w_inds[1] - difs[0].z * *(float *)&w_inds[2];
        difs[2].z = *(float *)&w_inds[2] * difs[0].y - difs[0].w * *(float *)w_inds;
        difs[2].w = *(float *)w_inds * difs[0].z - *(float *)&w_inds[1] * difs[0].y;
        v50 = difs[1].z;
        denom = difs[2].z * difs[1].z + difs[2].y * difs[1].y + difs[2].w * difs[1].w;
        v51 = denom;
        denom = fabs(denom);
        if (denom <= 0.0000099999997)
            return LODWORD(v8);
        denom = difs[2].z * v6[1] + *v6 * difs[2].y + difs[2].w * v6[2];
        c0 = -denom / v51;
        if (c0 < 0.0)
            return LODWORD(v8);
        crw2.x = v6[1] * difs[1].w - v50 * v6[2];
        crw2.y = difs[1].y * v6[2] - *v6 * difs[1].w;
        crw2.z = v50 * *v6 - v6[1] * difs[1].y;
        denom = difs[0].w * crw2.z + difs[0].y * crw2.x + difs[0].z * crw2.y;
        denom = denom / v51;
        if (denom < 0.0)
            return LODWORD(v8);
        v52 = denom;
        denom = crw2.z * *(float *)&w_inds[2] + crw2.x * *(float *)w_inds + crw2.y * *(float *)&w_inds[1];
        denom = -denom / v51;
        if (denom >= 0.0)
        {
            denom = 1.0 - v52 - denom - c0;
            if (denom >= 0.0)
                return 15;
        }
    }
    return LODWORD(v8);
}

// local variable allocation has failed, the output may be wrong!
void    phys_full_inv_multiply_mat(
                phys_mat44 *dest,
                const phys_mat44 *left,
                const phys_mat44 *right)
{
    phys_vec3 *v5; // eax
    phys_mat44 v6; // [esp-Ch] [ebp-5Ch] BYREF
    phys_vec3 temp; // [esp+34h] [ebp-1Ch] BYREF

    if (dest == left)
    {
        memcpy(&v6, left, sizeof(v6));
        phys_full_inv_multiply_mat(dest, &v6, right);
    }
    else
    {
        temp.x = left->x.x * right->x.x + left->x.y * right->x.y + left->x.z * right->x.z;
        temp.y = left->y.y * right->x.y + left->y.x * right->x.x + left->y.z * right->x.z;
        temp.z = left->z.y * right->x.y + left->z.x * right->x.x + left->z.z * right->x.z;
        dest->x.x = temp.x;
        dest->x.y = temp.y;
        dest->x.z = temp.z;
        temp.x = left->x.x * right->y.x + right->y.y * left->x.y + right->y.z * left->x.z;
        temp.y = left->y.y * right->y.y + left->y.x * right->y.x + left->y.z * right->y.z;
        temp.z = left->z.y * right->y.y + left->z.x * right->y.x + left->z.z * right->y.z;
        dest->y.x = temp.x;
        dest->y.y = temp.y;
        dest->y.z = temp.z;
        temp.x = right->z.x * left->x.x + left->x.y * right->z.y + right->z.z * left->x.z;
        temp.y = left->y.y * right->z.y + left->y.x * right->z.x + left->y.z * right->z.z;
        temp.z = left->z.y * right->z.y + right->z.x * left->z.x + left->z.z * right->z.z;
        dest->z.x = temp.x;
        dest->z.y = temp.y;
        dest->z.z = temp.z;
        v5 = phys_full_inv_multiply(&temp, left, &right->w);
        dest->w.x = v5->x;
        dest->w.y = v5->y;
        dest->w.z = v5->z;
    }
}

void __cdecl get_simplex(
                phys_gjk_geom *cg1,
                phys_gjk_geom *cg2,
                phys_gjk_cache_info *gjk_ci,
                phys_vec3 *a_verts,
                phys_vec3 *a_inds,
                phys_vec3 *b_verts,
                phys_vec3 *b_inds,
                int *vert_count)
{
    if ( (gjk_ci->m_flags & 8) == 0
        && _tlAssert("source/phys_gjk.cpp", 27, "gjk_ci->is_simplex_valid()", "") )
    {
        __debugbreak();
    }
    cg1->get_simplex(&gjk_ci->m_support_a, gjk_ci->m_support_count, a_verts, a_inds);
    cg2->get_simplex(&gjk_ci->m_support_b, gjk_ci->m_support_count, b_verts, b_inds);
    *vert_count = gjk_ci->m_support_count;
}

void __cdecl set_simplex(
                phys_gjk_geom *cg1,
                phys_gjk_geom *cg2,
                phys_gjk_cache_info *gjk_ci,
                const phys_vec3 *a_normal,
                const phys_vec3 *b_normal,
                const phys_vec3 *a_inds,
                const phys_vec3 *b_inds,
                int w_set)
{
    if ( w_set <= 0 || w_set > 15 )
    {
        if ( _tlAssert("source/phys_gjk.cpp", 36, "w_set > 0 && w_set <= 15", "") )
            __debugbreak();
    }
    gjk_ci->m_flags |= 8u;
    cg1->set_simplex(a_inds, w_set, a_normal, &gjk_ci->m_support_a);
    cg2->set_simplex(b_inds, w_set, b_normal, &gjk_ci->m_support_b);
    gjk_ci->m_support_count = BIT_COUNT[w_set];
}

int __thiscall phys_gjk_info::gjk_subalgorithm(int w_set, int new_index)
{
    int savedregs; // [esp+0h] [ebp+0h] BYREF

    return backup1(this, new_index, 0);
}

int phys_gjk_info::seed_simplex(int cached_vert_count)
{
    int v3; // edi
    int v5; // edx
    int v6; // ecx
    unsigned int v7; // edx
    float *p_y; // eax
    int v9; // ecx
    int v10; // ecx
    int v11; // ecx
    double v12; // st7
    float *v13; // eax
    int v14; // edi
    double v15; // st7
    float v17; // [esp-30h] [ebp-3Ch]
    float v18; // [esp-30h] [ebp-3Ch]
    float v19; // [esp-30h] [ebp-3Ch]
    float v20; // [esp-30h] [ebp-3Ch]
    float v21; // [esp-30h] [ebp-3Ch]
    float v22; // [esp-2Ch] [ebp-38h]
    float v23; // [esp-2Ch] [ebp-38h]
    float v24; // [esp-2Ch] [ebp-38h]
    float v25; // [esp-2Ch] [ebp-38h]
    float v26; // [esp-2Ch] [ebp-38h]
    float v27; // [esp-28h] [ebp-34h]
    float v28; // [esp-28h] [ebp-34h]
    float v29; // [esp-28h] [ebp-34h]
    float v30; // [esp-28h] [ebp-34h]
    float v31; // [esp-28h] [ebp-34h]
    float v32; // [esp-20h] [ebp-2Ch]
    float v33; // [esp-20h] [ebp-2Ch]
    float v34; // [esp-20h] [ebp-2Ch]
    float v35; // [esp-20h] [ebp-2Ch]
    float v36; // [esp-20h] [ebp-2Ch]
    float v37; // [esp-1Ch] [ebp-28h]
    float v38; // [esp-1Ch] [ebp-28h]
    float v39; // [esp-1Ch] [ebp-28h]
    float v40; // [esp-1Ch] [ebp-28h]
    float v41; // [esp-1Ch] [ebp-28h]
    float v42; // [esp-18h] [ebp-24h]
    float v43; // [esp-18h] [ebp-24h]
    float v44; // [esp-18h] [ebp-24h]
    float v45; // [esp-18h] [ebp-24h]
    float v46; // [esp-18h] [ebp-24h]
    int v47; // [esp-4h] [ebp-10h]
    //unsigned int v48[2]; // [esp+0h] [ebp-Ch] BYREF
    //_UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]
    //
    //v48[0] = a2;
    //v48[1] = retaddr;
    v3 = cached_vert_count;
    if ( cached_vert_count <= 0 && _tlAssert("source/phys_gjk.cpp", 381, "cached_vert_count > 0", "") )
        __debugbreak();
    if ( cached_vert_count >= 4 && _tlAssert("source/phys_gjk.cpp", 382, "cached_vert_count < 4", "") )
        __debugbreak();
    v5 = 0;
    this->m_w_set = 0;
    v6 = 1;
    if ( cached_vert_count >= 4 )
    {
        v7 = ((unsigned int)(cached_vert_count - 4) >> 2) + 1;
        p_y = &this->m_b_verts[0].y;
        v47 = 4 * v7;
        do
        {
            v32 = *(p_y - 17) - *(p_y - 1);
            v37 = *(p_y - 16) - *p_y;
            v42 = *(p_y - 15) - p_y[1];
            v17 = v32 - this->m_gjk_origin.x;
            v22 = v37 - this->m_gjk_origin.y;
            v27 = v42 - this->m_gjk_origin.z;
            *(p_y - 33) = v17;
            *(p_y - 32) = v22;
            *(p_y - 31) = v27;
            this->m_w_set |= v6;
            v9 = __ROL4__(v6, 1);
            v33 = *(p_y - 13) - p_y[3];
            v38 = *(p_y - 12) - p_y[4];
            v43 = *(p_y - 11) - p_y[5];
            v18 = v33 - this->m_gjk_origin.x;
            v23 = v38 - this->m_gjk_origin.y;
            v28 = v43 - this->m_gjk_origin.z;
            *(p_y - 29) = v18;
            *(p_y - 28) = v23;
            *(p_y - 27) = v28;
            this->m_w_set |= v9;
            v10 = __ROL4__(v9, 1);
            v34 = *(p_y - 9) - p_y[7];
            v39 = *(p_y - 8) - p_y[8];
            v44 = *(p_y - 7) - p_y[9];
            v19 = v34 - this->m_gjk_origin.x;
            v24 = v39 - this->m_gjk_origin.y;
            v29 = v44 - this->m_gjk_origin.z;
            *(p_y - 25) = v19;
            *(p_y - 24) = v24;
            *(p_y - 23) = v29;
            this->m_w_set |= v10;
            v11 = __ROL4__(v10, 1);
            v35 = *(p_y - 5) - p_y[11];
            v12 = *(p_y - 4);
            p_y += 16;
            v40 = v12 - *(p_y - 4);
            v45 = *(p_y - 19) - *(p_y - 3);
            v20 = v35 - this->m_gjk_origin.x;
            v25 = v40 - this->m_gjk_origin.y;
            v30 = v45 - this->m_gjk_origin.z;
            *(p_y - 37) = v20;
            *(p_y - 36) = v25;
            *(p_y - 35) = v30;
            this->m_w_set |= v11;
            v6 = __ROL4__(v11, 1);
            --v7;
        }
        while ( v7 );
        v3 = cached_vert_count;
        v5 = v47;
    }
    if ( v5 < v3 )
    {
        v13 = &this->m_b_verts[v5].y;
        v14 = v3 - v5;
        do
        {
            v15 = *(v13 - 17);
            v13 += 4;
            v36 = v15 - *(v13 - 5);
            v41 = *(v13 - 20) - *(v13 - 4);
            v46 = *(v13 - 19) - *(v13 - 3);
            v21 = v36 - this->m_gjk_origin.x;
            v26 = v41 - this->m_gjk_origin.y;
            v31 = v46 - this->m_gjk_origin.z;
            *(v13 - 37) = v21;
            *(v13 - 36) = v26;
            *(v13 - 35) = v31;
            this->m_w_set |= v6;
            v6 = __ROL4__(v6, 1);
            --v14;
        }
        while ( v14 );
    }
    return backup1(this, 0, 1);
}

#if 0
phys_gjk_info::gjk_retval_e phys_gjk_info::gjk(
                const phys_gjk_input *d,
                const phys_vec3 *initial_support_dir,
                bool in_separation_loop)
{
    phys_vec3 *p_m_support_dir; // edi
    bool v9; // zf
    double v10; // st7
    phys_gjk_info::gjk_retval_e result; // eax
    int m_w_set; // eax
    const phys_gjk_geom *gjk_cg1; // ecx
    const phys_gjk_geom *gjk_cg2; // ecx
    double v15; // st7
    double v16; // st6
    const phys_vec3 *v17; // eax
    float v18; // ecx
    double z; // st7
    const phys_vec3 *v20; // eax
    double v21; // st6
    double v22; // st5
    double v23; // st7
    double v24; // st4
    double v25; // st3
    int v26; // ecx
    int v27; // edx
    int v28; // ecx
    phys_vec3 *v29; // eax
    int v30; // eax
    phys_vec3 *v33; // [esp-54h] [ebp-94h]
    phys_vec3 v34; // [esp-4Ch] [ebp-8Ch] BYREF
    float v35[4]; // [esp-3Ch] [ebp-7Ch] BYREF
    float v36[4]; // [esp-2Ch] [ebp-6Ch] BYREF
    float v37; // [esp-1Ch] [ebp-5Ch]
    float v38; // [esp-18h] [ebp-58h]
    float v39; // [esp-14h] [ebp-54h]
    float v40; // [esp-Ch] [ebp-4Ch]
    float v41; // [esp-8h] [ebp-48h]
    float v42; // [esp-4h] [ebp-44h]
    phys_vec3 w; // [esp+0h] [ebp-40h]
    float v44; // [esp+14h] [ebp-2Ch]
    float v45; // [esp+18h] [ebp-28h]
    float v46; // [esp+1Ch] [ebp-24h]
    float lower_dist_sq; // [esp+28h] [ebp-18h]
    const phys_vec3 *v48; // [esp+2Ch] [ebp-14h]
    int new_index; // [esp+30h] [ebp-10h]
    //_UNKNOWN *v50[2]; // [esp+34h] [ebp-Ch] BYREF
    //void *in_separation_loopa; // [esp+40h] [ebp+0h]
    //
    //*(float *)v50 = a2;
    //v50[1] = in_separation_loopa;
    this->m_lower_dist_sq = -34.0;
    this->m_upper_dist_sq = 34.0;
    this->m_gjk_iter = phys_gjk_info::init_gjk(d, initial_support_dir, in_separation_loop);
    p_m_support_dir = &this->m_support_dir;
    while (1)
    {
        v9 = this->m_gjk_iter == 0;
        lower_dist_sq = this->m_support_dir.y * this->m_support_dir.y
            + p_m_support_dir->x * p_m_support_dir->x
            + this->m_support_dir.z * this->m_support_dir.z;
        v10 = lower_dist_sq;
        this->m_upper_dist_sq = lower_dist_sq;
        if (!v9 && this->m_gjk_pen_thresh_sq > v10)
            return GJK_PENETRATING;
        m_w_set = this->m_w_set;
        if ((m_w_set & 1) != 0)
        {
            if ((m_w_set & 2) != 0)
                new_index = (this->m_w_set & 4 | 8u) >> 2;
            else
                new_index = 1;
        }
        else
        {
            new_index = 0;
        }
        gjk_cg1 = d->gjk_cg1;
        v36[0] = -p_m_support_dir->x;
        v36[1] = -this->m_support_dir.y;
        v36[2] = -this->m_support_dir.z;
        LODWORD(lower_dist_sq) = &this->m_a_verts[new_index];
        ((void(__thiscall *)(const phys_gjk_geom *, float *, _DWORD, phys_vec3 *, float *, const phys_vec3 *, phys_vec3 *))gjk_cg1->support)(
            gjk_cg1,
            v36,
            LODWORD(lower_dist_sq),
            &this->m_a_inds[new_index],
            a3,
            a4,
            v33);
        gjk_cg2 = d->gjk_cg2;
        v15 = this->cg2_to_cg1_xform.x.y * this->m_support_dir.y + this->m_support_dir.x * this->cg2_to_cg1_xform.x.x;
        v16 = this->cg2_to_cg1_xform.x.z * this->m_support_dir.z;
        v48 = &this->m_b_verts[new_index];
        v33 = &this->m_b_inds[new_index];
        a4 = v48;
        v35[0] = v15 + v16;
        a3 = v35;
        v35[1] = this->cg2_to_cg1_xform.y.y * this->m_support_dir.y
            + this->m_support_dir.x * this->cg2_to_cg1_xform.y.x
            + this->cg2_to_cg1_xform.y.z * this->m_support_dir.z;
        v35[2] = this->cg2_to_cg1_xform.z.y * this->m_support_dir.y
            + this->cg2_to_cg1_xform.z.x * this->m_support_dir.x
            + this->cg2_to_cg1_xform.z.z * this->m_support_dir.z;
        //((void(__thiscall *)(const phys_gjk_geom *))gjk_cg2->support)(gjk_cg2);
        v17 = phys_multiply(&v34, &this->cg2_to_cg1_xform, v48);
        v18 = lower_dist_sq;
        v44 = v17->x + this->cg2_to_cg1_xform.w.x;
        v45 = v17->y + this->cg2_to_cg1_xform.w.y;
        z = v17->z;
        v20 = v48;
        v46 = z + this->cg2_to_cg1_xform.w.z;
        v48->x = v44;
        v20->y = v45;
        v20->z = v46;
        w.y = *(float *)LODWORD(v18) - v20->x;
        w.z = *(float *)(LODWORD(v18) + 4) - v20->y;
        w.w = *(float *)(LODWORD(v18) + 8) - v20->z;
        v40 = w.y - this->m_gjk_origin.x;
        v41 = w.z - this->m_gjk_origin.y;
        v42 = w.w - this->m_gjk_origin.z;
        v21 = v41;
        v22 = v40;
        v23 = v42;
        lower_dist_sq = this->m_support_dir.z * v42 + this->m_support_dir.y * v41 + v40 * this->m_support_dir.x;
        v24 = 0.0;
        if (lower_dist_sq > 0.0)
        {
            if (this->m_upper_dist_sq <= 0.0)
            {
                v24 = 0.0;
            }
            else
            {
                v24 = 0.0;
                lower_dist_sq = lower_dist_sq * lower_dist_sq;
                lower_dist_sq = lower_dist_sq / this->m_upper_dist_sq;
                v25 = lower_dist_sq;
                if (this->m_lower_dist_sq < (double)lower_dist_sq)
                {
                    v9 = (this->m_flags & 1) == 0;
                    this->m_lower_dist_sq = lower_dist_sq;
                    if (!v9)
                    {
                        lower_dist_sq = this->m_gjk_sep_thresh;
                        lower_dist_sq = lower_dist_sq * lower_dist_sq;
                        if (lower_dist_sq < v25)
                            return GJK_SEPARATED;
                    }
                }
            }
        }
        if (this->m_gjk_iter && v24 < this->m_lower_dist_sq)
            break;
    LABEL_24:
        v28 = new_index;
        v29 = &this->m_w_verts[new_index];
        v29->x = v22;
        v29->y = v21;
        v29->z = v23;
        this->m_w_set |= 1 << v28;
        this->m_last_w_set = this->m_w_set;
        v30 = backup1(COERCE_FLOAT(v50), this, v28, 0);
        this->m_w_set = v30;
        if (v30 == 15)
            return GJK_PENETRATING;
        phys_gjk_info::comp_v(this, v50, v30, &this->m_support_dir);
        result = GJK_VALID;
        if (++this->m_gjk_iter >= 30)
            return result;
    }
    lower_dist_sq = 1.0 - CONV_THRESH;
    lower_dist_sq = lower_dist_sq * lower_dist_sq;
    if (this->m_lower_dist_sq > lower_dist_sq * this->m_upper_dist_sq)
        return GJK_VALID;
    v26 = 0;
    v27 = 1;
    LODWORD(lower_dist_sq) = &this->m_w_verts[0].z;
    while (1)
    {
        if ((v27 & this->m_last_w_set) != 0)
        {
            v37 = v22 - *(float *)(LODWORD(lower_dist_sq) - 8);
            v38 = v21 - *(float *)(LODWORD(lower_dist_sq) - 4);
            v39 = v23 - *(float *)LODWORD(lower_dist_sq);
            *(float *)&v48 = v38 * v38 + v37 * v37 + v39 * v39;
            if (*(float *)&v48 < 0.0000010000001)
                return GJK_VALID;
        }
        LODWORD(lower_dist_sq) += 16;
        ++v26;
        v27 *= 2;
        if (v26 >= 4)
            goto LABEL_24;
    }
}
#else // aislop
phys_gjk_info::gjk_retval_e phys_gjk_info::gjk(
    const phys_gjk_input *d,
    const phys_vec3 *initial_support_dir,
    bool in_separation_loop)
{
    this->m_lower_dist_sq = -34.0f;
    this->m_upper_dist_sq = 34.0f;

    this->m_gjk_iter =
        phys_gjk_info::init_gjk(d, initial_support_dir, in_separation_loop);

    int new_index;

    while (true)
    {
        const bool first_iter = (this->m_gjk_iter == 0);

        // ||support_dir||^2
        float support_len_sq =
            this->m_support_dir.x * this->m_support_dir.x +
            this->m_support_dir.y * this->m_support_dir.y +
            this->m_support_dir.z * this->m_support_dir.z;

        this->m_upper_dist_sq = support_len_sq;

        if (!first_iter && this->m_gjk_pen_thresh_sq > support_len_sq)
            return GJK_PENETRATING;

        // ------------------------------------------------------------
        // Select new simplex index
        // ------------------------------------------------------------

        if (this->m_w_set & 1)
        {
            if (this->m_w_set & 2)
                new_index = (this->m_w_set & 4) ? 3 : 2;
            else
                new_index = 1;
        }
        else
        {
            new_index = 0;
        }

        // ------------------------------------------------------------
        // Support A (direction = -support_dir)
        // ------------------------------------------------------------

        phys_vec3 negDir;
        negDir.x = -this->m_support_dir.x;
        negDir.y = -this->m_support_dir.y;
        negDir.z = -this->m_support_dir.z;

        d->gjk_cg1->support(
            &negDir,
            &this->m_a_verts[new_index],
            &this->m_a_inds[new_index]);

        // ------------------------------------------------------------
        // Support B (direction transformed into B space)
        // ------------------------------------------------------------

        phys_vec3 dirB;

        dirB.x =
            this->cg2_to_cg1_xform.x.x * this->m_support_dir.x +
            this->cg2_to_cg1_xform.x.y * this->m_support_dir.y +
            this->cg2_to_cg1_xform.x.z * this->m_support_dir.z;

        dirB.y =
            this->cg2_to_cg1_xform.y.x * this->m_support_dir.x +
            this->cg2_to_cg1_xform.y.y * this->m_support_dir.y +
            this->cg2_to_cg1_xform.y.z * this->m_support_dir.z;

        dirB.z =
            this->cg2_to_cg1_xform.z.x * this->m_support_dir.x +
            this->cg2_to_cg1_xform.z.y * this->m_support_dir.y +
            this->cg2_to_cg1_xform.z.z * this->m_support_dir.z;

        d->gjk_cg2->support(
            &dirB,
            &this->m_b_verts[new_index],
            &this->m_b_inds[new_index]);

        // ------------------------------------------------------------
        // Transform B vertex into A space
        // ------------------------------------------------------------

        phys_vec3 bTransformed;
        phys_multiply(&bTransformed,
            &this->cg2_to_cg1_xform,
            &this->m_b_verts[new_index]);

        bTransformed.x += this->cg2_to_cg1_xform.w.x;
        bTransformed.y += this->cg2_to_cg1_xform.w.y;
        bTransformed.z += this->cg2_to_cg1_xform.w.z;

        this->m_b_verts[new_index] = bTransformed;

        // ------------------------------------------------------------
        // Minkowski difference point
        // ------------------------------------------------------------

        phys_vec3 w;
        w.x = this->m_a_verts[new_index].x - bTransformed.x;
        w.y = this->m_a_verts[new_index].y - bTransformed.y;
        w.z = this->m_a_verts[new_index].z - bTransformed.z;

        float vx = w.x - this->m_gjk_origin.x;
        float vy = w.y - this->m_gjk_origin.y;
        float vz = w.z - this->m_gjk_origin.z;

        float dot = this->m_support_dir.x * vx +
            this->m_support_dir.y * vy +
            this->m_support_dir.z * vz;

        if (dot > 0.0f)
        {
            if (this->m_upper_dist_sq > 0.0f)
            {
                float new_lower =
                    (dot * dot) / this->m_upper_dist_sq;

                if (this->m_lower_dist_sq < new_lower)
                {
                    this->m_lower_dist_sq = new_lower;

                    if (this->m_flags & 1)
                    {
                        float sep_sq =
                            this->m_gjk_sep_thresh *
                            this->m_gjk_sep_thresh;

                        if (sep_sq < new_lower)
                            return GJK_SEPARATED;
                    }
                }
            }
        }

        if (this->m_gjk_iter &&
            dot < this->m_lower_dist_sq)
            break;

        // ------------------------------------------------------------
        // Add to simplex
        // ------------------------------------------------------------

        this->m_w_verts[new_index] = w;

        this->m_w_set |= (1 << new_index);
        this->m_last_w_set = this->m_w_set;

        int new_mask = backup1(this, new_index, 0);
        this->m_w_set = new_mask;

        if (new_mask == 15)
            return GJK_PENETRATING;

        phys_gjk_info::comp_v(
            new_mask,
            &this->m_support_dir);

        if (++this->m_gjk_iter >= 30)
            return GJK_VALID;
    }

    // ------------------------------------------------------------
    // Convergence test
    // ------------------------------------------------------------

    float thresh = 1.0f - CONV_THRESH;
    thresh *= thresh;

    if (this->m_lower_dist_sq >
        thresh * this->m_upper_dist_sq)
        return GJK_VALID;

    // Degeneracy check
    for (int i = 0; i < 4; ++i)
    {
        if (this->m_last_w_set & (1 << i))
        {
            phys_vec3 &v = this->m_w_verts[i];

            float dx = v.x - this->m_w_verts[new_index].x;
            float dy = v.y - this->m_w_verts[new_index].y;
            float dz = v.z - this->m_w_verts[new_index].z;

            if (dx * dx + dy * dy + dz * dz < 1e-6f)
                return GJK_VALID;
        }
    }

    return GJK_VALID;
}

#endif


#if 0
// local variable allocation has failed, the output may be wrong!
phys_gjk_info::gjk_retval_e phys_gjk_info::gjk_ray_cast(
                const phys_gjk_input *d,
                const phys_vec3 *initial_support_dir,
                bool in_separation_loop)
{
    double v8; // st7
    int inited; // eax
    phys_gjk_cache_info *gjk_ci; // ecx
    int m_flags; // eax
    int v12; // eax
    double m_geom_radii_sum; // st7
    bool v14; // zf
    double v15; // st7
    int v16; // ecx
    int v17; // ecx
    phys_gjk_info::gjk_retval_e result; // eax
    int m_w_set; // eax
    unsigned int v20; // edi
    const phys_gjk_geom *gjk_cg1; // ecx
    double v22; // st7
    phys_vec3 *v23; // edi
    const phys_gjk_geom *gjk_cg2; // ecx
    const phys_vec3 *v25; // eax
    double z; // st7
    float *v27; // eax
    double v28; // st6
    double v29; // st7
    double v30; // st5
    double v31; // st4
    double v32; // st4
    int v33; // ecx
    double v34; // st3
    double v35; // st3
    int v36; // edi
    int v37; // ecx
    float *p_z; // edx
    double v39; // st4
    double v40; // st7
    double v41; // st6
    double m_lower_dist_sq; // st7
    int v43; // ecx
    phys_vec3 *v44; // eax
    int v45; // eax
    double v46; // st5
    double v47; // st5
    double v48; // st7
    double m_end_time; // st6
    double v50; // st6
    int v51; // eax
    phys_vec3 *v54; // [esp-84h] [ebp-B4h]
    phys_vec3 v55; // [esp-7Ch] [ebp-ACh] BYREF
    float v56[4]; // [esp-6Ch] [ebp-9Ch] BYREF
    float v57[4]; // [esp-5Ch] [ebp-8Ch] BYREF
    float v58; // [esp-4Ch] [ebp-7Ch]
    float v59; // [esp-48h] [ebp-78h]
    float v60; // [esp-44h] [ebp-74h]
    float v61; // [esp-3Ch] [ebp-6Ch]
    float v62; // [esp-38h] [ebp-68h]
    float v63; // [esp-34h] [ebp-64h]
    float v64; // [esp-2Ch] [ebp-5Ch]
    float v65; // [esp-28h] [ebp-58h]
    float v66; // [esp-24h] [ebp-54h]
    float v67; // [esp-1Ch] [ebp-4Ch]
    float v68; // [esp-18h] [ebp-48h]
    float v69; // [esp-14h] [ebp-44h]
    float v70; // [esp-Ch] [ebp-3Ch]
    float v71; // [esp-8h] [ebp-38h]
    float v72; // [esp-4h] [ebp-34h]
    phys_vec3 w; // [esp+0h] [ebp-30h]
    unsigned int v74; // [esp+10h] [ebp-20h]
    float v75; // [esp+14h] [ebp-1Ch]
    float support_dir_moveback; // [esp+18h] [ebp-18h]
    int new_index; // [esp+1Ch] [ebp-14h]
    float lambda; // [esp+20h] [ebp-10h]
    float ray_end_dist_numer; // [esp+24h] [ebp-Ch] BYREF
    void *v80; // [esp+28h] [ebp-8h] OVERLAPPED
    void *retaddr; // [esp+30h] [ebp+0h]

    ray_end_dist_numer = a2;
    v80 = retaddr;
    lambda = d->m_start_time;
    v8 = lambda;
    this->m_continuous_collision_lambda = lambda;
    lambda = -v8;
    v70 = this->m_cg1_relative_translation_loc.x * lambda;
    v71 = this->m_cg1_relative_translation_loc.y * lambda;
    v72 = lambda * this->m_cg1_relative_translation_loc.z;
    this->m_gjk_origin.x = v70;
    this->m_gjk_origin.y = v71;
    this->m_gjk_origin.z = v72;
    this->m_lower_dist_sq = -34.0;
    this->m_upper_dist_sq = 34.0;
    inited = phys_gjk_info::init_gjk(this, (int)&ray_end_dist_numer, d, initial_support_dir, in_separation_loop);
    this->m_flags &= 0xFFFFFFDA;
    this->m_gjk_iter = inited;
    gjk_ci = d->gjk_ci;
    m_flags = this->m_flags;
    if ( gjk_ci && (gjk_ci->m_flags & 4) != 0 )
        v12 = m_flags | 0x10;
    else
        v12 = m_flags & 0xFFFFFFEF;
    this->m_flags = v12;
    this->m_cc_reset_iter = 0;
    if ( this->m_geom_radii_sum >= 0.05099999904632568 )
        m_geom_radii_sum = this->m_geom_radii_sum;
    else
        m_geom_radii_sum = 0.050999999;
    w.w = m_geom_radii_sum;
    if ( this->m_gjk_sep_thresh <= (double)w.w
        && _tlAssert("source/phys_gjk.cpp", 1726, "support_dir_moveback < m_gjk_sep_thresh", "") )
    {
        __debugbreak();
    }
    while ( 1 )
    {
        while ( 1 )
        {
            v14 = this->m_gjk_iter == 0;
            lambda = this->m_support_dir.y * this->m_support_dir.y
                         + this->m_support_dir.x * this->m_support_dir.x
                         + this->m_support_dir.z * this->m_support_dir.z;
            v15 = lambda;
            this->m_upper_dist_sq = lambda;
            if ( !v14 )
            {
                v16 = this->m_flags;
                if ( (v16 & 0x10) != 0 )
                {
                    lambda = this->m_gjk_sep_thresh;
                    lambda = lambda * lambda;
                    if ( lambda > v15 )
                        this->m_flags = v16 | 1;
                }
                if ( this->m_gjk_pen_thresh_sq > (double)this->m_upper_dist_sq )
                {
                    v17 = this->m_flags;
                    if ( (v17 & 8) != 0 && 0.0 != this->m_continuous_collision_lambda && (v17 & 2) == 0 )
                        goto LABEL_19;
                    return 2;
                }
            }
            m_w_set = this->m_w_set;
            if ( (m_w_set & 1) != 0 )
            {
                if ( (m_w_set & 2) != 0 )
                {
                    v20 = (this->m_w_set & 4 | 8u) >> 2;
                    v74 = v20;
                    goto LABEL_27;
                }
                v20 = 1;
            }
            else
            {
                v20 = 0;
            }
            v74 = v20;
LABEL_27:
            gjk_cg1 = d->gjk_cg1;
            v57[0] = -this->m_support_dir.x;
            v57[1] = -this->m_support_dir.y;
            v22 = -this->m_support_dir.z;
            LODWORD(v75) = &this->m_a_verts[v20];
            v57[2] = v22;
            ((void (__thiscall *)(const phys_gjk_geom *, float *, float, phys_vec3 *, float *, phys_vec3 *, phys_vec3 *))gjk_cg1->support)(
                gjk_cg1,
                v57,
                COERCE_FLOAT(LODWORD(v75)),
                &this->m_a_inds[v20],
                a3,
                a4,
                v54);
            v23 = &this->m_b_verts[v74];
            gjk_cg2 = d->gjk_cg2;
            v54 = &this->m_b_inds[v74];
            a4 = v23;
            a3 = v56;
            v56[0] = this->cg2_to_cg1_xform.x.y * this->m_support_dir.y
                         + this->cg2_to_cg1_xform.x.x * this->m_support_dir.x
                         + this->cg2_to_cg1_xform.x.z * this->m_support_dir.z;
            v56[1] = this->cg2_to_cg1_xform.y.y * this->m_support_dir.y
                         + this->cg2_to_cg1_xform.y.x * this->m_support_dir.x
                         + this->cg2_to_cg1_xform.y.z * this->m_support_dir.z;
            v56[2] = this->cg2_to_cg1_xform.z.y * this->m_support_dir.y
                         + this->cg2_to_cg1_xform.z.x * this->m_support_dir.x
                         + this->cg2_to_cg1_xform.z.z * this->m_support_dir.z;
            ((void (__thiscall *)(const phys_gjk_geom *))gjk_cg2->support)(gjk_cg2);
            v25 = phys_multiply(&v55, &this->cg2_to_cg1_xform, v23);
            v64 = v25->x + this->cg2_to_cg1_xform.w.x;
            v65 = v25->y + this->cg2_to_cg1_xform.w.y;
            z = v25->z;
            v27 = (float *)LODWORD(v75);
            v66 = z + this->cg2_to_cg1_xform.w.z;
            v23->x = v64;
            v23->y = v65;
            v23->z = v66;
            v58 = *v27 - v23->x;
            v59 = v27[1] - v23->y;
            v60 = v27[2] - v23->z;
            v70 = v58 - this->m_gjk_origin.x;
            v71 = v59 - this->m_gjk_origin.y;
            v72 = v60 - this->m_gjk_origin.z;
            v28 = v71;
            v29 = v70;
            v30 = v72;
            v75 = this->m_support_dir.z * v72 + this->m_support_dir.x * v70 + this->m_support_dir.y * v71;
            v31 = 0.0;
            if ( v75 > 0.0 )
            {
                v32 = v75;
                if ( this->m_upper_dist_sq <= 0.0 )
                {
                    v31 = 0.0;
                }
                else
                {
                    this->m_flags |= 0x20u;
                    v33 = this->m_flags;
                    v34 = v32 * v32;
                    v31 = 0.0;
                    lambda = v34;
                    lambda = lambda / this->m_upper_dist_sq;
                    v35 = lambda;
                    if ( this->m_lower_dist_sq < (double)lambda )
                    {
                        this->m_lower_dist_sq = lambda;
                        if ( (v33 & 1) != 0 )
                        {
                            lambda = this->m_gjk_sep_thresh;
                            lambda = lambda * lambda;
                            if ( lambda < v35 )
                                return 0;
                        }
                    }
                }
            }
            v14 = this->m_gjk_iter == 0;
            HIBYTE(new_index) = 0;
            if ( !v14 && v31 < this->m_lower_dist_sq )
            {
                lambda = 1.0 - CONV_THRESH;
                lambda = lambda * lambda;
                HIBYTE(new_index) = this->m_lower_dist_sq > lambda * this->m_upper_dist_sq;
                v36 = 0;
                v37 = 1;
                p_z = &this->m_w_verts[0].z;
                do
                {
                    if ( HIBYTE(new_index) )
                        break;
                    if ( (v37 & this->m_last_w_set) != 0 )
                    {
                        v61 = v29 - *(p_z - 2);
                        v62 = v28 - *(p_z - 1);
                        v63 = v30 - *p_z;
                        lambda = v62 * v62 + v61 * v61 + v63 * v63;
                        HIBYTE(new_index) = lambda < 0.0000010000001;
                    }
                    ++v36;
                    p_z += 4;
                    v37 = __ROL4__(v37, 1);
                }
                while ( v36 < 4 );
            }
            if ( (this->m_flags & 4) != 0 )
                goto LABEL_53;
            if ( v75 <= 0.0 )
                goto LABEL_53;
            lambda = v75 * v75;
            v39 = lambda;
            lambda = w.w * w.w;
            if ( lambda * this->m_upper_dist_sq > v39 )
                goto LABEL_53;
            lambda = sqrt(this->m_upper_dist_sq);
            v40 = 0.0;
            if ( lambda <= 0.0 )
            {
                if ( _tlAssert("source/phys_gjk.cpp", 1813, "nsupport_dir > 0.0f", "") )
                    __debugbreak();
                v40 = 0.0;
            }
            support_dir_moveback = this->m_cg1_relative_translation_loc.y * this->m_support_dir.y
                                                     + this->m_cg1_relative_translation_loc.x * this->m_support_dir.x
                                                     + this->m_cg1_relative_translation_loc.z * this->m_support_dir.z;
            support_dir_moveback = -support_dir_moveback;
            v75 = v75 - lambda * w.w;
            v41 = support_dir_moveback;
            if ( support_dir_moveback <= v40 )
            {
                if ( v40 < v75 )
                {
                    support_dir_moveback = this->m_gjk_sep_thresh;
                    m_lower_dist_sq = this->m_lower_dist_sq;
                    support_dir_moveback = support_dir_moveback * support_dir_moveback;
                    if ( support_dir_moveback < m_lower_dist_sq )
                        return 0;
                    this->m_flags |= 5u;
                }
                goto LABEL_52;
            }
            v46 = v75;
            if ( v75 >= v40 )
            {
                support_dir_moveback = this->m_support_dir.y * this->m_gjk_origin.y
                                                         + this->m_support_dir.x * this->m_gjk_origin.x
                                                         + this->m_support_dir.z * this->m_gjk_origin.z;
                v75 = (support_dir_moveback + v46) / v41;
                if ( this->m_continuous_collision_lambda + 0.00009999999747378752 < v75 )
                    break;
            }
LABEL_52:
            v28 = v71;
            v30 = v72;
            v29 = v70;
LABEL_53:
            if ( HIBYTE(new_index) )
                return 1;
            v43 = v74;
            v44 = &this->m_w_verts[v74];
            v44->x = v29;
            v44->y = v28;
            v44->z = v30;
            this->m_w_set |= 1 << v43;
            this->m_last_w_set = this->m_w_set;
            v45 = backup1(COERCE_FLOAT(&ray_end_dist_numer), this, v43, 0);
            this->m_w_set = v45;
            if ( v45 == 15 )
            {
                if ( (this->m_flags & 8) != 0 && 0.0 != this->m_continuous_collision_lambda )
LABEL_19:
                    tlWarning("m_continuous_collision_lambda problem");
                return 2;
            }
            phys_gjk_info::comp_v(this, COERCE_FLOAT(&ray_end_dist_numer), v45, &this->m_support_dir);
            if ( ++this->m_gjk_iter >= 30 )
            {
                tlWarning("gjk reached the maximum number of iterations.");
                result = GJK_PENETRATING;
                if ( this->m_lower_dist_sq > 0.0 )
                    return 1;
                return result;
            }
        }
        if ( d->m_end_time < (double)v75 )
            break;
        v50 = v75;
        v48 = 0.00009999999747378752;
LABEL_65:
        if ( v50 - this->m_continuous_collision_lambda <= v48 )
            this->m_flags |= 5u;
        this->m_continuous_collision_lambda = v50;
        support_dir_moveback = -v50;
        v67 = this->m_cg1_relative_translation_loc.x * support_dir_moveback;
        v68 = this->m_cg1_relative_translation_loc.y * support_dir_moveback;
        v69 = support_dir_moveback * this->m_cg1_relative_translation_loc.z;
        this->m_gjk_origin.x = v67;
        this->m_gjk_origin.y = v68;
        this->m_gjk_origin.z = v69;
        v51 = ++this->m_cc_reset_iter;
        this->m_lower_dist_sq = -34.0;
        this->m_upper_dist_sq = 34.0;
        if ( v51 > 1 )
            this->m_flags |= 0x10u;
        if ( v51 >= MAX_CC_RESET_ITERS )
            this->m_flags |= 5u;
        if ( !this->m_w_set )
            this->m_w_set = 1 << v74;
        if ( !phys_gjk_info::init_gjk(this, (int)&ray_end_dist_numer, d, &this->m_support_dir, 1)
            && _tlAssert("source/phys_gjk.cpp", 1918, "did_init", "") )
        {
            __debugbreak();
        }
        if ( !this->m_w_set && _tlAssert("source/phys_gjk.cpp", 1919, "m_w_set != 0", "") )
            __debugbreak();
        if ( this->m_last_w_set != this->m_w_set
            && _tlAssert("source/phys_gjk.cpp", 1920, "m_last_w_set == m_w_set", "") )
        {
            __debugbreak();
        }
        ++this->m_gjk_iter;
    }
    support_dir_moveback = v46 - v41 * d->m_end_time + support_dir_moveback;
    v47 = support_dir_moveback;
    support_dir_moveback = this->m_gjk_sep_thresh * lambda;
    if ( support_dir_moveback >= v47 )
    {
        v48 = 0.00009999999747378752;
        m_end_time = d->m_end_time;
        this->m_flags |= 5u;
        v75 = m_end_time;
        v50 = v75;
        goto LABEL_65;
    }
    if ( v47 <= v40 && _tlAssert("source/phys_gjk.cpp", 1850, "ray_end_dist_numer > 0.0f", "") )
        __debugbreak();
    return 0;
}
#else  // aislop
phys_gjk_info::gjk_retval_e phys_gjk_info::gjk_ray_cast(
    const phys_gjk_input *d,
    const phys_vec3 *initial_support_dir,
    bool in_separation_loop)
{
    // ------------------------------------------------------------
    // Initial time setup
    // ------------------------------------------------------------

    float lambda = d->m_start_time;
    this->m_continuous_collision_lambda = lambda;

    float neg_lambda = -lambda;

    this->m_gjk_origin.x = this->m_cg1_relative_translation_loc.x * neg_lambda;
    this->m_gjk_origin.y = this->m_cg1_relative_translation_loc.y * neg_lambda;
    this->m_gjk_origin.z = this->m_cg1_relative_translation_loc.z * neg_lambda;

    this->m_lower_dist_sq = -34.0f;
    this->m_upper_dist_sq = 34.0f;

    this->m_gjk_iter =
        phys_gjk_info::init_gjk(d, initial_support_dir, in_separation_loop);

    this->m_flags &= ~(0x4 | 0x8 | 0x20);
    this->m_cc_reset_iter = 0;

    const float support_dir_moveback = max(this->m_geom_radii_sum, 0.051f);

    // ------------------------------------------------------------
    // Main loop
    // ------------------------------------------------------------

    while (true)
    {
        const bool first_iter = (this->m_gjk_iter == 0);

        float nsupport_dir =
            this->m_support_dir.x * this->m_support_dir.x +
            this->m_support_dir.y * this->m_support_dir.y +
            this->m_support_dir.z * this->m_support_dir.z;

        this->m_upper_dist_sq = nsupport_dir;

        if (!first_iter)
        {
            if (this->m_gjk_pen_thresh_sq > nsupport_dir)
                return GJK_PENETRATING;
        }

        // ------------------------------------------------------------
        // Choose simplex slot
        // ------------------------------------------------------------

        int new_index;
        if (this->m_w_set & 1)
        {
            if (this->m_w_set & 2)
                new_index = (this->m_w_set & 4) ? 3 : 2;
            else
                new_index = 1;
        }
        else
        {
            new_index = 0;
        }

        // ------------------------------------------------------------
        // Support A
        // ------------------------------------------------------------

        phys_vec3 negDir = {
            -this->m_support_dir.x,
            -this->m_support_dir.y,
            -this->m_support_dir.z
        };

        d->gjk_cg1->support(
            &negDir,
            &this->m_a_verts[new_index],
            &this->m_a_inds[new_index]);

        // ------------------------------------------------------------
        // Support B (transform dir)
        // ------------------------------------------------------------

        phys_vec3 dirB;

        dirB.x =
            this->cg2_to_cg1_xform.x.x * this->m_support_dir.x +
            this->cg2_to_cg1_xform.x.y * this->m_support_dir.y +
            this->cg2_to_cg1_xform.x.z * this->m_support_dir.z;

        dirB.y =
            this->cg2_to_cg1_xform.y.x * this->m_support_dir.x +
            this->cg2_to_cg1_xform.y.y * this->m_support_dir.y +
            this->cg2_to_cg1_xform.y.z * this->m_support_dir.z;

        dirB.z =
            this->cg2_to_cg1_xform.z.x * this->m_support_dir.x +
            this->cg2_to_cg1_xform.z.y * this->m_support_dir.y +
            this->cg2_to_cg1_xform.z.z * this->m_support_dir.z;

        d->gjk_cg2->support(
            &dirB,
            &this->m_b_verts[new_index],
            &this->m_b_inds[new_index]);

        // Transform B into A space

        phys_vec3 bTransformed;
        phys_multiply(&bTransformed,
            &this->cg2_to_cg1_xform,
            &this->m_b_verts[new_index]);

        bTransformed.x += this->cg2_to_cg1_xform.w.x;
        bTransformed.y += this->cg2_to_cg1_xform.w.y;
        bTransformed.z += this->cg2_to_cg1_xform.w.z;

        this->m_b_verts[new_index] = bTransformed;

        // ------------------------------------------------------------
        // Minkowski difference
        // ------------------------------------------------------------

        phys_vec3 w;
        w.x = this->m_a_verts[new_index].x - bTransformed.x;
        w.y = this->m_a_verts[new_index].y - bTransformed.y;
        w.z = this->m_a_verts[new_index].z - bTransformed.z;

        float vx = w.x - this->m_gjk_origin.x;
        float vy = w.y - this->m_gjk_origin.y;
        float vz = w.z - this->m_gjk_origin.z;

        float dot =
            this->m_support_dir.x * vx +
            this->m_support_dir.y * vy +
            this->m_support_dir.z * vz;

        // ------------------------------------------------------------
        // Conservative advancement
        // ------------------------------------------------------------

        if (dot > 0.0f)
        {
            float move =
                -(this->m_cg1_relative_translation_loc.x * this->m_support_dir.x +
                    this->m_cg1_relative_translation_loc.y * this->m_support_dir.y +
                    this->m_cg1_relative_translation_loc.z * this->m_support_dir.z);

            if (move > 0.0f)
            {
                float new_lambda =
                    (dot - std::sqrt(nsupport_dir) * support_dir_moveback)
                    / move;

                if (new_lambda > this->m_continuous_collision_lambda)
                {
                    if (new_lambda > d->m_end_time)
                        return GJK_SEPARATED;

                    this->m_continuous_collision_lambda = new_lambda;

                    float shift = -new_lambda;

                    this->m_gjk_origin.x =
                        this->m_cg1_relative_translation_loc.x * shift;
                    this->m_gjk_origin.y =
                        this->m_cg1_relative_translation_loc.y * shift;
                    this->m_gjk_origin.z =
                        this->m_cg1_relative_translation_loc.z * shift;

                    this->m_lower_dist_sq = -34.0f;
                    this->m_upper_dist_sq = 34.0f;
                    this->m_cc_reset_iter++;

                    this->m_gjk_iter =
                        phys_gjk_info::init_gjk(d, &this->m_support_dir, true);

                    continue;
                }
            }
        }

        // ------------------------------------------------------------
        // Add to simplex
        // ------------------------------------------------------------

        this->m_w_verts[new_index] = w;

        this->m_w_set |= (1 << new_index);
        this->m_last_w_set = this->m_w_set;

        int new_mask = backup1(this, new_index, 0);
        this->m_w_set = new_mask;

        if (new_mask == 15)
            return GJK_PENETRATING;

        phys_gjk_info::comp_v(new_mask,&this->m_support_dir);

        if (++this->m_gjk_iter >= 30)
            return GJK_VALID;
    }
}

#endif
phys_gjk_info::gjk_retval_e phys_gjk_info::collide(const phys_gjk_input *d)
{
    phys_gjk_info::gjk_retval_e result; // eax
    int m_flags; // eax
    phys_gjk_cache_info *gjk_ci; // eax
    phys_vec3 *v7; // eax
    float y; // edx
    float z; // ecx
    char v10; // al
    double v11; // st7
    const phys_gjk_geom *gjk_cg1; // ecx
    const phys_vec3 *v13; // eax
    double v14; // st7
    phys_gjk_cache_info *v15; // eax
    bool v16; // [esp-70h] [ebp-A0h]
    phys_vec3 v17; // [esp-5Ch] [ebp-8Ch] BYREF
    phys_vec3 v18; // [esp-4Ch] [ebp-7Ch] BYREF
    phys_vec3 v19; // [esp-3Ch] [ebp-6Ch] BYREF
    phys_vec3 v20; // [esp-2Ch] [ebp-5Ch] BYREF
    float v21; // [esp-1Ch] [ebp-4Ch]
    float v22; // [esp-18h] [ebp-48h]
    float v23; // [esp-14h] [ebp-44h]
    phys_vec3 v24; // [esp-Ch] [ebp-3Ch] BYREF
    phys_vec3 w_4; // [esp+4h] [ebp-2Ch] BYREF
    float support_dir_8; // [esp+18h] [ebp-18h]
    float support_dir_12; // [esp+1Ch] [ebp-14h]
    phys_gjk_info::gjk_retval_e v28; // [esp+20h] [ebp-10h]
    int v29; // [esp+24h] [ebp-Ch] BYREF
    //float nsupport_dir_sq; // [esp+28h] [ebp-8h]
    //float retaddr; // [esp+30h] [ebp+0h]
    //
    //v29 = a2;
    //nsupport_dir_sq = retaddr;
    this->m_gjk_origin.x = PHYS_ZERO_VEC.x;
    this->m_gjk_origin.y = PHYS_ZERO_VEC.y;
    this->m_gjk_origin.z = PHYS_ZERO_VEC.z;
    this->m_flags |= 1u;
    phys_gjk_info::get_initial_support_dir(&v24, d);
    if ((this->m_flags & 8) != 0)
        result = phys_gjk_info::gjk_ray_cast(
            d,
            &v24,
            0);
    else
        result = phys_gjk_info::gjk(
            d,
            &v24,
            0);
    v28 = result;
    if (result == GJK_INVALID)
    {
        if (_tlAssert("source/phys_gjk.cpp", 2058, "retv != GJK_INVALID", ""))
            __debugbreak();
    LABEL_9:
        if (_tlAssert("source/phys_gjk.cpp", 2062, "retv == GJK_SEPARATED || retv == GJK_VALID", ""))
            __debugbreak();
        return v28;
    }
    if (result != GJK_PENETRATING)
    {
        if ((unsigned int)result < GJK_PENETRATING)
            return result;
        goto LABEL_9;
    }
    m_flags = this->m_flags;
    if ((m_flags & 2) != 0)
        return GJK_PENETRATING;
    if ((m_flags & 8) != 0 && 0.0 != this->m_continuous_collision_lambda)
        tlWarning("m_continuous_collision_lambda problem");
    this->m_continuous_collision_lambda = 0.0;
    gjk_ci = d->gjk_ci;
    if (gjk_ci && (gjk_ci->m_flags & 4) != 0)
    {
        v7 = &v24;
    }
    else
    {
        v7 = (phys_vec3 *)gjk_sep_dir::comp_sep_dir(&v19, d, this);
    }
    y = v7->y;
    w_4.x = v7->x;
    z = v7->z;
    w_4.y = y;
    w_4.z = z;
    w_4.w = v7->w;
    support_dir_12 = y * y + w_4.x * w_4.x + z * z;
    if (support_dir_12 >= 0.0000000099999991)
        goto LABEL_27;
    support_dir_12 = v24.z * v24.z + v24.y * v24.y + v24.x * v24.x;
    if (support_dir_12 >= 0.0000000099999991)
    {
        w_4.x = v24.x;
        w_4.y = v24.y;
        w_4.z = v24.z;
    LABEL_27:
        v11 = w_4.y;
        this->m_flags &= ~1u;
        v28 = GJK_SEPARATED;
        support_dir_12 = v11 * v11 + w_4.x * w_4.x + w_4.z * w_4.z;
        while (1)
        {
            gjk_cg1 = d->gjk_cg1;
            //++v28;
            v28 = (gjk_retval_e)((int)v28 + 1);
            v20.x = -w_4.x;
            v20.y = -w_4.y;
            v20.z = -w_4.z;
            gjk_cg1->support(&v20, &v19, &v18);
            v13 = // phys_gjk_geom::support_only(
                //(phys_gjk_geom *)d->gjk_cg2,
                d->gjk_cg2->support_only(
                &v17,
                &this->cg2_to_cg1_xform,
                &w_4);
            v24.x = v19.x - v13->x;
            v24.y = v19.y - v13->y;
            v24.z = v19.z - v13->z;
            support_dir_8 = sqrt(support_dir_12);
            v14 = 17.0 / support_dir_8;
            v16 = v28 > GJK_VALID;
            support_dir_8 = v24.z * w_4.z + v24.x * w_4.x + v24.y * w_4.y;
            support_dir_8 = -(v14 - support_dir_8 / support_dir_12);
            v21 = w_4.x * support_dir_8;
            v22 = w_4.y * support_dir_8;
            v23 = w_4.z * support_dir_8;
            this->m_gjk_origin.x = v21;
            this->m_gjk_origin.y = v22;
            this->m_gjk_origin.z = v23;
            if (phys_gjk_info::gjk(
                d,
                &w_4,
                v16) == GJK_PENETRATING)
            {
                tlWarning("gjk has possibly failed.");
                if (v28 != GJK_VALID)
                    break;
                v15 = d->gjk_ci;
                if (!v15)
                    break;
                if ((v15->m_flags & 8) == 0)
                    break;
                v15->m_flags &= ~8u;
                if (phys_gjk_info::gjk(
                    d,
                    &w_4,
                    0) == GJK_PENETRATING)
                    break;
            }
            phys_gjk_info::comp_v(this->m_w_set, &w_4);
            support_dir_12 = w_4.y * w_4.y + w_4.x * w_4.x + w_4.z * w_4.z;
            if (this->m_gjk_pen_thresh_sq > (double)support_dir_12
                && _tlAssert("source/phys_gjk.cpp", 2115, "AbsSquared(support_dir) >= m_gjk_pen_thresh_sq", ""))
            {
                __debugbreak();
            }
            support_dir_8 = 1.0 - SEP_CONV_THRESH;
            support_dir_8 = support_dir_8 * support_dir_8;
            if (support_dir_8 * this->m_lower_dist_sq < 289.0 || v28 >= 10)
                return GJK_PENETRATING;
        }
        v10 = _tlAssert("source/phys_gjk.cpp", 2110, "0", "gjk has failed.");
        goto LABEL_23;
    }
    v10 = _tlAssert("source/phys_gjk.cpp", 2080, "0", "initial support dir invalid.");
LABEL_23:
    if (v10)
        __debugbreak();
    return GJK_INVALID;
}

char phys_gjk_info::phys_collide_do_gjk_collide(const phys_gjk_input *d)
{
    const phys_mat44 *cg1_to_world_xform; // eax
    double v5; // st6
    double v6; // st7
    double v7; // st7
    phys_gjk_info::gjk_retval_e v8; // eax
    phys_gjk_cache_info *gjk_ci; // eax
    int m_w_set; // ecx
    int v12; // eax
    double cg1_radius; // st7
    float v14; // [esp+0h] [ebp-40h]
    float v15; // [esp+14h] [ebp-2Ch]
    float v16; // [esp+14h] [ebp-2Ch]
    float v17; // [esp+14h] [ebp-2Ch]
    float v18; // [esp+14h] [ebp-2Ch]
    float v19; // [esp+18h] [ebp-28h]
    float v20; // [esp+18h] [ebp-28h]
    float v21; // [esp+18h] [ebp-28h]
    float v22; // [esp+18h] [ebp-28h]
    float v23; // [esp+1Ch] [ebp-24h]
    float v24; // [esp+1Ch] [ebp-24h]
    float v25; // [esp+1Ch] [ebp-24h]
    float v26; // [esp+1Ch] [ebp-24h]
    float v27; // [esp+30h] [ebp-10h]
    float v28; // [esp+30h] [ebp-10h]
    phys_gjk_info::gjk_retval_e v29; // [esp+30h] [ebp-10h]
    float cg2_radius; // [esp+30h] [ebp-10h]
    //unsigned int v31[2]; // [esp+34h] [ebp-Ch] BYREF
    //_UNKNOWN *retaddr; // [esp+40h] [ebp+0h]
    //
    //v31[0] = a2;
    //v31[1] = retaddr;
    if ( (d->m_start_time < 0.0 || d->m_end_time < (double)d->m_start_time || d->m_end_time > 1.0)
        && _tlAssert(
                 "source/phys_gjk.cpp",
                 2176,
                 "(d->m_start_time >= 0.0f) && (d->m_start_time <= d->m_end_time) && (d->m_end_time <= 1.0f)",
                 "") )
    {
        __debugbreak();
    }
    this->m_flags = 0;
    phys_full_inv_multiply_mat(&this->cg2_to_cg1_xform, d->cg1_to_world_xform, d->cg2_to_world_xform);
    if ( d->m_continuous_collision )
    {
        cg1_to_world_xform = d->cg1_to_world_xform;
        v15 = d->m_cg1_translation.x - d->m_cg2_translation.x;
        v19 = d->m_cg1_translation.y - d->m_cg2_translation.y;
        v23 = d->m_cg1_translation.z - d->m_cg2_translation.z;
        v5 = v19;
        v6 = v15;
        v16 = cg1_to_world_xform->x.z * v23 + cg1_to_world_xform->x.x * v15 + cg1_to_world_xform->x.y * v19;
        v20 = cg1_to_world_xform->y.y * v19 + cg1_to_world_xform->y.x * v6 + cg1_to_world_xform->y.z * v23;
        v24 = v6 * cg1_to_world_xform->z.x + v5 * cg1_to_world_xform->z.y + v23 * cg1_to_world_xform->z.z;
        this->m_cg1_relative_translation_loc.x = v16;
        this->m_cg1_relative_translation_loc.y = v20;
        this->m_cg1_relative_translation_loc.z = v24;
        this->m_flags |= 8u;
    }
    v27 = d->cg2_radius + d->cg1_radius;
    this->m_geom_radii_sum = v27;
    v28 = v27 + d->m_sep_thresh;
    this->m_gjk_sep_thresh = v28;
    if ( d->m_intersection_test_only )
    {
        this->m_flags |= 2u;
        v7 = v28 * v28;
    }
    else
    {
        v7 = 0.0011560001;
    }
    this->m_gjk_pen_thresh_sq = v7;
    v8 = phys_gjk_info::collide(d);
    v29 = v8;
    if ( v8 == GJK_INVALID )
    {
        if ( _tlAssert("source/phys_gjk.cpp", 2207, "0", "phys_gjk failed") )
            __debugbreak();
        gjk_ci = d->gjk_ci;
        if ( gjk_ci )
        {
            gjk_ci->m_flags |= 1u;
            d->gjk_ci->m_flags &= ~4u;
            d->gjk_ci->m_flags &= ~8u;
        }
        return 0;
    }
    else if ( v8 )
    {
        if ( (this->m_flags & 2) != 0 )
        {
            if ( v8 != GJK_VALID && v8 != GJK_PENETRATING )
            {
                if ( _tlAssert("source/phys_gjk.cpp", 2218, "retv == GJK_VALID || retv == GJK_PENETRATING", "") )
                    __debugbreak();
                v8 = v29;
            }
            m_w_set = this->m_w_set;
            if ( m_w_set == 15 )
            {
                phys_gjk_info::gjk_cache_update_invalid(d);
                return 1;
            }
            else if ( v8 == GJK_VALID )
            {
                phys_gjk_info::comp_v(m_w_set, &this->m_support_dir);
                phys_gjk_info::gjk_cache_update_test_only_valid(d);
                return 1;
            }
            else
            {
                if ( v8 != GJK_PENETRATING
                    && _tlAssert("source/phys_gjk.cpp", 2230, "retv == GJK_PENETRATING", "") )
                {
                    __debugbreak();
                }
                phys_gjk_info::gjk_cache_update_test_only_penetrating(d);
                return 1;
            }
        }
        else
        {
            if ( v8 != GJK_VALID
                && v8 != GJK_PENETRATING
                && _tlAssert("source/phys_gjk.cpp", 2237, "retv == GJK_VALID || retv == GJK_PENETRATING", "") )
            {
                __debugbreak();
            }
            v12 = this->m_w_set;
            if ( v12 == 15 )
            {
                if ( _tlAssert("source/phys_gjk.cpp", 2240, "0", "phys_gjk failed") )
                    __debugbreak();
                phys_gjk_info::gjk_cache_update_invalid(d);
                return 0;
            }
            else
            {
                if ( (v12 >= 15 || v12 <= 0)
                    && _tlAssert("source/phys_gjk.cpp", 2244, "m_w_set < 15 && m_w_set > 0", "") )
                {
                    __debugbreak();
                }
                phys_gjk_info::comp_closest_points(this->m_w_set, &this->cg1_cinfo_loc.m_p1, &this->cg1_cinfo_loc.m_p2);
                phys_gjk_info::comp_v(this->m_w_set, &this->cg1_cinfo_loc.m_n);
                v14 = Abs(&this->cg1_cinfo_loc.m_n.x);
                //phys_vec3::operator/=(&this->cg1_cinfo_loc.m_n, v14);
                this->cg1_cinfo_loc.m_n /= v14;
                cg1_radius = d->cg1_radius;
                v17 = this->cg1_cinfo_loc.m_n.x * cg1_radius;
                v21 = this->cg1_cinfo_loc.m_n.y * cg1_radius;
                v25 = cg1_radius * this->cg1_cinfo_loc.m_n.z;
                this->cg1_cinfo_loc.m_p1.x = this->cg1_cinfo_loc.m_p1.x - v17;
                this->cg1_cinfo_loc.m_p1.y = this->cg1_cinfo_loc.m_p1.y - v21;
                this->cg1_cinfo_loc.m_p1.z = this->cg1_cinfo_loc.m_p1.z - v25;
                cg2_radius = d->cg2_radius;
                v18 = this->cg1_cinfo_loc.m_n.x * cg2_radius;
                v22 = this->cg1_cinfo_loc.m_n.y * cg2_radius;
                v26 = cg2_radius * this->cg1_cinfo_loc.m_n.z;
                this->cg1_cinfo_loc.m_p2.x = this->cg1_cinfo_loc.m_p2.x + v18;
                this->cg1_cinfo_loc.m_p2.y = v22 + this->cg1_cinfo_loc.m_p2.y;
                this->cg1_cinfo_loc.m_p2.z = this->cg1_cinfo_loc.m_p2.z + v26;
                phys_gjk_info::gjk_cache_update_colliding(d);
                return 1;
            }
        }
    }
    else
    {
        phys_gjk_info::gjk_cache_update_test_only_valid(d);
        return 0;
    }
}

int __thiscall phys_gjk_info::compress_verts(char w_set)
{
    int result; // eax
    int v3; // edx
    float *v4; // edx
    int v5; // edx
    float *v6; // edx
    int v7; // edx
    float *v8; // edx
    phys_vec3 *v9; // edx
    int v10; // edx
    float *v11; // edx
    int v12; // edx
    float *v13; // edx
    int v14; // edx
    float *v15; // edx
    phys_vec3 *v16; // edx
    int v17; // edx
    float *v18; // edx
    int v19; // edx
    float *v20; // edx
    int v21; // edx
    float *v22; // edx
    phys_vec3 *v23; // edx

    result = 0;
    if ( (w_set & 1) != 0 )
    {
        result = 1;
        this->m_a_verts[0].x = this->m_a_verts[0].x;
        this->m_a_verts[0].y = this->m_a_verts[0].y;
        this->m_a_verts[0].z = this->m_a_verts[0].z;
        this->m_b_verts[0].x = this->m_b_verts[0].x;
        this->m_b_verts[0].y = this->m_b_verts[0].y;
        this->m_b_verts[0].z = this->m_b_verts[0].z;
        this->m_a_inds[0].x = this->m_a_inds[0].x;
        this->m_a_inds[0].y = this->m_a_inds[0].y;
        this->m_a_inds[0].z = this->m_a_inds[0].z;
        this->m_b_inds[0].x = this->m_b_inds[0].x;
        this->m_b_inds[0].y = this->m_b_inds[0].y;
        this->m_b_inds[0].z = this->m_b_inds[0].z;
    }
    if ( (w_set & 2) != 0 )
    {
        v3 = 16 * (result + 14);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v3) = this->m_a_verts[1].x;
        v4 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v3);
        v4[1] = this->m_a_verts[1].y;
        v4[2] = this->m_a_verts[1].z;
        v5 = 16 * (result + 18);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v5) = this->m_b_verts[1].x;
        v6 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v5);
        v6[1] = this->m_b_verts[1].y;
        v6[2] = this->m_b_verts[1].z;
        v7 = 16 * (result + 22);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v7) = this->m_a_inds[1].x;
        v8 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v7);
        v8[1] = this->m_a_inds[1].y;
        v8[2] = this->m_a_inds[1].z;
        v9 = &this->m_b_inds[result];
        v9->x = this->m_b_inds[1].x;
        ++result;
        v9->y = this->m_b_inds[1].y;
        v9->z = this->m_b_inds[1].z;
    }
    if ( (w_set & 4) != 0 )
    {
        v10 = 16 * (result + 14);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v10) = this->m_a_verts[2].x;
        v11 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v10);
        v11[1] = this->m_a_verts[2].y;
        v11[2] = this->m_a_verts[2].z;
        v12 = 16 * (result + 18);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v12) = this->m_b_verts[2].x;
        v13 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v12);
        v13[1] = this->m_b_verts[2].y;
        v13[2] = this->m_b_verts[2].z;
        v14 = 16 * (result + 22);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v14) = this->m_a_inds[2].x;
        v15 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v14);
        v15[1] = this->m_a_inds[2].y;
        v15[2] = this->m_a_inds[2].z;
        v16 = &this->m_b_inds[result];
        v16->x = this->m_b_inds[2].x;
        ++result;
        v16->y = this->m_b_inds[2].y;
        v16->z = this->m_b_inds[2].z;
    }
    if ( (w_set & 8) != 0 )
    {
        v17 = 16 * (result + 14);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v17) = this->m_a_verts[3].x;
        v18 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v17);
        v18[1] = this->m_a_verts[3].y;
        v18[2] = this->m_a_verts[3].z;
        v19 = 16 * (result + 18);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v19) = this->m_b_verts[3].x;
        v20 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v19);
        v20[1] = this->m_b_verts[3].y;
        v20[2] = this->m_b_verts[3].z;
        v21 = 16 * (result + 22);
        *(float *)((char *)&this->cg2_to_cg1_xform.x.x + v21) = this->m_a_inds[3].x;
        v22 = (float *)((char *)&this->cg2_to_cg1_xform.x.x + v21);
        v22[1] = this->m_a_inds[3].y;
        v22[2] = this->m_a_inds[3].z;
        v23 = &this->m_b_inds[result];
        v23->x = this->m_b_inds[3].x;
        ++result;
        v23->y = this->m_b_inds[3].y;
        v23->z = this->m_b_inds[3].z;
    }
    return result;
}

void __thiscall phys_gjk_info::comp_closest_points(int w_set, phys_vec3 *a, phys_vec3 *b)
{
    phys_gjk_info::phys_gjk_set_info *v4; // eax
    double v5; // st6
    float v6; // [esp-30h] [ebp-3Ch]
    float v7; // [esp-30h] [ebp-3Ch]
    float v8; // [esp-30h] [ebp-3Ch]
    float v9; // [esp-30h] [ebp-3Ch]
    float v10; // [esp-2Ch] [ebp-38h]
    float v11; // [esp-2Ch] [ebp-38h]
    float v12; // [esp-2Ch] [ebp-38h]
    float v13; // [esp-2Ch] [ebp-38h]
    float v14; // [esp-28h] [ebp-34h]
    float v15; // [esp-28h] [ebp-34h]
    float v16; // [esp-28h] [ebp-34h]
    float v17; // [esp-28h] [ebp-34h]
    float v18; // [esp-20h] [ebp-2Ch]
    float v19; // [esp-20h] [ebp-2Ch]
    float v20; // [esp-20h] [ebp-2Ch]
    float v21; // [esp-20h] [ebp-2Ch]
    float v22; // [esp-1Ch] [ebp-28h]
    float v23; // [esp-1Ch] [ebp-28h]
    float v24; // [esp-1Ch] [ebp-28h]
    float v25; // [esp-1Ch] [ebp-28h]
    float v26; // [esp-18h] [ebp-24h]
    float v27; // [esp-18h] [ebp-24h]
    float v28; // [esp-18h] [ebp-24h]
    float v29; // [esp-18h] [ebp-24h]
    float v30; // [esp-8h] [ebp-14h]
    float v31; // [esp-8h] [ebp-14h]
    float v32; // [esp-8h] [ebp-14h]
    float v33; // [esp-8h] [ebp-14h]
    float v34; // [esp-4h] [ebp-10h]

    a->x = 0.0;
    a->y = 0.0;
    a->z = 0.0;
    b->x = 0.0;
    v4 = &this->m_set_list[w_set];
    b->y = 0.0;
    b->z = 0.0;
    v34 = 0.0;
    if ( (w_set & 1) != 0 )
    {
        v5 = v4->m_lamda[0];
        v18 = this->m_a_verts[0].x * v5;
        v22 = this->m_a_verts[0].y * v5;
        v26 = this->m_a_verts[0].z * v5;
        a->x = a->x + v18;
        a->y = a->y + v22;
        a->z = a->z + v26;
        v6 = this->m_b_verts[0].x * v5;
        v10 = this->m_b_verts[0].y * v5;
        v14 = this->m_b_verts[0].z * v5;
        b->x = b->x + v6;
        b->y = b->y + v10;
        b->z = v14 + b->z;
        v34 = v5 + 0.0;
    }
    if ( (w_set & 2) != 0 )
    {
        v30 = v4->m_lamda[1];
        v19 = this->m_a_verts[1].x * v30;
        v23 = this->m_a_verts[1].y * v30;
        v27 = this->m_a_verts[1].z * v30;
        a->x = a->x + v19;
        a->y = a->y + v23;
        a->z = a->z + v27;
        v7 = this->m_b_verts[1].x * v30;
        v11 = this->m_b_verts[1].y * v30;
        v15 = this->m_b_verts[1].z * v30;
        b->x = b->x + v7;
        b->y = b->y + v11;
        b->z = v15 + b->z;
        v34 = v30 + v34;
    }
    if ( (w_set & 4) != 0 )
    {
        v31 = v4->m_lamda[2];
        v20 = this->m_a_verts[2].x * v31;
        v24 = this->m_a_verts[2].y * v31;
        v28 = this->m_a_verts[2].z * v31;
        a->x = a->x + v20;
        a->y = a->y + v24;
        a->z = a->z + v28;
        v8 = this->m_b_verts[2].x * v31;
        v12 = this->m_b_verts[2].y * v31;
        v16 = this->m_b_verts[2].z * v31;
        b->x = b->x + v8;
        b->y = b->y + v12;
        b->z = v16 + b->z;
        v34 = v31 + v34;
    }
    if ( (w_set & 8) != 0 )
    {
        v32 = v4->m_lamda[3];
        v21 = this->m_a_verts[3].x * v32;
        v25 = this->m_a_verts[3].y * v32;
        v29 = this->m_a_verts[3].z * v32;
        a->x = a->x + v21;
        a->y = a->y + v25;
        a->z = a->z + v29;
        v9 = this->m_b_verts[3].x * v32;
        v13 = this->m_b_verts[3].y * v32;
        v17 = this->m_b_verts[3].z * v32;
        b->x = b->x + v9;
        b->y = b->y + v13;
        b->z = v17 + b->z;
        v34 = v32 + v34;
    }
    if ( v34 < 0.0 && _tlAssert("source/phys_gjk.cpp", 173, "lambda_sum >= 0.0f", "") )
        __debugbreak();
    v33 = 1.0 / v34;
    a->x = a->x * v33;
    a->y = a->y * v33;
    a->z = a->z * v33;
    b->x = b->x * v33;
    b->y = b->y * v33;
    b->z = v33 * b->z;
}

// bad sp value at call has been detected, the output may be wrong!
const phys_vec3 *phys_gjk_info::get_initial_support_dir(
                phys_vec3 *result,
                const phys_gjk_input *d)
{
    phys_gjk_cache_info *gjk_ci; // edi
    const phys_vec3 *v6; // eax
    const phys_vec3 *v7; // eax
    float v8; // edx
    float v9; // ecx
    char *v10; // edx
    phys_vec3 v11; // [esp-3Ch] [ebp-5Ch] BYREF
    phys_vec3 v12; // [esp-2Ch] [ebp-4Ch] BYREF
    phys_vec3 v13; // [esp-1Ch] [ebp-3Ch] BYREF
    float v14; // [esp-Ch] [ebp-2Ch]
    float v15; // [esp-8h] [ebp-28h]
    float v16; // [esp-4h] [ebp-24h]
    const char *dir; // [esp+0h] [ebp-20h]
    int v18; // [esp+4h] [ebp-1Ch]
    const char *v19; // [esp+8h] [ebp-18h]
    const char *v20; // [esp+Ch] [ebp-14h]
    const phys_mat44 *v21; // [esp+10h] [ebp-10h]
    //_UNKNOWN *v22[2]; // [esp+14h] [ebp-Ch] BYREF
    //int vars0; // [esp+20h] [ebp+0h]
    //
    //v22[0] = a2;
    //v22[1] = (_UNKNOWN *)vars0;
    gjk_ci = d->gjk_ci;
    v21 = (const phys_mat44 *)this;
    if (gjk_ci && (gjk_ci->m_flags & 4) != 0)
    {
        phys_inv_multiply(result, d->cg1_to_world_xform, &gjk_ci->m_support_dir);
        return result;
    }
    else
    {
        v6 = d->gjk_cg2->get_center(&v12);
        phys_full_multiply(&v13, v21, v6);
        v7 = d->gjk_cg1->get_center(&v11);
        v14 = v7->x - v13.x;
        v15 = v7->y - v13.y;
        v16 = v7->z - v13.z;
        *(float *)&v21 = v14 * v14 + v15 * v15 + v16 * v16;
        if (*(float *)&v21 < 0.0000000099999991)
        {
            if ((pai_gjk_initial_support_dir.m_hits_total_count < pai_gjk_initial_support_dir.m_max_hits_total
                || !pai_gjk_initial_support_dir.m_max_hits_total)
                && pai_gjk_initial_support_dir.m_hits_frame_count < pai_gjk_initial_support_dir.m_max_hits_per_frame)
            {
                v20 = "degenerate gjk initial support dir.";
                v19 = "0";
                v18 = 2039;
                dir = "source/phys_gjk.cpp";
                if (pai_gjk_initial_support_dir.m_use_warnings_only)
                {
                    PHYS_WARNING(dir, v18, v19, v20);
                }
                else if (_tlAssert(dir, v18, v19, v20))
                {
                    __debugbreak();
                }
            }
            _InterlockedExchangeAdd(&pai_gjk_initial_support_dir.m_hits_total_count, 1u);
            _InterlockedExchangeAdd(&pai_gjk_initial_support_dir.m_hits_frame_count, 1u);
            *result = PHYS_X_VEC;
            return result;
        }
        else
        {
            v8 = v15;
            result->x = v14;
            v9 = v16;
            result->y = v8;
            //v10 = dir;
            result->z = v9;
            //LODWORD(result->w) = v10;
            result->w = 0.0f;
            return result;
        }
    }
}

void __thiscall phys_gjk_info::gjk_cache_update_invalid(const phys_gjk_input *d)
{
    phys_gjk_cache_info *gjk_ci; // eax

    gjk_ci = d->gjk_ci;
    if ( gjk_ci )
    {
        gjk_ci->m_flags |= 1u;
        d->gjk_ci->m_flags &= ~4u;
        d->gjk_ci->m_flags &= ~8u;
    }
}

void phys_gjk_info::gjk_cache_update_test_only_valid(const phys_gjk_input *d)
{
    phys_gjk_cache_info *gjk_ci; // eax
    const phys_vec3 *v4; // eax
    phys_gjk_cache_info *v5; // ecx
    phys_vec3 v6; // [esp-10h] [ebp-1Ch] BYREF
    //int v7; // [esp+0h] [ebp-Ch]
    //void *v8; // [esp+4h] [ebp-8h]
    //void *retaddr; // [esp+Ch] [ebp+0h]
    //
    //v7 = a2;
    //v8 = retaddr;
    gjk_ci = d->gjk_ci;
    if ( gjk_ci )
    {
        gjk_ci->m_flags |= 1u;
        v4 = phys_multiply(&v6, d->cg1_to_world_xform, &this->m_support_dir);
        v5 = d->gjk_ci;
        v5->m_flags |= 4u;
        v5->m_support_dir.x = v4->x;
        v5->m_support_dir.y = v4->y;
        v5->m_support_dir.z = v4->z;
        d->gjk_ci->m_flags &= ~8u;
    }
}

void phys_gjk_info::gjk_cache_update_colliding(const phys_gjk_input *d)
{
    phys_gjk_cache_info *gjk_ci; // eax
    const phys_vec3 *v5; // eax
    phys_gjk_cache_info *v6; // ecx
    phys_gjk_geom *gjk_cg1; // edx
    phys_gjk_geom *gjk_cg2; // [esp-58h] [ebp-64h]
    phys_gjk_cache_info *v9; // [esp-54h] [ebp-60h]
    int m_w_set; // [esp-40h] [ebp-4Ch]
    phys_vec3 v11; // [esp-20h] [ebp-2Ch] BYREF
    phys_vec3 v12; // [esp-10h] [ebp-1Ch] BYREF
    //int v13; // [esp+0h] [ebp-Ch]
    //void *v14; // [esp+4h] [ebp-8h]
    //void *retaddr; // [esp+Ch] [ebp+0h]
    //
    //v13 = a2;
    //v14 = retaddr;
    gjk_ci = d->gjk_ci;
    if ( gjk_ci )
    {
        gjk_ci->m_flags |= 1u;
        v5 = phys_multiply(&v11, d->cg1_to_world_xform, &this->cg1_cinfo_loc.m_n);
        v6 = d->gjk_ci;
        v6->m_flags |= 4u;
        v6->m_support_dir.x = v5->x;
        v6->m_support_dir.y = v5->y;
        v6->m_support_dir.z = v5->z;
        m_w_set = this->m_w_set;
        gjk_cg1 = (phys_gjk_geom *)d->gjk_cg1;
        v9 = d->gjk_ci;
        gjk_cg2 = (phys_gjk_geom *)d->gjk_cg2;
        v12.x = this->cg2_to_cg1_xform.x.y * this->cg1_cinfo_loc.m_n.y
                    + this->cg1_cinfo_loc.m_n.x * this->cg2_to_cg1_xform.x.x
                    + this->cg2_to_cg1_xform.x.z * this->cg1_cinfo_loc.m_n.z;
        v12.y = this->cg2_to_cg1_xform.y.y * this->cg1_cinfo_loc.m_n.y
                    + this->cg2_to_cg1_xform.y.x * this->cg1_cinfo_loc.m_n.x
                    + this->cg2_to_cg1_xform.y.z * this->cg1_cinfo_loc.m_n.z;
        v12.z = this->cg2_to_cg1_xform.z.y * this->cg1_cinfo_loc.m_n.y
                    + this->cg2_to_cg1_xform.z.x * this->cg1_cinfo_loc.m_n.x
                    + this->cg2_to_cg1_xform.z.z * this->cg1_cinfo_loc.m_n.z;
        v11.x = -this->cg1_cinfo_loc.m_n.x;
        v11.y = -this->cg1_cinfo_loc.m_n.y;
        v11.z = -this->cg1_cinfo_loc.m_n.z;
        set_simplex(gjk_cg1, gjk_cg2, v9, &v11, &v12, this->m_a_inds, this->m_b_inds, m_w_set);
    }
}

void __thiscall phys_gjk_info::gjk_cache_update_test_only_penetrating(const phys_gjk_input *d)
{
    phys_gjk_cache_info *gjk_ci; // eax

    gjk_ci = d->gjk_ci;
    if ( gjk_ci )
    {
        gjk_ci->m_flags |= 1u;
        d->gjk_ci->m_flags &= ~8u;
    }
}

const phys_vec3 *phys_gjk_geom::support_only(
                const phys_vec3 *result,
                const phys_mat44 *xform,
                const phys_vec3 *v) const
{
    void(__thiscall * support)(phys_gjk_geom *, const phys_vec3 *, phys_vec3 *, phys_vec3 *); // eax
    int v8; // [esp-30h] [ebp-3Ch] BYREF
    phys_vec3 v9; // [esp-20h] [ebp-2Ch] BYREF
    phys_vec3 v10; // [esp-10h] [ebp-1Ch] BYREF
    //_DWORD v11[3]; // [esp+0h] [ebp-Ch] BYREF
    //_UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]
    //
    //v11[0] = a2;
    //v11[1] = retaddr;
    v10.x = xform->x.y * v->y + xform->x.x * v->x + xform->x.z * v->z;
    v10.y = xform->y.y * v->y + xform->y.x * v->x + xform->y.z * v->z;
    //support = this->support;
    v10.z = xform->z.y * v->y + v->x * xform->z.x + xform->z.z * v->z;
    //((void(__thiscall *)(phys_gjk_geom *, phys_vec3 *, phys_vec3 *, int *, int))support)(this, &v10, &v9, &v8, a3);
    this->support(&v10, &v9, (phys_vec3*)&v8);
    phys_full_multiply((phys_vec3*)result, xform, &v9);
    return result;
}

// local variable allocation has failed, the output may be wrong!
void phys_gjk_info::comp_v(int w_set, phys_vec3 *v)
{
    char v5; // cl
    int v6; // edi
    float *p_y; // eax
    float *v8; // eax
    float *v9; // eax
    double v10; // st7
    double v11; // st7
    double v12; // st7
    int v13; // esi
    float *v14; // edi
    int v15; // esi
    float y; // [esp-Ch] [ebp-ACh] BYREF
    float z; // [esp-8h] [ebp-A8h]
    float w; // [esp-4h] [ebp-A4h]
    phys_vec3 side[3]; // [esp+0h] [ebp-A0h] BYREF
    phys_vec3 list_w_vert[3]; // [esp+30h] [ebp-70h]
    float v21; // [esp+60h] [ebp-40h]
    float v22; // [esp+64h] [ebp-3Ch]
    float v23; // [esp+68h] [ebp-38h]
    double ne1_sq; // [esp+6Ch] [ebp-34h] OVERLAPPED
    float v25; // [esp+74h] [ebp-2Ch]
    float v26; // [esp+78h] [ebp-28h]
    float v27; // [esp+7Ch] [ebp-24h]
    phys_vec3 normal; // [esp+80h] [ebp-20h] BYREF
    float nside_sq[4]; // [esp+90h] [ebp-10h]
    float retaddr; // [esp+A0h] [ebp+0h]

    //nside_sq[1] = a2;
    //nside_sq[2] = retaddr;
    v5 = w_set;
    if ( w_set <= 0 || w_set >= 15 )
    {
        if ( _tlAssert("source/phys_gjk.cpp", 79, "w_set > 0 && w_set < 15", "") )
            __debugbreak();
        v5 = w_set;
    }
    v6 = 0;
    if ( (v5 & 1) != 0 )
    {
        v6 = 1;
        side[2].y = this->m_w_verts[0].x;
        side[2].z = this->m_w_verts[0].y;
        side[2].w = this->m_w_verts[0].z;
    }
    if ( (v5 & 2) != 0 )
    {
        p_y = &side[v6 + 2].y;
        *p_y = this->m_w_verts[1].x;
        ++v6;
        p_y[1] = this->m_w_verts[1].y;
        p_y[2] = this->m_w_verts[1].z;
    }
    if ( (v5 & 4) != 0 )
    {
        v8 = &side[v6 + 2].y;
        *v8 = this->m_w_verts[2].x;
        ++v6;
        v8[1] = this->m_w_verts[2].y;
        v8[2] = this->m_w_verts[2].z;
    }
    if ( (v5 & 8) != 0 )
    {
        v9 = &side[v6 + 2].y;
        *v9 = this->m_w_verts[3].x;
        ++v6;
        v9[1] = this->m_w_verts[3].y;
        v9[2] = this->m_w_verts[3].z;
    }
    if ( (v6 <= 0 || v6 >= 4)
        && _tlAssert("source/phys_gjk.cpp", 87, "list_w_vert_count > 0 && list_w_vert_count < 4", "") )
    {
        __debugbreak();
    }
    if ( v6 == 1 )
    {
        v->x = side[2].y;
        v->y = side[2].z;
        v->z = side[2].w;
        return;
    }
    normal.y = list_w_vert[0].y - side[2].y;
    normal.z = list_w_vert[0].z - side[2].z;
    normal.w = list_w_vert[0].w - side[2].w;
    if ( v6 != 2 )
    {
        if ( v6 != 3 && _tlAssert("source/phys_gjk.cpp", 103, "list_w_vert_count == 3", "") )
            __debugbreak();
        v22 = list_w_vert[1].y - side[2].y;
        v23 = list_w_vert[1].z - side[2].z;
        *(float *)&ne1_sq = list_w_vert[1].w - side[2].w;
        v11 = *(float *)&ne1_sq;
        v25 = *(float *)&ne1_sq * normal.z - v23 * normal.w;
        v26 = normal.w * v22 - *(float *)&ne1_sq * normal.y;
        v27 = v23 * normal.y - normal.z * v22;
        ne1_sq = v27;
        *((float *)&ne1_sq + 1) = v26 * v26 + v25 * v25 + ne1_sq * ne1_sq;
        if ( *((float *)&ne1_sq + 1) > 0.010000001 )
        {
            v12 = *((float *)&ne1_sq + 1);
            *((float *)&ne1_sq + 1) = v26 * side[2].z + v25 * side[2].y + v27 * side[2].w;
            *((float *)&ne1_sq + 1) = *((float *)&ne1_sq + 1) / v12;
            v25 = v25 * *((float *)&ne1_sq + 1);
            v26 = v26 * *((float *)&ne1_sq + 1);
            v27 = *((float *)&ne1_sq + 1) * v27;
            v->x = v25;
            v->y = v26;
            v->z = v27;
            return;
        }
        y = normal.y;
        side[0].y = list_w_vert[1].y - list_w_vert[0].y;
        w = normal.w;
        z = normal.z;
        side[0].x = nside_sq[0];
        side[0].z = list_w_vert[1].z - list_w_vert[0].z;
        side[0].w = list_w_vert[1].w - list_w_vert[0].w;
        side[1].y = -v22;
        side[1].z = -v23;
        side[1].w = -v11;
        normal.z = normal.z * normal.z + normal.y * normal.y + normal.w * normal.w;
        normal.w = side[0].z * side[0].z + side[0].y * side[0].y + side[0].w * side[0].w;
        nside_sq[0] = side[1].z * side[1].z + side[1].y * side[1].y + side[1].w * side[1].w;
        if ( normal.w > (double)normal.z )
        {
            v13 = 1;
            if ( nside_sq[0] <= (double)normal.w )
                goto LABEL_35;
        }
        else if ( nside_sq[0] <= (double)normal.z )
        {
            v13 = 0;
            goto LABEL_35;
        }
        v13 = 2;
LABEL_35:
        v14 = &normal.z + v13;
        if ( *v14 <= 0.0 && _tlAssert("source/phys_gjk.cpp", 137, "nside_sq[side_i] > 0.0f", "") )
            __debugbreak();
        v15 = v13;
        *((float *)&ne1_sq + 1) = side[v15 + 2].z * *(float *)((char *)&z + v15 * 16)
                                                        + side[v15 + 2].y * *(float *)((char *)&y + v15 * 16)
                                                        + side[v15 + 2].w * *(float *)((char *)&w + v15 * 16);
        *((float *)&ne1_sq + 1) = *((float *)&ne1_sq + 1) / *v14;
        v25 = *((float *)&ne1_sq + 1) * *(float *)((char *)&y + v15 * 16);
        v26 = *(float *)((char *)&y + v15 * 16 + 4) * *((float *)&ne1_sq + 1);
        v27 = *((float *)&ne1_sq + 1) * *(float *)((char *)&w + v15 * 16);
        normal.y = side[v15 + 2].y - v25;
        normal.z = side[v15 + 2].z - v26;
        v10 = side[v15 + 2].w;
        goto LABEL_39;
    }
    v21 = normal.z * normal.z + normal.y * normal.y + normal.w * normal.w;
    if ( v21 <= 0.0 )
    {
        if ( _tlAssert("source/phys_gjk.cpp", 98, "ne1_sq > 0.0f", "") )
            __debugbreak();
    }
    *((float *)&ne1_sq + 1) = normal.y * side[2].y + normal.z * side[2].z + normal.w * side[2].w;
    *((float *)&ne1_sq + 1) = *((float *)&ne1_sq + 1) / v21;
    v25 = normal.y * *((float *)&ne1_sq + 1);
    v26 = normal.z * *((float *)&ne1_sq + 1);
    v27 = normal.w * *((float *)&ne1_sq + 1);
    normal.y = side[2].y - v25;
    normal.z = side[2].z - v26;
    v10 = side[2].w;
LABEL_39:
    normal.w = v10 - v27;
    v->x = normal.y;
    v->y = normal.z;
    v->z = normal.w;
}

int phys_gjk_info::init_gjk(
                const phys_gjk_input *d,
                const phys_vec3 *initial_support_dir,
                bool in_separation_loop)
{
    int v6; // eax
    phys_gjk_cache_info *gjk_ci; // ecx
    phys_vec3 *m_b_verts; // edi
    const phys_vec3 *v9; // eax
    double y; // st7
    bool v11; // zf
    int v12; // eax
    int result; // eax
    phys_vec3 v14; // [esp-20h] [ebp-2Ch] BYREF
    int v15; // [esp-8h] [ebp-14h]
    int v16; // [esp-4h] [ebp-10h] BYREF
    //unsigned int v17[2]; // [esp+0h] [ebp-Ch] BYREF
    //_UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]
    //
    //v17[0] = a2;
    //v17[1] = retaddr;
    if ( in_separation_loop )
    {
        v6 = phys_gjk_info::compress_verts(this->m_w_set);
    }
    else
    {
        gjk_ci = d->gjk_ci;
        if ( !gjk_ci || (gjk_ci->m_flags & 8) == 0 )
            goto LABEL_11;
        m_b_verts = this->m_b_verts;
        get_simplex(
            (phys_gjk_geom *)d->gjk_cg1,
            (phys_gjk_geom *)d->gjk_cg2,
            gjk_ci,
            this->m_a_verts,
            this->m_a_inds,
            this->m_b_verts,
            this->m_b_inds,
            &v16);
        v6 = v16;
        if ( v16 > 0 )
        {
            v15 = v16;
            do
            {
                v9 = phys_full_multiply(&v14, &this->cg2_to_cg1_xform, m_b_verts);
                m_b_verts->x = v9->x;
                y = v9->y;
                ++m_b_verts;
                v11 = v15-- == 1;
                m_b_verts[-1].y = y;
                m_b_verts[-1].z = v9->z;
            }
            while ( !v11 );
            v6 = v16;
        }
    }
    if ( v6 )
    {
        v12 = phys_gjk_info::seed_simplex(v6);
        this->m_w_set = v12;
        phys_gjk_info::comp_v(v12, &this->m_support_dir);
        this->m_last_w_set = this->m_w_set;
        return 1;
    }
LABEL_11:
    this->m_support_dir.x = initial_support_dir->x;
    this->m_support_dir.y = initial_support_dir->y;
    result = 0;
    this->m_support_dir.z = initial_support_dir->z;
    this->m_last_w_set = 0;
    this->m_w_set = 0;
    return result;
}

