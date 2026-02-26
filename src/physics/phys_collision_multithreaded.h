#pragma once
#include "phys_local.h"
#include "phys_collision.h"
#include "phys_colgeom.h"
#include "phys_task_manager.h"
#include "phys_gjk.h"

struct __declspec(align(16)) phys_contact_manifold_process // sizeof=0x4180
{
    struct bridge // sizeof=0x10
    {
        phys_vec2 m_intersection_p;
        contact_manifold_mesh_point **m_left_i;
        contact_manifold_mesh_point **m_right_i;
    };

    struct isect_info // sizeof=0x18
    {                                       // XREF: ?intersect_poly_poly@phys_contact_manifold_process@@QAEXXZ/r
        phys_contact_manifold *m_cman;      // XREF: phys_contact_manifold_process::intersect_poly_poly(void)+1E1/r
                                            // phys_contact_manifold_process::intersect_poly_poly(void)+1E9/r
        contact_manifold_mesh_point **m_i;
        contact_manifold_mesh_point **m_next_i;
        contact_manifold_mesh_point **m_last_i;
        phys_vec2 m_edge;

        void update();
        void init(phys_contact_manifold *cman);
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

    phys_contact_manifold_process();

    void process(phys_collision_pair *pcp, phys_gjk_info *gjk_info);
    void intersect_poly_segment(
        phys_contact_manifold *cman,
        const phys_vec2 *p0,
        const phys_vec2 *p1);
    void intersect_poly_poly();
    char find_bottom(
        phys_contact_manifold_process::bridge *b,
        contact_manifold_mesh_point **left_cman,
        phys_contact_manifold_process::isect_info *right_cman);
    void comp_contact_mat(const phys_vec3 *contact_normal);
};

void __cdecl process_cpi(contact_point_info *cpi);
void __cdecl process_list_do_gjk_collide_and_contact_manifold(phys_link_list<phys_collision_pair> *list_pcd);
int    phys_gjk_collide_jq_batch_function(jqBatch *pBatch);
void    phys_collide_do_gjk_collide_and_contact_manifold(
    phys_collision_pair *pcp,
    phys_gjk_info *gjk_info,
    phys_contact_manifold_process *cman_process);