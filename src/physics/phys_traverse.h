#pragma once

#include <qcommon/cm_trace.h>
#include <qcommon/cm_test.h>
#include <qcommon/common.h>
#include <gfx_d3d/r_material.h>
#include <qcommon/statmonitor.h>

#include "phys_colgeom.h"

extern clipMap_t cm;

struct cbrush_t;
struct CollisionAabbTree;
struct TraceThreadInfo;

union CollisionAabbTreeIndex // sizeof=0x4
{                                                                             // XREF: CollisionAabbTree/r
    int firstChildIndex;
    int partitionIndex;
};

struct CollisionAabbTree // sizeof=0x20
{
    float origin[3];
    unsigned __int16 materialIndex;
    unsigned __int16 childCount;
    float halfSize[3];
    CollisionAabbTreeIndex u;
};

struct col_prim_t // sizeof=0x8
{                                       // XREF: colgeom_visitor_inlined_t<200>/r
                                        // colgeom_visitor_inlined_t<500>/r
    int type;
    //$9DB6BB71550D5B679E9F92FB2EA07EBE ___u1;
    union //$9DB6BB71550D5B679E9F92FB2EA07EBE // sizeof=0x4
    {                                       // XREF: col_prim_t/r
        const CollisionAabbTree *tree;
        const cbrush_t *brush;
    };
};

struct visitor_base_t // sizeof=0x4
{                                                                             // XREF: colgeom_visitor_t/r
        //visitor_base_t_vtbl *__vftable;         // XREF: CG_Vehicle_PreControllers(int,DObj const *,centity_s *)+2AD/w
                                                                                // CG_Vehicle_PreControllers(int,DObj const *,centity_s *):loc_50E81E/r ...
    virtual ~visitor_base_t() = default;
};

struct colgeom_visitor_t : visitor_base_t // sizeof=0x70
{                                                                             // XREF: colgeom_visitor_inlined_t<200>/r
    //struct /*VFT*/ colgeom_visitor_t_vtbl // sizeof=0x18
    //{
    //    void (__thiscall *~colgeom_visitor_t)(struct colgeom_visitor_t *this);
    //    void (__thiscall *prolog)(colgeom_visitor_t *this);
    //    void (__thiscall *epilog)(colgeom_visitor_t *this);
    //    void (__thiscall *visit)(colgeom_visitor_t *this, const cbrush_t *);
    //    void (__thiscall *visit)(colgeom_visitor_t *this, const CollisionAabbTree *);
    //    void (__thiscall *update)(colgeom_visitor_t *this, const float *, const float *, int, const float *);
    //};

    colgeom_visitor_t();
    colgeom_visitor_t(const colgeom_visitor_t *__that);

    virtual ~colgeom_visitor_t() = default;
    virtual void prolog()
    {
        iassert(0);
    }
    virtual void epilog()
    {
        iassert(0);
    }
    virtual void visit(const cbrush_t *) = 0;
    virtual void visit(const CollisionAabbTree *) = 0;
    virtual void update(const float *, const float *, int, const float *) = 0;

    colgeom_visitor_t &operator=(colgeom_visitor_t *that)
    {
        this->m_mn = that->m_mn;
        this->m_mx = that->m_mx;
        this->m_p0 = that->m_p0;
        this->m_p1 = that->m_p1;
        this->m_delta = that->m_delta;
        this->m_rvec = that->m_rvec;
        this->m_radius = that->m_radius;
        this->m_mask = that->m_mask;
        this->m_threadInfo = that->m_threadInfo;

        return *this;
    }

    hybrid_vector m_mn;                                 // XREF: query_brush_model_gjk_geom(ushort,int,gjk_collision_visitor *)+8F/o
    hybrid_vector m_mx;                                 // XREF: query_brush_model_gjk_geom(ushort,int,gjk_collision_visitor *)+8B/o
    hybrid_vector m_p0;
    hybrid_vector m_p1;
    hybrid_vector m_delta;
    hybrid_vector m_rvec;
    float m_radius;
    int m_mask;                                                 // XREF: query_brush_model_gjk_geom(ushort,int,gjk_collision_visitor *)+A3/w
    TraceThreadInfo *m_threadInfo;

