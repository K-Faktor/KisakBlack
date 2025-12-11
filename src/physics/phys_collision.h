#pragma once

#include <universal/q_shared.h>
#include <universal/com_math.h>

#include "phys_local.h"
#include "rigid_body.h"

struct pulse_sum_cache // sizeof=0x4
{                                                                             // XREF: rigid_body_constraint_point/r
                                                                                // rigid_body_constraint_hinge/r ...
        float m_pulse_sum;
};

struct pulse_sum_node : phys_link_list_base<pulse_sum_node> // sizeof=0x80
{
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
        phys_mat44 m_world_inv_inertia;
        phys_vec3 t_vel;
        phys_vec3 a_vel;
        float m_inv_mass;
        rigid_body *m_rb;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
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

        broad_phase_info *get_bpi()
        {
            return (broad_phase_info*)this;
        }

        struct broad_phase_group *get_bpg()
        {
            return (broad_phase_group *)this;
        }

        broad_phase_info *get_bpi_env()
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

struct __declspec(align(8)) broad_phase_info : broad_phase_base // sizeof=0x70
{                                                                             // XREF: phys_free_list<broad_phase_info>::T_internal/r
                                                                                // broad_phase_info_env/r
        rigid_body *m_rb;
        const phys_mat44 *m_rb_to_world_xform;
        const phys_mat44 *m_cg_to_world_xform;
        const phys_mat44 *m_cg_to_rb_xform;
        const struct phys_gjk_geom *m_gjk_geom;
        unsigned int m_gjk_geom_id;
        int m_surface_type;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
};

struct phys_collision_pair : phys_link_list_base<phys_collision_pair> // sizeof=0x14
{                                                                             // XREF: ?do_initial_tunnel_test@@YAXPAVbroad_phase_group@@ABVbroad_phase_environement_query_results@@@Z/r
                                                                                // ?bpi_do_gjk_intersect@@YA?B_NPAVbroad_phase_info@@0M@Z/r
        broad_phase_info *m_bpi1;
        broad_phase_info *m_bpi2;
        float m_hit_time;
        struct phys_gjk_cache_info *m_gjk_ci;
};

class __declspec(align(16)) contact_point_info // sizeof=0x50
{
        struct pulse_sum_cache_info // sizeof=0xC
        {
                pulse_sum_cache m_ps_cache_list[3];
        };
public:
        phys_vec3 m_normal;
        float m_fric_coef;
        float m_bounce_coef;
        float m_max_restitution_vel;
        int m_flags;
        int m_point_pair_count;
        phys_vec3 *m_list_b1_r_loc;
        phys_vec3 *m_list_b2_r_loc;
        contact_point_info::pulse_sum_cache_info *m_list_pulse_sum_cache_info;
        contact_point_info *m_next_link;
        const void *m_rb2_entity;
        float m_translation_lambda;
        phys_collision_pair *m_pcp;
        rigid_body_constraint_contact *m_rbc_contact;
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

//void __thiscall contact_point_info::get_closest_psc(
//                contact_point_info *this,
//                const phys_vec3 *normal,
//                const phys_vec3 *b1_r_loc,
//                const phys_vec3 *b2_r_loc,
//                float *closest_error,
//                const contact_point_info::pulse_sum_cache_info **closest_psc);
//void __userpurge phys_contact_manifold::xform_and_translate_mesh_points(
//                phys_contact_manifold *this@<ecx>,
//                int a2@<ebp>,
//                const phys_mat44 *xform,
//                const phys_vec3 *translation);
//double __cdecl phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_DISTANCE_EPS(float penetration_t);
//double __cdecl phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ(float penetration_t);
//double __cdecl phys_contact_manifold::get_STD_GET_FEATURE_DISTANCE_EPS(float penetration_t);
//double __cdecl phys_contact_manifold::get_STD_GET_FEATURE_SIN_ANGULAR_EPS_SQ(float penetration_t);
//void __thiscall phys_contact_manifold_process::comp_contact_mat(
//                phys_contact_manifold_process *this,
//                const phys_vec3 *contact_normal);
//rigid_body_constraint_contact *__cdecl avl_tree_find<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor>(
//                rigid_body_constraint_contact *tree_root,
//                const rigid_body_pair_key *key);
//contact_point_info *__cdecl contact_point_info::create_cpi(
//                int point_pair_count,
//                bool no_error,
//                phys_transient_allocator *allocator);
//void __thiscall phys_contact_manifold::set_get_feature_params(
//                phys_contact_manifold *this,
//                const phys_vec3 *hitp,
//                const phys_vec3 *hitn,
//                float feature_distance_eps,
//                float sin_feautre_angular_eps_sq);
//void __cdecl contact_point_info::set_closest_cached_psc(
//                contact_point_info *cached_cpi,
//                const phys_vec3 *normal,
//                const phys_vec3 *b1_r_loc,
//                const phys_vec3 *b2_r_loc,
//                contact_point_info::pulse_sum_cache_info *psc);
//void __thiscall contact_point_info::set_closest_cached_psc(contact_point_info *this, contact_point_info *cached_cpi);
//// bad sp value at call has been detected, the output may be wrong!
//void __userpurge phys_contact_manifold_process::process(
//                phys_contact_manifold_process *this@<ecx>,
//                float a2@<ebp>,
//                int a3@<edi>,
//                int a4@<esi>,
//                phys_collision_pair *pcp,
//                phys_gjk_info *gjk_info);
