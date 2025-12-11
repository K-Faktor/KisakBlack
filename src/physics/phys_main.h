#pragma once
#include "rigid_body.h"
#include "phys_colgeom.h"
#include "phys_gjk_collision_detection.h"

enum PhysicsGeomType : __int32
{                                       // XREF: BoneDef/r
                                        // ?Phys_CreateUserBody@@YAHQAMHW4PhysicsGeomType@@@Z/r
    PHYS_GEOM_NONE           = 0x0,
    PHYS_GEOM_BOX            = 0x1,
    PHYS_GEOM_BRUSH          = 0x2,
    PHYS_GEOM_CYLINDER       = 0x3,
    PHYS_GEOM_CYLINDER_LARGE = 0x4,
    PHYS_GEOM_CAPSULE        = 0x5,
    PHYS_GEOM_POINT          = 0x6,
    PHYS_GEOM_COUNT          = 0x7,
};
inline PhysicsGeomType &operator++(PhysicsGeomType &t)
{
    t = static_cast<PhysicsGeomType>((static_cast<int>(t) + 1));
    return t;
}
inline PhysicsGeomType operator++(PhysicsGeomType &t, int)
{
    PhysicsGeomType old = t;
    t = static_cast<PhysicsGeomType>((static_cast<int>(t) + 1));
    return old;
}

enum Phys_SurfaceType : __int32
{                                       // XREF: Phys_GetSoundAliasIndex/r
    EFFECT_SURF_TYPE_HARD    = 0x0,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_SOFT    = 0x1,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_METAL   = 0x2,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_GLASS   = 0x3,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_WATER   = 0x4,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_WOOD    = 0x5,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_FOLIAGE = 0x6,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_FLESH   = 0x7,     // XREF: .data:remap_table/s
    EFFECT_SURF_TYPE_COUNT   = 0x8,
};
inline Phys_SurfaceType &operator++(Phys_SurfaceType &t)
{
    t = static_cast<Phys_SurfaceType>((static_cast<int>(t) + 1));
    return t;
}
inline Phys_SurfaceType operator++(Phys_SurfaceType &t, int)
{
    Phys_SurfaceType old = t;
    t = static_cast<Phys_SurfaceType>((static_cast<int>(t) + 1));
    return old;
}

struct BodyState // sizeof=0x88
{                                       // XREF: Phys_ObjCreateAxis/r
    float position[3];                  // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+1A5/w
    float rotation[3][3];               // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+18B/o
    float velocity[3];                  // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+1D4/w
    float angVelocity[3];               // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+1FB/w
    float centerOfMassOffset[3];        // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+222/w
    float buoyancyBoxMin[3];            // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+249/w
    float buoyancyBoxMax[3];            // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+270/w
    float mass;                         // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+297/w
    float friction;                     // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+2B1/w
    float bounce;                       // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+2A4/w
    int timeLastAsleep;                 // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+2BC/w
    int id;                             // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+304/w
    int buoyancy;                       // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+2DE/w
    int underwater;                     // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *):loc_81FDAD/w
};

struct PhysObjUserData // sizeof=0x130
{                                       // XREF: phys_free_list<PhysObjUserData>::T_internal/r
    rigid_body *body;
    struct NitrousVehicle *vehicle;
    gjk_geom_list_t m_gjk_geom_list;
    PhysObjUserData *m_next_link;
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
    phys_mat44 cg2rb;
    phys_mat44 cg2w;
    broad_phase_base *m_bpb;
    float savedPos[3];
    float savedRot[3][3];
    float friction;
    float bounce;
    int id;
    int refcount;
    int underwater;
    int buoyancy;
    int timeBuoyant;
    int timeRipple;
    unsigned __int16 trDuration;
    // padding byte
    // padding byte
    float m_time_since_last_event;
    float m_time_since_last_reeval;
    int m_flags;
    float mass;
    float buoyancyBoxMin[3];
    float buoyancyBoxMax[3];
    float centerOfMassOffset[3];
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct __declspec(align(4)) phys_surface_type_info // sizeof=0x10
{                                       // XREF: ?Phys_Init@@YAXXZ/r
    float m_friction_coef;              // XREF: Phys_Init(void)+8E5/w
    float m_bounce_coef;                // XREF: Phys_Init(void)+8F2/w
    unsigned int m_solver_priority;     // XREF: Phys_Init(void)+8F7/w
    bool m_no_overflow_error;           // XREF: Phys_Init(void)+8FE/w
    // padding byte
    // padding byte
    // padding byte
};

struct broad_phase_group : broad_phase_base // sizeof=0x60
{                                       // XREF: phys_free_list<broad_phase_group>::T_internal/r
    broad_phase_info *m_list_bpi_head;
    int m_bpi_count;
    struct rb_vehicle_model *m_rbvm;
    struct phys_wheel_collide_info *m_list_wci;