    void intersect_box(float *mn, float *mx, int mask);
    void intersect_box_brushes(struct cLeaf_s *leaf);
    void intersect_box_partitions(struct cLeaf_s *leaf);
    void intersect_box_brushnode(struct cLeafBrushNode_s *node);
    void intersect_box_partitions_r(struct CollisionAabbTree *aabbTree);
};

struct query_brush_model_gjk_geom_visitor : colgeom_visitor_t // sizeof=0x74
{                                                                             // XREF: ?query_brush_model_gjk_geom@@YAXGHPAVgjk_collision_visitor@@@Z/r
    struct gjk_collision_visitor *m_allocator; // XREF: query_brush_model_gjk_geom(ushort,int,gjk_collision_visitor *)+A9/w

    void visit(const cbrush_t *brush);
    void visit(const CollisionAabbTree *);
    void update(
        const float *_mn,
        const float *_mx,
        int _mask,
        const float *expand_vec);
};

struct static_colgeom_visitor_t : colgeom_visitor_t // sizeof=0x1078
{
    int ntrees;
    const CollisionAabbTree *trees[512];
    int nbrushes;
    const cbrush_t *brushes[512];

    //00000000 struct /*VFT*/ static_colgeom_visitor_t_vtbl // sizeof=0x18
    //00000000 {
    //00000000     void (__thiscall *~static_colgeom_visitor_t)(struct static_colgeom_visitor_t *this);
    //00000004     void (__thiscall *prolog)(struct static_colgeom_visitor_t *this);
    //00000008     void (__thiscall *epilog)(struct static_colgeom_visitor_t *this);
    //0000000C     void (__thiscall *visit)(struct static_colgeom_visitor_t *this, const cbrush_t *);
    //00000010     void (__thiscall *visit)(struct static_colgeom_visitor_t *this, const CollisionAabbTree *);
    //00000014     void (__thiscall *update)(struct static_colgeom_visitor_t *this, const float *, const float *, int, const float *);
    //00000018 };

    // LWSS ADD constructor 
    static_colgeom_visitor_t()
    {
        ntrees = 0;
        //memset(trees, 0, sizeof(trees));
        nbrushes = 0;
        //memset(brushes, 0, sizeof(brushes));
    }
    virtual ~static_colgeom_visitor_t() = default;

    virtual void prolog()
    {
        ntrees = 0;
        nbrushes = 0;
    }
    virtual void epilog()
    {

    }
    virtual void visit(const cbrush_t *);
    virtual void visit(const CollisionAabbTree *);
    virtual void update(const float *, const float *, int, const float *);

    //void visit(const CollisionAabbTree *tree);
    //void visit(const cbrush_t *brush);
    //void update(
    //    const float *_mn,
    //    const float *_mx,
    //    int mask,
    //    const float *expand_vec);
};


template <int NUM_PRIMS>
struct colgeom_visitor_inlined_t : colgeom_visitor_t // sizeof=0x6B8
{                                                                             // XREF: .data:dummy/r
    int nprims;                                                 // XREF: debug_loop(void)+385/r
    // Rope_CollideWorld(int)+28A/r
    bool overflow;
    // padding byte
    // padding byte
    // padding byte
    col_prim_t prims[NUM_PRIMS];                            // XREF: debug_loop(void)+38C/o
    // Rope_CollideWorld(int)+27E/o

    colgeom_visitor_inlined_t() : colgeom_visitor_t()
    {
        reset();
    }

    colgeom_visitor_inlined_t &operator=(const colgeom_visitor_inlined_t *that)
    {
        colgeom_visitor_t::operator=((colgeom_visitor_t *)that);
        this->nprims = that->nprims;
        this->overflow = that->overflow;

        for (int i = 0; i < NUM_PRIMS; i++)
        {
            this->prims[i].type = that->prims[i].type;
            this->prims[i].tree = that->prims[i].tree;
        }
    }

    void visit(const cbrush_t *brush)
    {
        col_prim_t *prim;

        if (this->nprims < NUM_PRIMS)
        {
            prim = &this->prims[this->nprims++];
            prim->type = 1;
            prim->tree = (const CollisionAabbTree *)brush;
        }
    }

    void visit(const CollisionAabbTree *tree)
    {
        col_prim_t *prim; // [esp+4h] [ebp-4h]

        if (this->nprims < NUM_PRIMS)
        {
            prim = &this->prims[this->nprims++];
            prim->type = 0;
            prim->tree = tree;
        }
    }

