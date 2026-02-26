#pragma once

#include "phys_local.h"
#include "phys_traverse.h"
#include <xanim/xanim.h>
#include <universal/q_shared.h>
#include <qcommon/cm_trace.h>
#include <qcommon/statmonitor.h>
#include "phys_gjk_collision_detection.h"

#define SURF_TYPECOUNT 31

struct cpose_t;
struct gentity_s;
struct Glass;
struct DynEntityDef;
struct gjk_base_t;
struct cLeafBrushNode_s;
struct cbrush_t;

struct visitor_base_t // sizeof=0x4
{                                                                             // XREF: colgeom_visitor_t/r
        //visitor_base_t_vtbl *__vftable;         // XREF: CG_Vehicle_PreControllers(int,DObj const *,centity_s *)+2AD/w
                                                                                // CG_Vehicle_PreControllers(int,DObj const *,centity_s *):loc_50E81E/r ...
        virtual ~visitor_base_t() = default;
};

struct col_prim_t // sizeof=0x8
{                                       // XREF: colgeom_visitor_inlined_t<200>/r
                                        // colgeom_visitor_inlined_t<500>/r
    int type;
    //$9DB6BB71550D5B679E9F92FB2EA07EBE ___u1;
    union //$9DB6BB71550D5B679E9F92FB2EA07EBE // sizeof=0x4
    {                                       // XREF: col_prim_t/r
        const struct CollisionAabbTree *tree;
        const struct cbrush_t *brush;
    };
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
        colgeom_visitor_t::operator=((colgeom_visitor_t*)that);
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

        this->m_mn.vec.v[0] = 9.9999997e37; // cool float constant, man
        this->m_mn.vec.v[1] = 9.9999997e37;
        this->m_mn.vec.v[2] = 9.9999997e37;

        this->m_mx.vec.v[0] = -9.9999997e37;
        this->m_mx.vec.v[1] = -9.9999997e37;
        this->m_mx.vec.v[2] = -9.9999997e37;
    }

    void intersect_box(float *mn, float *mx, int mask);
    void intersect_box_brushes(cLeaf_s *leaf);
    void intersect_box_brushnode(cLeafBrushNode_s *node);
    void intersect_box_partitions(cLeaf_s *leaf);
    void intersect_box_partitions_r(CollisionAabbTree *aabbTree);

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
            intersect_box(mn, mx, mask);
            if (this->nprims == NUM_PRIMS)
            {
                StatMon_Warning(8, 3000, (char*)"code_warning_collision");
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
};

struct query_brush_model_gjk_geom_visitor : colgeom_visitor_t // sizeof=0x74
{                                                                             // XREF: ?query_brush_model_gjk_geom@@YAXGHPAVgjk_collision_visitor@@@Z/r
        struct gjk_collision_visitor *m_allocator; // XREF: query_brush_model_gjk_geom(ushort,int,gjk_collision_visitor *)+A9/w

        void visit(const cbrush_t *brush);
        void update(
                const float *_mn,
                const float *_mx,
                int _mask,
                const float *expand_vec);
};

const struct cached_simplex_info // sizeof=0x30
{                                                                             // XREF: phys_gjk_cache_info/r
    phys_vec3 m_indices[3];

    //cached_simplex_info& operator=(const cached_simplex_info *__that);
    cached_simplex_info &operator=(const cached_simplex_info &) = default; // using default compiler operator
};

struct __declspec(align(16)) contact_manifold_mesh_point // sizeof=0x20
{
        phys_vec3 m_p;
        phys_vec2 m_contact_p;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
};

struct __declspec(align(16)) phys_contact_manifold // sizeof=0x60
{                                                                             // XREF: phys_contact_manifold_process/r
                                                                                // phys_contact_manifold_process/r
        phys_vec3 m_feature_normal;
        phys_vec3 m_feature_hitp;
        phys_vec3 m_feature_hitn;
        float m_feature_distance_eps;
        float m_sin_feautre_angular_eps_sq;
        int m_close_mesh_point_count;
        phys_memory_heap *m_allocator;
        contact_manifold_mesh_point *m_list_mesh_point;
        int m_list_mesh_point_count;
        contact_manifold_mesh_point **m_list_sorted_mesh_point;
        contact_manifold_mesh_point **m_list_contact_point;
        int m_list_contact_point_count;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        void add_feature_point(const phys_vec3 *p);
        void comp_feature_normal();
        double compute_convex_poly_area();
        double compute_convex_poly_perimeter();
        void generate_convex_poly(const phys_mat44 *contact_mat);
        void generate_convex_poly_internal();
        double __cdecl get_STD_COMP_FEATURE_NORMAL_DISTANCE_EPS(float penetration_t);
        double __cdecl get_STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ(float penetration_t);
        double __cdecl get_STD_GET_FEATURE_DISTANCE_EPS(float penetration_t);
        double __cdecl get_STD_GET_FEATURE_SIN_ANGULAR_EPS_SQ(float penetration_t);
        bool __cdecl rht(const phys_vec2 *e1, const phys_vec2 *e2, float min_length2, float min_sin_sq);
        void __thiscall set_get_feature_params(
            const phys_vec3 *hitp,
            const phys_vec3 *hitn,
            float feature_distance_eps,
            float sin_feautre_angular_eps_sq);
        void xform_and_translate_mesh_points(
            const phys_mat44 *xform,
            const phys_vec3 *translation);
};