    void set();
    void add_bpi(broad_phase_info *bpi);
};

const struct __declspec(align(16)) broad_phase_environment_query_input // sizeof=0x40
{                                       // XREF: ?bp_env_jq_batch_function2@@YAHPAUjqBatch@@@Z/r
    phys_vec3 trace_aabb_min_wace;
    phys_vec3 trace_aabb_max_wace;
    phys_vec3 trace_translation;
    unsigned int env_collision_flags;
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
};

struct broad_phase_base_list // sizeof=0x8
{                                       // XREF: broad_phase_environement_query_results/r
    struct node // sizeof=0x8
    {
        broad_phase_base *m_bpb;
        broad_phase_base_list::node *m_next;
    };
    broad_phase_base_list::node *m_list;
    broad_phase_base_list::node **m_list_cur;
};

struct broad_phase_environement_query_results // sizeof=0x14
{                                       // XREF: ?bp_env_jq_batch_function2@@YAHPAUjqBatch@@@Z/r
    broad_phase_base_list m_list_bpi_env;
    int m_list_bpi_env_count;
    int m_thread_id;                    // XREF: bp_env_jq_batch_function2(jqBatch *)+10/w
    unsigned int m_env_collision_flags;
};

struct broad_phase_terrain_query_callback // sizeof=0x4
{                                       // XREF: standard_query/r
    //broad_phase_terrain_query_callback_vtbl *__vftable;
                                        // XREF: _dynamic_initializer_for__g_standard_query__+3/w
                                        // _dynamic_initializer_for__g_standard_query__+D/w
    //struct /*VFT*/ broad_phase_terrain_query_callback_vtbl // sizeof=0x4
    //{
    //    void (__thiscall *query)(broad_phase_terrain_query_callback *this, const broad_phase_environment_query_input *, broad_phase_environement_query_results *);
    //};
    virtual void query(const broad_phase_environment_query_input *, broad_phase_environement_query_results *);
};

struct broad_phase_collision_pair // sizeof=0xC
{                                       // XREF: phys_free_list<broad_phase_collision_pair>::T_internal/r
    broad_phase_info *m_bpi1;
    broad_phase_info *m_bpi2;
    broad_phase_collision_pair *m_next_bpcp;
};

struct broad_phase_memory // sizeof=0xCD8
{
    tlAtomicMutex g_bp_auto_activate_mutex;
    tlAtomicReadWriteMutex g_bp_gjk_cache_mutex;
    phys_heap_gjk_cache_system_avl_tree g_phys_gjk_cache_system;
    phys_free_list<broad_phase_info> g_list_broad_phase_info;
    phys_free_list<broad_phase_group> g_list_broad_phase_group;
    phys_free_list<broad_phase_collision_pair> g_list_broad_phase_collision_pair;
    bpei_database_t g_bpei_database;
    broad_phase_terrain_query_callback *g_broad_phase_terrain_query_callback;
    broad_phase_base *g_list_bpb;
    int g_list_bpb_count;
    broad_phase_info *m_list_bpi_env;
    int m_bpi_env_count;
    int m_bpg_env_count;
    int m_bpg_env_bpi_count;
    int m_bpi_env_no_database_count;
    int m_memory_high_water;
    phys_link_list<phys_collision_pair> g_list_phys_collide_data;
    phys_transient_allocator g_collision_memory_buffer;
    phys_surface_type_info *g_surface_type_info_database;
    int m_max_num_surface_types;
    int m_max_num_surface_type_infos;