    void reset()
    {
        this->nprims = 0;
        this->overflow = 0;

        this->m_mn.vec.v[0] = 9.9999997e37f; // cool float constant, man
        this->m_mn.vec.v[1] = 9.9999997e37f;
        this->m_mn.vec.v[2] = 9.9999997e37f;

        this->m_mx.vec.v[0] = -9.9999997e37f;
        this->m_mx.vec.v[1] = -9.9999997e37f;
        this->m_mx.vec.v[2] = -9.9999997e37f;
    }

    void update(
        const float *_mn,
        const float *_mx,
        int mask,
        const float *expand_vec)
    {
        bool v5; // [esp+0h] [ebp-58h]
        float result[3]; // [esp+18h] [ebp-40h] BYREF
        float b[3]; // [esp+24h] [ebp-34h] BYREF
        float a[3]; // [esp+30h] [ebp-28h] BYREF
        bool inside; // [esp+3Fh] [ebp-19h]
        float mx[3]; // [esp+40h] [ebp-18h] BYREF
        float mn[3]; // [esp+4Ch] [ebp-Ch] BYREF

        a[0] = this->m_mn.vec.v[0] - *_mn;
        a[1] = this->m_mn.vec.v[1] - _mn[1];
        a[2] = this->m_mn.vec.v[2] - _mn[2];
        b[0] = *_mx - this->m_mx.vec.v[0];
        b[1] = _mx[1] - this->m_mx.vec.v[1];
        b[2] = _mx[2] - this->m_mx.vec.v[2];
        Vec3Max(a, b, result);
        v5 = result[0] < 0.0 && result[1] < 0.0 && result[2] < 0.0;
        inside = v5;
        if (this->m_mask != mask || !inside)
        {
            mn[0] = *_mn - *expand_vec;
            mn[1] = _mn[1] - expand_vec[1];
            mn[2] = _mn[2] - expand_vec[2];
            mx[0] = *_mx + *expand_vec;
            mx[1] = _mx[1] + expand_vec[1];
            mx[2] = _mx[2] + expand_vec[2];
            //colgeom_visitor_inlined_t<500>::reset(this);
            reset();
            //colgeom_visitor_t::intersect_box(this, mn, mx, mask);
            //colgeom_visitor_t::intersect_box(mn, mx, mask);
            //static_cast<colgeom_visitor_t *>(this)->intersect_box(mn, mx, mask);
            //this->colgeom_visitor_t::intersect_box(mn, mx, mask);
            intersect_box_HACK(mn, mx, mask);
            //colgeom_visitor_t *base = this;
            //base->intersect_box(mn, mx, mask);  // resolves to colgeom_visitor_t::intersect_box

            if (this->nprims == NUM_PRIMS)
            {
                StatMon_Warning(8, 3000, (char *)"code_warning_collision");
                this->nprims = 0;
                this->overflow = 1;
            }
        }
    }

    void update(
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        int mask)
    {
        float _mn[3]; // [esp+14h] [ebp-60h] BYREF
        float extents_start[3]; // [esp+20h] [ebp-54h] BYREF
        float extents_end[3]; // [esp+2Ch] [ebp-48h] BYREF
        float _mx[3]; // [esp+38h] [ebp-3Ch] BYREF
        float offset[3]; // [esp+44h] [ebp-30h]
        float size[3]; // [esp+50h] [ebp-24h]
        float expand_vec[3]; // [esp+5Ch] [ebp-18h] BYREF
        float fudge[3]; // [esp+68h] [ebp-Ch]

        fudge[0] = 1.0f;//`colgeom_visitor_inlined_t<200 > ::update'::`2'::fFudge;
        fudge[1] = 1.0f;//`colgeom_visitor_inlined_t<200 > ::update'::`2'::fFudge;
        fudge[2] = 1.0f;//`colgeom_visitor_inlined_t<200 > ::update'::`2'::fFudge;
        offset[0] = (float)(0.5 * *mins) + (float)(0.5 * *maxs);
        offset[1] = (float)(0.5 * mins[1]) + (float)(0.5 * maxs[1]);
        offset[2] = (float)(0.5 * mins[2]) + (float)(0.5 * maxs[2]);
        size[0] = *maxs - offset[0];
        size[1] = maxs[1] - offset[1];
        size[2] = maxs[2] - offset[2];
        extents_start[0] = *start + offset[0];
        extents_start[1] = start[1] + offset[1];
        extents_start[2] = start[2] + offset[2];
        extents_end[0] = *end + offset[0];
        extents_end[1] = end[1] + offset[1];
        extents_end[2] = end[2] + offset[2];
        Vec3Min(extents_start, extents_end, _mn);
        Vec3Max(extents_start, extents_end, _mx);
        _mn[0] = _mn[0] - size[0];
        _mn[1] = _mn[1] - size[1];
        _mn[2] = _mn[2] - size[2];
        _mx[0] = _mx[0] + size[0];
        _mx[1] = _mx[1] + size[1];
        _mx[2] = _mx[2] + size[2];
        _mn[0] = _mn[0] - fudge[0];
        _mn[1] = _mn[1] - fudge[1];
        _mn[2] = _mn[2] - fudge[2];
        _mx[0] = _mx[0] + fudge[0];
        _mx[1] = _mx[1] + fudge[1];
        _mx[2] = _mx[2] + fudge[2];
        expand_vec[0] = 70.0f;
        expand_vec[1] = 70.0f;
        expand_vec[2] = 20.0f;
        this->update(_mn, _mx, mask, expand_vec);
    }

#pragma region INTERSECT_HACK_BECAUSE_I_CANT_LINK