struct gjk_base_t : public phys_gjk_geom // sizeof=0x50
{                                                                             // XREF: gjk_brush_t/r
public:
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        phys_vec3 m_aabb_mn_loc;
        phys_vec3 m_aabb_mx_loc;
        unsigned int m_gjk_geom_id;
        const phys_mat44 *m_xform_;
        gjk_base_t *m_next_geom;
        unsigned int m_flags;
        int stype;
        int m_contents;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        static const int FLAG_TEMP_ALLOCATION = 1;

        inline int get_flag(int flag)
        {
                return m_flags & flag;
        }

        virtual ~gjk_base_t()
        {
            iassert(!get_flag(FLAG_TEMP_ALLOCATION));
        }

        gjk_base_t() : phys_gjk_geom()
        {
            this->m_flags = 0;
        }

        ~gjk_base_t();


        unsigned int get_geom_id();
        const phys_mat44 *get_xform();

        int get_contents();
        void set_contents(int contents);

        void set_geom_id_new(unsigned int geom_id);
        void set_xform(const phys_mat44 *xform);

        virtual void comp_aabb_loc();
        virtual unsigned int get_type() const = 0;
        virtual bool is_foot(const phys_vec3 *hit_point) const;
        virtual bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);
        virtual const cbrush_t *get_brush() const;
};

struct gjk_geom_list_t // sizeof=0x8
{                                                                             // XREF: PhysObjUserData/r
                                                                                // ?InitPhysicsObj@GlassShard@@QAE_N_N@Z/r ...
        gjk_base_t *m_first_geom;                     // XREF: DynEntCl_CreatePhysObj(DynEntityDef const *,DynEntityClient *,GfxPlacement const *):loc_5B18F9/w
                                                                                // DynEntPieces_SpawnPhysObj:loc_5BCF29/w ...
        int m_geom_count;                                     // XREF: DynEntCl_CreatePhysObj(DynEntityDef const *,DynEntityClient *,GfxPlacement const *)+D0/w
                                                                                // DynEntPieces_SpawnPhysObj+50/w ...
public:
        int get_geom_count();
        void add_geom(gjk_base_t *geom)
        {
            iassert(geom);
            geom->m_next_geom = this->m_first_geom;
            this->m_first_geom = geom;
            ++this->m_geom_count;
        }
        void comp_aabb_loc(
            phys_vec3 *aabb_mn_loc,
            phys_vec3 *aabb_mx_loc);
};

struct __declspec(align(16)) gjk_aabb_t : gjk_base_t // sizeof=0x80
{
        phys_vec3 m_center_local;
        phys_vec3 m_dims;
        const struct cbrush_t *m_brush;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);

        static gjk_aabb_t *__cdecl create(
                const phys_vec3 *center,
                const phys_vec3 *dims,
                int stype,
                gjk_collision_visitor *allocator);


        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        const phys_vec3 * get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        const cbrush_t * get_brush();
        static void __cdecl destroy(gjk_aabb_t *geom);

        unsigned int get_type() const;
};

struct BrushWrapper // sizeof=0x60
{
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t *sides;
        int axial_cflags[2][3];
        int axial_sflags[2][3];
        unsigned int numverts;
        float (*verts)[3];
        cplane_s *planes;
};

struct PhysGeomInfo // sizeof=0x44
{
        BrushWrapper *brush;
        int type;
        float orientation[3][3];
        float offset[3];
        float halfLengths[3];
};

struct PhysGeomList // sizeof=0xC
{
        unsigned int count;
        PhysGeomInfo *geoms;
        int contents;
};

struct gjk_obb_t : gjk_base_t // sizeof=0xA0
{
        phys_vec3 m_dims;
        phys_mat44 m_xform;

        static gjk_obb_t *__cdecl create(
            const phys_mat44 *xform,
            const phys_vec3 *dims,
            int stype,
            gjk_collision_visitor *allocator);

        void calc_aabb(
            const phys_mat44 *xform,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max);

        static void __cdecl destroy(gjk_obb_t *geom);
        const phys_vec3 *get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman);
        void get_simplex(
            const cached_simplex_info *cache_info,
            int index_count,
            phys_vec3 *simplex_verts,
            phys_vec3 *simplex_inds);
        unsigned int get_type() const;
        bool is_polyhedron();
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
};

struct __declspec(align(8)) gjk_brush_t : gjk_base_t // sizeof=0x60
{
        int nverts;
        const float (*verts)[3];
        const cbrush_t *brush;
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);

        static gjk_brush_t *create(
                const cbrush_t *brush,
                int stype,
                gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        const cbrush_t *get_brush();
        unsigned int get_type() const;
        static void __cdecl destroy(gjk_brush_t *geom);
        const phys_vec3 *get_center(phys_vec3 *result) const;
};

struct CollisionBorder // sizeof=0x1C
{
        float distEq[3];
        float zBase;
        float zSlope;
        float start;
        float length;
};