    void list_bpb_remove(broad_phase_base *bpb_to_remove);
};

struct phys_convex_hull // sizeof=0x20D60
{                                       // XREF: .data:phys_convex_hull g_hull/r
    struct __declspec(align(8)) ch_triangle // sizeof=0x20
    {
        phys_vec3 m_normal;
        phys_vec3 *m_verts[3];
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        double get_dist(const phys_vec3 *vert);

        ch_triangle &operator=(const phys_convex_hull::ch_triangle *__that);
    };
    struct ch_edge // sizeof=0x8
    {
        phys_vec3 *m_verts[2];
    };
    phys_static_array<phys_vec3,6144> m_vertex_buffer;
                                        // XREF: generate_brush_chull(cbrush_t const *)+3B/r
                                        // generate_brush_chull(cbrush_t const *):loc_84E685/w ...
    phys_static_array<phys_vec3 *,6144> m_intermediate_vertex_list;
    phys_static_array<phys_convex_hull::ch_triangle,256> m_intermediate_triangle_list;
    phys_static_array<phys_convex_hull::ch_edge,128> m_intermediate_edge_list;
    phys_static_array<phys_vec3 *,64> m_convex_hull_vert_list;
    phys_static_array<phys_convex_hull::ch_triangle,64> m_convex_hull_triangle_list;

    phys_convex_hull();
    ~phys_convex_hull();

    void compute_convex_hull(int max_verts, float min_expansion_volume_percent);
    void add_convex_hull_vert(phys_vec3 **vert);
    void create_intermediate_triangle(phys_vec3 *v0, phys_vec3 *v1, phys_vec3 *v2);
    phys_vec3 **support_intermediate_verts(const phys_vec3 *dir);
    void init_convex_hull();
    void calculate_initial_triangle_vertices();
    double calc_expansion_volume(const phys_vec3 *vert);
    double tetrahedron_volume(
        const phys_vec3 *a,
        const phys_vec3 *b,
        const phys_vec3 *c,
        const phys_vec3 *d);
    void create_edge_list(const phys_vec3 *vert);
    void add_triangle_edges(phys_convex_hull::ch_triangle *tri);
    void add_intermediate_edge(phys_vec3 *v0, phys_vec3 *v1);
    void remove_inside_verts();

};

struct PhysGlob // sizeof=0x480
{                                       // XREF: .data:PhysGlob physGlob/r
    PhysGlob();

    phys_free_list<PhysObjUserData> objects;
    phys_link_list1<PhysObjUserData> objects_by_world[3];
    PhysObjUserData *userRigidBodies[16];
    int timeLastSnapshot;               // XREF: Phys_Init(void)+6/w
    int timeLastUpdate;                 // XREF: Phys_ObjCreateNitrousVehicle(float const * const,float const * const,float const * const,float const * const,PhysPreset const *,gjk_geom_list_t *)+2B6/r
};

struct cdl_proftimer // sizeof=0x60
{                                       // XREF: .data:cdl_proftimer proftimer_physics_frame_advance/r
                                        // .data:cdl_proftimer cl_flame_proftimer/r ...
    unsigned __int64 stamp;             // XREF: Phys_RunToTime(int)+B/w
                                        // Phys_RunToTime(int)+10/w ...
    unsigned __int64 value;             // XREF: Phys_RunToTime(int)+1A4/r
                                        // Phys_RunToTime(int)+1AA/r ...
    unsigned __int64 avr;               // XREF: CG_DrawFPS(int,ScreenPlacement const *,float,meminfo_t *)+CE9/r
                                        // CG_DrawFPS(int,ScreenPlacement const *,float,meminfo_t *)+CEF/r ...
    unsigned __int64 tot;
    unsigned __int64 capture_count;
    unsigned __int64 calls;             // XREF: CG_DrawFPS(int,ScreenPlacement const *,float,meminfo_t *)+D9B/r
                                        // CG_DrawFPS(int,ScreenPlacement const *,float,meminfo_t *)+DA2/r ...
    bool capture;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    unsigned __int64 mx[5];