    inline void intersect_box_HACK(float *mn, float *mx, int mask)
    {
        cLeaf_s *leaf; // [esp+18h] [ebp-1050h]
        int i; // [esp+1Ch] [ebp-104Ch]
        leafList_s ll; // [esp+20h] [ebp-1048h] BYREF
        //char v8; // [esp+4Ch] [ebp-101Ch] BYREF
        TraceThreadInfo v8; // [esp+4Ch] [ebp-101Ch] BYREF
        _WORD v9[2048]; // [esp+60h] [ebp-1008h] BYREF
        int v10; // [esp+1064h] [ebp-4h]

        v10 = 2048;
        this->m_mn.vec.v[0] = *mn;
        this->m_mn.vec.v[1] = mn[1];
        this->m_mn.vec.v[2] = mn[2];
        this->m_mx.vec.v[0] = *mx;
        this->m_mx.vec.v[1] = mx[1];
        this->m_mx.vec.v[2] = mx[2];
        this->m_mask = mask;

        ll.bounds[0][0] = this->m_mn.vec.v[0];
        ll.bounds[0][1] = this->m_mn.vec.v[1];
        ll.bounds[0][2] = this->m_mn.vec.v[2];
        //*(_QWORD *)&ll.bounds[0][0] = *(_QWORD *)this->m_mn.vec.v;
        //LODWORD(ll.bounds[0][2]) = this->m_mn.vec.u[2];
        ll.bounds[1][0] = this->m_mx.vec.v[0];
        ll.bounds[1][1] = this->m_mx.vec.v[1];
        ll.bounds[1][2] = this->m_mx.vec.v[2];
        ll.count = 0;
        ll.maxcount = 2048;
        ll.list = v9;
        ll.lastLeaf = 0;
        ll.overflowed = 0;
        CM_BoxLeafnums_r(&ll, 0);
        if (ll.count)
        {
            if (ll.overflowed)
                Com_Printf(0, "colgeom_visitor_t::intersect_box: leafList overflow (max %d)\n", 2048);
            this->m_threadInfo = &v8;
            CM_GetTraceThreadInfo(this->m_threadInfo);
            if (this->m_threadInfo->checkcount.partitions || !cm.partitionCount)
            {
                for (i = 0; i < ll.count; ++i)
                {
                    leaf = &cm.leafs[(unsigned __int16)v9[i]];
                    if ((this->m_mask & leaf->brushContents) != 0)
                        intersect_box_brushes_HACK(leaf);
                    if ((this->m_mask & leaf->terrainContents) != 0)
                        intersect_box_partitions_HACK(leaf);
                }
            }
        }
    }


    inline void intersect_box_brushes_HACK(cLeaf_s *leaf)
    {
        if (leaf->leafBrushNode)
        {
            if (this->m_mx.vec.v[0] >= leaf->mins[0]
                && this->m_mx.vec.v[1] >= leaf->mins[1]
                && this->m_mx.vec.v[2] >= leaf->mins[2]
                && leaf->maxs[0] >= this->m_mn.vec.v[0]
                && leaf->maxs[1] >= this->m_mn.vec.v[1]
                && leaf->maxs[2] >= this->m_mn.vec.v[2])
            {
                intersect_box_brushnode_HACK(&cm.leafbrushNodes[leaf->leafBrushNode]);
            }
        }
    }