const struct CollisionPartition // sizeof=0x14
{
        unsigned __int8 triCount;
        unsigned __int8 borderCount;
        // padding byte
        // padding byte
        int firstTri;
        int nuinds;
        int fuind;
        CollisionBorder *borders;
};

union CollisionAabbTreeIndex // sizeof=0x4
{                                                                             // XREF: CollisionAabbTree/r
        int firstChildIndex;
        int partitionIndex;
};

const struct CollisionAabbTree // sizeof=0x20
{
        float origin[3];
        unsigned __int16 materialIndex;
        unsigned __int16 childCount;
        float halfSize[3];
        CollisionAabbTreeIndex u;
};

struct __declspec(align(16)) gjk_partition_t : gjk_base_t // sizeof=0x70
{
        int ninds;
        const float (*verts)[3];
        unsigned __int16 *inds;
        const CollisionPartition *partition;
        const CollisionAabbTree *tree;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);


        static gjk_partition_t *__cdecl create(const CollisionAabbTree *tree, gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind);
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        unsigned int get_type();
        static void __cdecl destroy(gjk_partition_t *geom);

        void support(
            const phys_vec3 *v,
            phys_vec3 *support_vert,
            phys_vec3 *support_ind) const;

        inline unsigned int get_type() const
        {
            return 3;
        }
};

struct gjk_double_sphere_t : gjk_base_t // sizeof=0x90
{
        phys_vec3 m_list_center[2];
        phys_vec3 m_center;
        float m_list_radius[2];
        float m_geom_radius;
        int m_count;

        gjk_double_sphere_t();

        bool is_foot(const phys_vec3 *hit_point);

        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void set_simplex(
                const phys_vec3 *simplex_inds,
                int w_set,
                const phys_vec3 *normal,
                cached_simplex_info *cache_info);
        const phys_vec3 *get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        float get_geom_radius() const;
        bool is_polyhedron();
        static gjk_double_sphere_t *__cdecl create(
                const phys_vec3 *c0,
                const phys_vec3 *c1,
                float r,
                int stype,
                gjk_collision_visitor *allocator);

        unsigned int get_type() const // LWSS: this isnt even a real function?
        {
            return 69;
        }

        static void __cdecl destroy(gjk_double_sphere_t *geom);

};

struct gjk_cylinder_t : gjk_base_t // sizeof=0xA0
{
        int direction;
        float halfHeight;
        float radius;
        float m_geom_radius;
        phys_mat44 xform;

        bool is_foot(const phys_vec3 *hit_point);

        static gjk_cylinder_t *__cdecl create(
                int _direction,
                float _halfHeight,
                float _radius,
                const phys_mat44 *_xform,
                int stype,
                gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
        const phys_vec3 * get_dims(phys_vec3 *result) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        const phys_vec3 * get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform_,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        unsigned int get_type() const;
        float get_geom_radius() const;
        static void __cdecl destroy(gjk_cylinder_t *geom);
};

struct gjk_unique_id_database_t // sizeof=0x4
{                                                                             // XREF: .data:gjk_unique_id_database_t g_gjk_unique_id_database/r
        volatile unsigned int m_counter;


        unsigned int get_unique_id();
};

struct __declspec(align(8)) gjk_polygon_cylinder_t : gjk_base_t // sizeof=0x80
{                                                                             // XREF: ?render_gjkcc_collision@@YAXAAY02$$CBM00@Z/r
        phys_vec3 m_center;
        float m_polygon_cylinder_radius;
        float m_capsule_radius;
        float m_half_height;
        float m_geom_radius;
        float m_head_offset;
        float m_foot_offset;
        int m_mode;                                                 // XREF: render_gjkcc_collision(float const (&)[3],float const (&)[3],float const (&)[3])+10/w
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        struct poly_verts // sizeof=0x20
        {                                                                             // XREF: .data:poly_verts s_poly_verts/r
                float m_co[4];
                float m_si[4];

                poly_verts();
                //void poly_verts::poly_verts(poly_verts *this);

                //void get_co_si(int i, float *co_, float *si_);
                //void __thiscall gjk_polygon_cylinder_t::poly_verts::get_co_si(int i, float *co_, float *si_)
                inline void __thiscall get_co_si(int i, float *co_, float *si_)
                {
#if 0
                    int ii; // [esp+8h] [ebp-4h]

                    if ((unsigned int)i >= 0xC
                        && !Assert_MyHandler(
                            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                            802,
                            0,
                            "%s",
                            "i >= 0 && i < NUM_VERTS_"))
                    {
                        __debugbreak();
                    }
                    ii = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::a[i / 3]
                        + i * `gjk_polygon_cylinder_t: : poly_verts::get_co_si'::`2'::b[i / 3];
                    *co_ = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::cos[i / 3] * this->m_co[ii];
                    *si_ = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::sis[i / 3] * this->m_si[ii];
#endif

                    static constexpr int NUM_VERTS_ = 12;
                    static constexpr int VERTS_PER_FACE = 3;

                    static constexpr float sis[4] = { 1.0f,  1.0f, -1.0f, -1.0f };
                    static constexpr float cosv[4] = { 1.0f, -1.0f, -1.0f,  1.0f };
                    static constexpr int a[4] = { 0, 6, -6, 12 };
                    static constexpr int b[4] = { 1, -1, 1, -1 };

                    iassert(i >= 0 && i < NUM_VERTS_);

                    const int face = i / VERTS_PER_FACE;
                    const int ii = a[face] + i * b[face];

                    *co_ = cosv[face] * m_co[ii];
                    *si_ = sis[face] * m_si[ii];
                }
                //int __thiscall support(const phys_vec3 *v);
                //int gjk_polygon_cylinder_t::poly_verts::support(const phys_vec3 *v)
                inline int support(const phys_vec3 *v)
                {
                    // aislop
                    float at;

                    // atan2(y, x)
                    at = atan2f(v->y, v->x);

                    // wrap to [0, 2PI)
                    if (at < 0.0f)
                        at += 2.0f * 3.1415927f;

                    // map angle to vertex index
                    int best_i = (int)((12.0f * at) / (2.0f * 3.1415927f) + 0.5f) % 12;

                    if (best_i < 0 || best_i >= 12)
                    {
                        Assert_MyHandler(
                            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                            791,
                            0,
                            "%s",
                            "best_i >= 0 && best_i < NUM_VERTS_");
                        __debugbreak();
                    }

                    return best_i;
                }
        };