    cdl_proftimer();

    void start_capture();
};

void __cdecl destroy_gjk_geom(gjk_aabb_t *geom);
void __cdecl Phys_Init();
void __cdecl surface_type_info_database_set(int surface_type_1, int surface_type_2, const phys_surface_type_info *pst);
void __cdecl create_broad_phase_info(rigid_body *body);
void __cdecl environment_collision_list_add(broad_phase_base *bpb);
void __cdecl environment_collision_list_remove(broad_phase_base *bpb);
void __cdecl destroy_broad_phase_info(rigid_body *body);
PhysObjUserData * Phys_CreateUserBody(
                float *position,
                int id,
                PhysicsGeomType geomType);
PhysObjUserData *__cdecl Phys_CreateUserData(int worldIndex);
PhysObjUserData * Phys_CreateBodyFromState(
                int worldIndex,
                const BodyState *state,
                gjk_geom_list_t *gjk_geom_list,
                bool do_collision_test);
void    phys_full_inverse(phys_mat44 *dest, const phys_mat44 *source);
void __cdecl destroy_gjk_geom(gjk_geom_list_t *gjk_geom_list);
PhysObjUserData *__cdecl Phys_ObjCreate(
                int worldIndex,
                const float *position,
                const float *quat,
                const float *velocity,
                const PhysPreset *physPreset,
                gjk_geom_list_t *gjk_geom_list,
                bool do_collision_test,
                int id);
PhysObjUserData *__cdecl Phys_ObjCreateAxis(
                int worldIndex,
                const float *position,
                const float (*axis)[3],
                const float *velocity,
                const PhysPreset *physPreset,
                gjk_geom_list_t *gjk_geom_list,
                int id,
                bool do_collision_test);
void    Phys_ObjSetPosition(int id, float *newPosition);
void __cdecl Phys_ObjSetOrientation(int id, const float *newPosition, const float *newOrientation);
void    Phys_ObjSetAngularVelocity(int id, float *angularVel);
void    Phys_ObjSetVelocity(int id, float *velocity);
void    Phys_ObjSetAngularVelocityRaw(int id, float *angularVel);
void    Phys_ObjGetPosition(int id, float *outPosition, float (*outRotation)[3]);
void __cdecl Phys_ObjGetVelocities(int id, float *tvel, float *avel);
void __cdecl Phys_ObjGetCenterOfMass(int id, float *outPosition);
void __cdecl Phys_ObjAddCollFlags(int physObjId, int collFlags);
void __cdecl Phys_ObjRemoveCollFlags(int physObjId, int collFlags);
void __cdecl fixup_wheel_constraints(rigid_body *rb);
void __cdecl Phys_ObjDestroy(int worldIndex, int id);
void __cdecl Phys_DestroyUserData(int worldIndex, PhysObjUserData *userData);
void    Phys_AddCacheImpulses();
void __cdecl Phys_ObjAddCustomForce(
                int physObjId,
                const float *hitPos,
                const float *hitDir,
                int mod,
                PhysPreset *physPreset);
void    Phys_ObjAddForce(int id, float *worldPos, float *impulse, bool relative);
void    Phys_ObjAddTorque(int id, float *torque);
void __cdecl Phys_ObjBulletImpact(
                int id,
                const float *worldPosRaw,
                const float *bulletDirRaw,
                float bulletSpeed,
                float scale);
void __cdecl Phys_TweakBulletImpact(float *worldPos, float *bulletDir, const float *centerOfMass);
int __cdecl Phys_ObjGetSnapshot(int id, float *outPos, float (*outMat)[3]);
void __cdecl Phys_RenderBulletMeshInfo(
                float *pos,
                const XModel *model,
                const char *type,
                int numTriangles,
                float avgArea);
void __cdecl Phys_ComputeStringPosition(const float *start, const float *end, float *position);
void __cdecl Phys_FindAndRenderBulletMesh(const float *start, const float *end);
chull_t *__cdecl create_chull(phys_convex_hull *pch);
void __cdecl free_chull(chull_t *first);
void __cdecl chull_list_add(chull_t *chull, unsigned int key);
void __cdecl free_chull_lists();
chull_t * generate_brush_chull(const cbrush_t *brush);

chull_t * generate_partition_chull(const CollisionAabbTree *tree);
void __cdecl generate_brushmodel_chull_r(cLeafBrushNode_s *node, chull_t **hull);
chull_t *__cdecl generate_brushmodel_chull(unsigned int brushmodel);
chull_t *__cdecl generate_collmap_chull(PhysGeomList *geomList);
chull_t *__cdecl get_collmap_chull(PhysGeomList *geomList, unsigned int key);
chull_t *__cdecl get_brushmodel_chull(unsigned int brushmodel, unsigned int key);
chull_t *__cdecl get_brush_chull(const cbrush_t *brush);
chull_t *__cdecl get_partition_chull(const CollisionAabbTree *tree);
void Phys_FindAndRenderEntityBrushes(const float *pos, int contentmask);
char __cdecl Phys_DebugRenderChanged();
int __cdecl Phys_GetMaskFromDVar();
void __cdecl Phys_RenderWorldCollMesh(const float *pos, bool bRenderStaticCollision, bool bRenderBrushes);

void __cdecl debug_loop();
void __cdecl print_smodel(struct cStaticModel_s *smodel, int refcount, const float *dims);
void __cdecl print_static_models_stats();
int __cdecl SortModelsFunc(const void *a, const void *b);
void __cdecl draw_static_models_bounds();
int __cdecl SortSModelsByDist(float *a, float *b);
int __cdecl buoyancy_worker();
void __cdecl Phys_BodyGrabSnapshotNitrous(PhysObjUserData *userData, float deltaT);
void    UpdateRigidBody(float delta_t);
void __cdecl Phys_DebugRender(PhysObjUserData *userData);
void    Phys_CollisionCallback();
char __cdecl Phys_ShouldCollideCallback(const broad_phase_base *bpi1, const broad_phase_base *bpi2);
void __cdecl PhysicsSystem_Update();
void __cdecl Phys_RunToTime(int timeNow);
void __cdecl Phys_ObjGetInterpolatedState(int id, float *outPos, float *outQuat);
void    Phys_SetUserBody(int id, float *position);
bool __cdecl Phys_ObjIsAsleep(int id);
bool __cdecl Phys_ObjIsAsleepSingle(int id);

void __cdecl Phys_SetHingeParams(rigid_body_constraint_ragdoll *id, float motorSpeed, float damp);
rigid_body_constraint_ragdoll * Phys_CreateHinge(
                int obj1,
                int obj2,
                const float *anchor,
                float *axis,
                float motorSpeed,
                float damp,
                float lowStop,
                float highStop);
void __cdecl Phys_SetAngularMotorParams(rigid_body_constraint_ragdoll *id, const float *motorSpeeds, float *damp);
rigid_body_constraint_ragdoll * Phys_CreateSwivel(
                int obj1,
                int obj2,
                const float *anchor,
                unsigned int numAxes,
                float (*axes)[3],
                const float *motorSpeeds,
                const float *motorFMaxs,
                float *lowStops,
                float *highStops);
// local variable allocation has failed, the output may be wrong!
void    phys_inv_multiply_mat(phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right);
void __cdecl Phys_CalcPreset(PhysPreset *physPreset, float *dims, int surfaceType);
int __cdecl Phys_GetCurrentTime();
broad_phase_info *__cdecl create_broad_phase_info();
void __cdecl destroy_broad_phase_info(broad_phase_info *bpi);


extern cdl_proftimer proftimer_physics_frame_advance;
extern PhysGlob physGlob;