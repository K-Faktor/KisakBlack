#pragma once
#include "rigid_body.h"
#include "phys_gjk_collision_detection.h"

struct centity_s;

struct auto_rigid_body // sizeof=0x1C
{
    user_rigid_body *rb;
    const centity_s *cent;
    int frame_count;
    auto_rigid_body *next;
    phys_inplace_avl_tree_node<auto_rigid_body> m_avl_node_info;

    void add(const centity_s *cent, gjk_physics_collision_visitor *collision_visitor, int mask);
    void remove_ent(const centity_s *cent);
    user_rigid_body *ent_has_auto_rigid_body(const centity_s *cent);
    void update();
};

void __cdecl fixup_wheel_constraints_arb(auto_rigid_body *arb);