        static poly_verts s_poly_verts;

        static gjk_polygon_cylinder_t *__cdecl create(
                float (*mins)[3],
                float (*maxs)[3],
                float radius_adjust,
                int stype,
                gjk_collision_visitor *allocator);
        static void __cdecl destroy(gjk_polygon_cylinder_t *geom);

        
        inline void support(
            const phys_vec3 *v,
            phys_vec3 *support_vert,
            phys_vec3 *support_ind) const
        {
            float v5; // [esp-Ch] [ebp-11Ch]
            float v6; // [esp-8h] [ebp-118h]
            float v7; // [esp-4h] [ebp-114h]
            float v8; // [esp+14h] [ebp-FCh]
            float v9; // [esp+18h] [ebp-F8h]
            float v10; // [esp+1Ch] [ebp-F4h]
            float v11; // [esp+3Ch] [ebp-D4h]
            float v12; // [esp+74h] [ebp-9Ch]
            float v13; // [esp+78h] [ebp-98h]
            float v14; // [esp+7Ch] [ebp-94h]
            float v15; // [esp+A4h] [ebp-6Ch]
            float v16; // [esp+A8h] [ebp-68h]
            float v17; // [esp+ACh] [ebp-64h]
            float v18; // [esp+E4h] [ebp-2Ch] BYREF
            float v19; // [esp+E8h] [ebp-28h] BYREF
            int v20; // [esp+ECh] [ebp-24h]
            float co_; // [esp+F0h] [ebp-20h]
            float si_; // [esp+F4h] [ebp-1Ch]
            int foot_or_head_ind; // [esp+F8h] [ebp-18h]
            float foot_or_head_z; // [esp+FCh] [ebp-14h]
            gjk_polygon_cylinder_t *thisptr; // [esp+100h] [ebp-10h]
            int v26; // [esp+104h] [ebp-Ch]
            //int best_i; // [esp+108h] [ebp-8h]
            //const gjk_polygon_cylinder_t *thisa; // [esp+10Ch] [ebp-4h] BYREF
            //int retaddr; // [esp+110h] [ebp+0h]
            //
            //v26 = a2;
            //best_i = retaddr;
            thisptr = this;
            if (this->m_mode)
            {
                if (thisptr->m_half_height < thisptr->m_capsule_radius
                    && !Assert_MyHandler(
                        "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                        859,
                        0,
                        "%s",
                        "m_half_height >= m_capsule_radius"))
                {
                    __debugbreak();
                }
                v11 = thisptr->m_half_height - thisptr->m_capsule_radius;
                v8 = thisptr->m_center.x + 0.0;
                v9 = thisptr->m_center.y + 0.0;
                v10 = thisptr->m_center.z + v11;
                v5 = thisptr->m_center.x - 0.0;
                v6 = thisptr->m_center.y - 0.0;
                v7 = thisptr->m_center.z - v11;
                if ((float)((float)((float)((float)(v8 - v5) * v->x) + (float)((float)(v9 - v6) * v->y))
                    + (float)((float)(v10 - v7) * v->z)) < 0.0)
                {
                    LODWORD(support_ind->x) = 1;
                    support_vert->x = v5;
                    support_vert->y = v6;
                    support_vert->z = v7;
                }
                else
                {
                    support_ind->x = 0.0;
                    support_vert->x = v8;
                    support_vert->y = v9;
                    support_vert->z = v10;
                }
            }
            else
            {
                //foot_or_head_z = COERCE_FLOAT(gjk_polygon_cylinder_t::poly_verts::support(&gjk_polygon_cylinder_t::s_poly_verts,(VDCArgs *)&thisa, v));
                foot_or_head_z = (float)gjk_polygon_cylinder_t::s_poly_verts.support(v);
                foot_or_head_ind = SLODWORD(v->z);
                if (*(float *)&foot_or_head_ind < 0.0)
                {
                    LODWORD(support_ind->x) = LODWORD(foot_or_head_z) + 12;
                    si_ = (-(thisptr->m_half_height)) + thisptr->m_foot_offset;
                    co_ = -thisptr->m_half_height;
                    v20 = 25;
                }
                else
                {
                    support_ind->x = foot_or_head_z;
                    si_ = thisptr->m_half_height - thisptr->m_head_offset;
                    co_ = thisptr->m_half_height;
                    v20 = 24;
                }

                //gjk_polygon_cylinder_t::poly_verts::get_co_si(
                //    &gjk_polygon_cylinder_t::s_poly_verts,
                //    SLODWORD(foot_or_head_z),
                //    &v18,
                //    &v19);

                gjk_polygon_cylinder_t::s_poly_verts.get_co_si(SLODWORD(foot_or_head_z), &v18, &v19);

                v15 = (float)(thisptr->m_polygon_cylinder_radius * v18) + thisptr->m_center.x;
                v16 = (float)(thisptr->m_polygon_cylinder_radius * v19) + thisptr->m_center.y;
                v17 = si_ + thisptr->m_center.z;
                v12 = thisptr->m_center.x + 0.0;
                v13 = thisptr->m_center.y + 0.0;
                v14 = co_ + thisptr->m_center.z;
                if ((float)((float)((float)((float)(v15 - v12) * v->x) + (float)((float)(v16 - v13) * v->y))
                    + (float)((float)(v17 - v14) * v->z)) < 0.0)
                {
                    LODWORD(support_ind->x) = v20;
                    support_vert->x = v12;
                    support_vert->y = v13;
                    support_vert->z = v14;
                }
                else
                {
                    support_vert->x = v15;
                    support_vert->y = v16;
                    support_vert->z = v17;
                }
            }
        }

