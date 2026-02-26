#pragma once
#include "phys_local.h"
#include "phys_colgeom.h"
#include "phys_collision.h"

struct __declspec(align(16)) phys_contact_manifold // sizeof=0x60
{                                       // XREF: phys_contact_manifold_process/r
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

    double __thiscall compute_convex_poly_area();
    double __thiscall compute_convex_poly_perimeter();
    void __thiscall generate_convex_poly_internal();
    void __thiscall comp_feature_normal();
    void __thiscall generate_convex_poly(const phys_mat44 *contact_mat);
    bool __cdecl rht(const phys_vec2 *e1, const phys_vec2 *e2, float min_length2, float min_sin_sq);
};

struct __declspec(align(16)) phys_contact_manifold_process // sizeof=0x4180
{
    enum : __int32
    {
        ALLOCATOR_MEMORY_SIZE = 0x4000,
    };
    struct isect_info // sizeof=0x18
    {                                       // XREF: ?intersect_poly_poly@phys_contact_manifold_process@@QAEXXZ/r
        phys_contact_manifold *m_cman;      // XREF: phys_contact_manifold_process::intersect_poly_poly(void)+1E1/r
        contact_manifold_mesh_point **m_i;
        contact_manifold_mesh_point **m_next_i;
        contact_manifold_mesh_point **m_last_i;
        phys_vec2 m_edge;

        void init(phys_contact_manifold *cman);
        void update();
    };
    struct bridge // sizeof=0x10
    {
        phys_vec2 m_intersection_p;
        contact_manifold_mesh_point **m_left_i;
        contact_manifold_mesh_point **m_right_i;
    };
    phys_mat44 contact_mat;
    phys_mat44 cg1_to_rb2_xform;
    phys_transient_allocator *m_cpi_allocator;
    phys_link_list<contact_point_info> m_list_cpi;
    contact_point_info *m_cpi;
    contact_manifold_mesh_point **m_list_isect_point;
    rigid_body_constraint_contact *m_rbc_contact_search_tree_root;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    phys_contact_manifold cman1;
    phys_contact_manifold cman2;
    int m_contact_point_count;
    phys_memory_heap m_allocator;
    char m_allocator_memory[16384];
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

    void intersect_poly_segment(
        phys_contact_manifold *cman,
        const phys_vec2 *p0,
        const phys_vec2 *p1);
    bool find_bottom(phys_contact_manifold_process::bridge *b, contact_manifold_mesh_point **left_cman,
        phys_contact_manifold_process::isect_info *right_cman);
    void intersect_poly_poly();
};

const phys_vec3 *__cdecl phys_v2_to_v3_multiply(phys_vec3 *result, const phys_mat44 *m, const phys_vec2 *v);
void    displace_contact_p(
                contact_manifold_mesh_point **mp,
                const phys_vec2 *d,
                const phys_mat44 *contact_mat);
void __cdecl feature_qsort(contact_manifold_mesh_point **list, int list_count);
void __cdecl convex_poly_qsort(contact_manifold_mesh_point **list, int list_count);