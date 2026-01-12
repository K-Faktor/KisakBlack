#pragma once
#include "rigid_body.h"

struct broad_phase_memory_info // sizeof=0xC
{                                       // XREF: ?Phys_Init@@YAXXZ/r
    int m_max_num_gjk_ci;               // XREF: Phys_Init(void)+88C/w
    int m_max_num_sap_active_pairs;     // XREF: Phys_Init(void)+893/w
    int m_max_num_surface_types;        // XREF: Phys_Init(void)+89A/w

    broad_phase_memory_info();
};

struct __declspec(align(8)) broad_phase_base // sizeof=0x50
{                                                                             // XREF: broad_phase_info/r
                                                                                // broad_phase_group/r ...
    phys_vec3 m_trace_aabb_min_whace;
    phys_vec3 m_trace_aabb_max_whace;
    phys_vec3 m_trace_translation;
    unsigned int m_flags;
    broad_phase_base *m_list_bpb_next;
    broad_phase_base *m_list_bpb_cluster_next;
    void *m_sap_node;
    void *m_user_data;
    unsigned int m_env_collision_flags;
    unsigned int m_my_collision_type_flags;
    // padding byte
    // padding byte
    // padding byte
    // padding byte

    struct broad_phase_info *get_bpi()
    {
        return (broad_phase_info *)this;
    }

    struct broad_phase_group *get_bpg()
    {
        return (broad_phase_group *)this;
    }

    struct broad_phase_info *get_bpi_env()
    {
        return get_bpi();
    }

    void get_aabb(phys_vec3 *aabb)
    {
        const phys_vec3 *v4; // eax
        const phys_vec3 *v5; // eax
        phys_vec3 v6; // [esp-20h] [ebp-2Ch] BYREF
        phys_vec3 v7; // [esp-10h] [ebp-1Ch] BYREF
        void *retaddr; // [esp+Ch] [ebp+0h]

        v7.x = this->m_trace_translation.x + this->m_trace_aabb_min_whace.x;
        v7.y = this->m_trace_translation.y + this->m_trace_aabb_min_whace.y;
        v7.z = this->m_trace_translation.z + this->m_trace_aabb_min_whace.z;
        v4 = phys_min(&v6, &this->m_trace_aabb_min_whace, &v7);
        aabb->x = v4->x;
        aabb->y = v4->y;
        aabb->z = v4->z;
        v7.x = this->m_trace_aabb_max_whace.x + this->m_trace_translation.x;
        v7.y = this->m_trace_aabb_max_whace.y + this->m_trace_translation.y;
        v7.z = this->m_trace_aabb_max_whace.z + this->m_trace_translation.z;
        v5 = phys_max(&v6, &this->m_trace_aabb_max_whace, &v7);
        aabb[1].x = v5->x;
        aabb[1].y = v5->y;
        aabb[1].z = v5->z;
    }
};

struct broad_phase_group : broad_phase_base // sizeof=0x60
{                                       // XREF: phys_free_list<broad_phase_group>::T_internal/r
    broad_phase_info *m_list_bpi_head;
    int m_bpi_count;
    struct rb_vehicle_model *m_rbvm;
    struct phys_wheel_collide_info *m_list_wci;