        inline void get_simplex(
            const cached_simplex_info *cache_info,
            int index_count,
            phys_vec3 *simplex_verts,
            phys_vec3 *simplex_inds)
        {
            phys_vec3 *v6; // eax
            phys_vec3 *v7; // eax
            phys_vec3 *v8; // edx
            float v9; // [esp-60h] [ebp-1A8h]
            float v10; // [esp-5Ch] [ebp-1A4h]
            float v11; // [esp-30h] [ebp-178h]
            float v12; // [esp-2Ch] [ebp-174h]
            float x; // [esp-Ch] [ebp-154h]
            int j; // [esp-8h] [ebp-150h]
            float v15; // [esp+4h] [ebp-144h]
            float v16; // [esp+20h] [ebp-128h]
            float v17; // [esp+24h] [ebp-124h]
            phys_vec3 *v18; // [esp+58h] [ebp-F0h]
            float v19; // [esp+60h] [ebp-E8h]
            float v20; // [esp+64h] [ebp-E4h]
            float v21; // [esp+A0h] [ebp-A8h]
            float v22; // [esp+A4h] [ebp-A4h]
            float v23; // [esp+DCh] [ebp-6Ch] BYREF
            float v24[2]; // [esp+E0h] [ebp-68h] BYREF
            phys_vec3 *v25; // [esp+E8h] [ebp-60h]
            float v26; // [esp+ECh] [ebp-5Ch]
            int ind1; // [esp+F0h] [ebp-58h]
            float v28; // [esp+F4h] [ebp-54h]
            float v29; // [esp+FCh] [ebp-4Ch]
            int v30; // [esp+100h] [ebp-48h]
            float v31; // [esp+104h] [ebp-44h]
            int p_m_center; // [esp+108h] [ebp-40h]
            float v33; // [esp+10Ch] [ebp-3Ch]
            float v34; // [esp+110h] [ebp-38h]
            float v35; // [esp+114h] [ebp-34h]
            float v36; // [esp+11Ch] [ebp-2Ch]
            float v37; // [esp+120h] [ebp-28h]
            float v38; // [esp+124h] [ebp-24h]
            float v39; // [esp+128h] [ebp-20h] BYREF
            float v40; // [esp+12Ch] [ebp-1Ch] BYREF
            float v41; // [esp+130h] [ebp-18h]
            int co_; // [esp+134h] [ebp-14h]
            gjk_polygon_cylinder_t *si_; // [esp+138h] [ebp-10h]
            int ind; // [esp+13Ch] [ebp-Ch]
            //int i; // [esp+140h] [ebp-8h]
            //int retaddr; // [esp+148h] [ebp+0h]
            //
            //ind = a2;
            //i = retaddr;
            si_ = this;
            if (this->m_mode)
            {
                if (si_->m_half_height < si_->m_capsule_radius
                    && !Assert_MyHandler(
                        "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                        911,
                        0,
                        "%s",
                        "m_half_height >= m_capsule_radius"))
                {
                    __debugbreak();
                }
                v15 = si_->m_half_height - si_->m_capsule_radius;
                for (j = 0; j < index_count; ++j)
                {
                    x = cache_info->m_indices[j].x;
                    if (LODWORD(x) >= 2
                        && _tlAssert(
                            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                            916,
                            "ind == 0 || ind == 1",
                            ""))
                    {
                        __debugbreak();
                    }
                    simplex_inds[j].x = x;
                    if (x == 0.0)
                    {
                        v11 = si_->m_center.y + 0.0;
                        v12 = si_->m_center.z + v15;
                        v8 = &simplex_verts[j];
                        v8->x = si_->m_center.x + 0.0;
                        v8->y = v11;
                        v8->z = v12;
                    }
                    else
                    {
                        v9 = si_->m_center.y - 0.0;
                        v10 = si_->m_center.z - v15;
                        simplex_verts[j].x = si_->m_center.x - 0.0;
                        simplex_verts[j].y = v9;
                        simplex_verts[j].z = v10;
                    }
                }
            }
            else
            {
                for (co_ = 0; co_ < index_count; ++co_)
                {
                    v41 = cache_info->m_indices[co_].x;
                    if (v41 < 0.0
                        && _tlAssert("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 883, "ind >= 0", ""))
                    {
                        __debugbreak();
                    }
                    simplex_inds[co_].x = v41;
                    if (SLODWORD(v41) >= 12)
                    {
                        if (SLODWORD(v41) >= 24)
                        {
                            if (LODWORD(v41) == 24)
                            {
                                v19 = si_->m_center.y + 0.0;
                                v20 = si_->m_half_height + si_->m_center.z;
                                v18 = &simplex_verts[co_];
                                v18->x = si_->m_center.x + 0.0;
                                v18->y = v19;
                                v18->z = v20;
                            }
                            else
                            {
                                if (LODWORD(v41) != 25)
                                {
                                    if (_tlAssert(
                                        "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                                        904,
                                        "ind == 2 * s_poly_verts.get_num_verts() + 1",
                                        ""))
                                    {
                                        __debugbreak();
                                    }
                                }
                                v16 = si_->m_center.y + 0.0;
                                v17 = (-(si_->m_half_height)) + si_->m_center.z;
                                v7 = &simplex_verts[co_];
                                v7->x = si_->m_center.x + 0.0;
                                v7->y = v16;
                                v7->z = v17;
                            }
                        }
                        else
                        {
                            LODWORD(v24[1]) = LODWORD(v41) - 12;

                            //gjk_polygon_cylinder_t::poly_verts::get_co_si(
                            //    &gjk_polygon_cylinder_t::s_poly_verts,
                            //    LODWORD(v41) - 12,
                            //    &v23,
                            //    v24);
                            gjk_polygon_cylinder_t::s_poly_verts.get_co_si(LODWORD(v41) - 12, &v23, v24);

                            v21 = (float)(si_->m_polygon_cylinder_radius * v24[0]) + si_->m_center.y;
                            v22 = (float)((-(si_->m_half_height)) + si_->m_foot_offset) + si_->m_center.z;
                            v6 = &simplex_verts[co_];
                            v6->x = (float)(si_->m_polygon_cylinder_radius * v23) + si_->m_center.x;
                            v6->y = v21;
                            v6->z = v22;
                        }
                    }
                    else
                    {
                        //gjk_polygon_cylinder_t::poly_verts::get_co_si(&gjk_polygon_cylinder_t::s_poly_verts, SLODWORD(v41), &v39, &v40);
                        gjk_polygon_cylinder_t::s_poly_verts.get_co_si(SLODWORD(v41), &v39, &v40);
                        v38 = si_->m_polygon_cylinder_radius * v39;
                        v37 = si_->m_polygon_cylinder_radius * v40;
                        v36 = si_->m_half_height - si_->m_head_offset;
                        v33 = v38;
                        v34 = v37;
                        v35 = v36;
                        p_m_center = (int)&si_->m_center;
                        v31 = v38 + si_->m_center.x;
                        *(float *)&v30 = v37 + si_->m_center.y;
                        v29 = v36 + si_->m_center.z;
                        v26 = v31;
                        ind1 = v30;
                        v28 = v29;
                        v25 = &simplex_verts[co_];
                        v25->x = v31;
                        LODWORD(v25->y) = ind1;
                        v25->z = v28;
                    }
                }
            }
        }

        static inline void calc_disc_aabb(
            const phys_vec3 *axis,
            float radius,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max)
        {
            float v4; // [esp-88h] [ebp-A8h]
            float v5; // [esp-84h] [ebp-A4h]
            float v6; // [esp-20h] [ebp-40h]
            float v7; // [esp-14h] [ebp-34h]
            float v8; // [esp-10h] [ebp-30h]

            v8 = axis->x * axis->x;
            v7 = axis->y * axis->y;
            v6 = axis->z * axis->z;
            aabb_max->x = radius * sqrtf(v7 + v6);
            aabb_max->y = radius * sqrtf(v8 + v6);
            aabb_max->z = radius * sqrtf(v8 + v7);
            v4 = -aabb_max->y;
            v5 = -aabb_max->z;
            aabb_min->x = -aabb_max->x;
            aabb_min->y = v4;
            aabb_min->z = v5;
        }

        inline void calc_aabb(
            const phys_mat44 *xform,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max)
        {
            const phys_vec3 *v5; // eax
            float v6; // [esp+10h] [ebp-188h]
            float v7; // [esp+14h] [ebp-184h]
            float v8; // [esp+50h] [ebp-148h]
            float v9; // [esp+54h] [ebp-144h]
            float m_geom_radius; // [esp+8Ch] [ebp-10Ch]
            phys_vec3 v11; // [esp+ACh] [ebp-ECh] OVERLAPPED BYREF
            phys_vec3 center; // [esp+BCh] [ebp-DCh] BYREF
            float v13; // [esp+CCh] [ebp-CCh]
            float v14; // [esp+D0h] [ebp-C8h]
            float v15; // [esp+D4h] [ebp-C4h]
            float v16; // [esp+DCh] [ebp-BCh]
            float v17; // [esp+E0h] [ebp-B8h]
            float v18; // [esp+E4h] [ebp-B4h]
            const phys_vec3 *v19; // [esp+E8h] [ebp-B0h]
            phys_vec3 v20; // [esp+ECh] [ebp-ACh] BYREF
            float v21; // [esp+FCh] [ebp-9Ch]
            float v22; // [esp+100h] [ebp-98h]
            float v23; // [esp+104h] [ebp-94h]
            float v24; // [esp+10Ch] [ebp-8Ch]
            float v25; // [esp+110h] [ebp-88h]
            float v26; // [esp+114h] [ebp-84h]
            const phys_vec3 *v27; // [esp+118h] [ebp-80h]
            phys_vec3 v28; // [esp+11Ch] [ebp-7Ch] BYREF
            phys_vec3 bot; // [esp+12Ch] [ebp-6Ch] BYREF
            float v30; // [esp+148h] [ebp-50h]
            float v31; // [esp+14Ch] [ebp-4Ch]
            float v32; // [esp+150h] [ebp-48h]
            float negaHalfHeight; // [esp+154h] [ebp-44h]
            const phys_vec3 *v34; // [esp+158h] [ebp-40h]
            phys_vec3 v35; // [esp+15Ch] [ebp-3Ch] BYREF
            float top_12; // [esp+174h] [ebp-24h]
            float v37; // [esp+178h] [ebp-20h]
            float v38; // [esp+17Ch] [ebp-1Ch]
            float m_half_height; // [esp+180h] [ebp-18h]
            const phys_vec3 *p_z; // [esp+184h] [ebp-14h]
            gjk_polygon_cylinder_t *thisa; // [esp+188h] [ebp-10h]
            //int v42; // [esp+18Ch] [ebp-Ch]
            //void *xforma; // [esp+190h] [ebp-8h]
            //void *aabb_maxa; // [esp+198h] [ebp+0h]
            //
            //v42 = a2;
            //xforma = aabb_maxa;
            thisa = this;
            gjk_polygon_cylinder_t::calc_disc_aabb(&xform->z, this->m_polygon_cylinder_radius, aabb_min, aabb_max);
            p_z = &xform->z;
            m_half_height = thisa->m_half_height;
            v38 = m_half_height * xform->z.x;
            v37 = m_half_height * xform->z.y;
            top_12 = m_half_height * xform->z.z;
            v35.x = v38;
            v35.y = v37;
            v35.z = top_12;
            v34 = &xform->z;
            negaHalfHeight = -thisa->m_half_height;
            v32 = negaHalfHeight * xform->z.x;
            v31 = negaHalfHeight * xform->z.y;
            v30 = negaHalfHeight * xform->z.z;
            bot.x = v32;
            bot.y = v31;
            bot.z = v30;
            v27 = phys_min(&v28, &v35, &bot);
            v26 = aabb_min->x + v27->x;
            v25 = aabb_min->y + v27->y;
            v24 = aabb_min->z + v27->z;
            v21 = v26;
            v22 = v25;
            v23 = v24;
            aabb_min->x = v26;
            aabb_min->y = v22;
            aabb_min->z = v23;
            v19 = phys_max(&v20, &v35, &bot);
            v18 = aabb_max->x + v19->x;
            v17 = aabb_max->y + v19->y;
            v16 = aabb_max->z + v19->z;
            v13 = v18;
            v14 = v17;
            v15 = v16;
            aabb_max->x = v18;
            aabb_max->y = v14;
            aabb_max->z = v15;
            if (thisa->m_half_height < thisa->m_head_offset
                && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                    961,
                    0,
                    "%s",
                    "m_head_offset <= m_half_height"))
            {
                __debugbreak();
            }
            if (thisa->m_half_height < thisa->m_foot_offset
                && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                    962,
                    0,
                    "%s",
                    "m_foot_offset <= m_half_height"))
            {
                __debugbreak();
            }
            if (thisa->m_polygon_cylinder_radius < thisa->m_capsule_radius
                && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                    972,
                    0,
                    "%s",
                    "m_capsule_radius <= m_polygon_cylinder_radius"))
            {
                __debugbreak();
            }
            v5 = phys_multiply(&center, xform, &thisa->m_center);
            //operator+(&v11, v5, &xform->w);
            v11 = xform->w + *v5;
            m_geom_radius = thisa->m_geom_radius;
            v8 = (float)(aabb_min->y + v11.y) - m_geom_radius;
            v9 = (float)(aabb_min->z + v11.z) - m_geom_radius;
            aabb_min->x = (float)(aabb_min->x + v11.x) - m_geom_radius;
            aabb_min->y = v8;
            aabb_min->z = v9;
            v6 = (float)(aabb_max->y + v11.y) + m_geom_radius;
            v7 = (float)(aabb_max->z + v11.z) + m_geom_radius;
            aabb_max->x = (float)(aabb_max->x + v11.x) + m_geom_radius;
            aabb_max->y = v6;
            aabb_max->z = v7;
        }