    inline void intersect_box_partitions_HACK(cLeaf_s *leaf)
    {
        CollisionAabbTree *aabbTree; // [esp+8h] [ebp-8h]
        int k; // [esp+Ch] [ebp-4h]

        for (k = 0; k < leaf->collAabbCount; ++k)
        {
            aabbTree = &cm.aabbTrees[k + leaf->firstCollAabbIndex];
            if ((cm.materials[aabbTree->materialIndex].contentFlags & this->m_mask) != 0)
                intersect_box_partitions_r_HACK(aabbTree);
        }
    }

    inline void intersect_box_brushnode_HACK(cLeafBrushNode_s *node)
    {
        cbrush_t *b; // [esp+14h] [ebp-Ch]
        int i; // [esp+1Ch] [ebp-4h]

        while ((this->m_mask & node->contents) != 0)
        {
            if (node->leafBrushCount)
            {
                if (node->leafBrushCount > 0)
                {
                    for (i = 0; i < node->leafBrushCount; ++i)
                    {
                        b = &cm.brushes[node->data.leaf.brushes[i]];
                        if ((this->m_mask & b->contents) != 0
                            && this->m_mx.vec.v[0] >= b->mins[0]
                            && this->m_mx.vec.v[1] >= b->mins[1]
                            && this->m_mx.vec.v[2] >= b->mins[2]
                            && b->maxs[0] >= this->m_mn.vec.v[0]
                            && b->maxs[1] >= this->m_mn.vec.v[1]
                            && b->maxs[2] >= this->m_mn.vec.v[2])
                        {
                            this->visit(b);
                        }
                    }
                    return;
                }
                intersect_box_brushnode_HACK(node + 1);
            }
            if (this->m_mn.vec.v[node->axis] <= node->data.children.dist)
            {
                if (this->m_mx.vec.v[node->axis] >= node->data.children.dist)
                    intersect_box_brushnode_HACK(&node[node->data.children.childOffset[0]]);
                node += node->data.children.childOffset[1];
            }
            else
            {
                node += node->data.children.childOffset[0];
            }
        }
    }

    inline void intersect_box_partitions_r_HACK(CollisionAabbTree *aabbTree)
    {
        bool v3; // [esp+7h] [ebp-25h]
        int childIndex; // [esp+18h] [ebp-14h]
        int partitionIndex; // [esp+1Ch] [ebp-10h]
        CollisionAabbTree *child; // [esp+20h] [ebp-Ch]
        int checkStamp; // [esp+24h] [ebp-8h]

        if (this->m_mn.vec.v[0] <= (float)(aabbTree->origin[0] + aabbTree->halfSize[0]))
        {
            if (this->m_mn.vec.v[1] <= (float)(aabbTree->origin[1] + aabbTree->halfSize[1]))
            {
                if (this->m_mn.vec.v[2] <= (float)(aabbTree->origin[2] + aabbTree->halfSize[2]))
                    v3 = (float)(aabbTree->origin[0] - aabbTree->halfSize[0]) <= this->m_mx.vec.v[0]
                    && (float)(aabbTree->origin[1] - aabbTree->halfSize[1]) <= this->m_mx.vec.v[1]
                    && (float)(aabbTree->origin[2] - aabbTree->halfSize[2]) <= this->m_mx.vec.v[2];
                else
                    v3 = 0;
            }
            else
            {
                v3 = 0;
            }
        }
        else
        {
            v3 = 0;
        }
        if (v3)
        {
            if (aabbTree->childCount)
            {
                childIndex = 0;
                child = &cm.aabbTrees[aabbTree->u.firstChildIndex];
                while (childIndex < aabbTree->childCount)
                {
                    intersect_box_partitions_r_HACK(child);
                    ++childIndex;
                    ++child;
                }
            }
            else
            {
                partitionIndex = aabbTree->u.firstChildIndex;
                checkStamp = this->m_threadInfo->checkcount.global;
                if (this->m_threadInfo->checkcount.partitions[partitionIndex] != checkStamp)
                {
                    this->m_threadInfo->checkcount.partitions[partitionIndex] = checkStamp;
                    this->visit(aabbTree);
                }
            }
        }
    }
#pragma endregion
};