    void set();
    void add_bpi(broad_phase_info *bpi);
    void collision_prolog();
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

struct bpi_environment_collision_info // sizeof=0x10
{                                       // XREF: ?broad_phase_process_object_environment_collision@@YAXXZ/r
    broad_phase_base *m_bpb_i_start;    // XREF: broad_phase_process_object_environment_collision(void)+17/w
                                        // broad_phase_process_object_environment_collision(void)+39/r ...
    broad_phase_base *m_bpb_i_end;      // XREF: broad_phase_process_object_environment_collision(void)+1A/w
                                        // broad_phase_process_object_environment_collision(void)+3C/w
    int m_bpb_count;                    // XREF: broad_phase_process_object_environment_collision(void)+21/w
                                        // broad_phase_process_object_environment_collision(void)+5B/w
    int m_bpb_last_count;               // XREF: broad_phase_process_object_environment_collision(void)+24/w
                                        // broad_phase_process_object_environment_collision(void)+55/r ...
};

void __cdecl create_broad_phase_info(rigid_body *body);
void __cdecl destroy_broad_phase_info(rigid_body *body);

void    comp_trace_volume(
                const phys_vec3 *aabb1_min,
                const phys_vec3 *aabb1_max,
                const phys_vec3 *aabb2_min,
                const phys_vec3 *aabb2_max,
                phys_vec3 *p1,
                phys_vec3 *p2,
                phys_vec3 *half_dims);

void __userpurge phys_wheel_collide_info::collision_process(
                phys_wheel_collide_info *this@<ecx>,
                int a2@<ebp>,
                int a3@<edi>,
                int a4@<esi>,
                broad_phase_info *bpi);
void __userpurge phys_wheel_collide_info::collision_epilog(
                phys_wheel_collide_info *this@<ecx>,
                int a2@<ebp>,
                rigid_body_constraint_wheel *rbc_wheel);


void __thiscall axis_aligned_sweep_and_prune::sap_node::init(
                axis_aligned_sweep_and_prune::sap_node *this,
                broad_phase_base *bpb,
                axis_aligned_sweep_and_prune::axis_element **xlist,
                axis_aligned_sweep_and_prune::axis_element **ylist,
                axis_aligned_sweep_and_prune::axis_element **zlist);
bool __thiscall axis_aligned_sweep_and_prune::are_overlapping(
                axis_aligned_sweep_and_prune *this,
                axis_aligned_sweep_and_prune::sap_node *n1,
                axis_aligned_sweep_and_prune::sap_node *n2);
void __thiscall axis_aligned_sweep_and_prune::remove(
                axis_aligned_sweep_and_prune *this,
                axis_aligned_sweep_and_prune::axis_element **axis_list,
                axis_aligned_sweep_and_prune::sap_node *node);
void __thiscall axis_aligned_sweep_and_prune::swap(
    axis_aligned_sweep_and_prune *this,
    axis_aligned_sweep_and_prune::axis_element **a1_ptr,
    axis_aligned_sweep_and_prune::axis_element **a2_ptr);
void __thiscall axis_aligned_sweep_and_prune::merge_sort(
    axis_aligned_sweep_and_prune *this,
    axis_aligned_sweep_and_prune::axis_element **list,
    int list_count);
void    axis_aligned_sweep_and_prune::sap_node::update_ae_val(
    axis_aligned_sweep_and_prune::sap_node *this@<ecx>,
    float a2@<ebp>);
void __thiscall axis_aligned_sweep_and_prune::init_system(axis_aligned_sweep_and_prune *this, int max_num_active_pairs);
void __thiscall axis_aligned_sweep_and_prune::add_active_pair(
    axis_aligned_sweep_and_prune *this,
    axis_aligned_sweep_and_prune::sap_node *p1,
    axis_aligned_sweep_and_prune::sap_node *p2);
void __thiscall axis_aligned_sweep_and_prune::create_sap_node(
    axis_aligned_sweep_and_prune *this,
    broad_phase_base *bpb);
void __thiscall axis_aligned_sweep_and_prune::remove(
    axis_aligned_sweep_and_prune *this,
    axis_aligned_sweep_and_prune::active_pair **list_ap,
    axis_aligned_sweep_and_prune::sap_node *node);
void __thiscall axis_aligned_sweep_and_prune::destroy_sap_node(
    axis_aligned_sweep_and_prune *this,
    broad_phase_base *bpb);
void    axis_aligned_sweep_and_prune::process_active_pair_list(
    axis_aligned_sweep_and_prune *this@<ecx>,
    axis_aligned_sweep_and_prune::active_pair **a2@<ebp>);
void __thiscall axis_aligned_sweep_and_prune::process(axis_aligned_sweep_and_prune *this);

void __cdecl init_bpeqi(broad_phase_environment_query_input *bpeqi, broad_phase_base *bpb);
void __cdecl check_terrain_query_params(broad_phase_base *bpb);
void    calc_largest_vel_sq(broad_phase_info *bpi);
void __cdecl broad_phase_reset_buffer();
int    bp_env_jq_batch_function1(jqBatch *pBatch);
void __cdecl merge_sort_bpb(broad_phase_base **list, int list_count);
void    broad_phase_process_object_environment_collision(bpi_environment_collision_info *eci);
void __cdecl broad_phase_process_object_environment_collision();

void __cdecl aasap_list_add(broad_phase_base *bpb);
void __cdecl aasap_list_remove(broad_phase_base *bpb);
void    do_initial_tunnel_test(
                broad_phase_group *bpg,
                const broad_phase_environement_query_results *bpeqr);
broad_phase_info *__cdecl create_broad_phase_info();
void __cdecl destroy_broad_phase_info(broad_phase_info *bpi);
void __cdecl destroy_broad_phase_info_list(broad_phase_info *list_bpi);
broad_phase_collision_pair *__cdecl create_broad_phase_collision_pair();
void __cdecl destroy_broad_phase_collision_pair(broad_phase_collision_pair *bpcp);
void __cdecl destroy_broad_phase_collision_pair_list(broad_phase_collision_pair *list_bpcp);
broad_phase_group *__cdecl create_broad_phase_group();
void __cdecl destroy_broad_phase_group(broad_phase_group *bpg);
char    bpi_do_gjk_intersect(
                broad_phase_info *p1,
                broad_phase_info *p2,
                phys_collision_pair *hit_time);
void __cdecl collide_bpi_environment(float bpi, broad_phase_base_list::node *bpeqr);
void    collide_bpg_environment(
                broad_phase_info *bpg,
                const broad_phase_environement_query_results *bpeqr);
int    bp_env_jq_batch_function2();
void    broad_phase_process();
void __cdecl broad_phase_system_init(
                const broad_phase_memory_info *bpmi,
                bool (__cdecl *should_collide_callback)(const broad_phase_base *, const broad_phase_base *));
void __cdecl broad_phase_system_shutdown();

void __thiscall tlAtomicReadWriteMutex::~tlAtomicReadWriteMutex(tlAtomicReadWriteMutex *this);
void __thiscall rigid_body::adjust_col_moved_vec(rigid_body *this, float lambda);
void __thiscall broad_phase_group::collision_epilog(broad_phase_group *this);

void    process_cluster_environment_collision_prolog(
                broad_phase_info *bpb,
                broad_phase_base *info);
bool __cdecl compare_bpb(broad_phase_base *bpb1, broad_phase_base *bpb2);


char    phys_are_potentially_colliding_whace<broad_phase_info,broad_phase_info>(
                broad_phase_info *p1,
                broad_phase_info *p2,
                float *hit_time);
void __cdecl add_collision_pair(
                broad_phase_info *bpi1,
                broad_phase_info *bpi2,
                float hit_time,
                phys_gjk_cache_info *gjk_ci);
void __cdecl add_collision_pair_mutex(
                broad_phase_info *bpi1,
                broad_phase_info *bpi2,
                float hit_time,
                phys_gjk_cache_info *gjk_ci);


void    broad_phase_process_collision_pairs(float a1@<ebp>);
void __thiscall phys_free_list<broad_phase_info>::remove_all(phys_free_list<broad_phase_info> *this);
void __thiscall phys_free_list<broad_phase_collision_pair>::remove_all(
                phys_free_list<broad_phase_collision_pair> *this);
// attributes: thunk
void __thiscall phys_heap_gjk_cache_system_avl_tree::~phys_heap_gjk_cache_system_avl_tree(
                phys_heap_gjk_cache_system_avl_tree *this);
// attributes: thunk
void __thiscall bpei_database_t::~bpei_database_t(bpei_database_t *this);
void __thiscall phys_free_list<broad_phase_info>::~phys_free_list<broad_phase_info>(
                phys_free_list<broad_phase_info> *this);
void __thiscall phys_free_list<broad_phase_group>::~phys_free_list<broad_phase_group>(
                phys_free_list<broad_phase_group> *this);
void __thiscall phys_free_list<broad_phase_collision_pair>::~phys_free_list<broad_phase_collision_pair>(
                phys_free_list<broad_phase_collision_pair> *this);
phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info_mutex(
                phys_heap_gjk_cache_system_avl_tree *this,
                unsigned int id1,
                unsigned int id2,
                tlAtomicReadWriteMutex *query_mutex,
                bool __formal);
broad_phase_memory *__thiscall broad_phase_memory::broad_phase_memory(broad_phase_memory *this);

void __cdecl process_cluster_environment_collision(broad_phase_base *bpb, broad_phase_base_list::node *bpeqr);
void __thiscall broad_phase_memory::~broad_phase_memory(broad_phase_memory *this);
broad_phase_memory *__cdecl broad_phase_memory::allocate_buffer(const broad_phase_memory_info *bpmi);


void calc_largest_vel_sq(broad_phase_info *bpi);

extern broad_phase_memory *G_BPM;