        inline bool is_foot(const phys_vec3 *hit_point) const
        {
            static float offset_0;
            //if (this->m_mode)
            //    return (float)((float)((float)(this->m_center.z - this->m_half_height) + this->m_capsule_radius) - 1.0) >= hit_point->z;
            //
            //static int _S2 = 0;
            //if ((_S2 & 1) == 0)
            //{
            //    _S2 |= 1u;
            //    //__libm_sse2_sin(*(long double *)&thisa);
            //    offset_0 = this->m_geom_radius * (float)((float)(30.0 * 3.1415927) / 180.0);
            //}
            //return (float)((float)((float)(this->m_center.z - this->m_half_height) + this->m_foot_offset) - offset_0) >= hit_point->z;

            // aislop rewrote
            const float bottom_z = m_center.z - m_half_height;

            if (m_mode)
            {
                // Capsule-style foot
                return hit_point->z <= (bottom_z + m_capsule_radius - 1.0f);
            }

            // Cylinder-style foot with beveled edge (30 degrees)
            constexpr float sin_30 = 0.5f;
            const float bevel_offset = m_geom_radius * sin_30;

            return hit_point->z <= (bottom_z + m_foot_offset - bevel_offset);
        }
        inline unsigned int get_type() const
        {
            return 7;
        }

        float get_geom_radius() const
        {
            if (this->m_mode)
                return this->m_geom_radius + this->m_capsule_radius;
            else
                return this->m_geom_radius;
        }

        //gjk_polygon_cylinder_t::poly_verts gjk_polygon_cylinder_t::s_poly_verts
};

PhysGeomList *__cdecl xmodel_get_geomlist(const XModel *model, int bone_index);

void    phys_aabb_add_point(
        const phys_mat44 *xform,
        const phys_vec3 *point,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);

void    phys_aabb_init_sphere(
                const phys_mat44 *xform,
                const phys_vec3 *center,
                float radius,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
void    phys_aabb_add_sphere(
                const phys_mat44 *xform,
                const phys_vec3 *center,
                float radius,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);


void    setup_gjk_polygon_cylinder(
                float *mins,
                float *maxs,
                float radius_adjust,
                gjk_polygon_cylinder_t *gjk_cylinder);

gjk_aabb_t * create_aabb_gjk_geom(
                const float *mn,
                const float *mx,
                int stype,
                gjk_collision_visitor *allocator);
gjk_obb_t * create_obb_gjk_geom(
                float (*orientation)[3],
                float *offset,
                float *halfLengths,
                int stype,
                gjk_collision_visitor *allocator);
gjk_brush_t *__cdecl create_brush_gjk_geom(const cbrush_t *brush, int stype, gjk_collision_visitor *allocator);
gjk_double_sphere_t * create_capsule_gjk_geom(
                float *center,
                float radius,
                float halfHeight,
                unsigned int direction,
                int stype,
                gjk_collision_visitor *allocator);
gjk_cylinder_t * create_cylinder_gjk_geom(
                float (*rot)[3],
                float *trans,
                float radius,
                float halfHeight,
                int stype,
                gjk_collision_visitor *allocator);
void __cdecl create_brush_model_gjk_geom_r(
                cLeafBrushNode_s *node,
                int *index_base,
                gjk_collision_visitor *allocator,
                int contents_mask);
void __cdecl query_brush_model_gjk_geom(
                unsigned __int16 brushModel,
                int contents_mask,
                gjk_collision_visitor *allocator);
void __cdecl create_brush_model_gjk_geom(
                unsigned __int16 brushModel,
                int contents_mask,
                gjk_collision_visitor *allocator);
void __cdecl create_xmodel_gjk_geom(
                const XModel *model,
                gjk_collision_visitor *allocator,
                int bone_index,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                int contents_for_proxy_collision,
                phys_mat44 *worldMat);
void __cdecl create_gjk_geom(
                int localClientNum,
                const centity_s *cent,
                gjk_collision_visitor *allocator,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                bool b_use_smallerbox_for_characters);
void __cdecl create_gjk_dobj_geom(
                DObj *obj,
                const cpose_t *pose,
                gentity_s *gent,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                int contents_for_proxy_collision,
                gjk_collision_visitor *allocator);
void __cdecl create_gjk_geom(
                gentity_s *gent,
                gjk_collision_visitor *allocator,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box);


void __cdecl create_gjk_geom(const Glass *glass, gjk_collision_visitor *allocator, unsigned int brush_mask);
void __cdecl create_gjk_geom(const DynEntityDef *dynEntDef, gjk_collision_visitor *allocator, unsigned int brush_mask);

void __cdecl destroy_gjk_geom(gjk_geom_list_t *gjk_geom_list);


extern create_gjk_geom_collision_visitor g_empty_collision_visitor;

extern gjk_unique_id_database_t g_gjk_unique_id_database;