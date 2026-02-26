#include "rbc_def_vehicle.h"
#include <physics/phys_constraint_solver_multithreaded.h>
#include "rbc_def_generic.h"

void __thiscall pulse_sum_normal::set_pulse_sum_limits_parent_ratio(float limit_ratio, pulse_sum_normal *const parent)
{
  if ( limit_ratio < 0.0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\constraint_solver\\pulse_sum_normal.h",
         269,
         "limit_ratio >= 0.0f",
         "") )
  {
    __debugbreak();
  }
  if ( !parent
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\constraint_solver\\pulse_sum_normal.h",
         270,
         "parent",
         "") )
  {
    __debugbreak();
  }
  this->m_flags |= 1u;
  this->m_pulse_limit_ratio = limit_ratio;
  this->m_pulse_parent = parent;
  this->m_pulse_sum_min = 0.0;
  this->m_pulse_sum_max = 0.0;
}

void __thiscall pulse_sum_wheel::set_side_fwd_ratios(
        float side_ratio,
        float fwd_ratio,
        float side_fric_max)
{
  if ( !this->m_side || !this->m_fwd )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\constraint_solver\\pulse_sum_wheel.h",
           34,
           "m_side && m_fwd",
           "") )
    {
      __debugbreak();
    }
  }
  this->m_side->m_pulse_limit_ratio = side_ratio;
  this->m_fwd->m_pulse_limit_ratio = fwd_ratio;
  this->m_side_fric_max = side_fric_max;
}

void __thiscall rigid_body_constraint_wheel::set_wheel_state_accelerating(
        float desired_speed_k,
        float acceleration_factor_k)
{
  this->m_wheel_state = 0;
  this->m_desired_speed_k = desired_speed_k;
  this->m_acceleration_factor_k = acceleration_factor_k;
}

void __thiscall rigid_body_constraint_wheel::get_wheel_state_accelerating(
        float *desired_speed_k,
        float *acceleration_factor_k)
{
  *desired_speed_k = this->m_desired_speed_k;
  *acceleration_factor_k = this->m_acceleration_factor_k;
}

void __thiscall rigid_body_constraint_wheel::set_wheel_state_braking(
        float braking_factor_k)
{
  this->m_wheel_state = 1;
  this->m_braking_factor_k = braking_factor_k;
}

void __thiscall rigid_body_constraint_wheel::set_no_collision()
{
  this->m_wheel_flags &= ~1u;
  this->b2 = 0;
}

void __thiscall rigid_body_constraint_wheel::set_collision(
        rigid_body *const rb,
        const phys_vec3 *hitp_loc,
        const phys_vec3 *hitn_loc)
{
  this->m_wheel_flags |= 1u;
  this->b2 = rb;
  this->m_b2_hitp_loc.x = hitp_loc->x;
  this->m_b2_hitp_loc.y = hitp_loc->y;
  this->m_b2_hitp_loc.z = hitp_loc->z;
  this->m_b2_hitn_loc.x = hitn_loc->x;
  this->m_b2_hitn_loc.y = hitn_loc->y;
  this->m_b2_hitn_loc.z = hitn_loc->z;
}

double __cdecl lerp_float(const float tgt, float cur, float rate, float delta_t)
{
  double result; // st7
  double v5; // st6
  long double v6; // st5
  double v7; // st4
  double v8; // st4
  float stepa; // [esp+8h] [ebp+8h]
  float step; // [esp+8h] [ebp+8h]
  float stepb; // [esp+8h] [ebp+8h]
  float cura; // [esp+Ch] [ebp+Ch]

  result = tgt;
  v5 = cur;
  stepa = tgt - cur;
  v6 = stepa;
  if ( stepa <= 0.0 )
    v7 = -rate;
  else
    v7 = rate;
  step = v7 * delta_t;
  cura = fabs(v6);
  if ( cura > 0.05000000074505806 )
  {
    v8 = step;
    stepb = fabs(step);
    if ( stepb <= (double)cura )
      return (float)(v8 + v5);
  }
  return result;
}

void __thiscall rigid_body_constraint_wheel::set(
        const phys_vec3 *wheel_center_loc,
        const phys_vec3 *suspension_dir_loc,
        const phys_vec3 *wheel_axis_loc,
        float wheel_radius,
        float fwd_fric_k,
        float side_fric_k,
        float suspension_stiffness_k,
        float suspension_damp_k,
        float hard_limit_dist,
        float roll_stability_factor,
        float pitch_stability_factor,
        float side_fric_max)
{
  phys_vec3 *p_m_b1_suspension_dir_loc; // esi
  phys_vec3 *p_m_b1_wheel_axis_loc; // edi

  this->m_b1_wheel_center_loc.x = wheel_center_loc->x;
  p_m_b1_suspension_dir_loc = &this->m_b1_suspension_dir_loc;
  this->m_b1_wheel_center_loc.y = wheel_center_loc->y;
  p_m_b1_wheel_axis_loc = &this->m_b1_wheel_axis_loc;
  this->m_b1_wheel_center_loc.z = wheel_center_loc->z;
  this->m_b1_suspension_dir_loc.x = suspension_dir_loc->x;
  this->m_b1_suspension_dir_loc.y = suspension_dir_loc->y;
  this->m_b1_suspension_dir_loc.z = suspension_dir_loc->z;
  this->m_b1_wheel_axis_loc.x = wheel_axis_loc->x;
  this->m_b1_wheel_axis_loc.y = wheel_axis_loc->y;
  this->m_b1_wheel_axis_loc.z = wheel_axis_loc->z;
  this->m_wheel_state = 1;
  this->m_wheel_flags = 0;
  this->m_wheel_radius = wheel_radius;
  this->m_fwd_fric_k = fwd_fric_k;
  this->m_side_fric_k = side_fric_k;
  this->m_side_fric_max = side_fric_max;
  this->m_suspension_stiffness_k = suspension_stiffness_k;
  this->m_suspension_damp_k = suspension_damp_k;
  this->m_hard_limit_dist = hard_limit_dist;
  this->m_roll_stability_factor = roll_stability_factor;
  this->m_pitch_stability_factor = pitch_stability_factor;
  this->m_braking_factor_k = 0.0;
  this->m_turning_radius_ratio_max_speed = 1.0;
  this->m_turning_radius_ratio_accel = 1.0;
  this->m_wheel_vel = 0.0;
  this->m_wheel_pos = 0.0;
  this->m_wheel_fwd = 0.0;
  this->m_wheel_displaced_center_dist = 0.0;
  PHYS_ASSERT_UNIT(&this->m_b1_suspension_dir_loc);
  PHYS_ASSERT_UNIT(p_m_b1_wheel_axis_loc);
  PHYS_ASSERT_ORTHOGONAL(p_m_b1_suspension_dir_loc, p_m_b1_wheel_axis_loc);
}

void rigid_body_constraint_wheel::get_wheel_collide_segment(
        const phys_mat44 *b1_mat,
        phys_vec3 *const p0,
        phys_vec3 *const p1)
{
  const phys_vec3 *v6; // eax
  double v7; // st7
  double v8; // st6
  double v9; // st5
  double v10; // st4
  double v11; // st3
  double v12; // st2
  float v13; // [esp-Ch] [ebp-4Ch] BYREF
  float v14; // [esp-8h] [ebp-48h]
  float v15; // [esp-4h] [ebp-44h]
  phys_vec3 wheel_center; // [esp+0h] [ebp-40h] BYREF
  float v17; // [esp+14h] [ebp-2Ch]
  float v18; // [esp+18h] [ebp-28h]
  float v19; // [esp+1Ch] [ebp-24h]
  float m_wheel_radius; // [esp+30h] [ebp-10h]
  unsigned int v21[3]; // [esp+34h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+40h] [ebp+0h]

  v21[0] = a2;
  v21[1] = retaddr;
  if ( ((unsigned __int8)p0 & 0xF) != 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_math.h",
         444,
         "uint(v) % PHYS_ALIGNOF(phys_vec3) == 0",
         "") )
  {
    __debugbreak();
  }
  if ( ((unsigned __int8)p1 & 0xF) != 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_math.h",
         444,
         "uint(v) % PHYS_ALIGNOF(phys_vec3) == 0",
         "") )
  {
    __debugbreak();
  }
  phys_full_multiply((int)v21, (const phys_vec3 *)&v13, b1_mat, &this->m_b1_wheel_center_loc);
  v6 = phys_multiply((phys_vec3 *)&wheel_center.y, b1_mat, &this->m_b1_suspension_dir_loc);
  m_wheel_radius = this->m_wheel_radius;
  v17 = v6->x * m_wheel_radius;
  v18 = v6->y * m_wheel_radius;
  v19 = m_wheel_radius * v6->z;
  v7 = v13;
  v8 = v17;
  wheel_center.y = v13 - v17;
  v9 = v14;
  v10 = v18;
  wheel_center.z = v14 - v18;
  v11 = v15;
  v12 = v19;
  wheel_center.w = v15 - v19;
  p0->x = wheel_center.y;
  p0->y = wheel_center.z;
  p0->z = wheel_center.w;
  wheel_center.y = v8 + v7;
  wheel_center.z = v9 + v10;
  wheel_center.w = v12 + v11;
  p1->x = wheel_center.y;
  p1->y = wheel_center.z;
  p1->z = wheel_center.w;
}

void rigid_body_constraint_wheel::epilog_vel_constraint(float delta_t)
{
  float z; // edx
  rigid_body *b1; // edi
  rigid_body *b2; // eax
  rigid_body *v7; // eax
  double w; // st7
  double v9; // st7
  double v10; // st7
  double y; // st6
  double m_pulse_sum; // st6
  pulse_sum_normal *m_ps_side_fric; // eax
  pulse_sum_normal *m_ps_fwd_fric; // ecx
  double v15; // st6
  float v16; // [esp+Ch] [ebp-70h]
  phys_vec3 v17; // [esp+20h] [ebp-5Ch] BYREF
  float v18; // [esp+30h] [ebp-4Ch]
  float v19; // [esp+34h] [ebp-48h]
  float v20; // [esp+38h] [ebp-44h]
  phys_vec3 b1_r; // [esp+3Ch] [ebp-40h] BYREF
  phys_vec3 b2_r; // [esp+4Ch] [ebp-30h] BYREF
  phys_vec3 b1_suspension_dir; // [esp+5Ch] [ebp-20h]
  float prev_wheel_displaced_center_dist; // [esp+6Ch] [ebp-10h]
  int v25; // [esp+70h] [ebp-Ch]
  float lerp_rate; // [esp+74h] [ebp-8h]
  float retaddr; // [esp+7Ch] [ebp+0h]

  v25 = a2;
  lerp_rate = retaddr;
  z = *(float *)&this->m_wheel_flags;
  b1_suspension_dir.y = this->m_wheel_displaced_center_dist;
  b1_suspension_dir.z = z;
  if ( (LOBYTE(z) & 1) != 0 )
  {
    b1 = this->b1;
    phys_multiply((phys_vec3 *)&b2_r.y, &this->b1->m_mat, &this->m_b1_suspension_dir_loc);
    b1_suspension_dir.w = this->m_wheel_radius;
    b1_r.y = b2_r.y * b1_suspension_dir.w;
    b1_r.z = b2_r.z * b1_suspension_dir.w;
    b1_r.w = b1_suspension_dir.w * b2_r.w;
    phys_multiply(&v17, &b1->m_mat, &this->m_b1_wheel_center_loc);
    b2 = this->b2;
    v18 = v17.x + b1_r.y;
    v19 = v17.y + b1_r.z;
    v20 = v17.z + b1_r.w;
    phys_multiply((phys_vec3 *)&b1_r.y, &b2->m_mat, &this->m_b2_hitp_loc);
    v7 = this->b2;
    v17.x = v7->m_mat.w.x + b1_r.y;
    v17.y = v7->m_mat.w.y + b1_r.z;
    v17.z = v7->m_mat.w.z + b1_r.w;
    b1_r.y = b1->m_mat.w.x + v18;
    b1_r.z = b1->m_mat.w.y + v19;
    b1_r.w = b1->m_mat.w.z + v20;
    v18 = b1_r.y - v17.x;
    v19 = b1_r.z - v17.y;
    v20 = b1_r.w - v17.z;
    b1_suspension_dir.w = v19 * b2_r.z + v18 * b2_r.y + v20 * b2_r.w;
    w = b1_suspension_dir.w;
    if ( this->m_hard_limit_dist < (double)b1_suspension_dir.w )
      w = this->m_hard_limit_dist;
    z = b1_suspension_dir.z;
    b1_suspension_dir.w = w;
    v9 = b1_suspension_dir.w;
  }
  else
  {
    v9 = 0.0;
  }
  this->m_wheel_displaced_center_dist = v9;
  if ( this->m_wheel_displaced_center_dist >= (double)b1_suspension_dir.y )
  {
    v10 = delta_t;
  }
  else
  {
    v10 = delta_t;
    b1_suspension_dir.y = b1_suspension_dir.y - 51.0 * delta_t;
    y = b1_suspension_dir.y;
    if ( b1_suspension_dir.y < (double)this->m_wheel_displaced_center_dist )
      y = this->m_wheel_displaced_center_dist;
    b1_suspension_dir.w = y;
    this->m_wheel_displaced_center_dist = b1_suspension_dir.w;
  }
  if ( this->m_ps_suspension )
    m_pulse_sum = this->m_ps_cache_list[1].m_pulse_sum;
  else
    m_pulse_sum = 0.0;
  this->m_wheel_normal_force = m_pulse_sum;
  b1_suspension_dir.y = this->m_wheel_vel * this->m_wheel_radius;
  b1_suspension_dir.y = fabs(b1_suspension_dir.y);
  HIBYTE(prev_wheel_displaced_center_dist) = velocity_clamp < (double)b1_suspension_dir.y;
  m_ps_side_fric = this->m_ps_side_fric;
  if ( m_ps_side_fric )
  {
    m_ps_fwd_fric = this->m_ps_fwd_fric;
    if ( m_ps_fwd_fric )
    {
      if ( (m_ps_fwd_fric->m_flags & 4) != 0 )
        this->m_wheel_flags = LODWORD(z) | 4;
      this->m_wheel_vel = pulse_sum_normal::get_unclamped_pulse_sum(m_ps_fwd_fric)
                        * this->m_wheel_fwd
                        / this->m_wheel_radius
                        + this->m_wheel_vel;
      v10 = delta_t;
    }
    else if ( (m_ps_side_fric->m_flags & 2) != 0 )
    {
      this->m_wheel_flags = LODWORD(z) | 4;
    }
  }
  if ( HIBYTE(prev_wheel_displaced_center_dist) )
    this->m_wheel_pos = this->m_wheel_vel * v10 + this->m_wheel_pos;
  if ( (this->m_wheel_flags & 1) == 0 )
  {
    if ( this->m_braking_factor_k >= 50.0 )
      v15 = lr;
    else
      v15 = lr_0;
    b1_suspension_dir.w = v15;
    v16 = v10;
    this->m_wheel_vel = lerp_float(0.0, this->m_wheel_vel, b1_suspension_dir.w, v16);
  }
}

// local variable allocation has failed, the output may be wrong!
void __userpurge rigid_body_constraint_wheel::setup_constraint(
        rigid_body_constraint_wheel *this@<ecx>,
        int a2@<ebp>,
        pulse_sum_constraint_solver *psys,
        float delta_t)
{
  rigid_body *b1; // edi
  rigid_body *b2; // eax
  rigid_body *v7; // ecx
  double z; // st7
  double w; // st6
  double y; // st5
  double v11; // st4
  double v12; // st3
  double v13; // st2
  unsigned int v14; // eax
  pulse_sum_normal *pulse_sum_normal; // eax
  rigid_body *v16; // ecx
  pulse_sum_normal *v17; // edi
  rigid_body *v18; // eax
  double v19; // st5
  double v20; // rt2
  double v21; // rtt
  pulse_sum_wheel *pulse_sum_wheel; // eax
  rigid_body *v23; // ecx
  pulse_sum_normal *m_ps_suspension; // eax
  pulse_sum_normal *v25; // edi
  double pos; // st7
  double v27; // st6
  pulse_sum_normal *v28; // eax
  pulse_sum_normal *m_ps_side_fric; // eax
  pulse_sum_normal *v30; // ecx
  const phys_vec3 *relative_velocity; // eax
  unsigned int m_wheel_state; // ecx
  pulse_sum_normal *v33; // eax
  pulse_sum_wheel *v34; // edx
  pulse_sum_normal *pulse_sum_wheel_fwd; // eax
  rigid_body *v36; // ecx
  rigid_body *v37; // ecx
  double v38; // st7
  double v39; // st7
  double v40; // st6
  double v41; // st7
  pulse_sum_normal *v42; // eax
  double v43; // st7
  double v44; // st5
  double v45; // st6
  double v46; // st7
  pulse_sum_normal *v47; // eax
  double v48; // st6
  pulse_sum_normal *m_ps_fwd_fric; // eax
  pulse_sum_wheel *v50; // ecx
  const phys_vec3 *v51; // eax
  rigid_body *v52; // [esp-10h] [ebp-148h]
  rigid_body *v53; // [esp-8h] [ebp-140h]
  rigid_body *v54; // [esp-8h] [ebp-140h]
  rigid_body *v55; // [esp-8h] [ebp-140h]
  _BYTE v56[12]; // [esp+1Ch] [ebp-11Ch] BYREF
  phys_mat44 rot_mat; // [esp+28h] [ebp-110h] BYREF
  float v58; // [esp+6Ch] [ebp-CCh] BYREF
  float v59; // [esp+70h] [ebp-C8h]
  float v60; // [esp+74h] [ebp-C4h]
  phys_vec3 fric_ud; // [esp+78h] [ebp-C0h] BYREF
  phys_vec3 b1_r_displace; // [esp+88h] [ebp-B0h] BYREF
  phys_vec3 b2_r; // [esp+98h] [ebp-A0h] BYREF
  phys_vec3 wheel_axis; // [esp+A8h] [ebp-90h] BYREF
  phys_vec3 b1_suspension_dir; // [esp+B8h] [ebp-80h] BYREF
  phys_vec3 b1_r; // [esp+C8h] [ebp-70h] BYREF
  unsigned int m_wheel_flags; // [esp+E8h] [ebp-50h]
  float v68; // [esp+ECh] [ebp-4Ch] BYREF
  float v69; // [esp+F0h] [ebp-48h]
  double ps_wheel; // [esp+F4h] [ebp-44h] OVERLAPPED
  float m_wheel_radius; // [esp+108h] [ebp-30h]
  float v72; // [esp+10Ch] [ebp-2Ch] BYREF
  float v73; // [esp+110h] [ebp-28h]
  float k_; // [esp+114h] [ebp-24h]
  float m_hard_limit_dist; // [esp+128h] [ebp-10h]
  unsigned int v76[2]; // [esp+12Ch] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+138h] [ebp+0h]

  v76[0] = a2;
  v76[1] = retaddr;
  this->m_wheel_flags &= ~4u;
  this->m_wheel_fwd = 0.0;
  this->m_ps_suspension = 0;
  this->m_ps_side_fric = 0;
  this->m_ps_fwd_fric = 0;
  m_wheel_flags = this->m_wheel_flags;
  if ( (m_wheel_flags & 1) != 0 )
  {
    b1 = this->b1;
    phys_multiply((phys_vec3 *)&wheel_axis.y, &this->b1->m_mat, &this->m_b1_suspension_dir_loc);
    m_wheel_radius = this->m_wheel_radius;
    v72 = wheel_axis.y * m_wheel_radius;
    v73 = wheel_axis.z * m_wheel_radius;
    k_ = m_wheel_radius * wheel_axis.w;
    phys_multiply((const phys_vec3 *)&v68, &b1->m_mat, &this->m_b1_wheel_center_loc);
    b2 = this->b2;
    b1_suspension_dir.y = v68 + v72;
    LODWORD(m_hard_limit_dist) = &b2->m_mat;
    b1_suspension_dir.z = v69 + v73;
    b1_suspension_dir.w = *(float *)&ps_wheel + k_;
    phys_multiply((phys_vec3 *)&b1_r_displace.y, &b2->m_mat, &this->m_b2_hitp_loc);
    v7 = this->b2;
    b1_r.y = v7->m_mat.w.x + b1_r_displace.y;
    z = b1_r_displace.z;
    b1_r.z = v7->m_mat.w.y + b1_r_displace.z;
    w = b1_r_displace.w;
    b1_r.w = v7->m_mat.w.z + b1_r_displace.w;
    y = b1_suspension_dir.y;
    v72 = b1->m_mat.w.x + b1_suspension_dir.y;
    v11 = b1_suspension_dir.z;
    v73 = b1->m_mat.w.y + b1_suspension_dir.z;
    v12 = b1_suspension_dir.w;
    k_ = b1->m_mat.w.z + b1_suspension_dir.w;
    v68 = v72 - b1_r.y;
    v69 = v73 - b1_r.z;
    *(float *)&ps_wheel = k_ - b1_r.w;
    m_wheel_radius = v68 * wheel_axis.y + v69 * wheel_axis.z + *(float *)&ps_wheel * wheel_axis.w;
    v13 = m_wheel_radius;
    m_wheel_radius = this->m_hard_limit_dist - 3.400000095367432;
    if ( m_wheel_radius > v13 )
      v14 = m_wheel_flags & 0xFFFFFFFD;
    else
      v14 = m_wheel_flags | 2;
    this->m_wheel_flags = v14;
    b1_r.y = v7->m_mat.w.x + b1_r_displace.y;
    b1_r.z = z + v7->m_mat.w.y;
    b1_r.w = w + v7->m_mat.w.z;
    v72 = y + b1->m_mat.w.x;
    v73 = v11 + b1->m_mat.w.y;
    k_ = v12 + b1->m_mat.w.z;
    v68 = v72 - b1_r.y;
    v69 = v73 - b1_r.z;
    *(float *)&ps_wheel = k_ - b1_r.w;
    if ( (v14 & 2) != 0 )
    {
      phys_multiply((const phys_vec3 *)&v72, (const phys_mat44 *)LODWORD(m_hard_limit_dist), &this->m_b2_hitn_loc);
      rot_mat.w.y = -v72;
      rot_mat.w.z = -v73;
      rot_mat.w.w = -k_;
      pulse_sum_normal = pulse_sum_constraint_solver::create_pulse_sum_normal(psys);
      m_hard_limit_dist = this->m_hard_limit_dist;
      v16 = this->b2;
      v17 = pulse_sum_normal;
      v72 = m_hard_limit_dist * wheel_axis.y;
      v18 = this->b1;
      v73 = wheel_axis.z * m_hard_limit_dist;
      k_ = m_hard_limit_dist * wheel_axis.w;
      b2_r.y = b1_suspension_dir.y - v72;
      b2_r.z = b1_suspension_dir.z - v73;
      b2_r.w = b1_suspension_dir.w - k_;
      pulse_sum_normal::set(
        v17,
        (int)v76,
        v18,
        (phys_vec3 *)&b2_r.y,
        v16,
        (phys_vec3 *)&b1_r_displace.y,
        (const phys_vec3 *)&rot_mat.w.y,
        this->m_ps_cache_list,
        &PHYS_ZERO_VEC_76);
      v17->m_pulse_sum_min = -10000000.0;
      v17->m_pulse_sum_max = 0.0;
      pulse_sum_normal::setup_vel_uni_standard(v17, delta_t, 170.0);
    }
    phys_multiply((phys_vec3 *)&b2_r.y, &this->b2->m_mat, &this->m_b2_hitn_loc);
    b1_r.y = -b2_r.y;
    b1_r.z = -b2_r.z;
    b1_r.w = -b2_r.w;
    m_hard_limit_dist = b1_r.z * v69 + b1_r.y * v68 + b1_r.w * *(float *)&ps_wheel;
    v19 = m_hard_limit_dist;
    m_hard_limit_dist = b1_r.y * m_hard_limit_dist;
    v72 = m_hard_limit_dist;
    v20 = m_hard_limit_dist;
    m_hard_limit_dist = b1_r.z * v19;
    v73 = m_hard_limit_dist;
    v21 = m_hard_limit_dist;
    m_hard_limit_dist = v19 * b1_r.w;
    b1_suspension_dir.y = b1_suspension_dir.y - v72;
    b1_suspension_dir.z = b1_suspension_dir.z - v73;
    b1_suspension_dir.w = b1_suspension_dir.w - m_hard_limit_dist;
    v72 = v20;
    v73 = v21;
    k_ = m_hard_limit_dist;
    b1_r_displace.y = b1_r_displace.y - v72;
    b1_r_displace.z = b1_r_displace.z - v73;
    b1_r_displace.w = b1_r_displace.w - m_hard_limit_dist;
    m_hard_limit_dist = -this->m_roll_stability_factor;
    fric_ud.y = m_hard_limit_dist * wheel_axis.y;
    fric_ud.z = wheel_axis.z * m_hard_limit_dist;
    fric_ud.w = m_hard_limit_dist * wheel_axis.w;
    pulse_sum_wheel = pulse_sum_constraint_solver::create_pulse_sum_wheel(psys);
    pulse_sum_wheel->m_side = 0;
    pulse_sum_wheel->m_fwd = 0;
    v53 = this->b2;
    v23 = this->b1;
    m_wheel_flags = (unsigned int)pulse_sum_wheel;
    this->m_ps_suspension = &pulse_sum_wheel->m_suspension;
    pulse_sum_normal::set(
      &pulse_sum_wheel->m_suspension,
      (int)v76,
      v23,
      (phys_vec3 *)&b1_suspension_dir.y,
      v53,
      (phys_vec3 *)&b1_r_displace.y,
      (phys_vec3 *)&b1_r.y,
      &this->m_ps_cache_list[1],
      (phys_vec3 *)&fric_ud.y);
    m_ps_suspension = this->m_ps_suspension;
    m_ps_suspension->m_pulse_sum_min = -10000000.0;
    m_ps_suspension->m_pulse_sum_max = 0.0;
    v25 = this->m_ps_suspension;
    m_hard_limit_dist = this->m_suspension_stiffness_k * delta_t;
    ps_wheel = m_hard_limit_dist * delta_t;
    m_hard_limit_dist = delta_t * this->m_suspension_damp_k;
    m_hard_limit_dist = 1.0 / (ps_wheel + m_hard_limit_dist);
    pos = pulse_sum_normal::get_pos(v25, (int)v76);
    v27 = m_hard_limit_dist;
    m_hard_limit_dist = ps_wheel * m_hard_limit_dist;
    v25->m_right_side = pos * (-m_hard_limit_dist / delta_t);
    v25->m_big_dirt = 0.0;
    v25->m_cfm = v27;
    v25->m_denom = v27 + v25->m_denom;
    phys_multiply((phys_vec3 *)&b2_r.y, &this->b1->m_mat, &this->m_b1_wheel_axis_loc);
    make_rotate(COERCE_FLOAT(v76), (phys_mat44 *)v56, (phys_vec3 *)&wheel_axis.y, (phys_vec3 *)&b1_r.y);
    phys_multiply((const phys_vec3 *)&v58, (const phys_mat44 *)v56, (phys_vec3 *)&b2_r.y);
    v28 = (pulse_sum_normal *)phys_transient_allocator::allocate(
                                &psys->m_solver_memory_allocator,
                                160,
                                16,
                                0,
                                SOLVER_MEMORY_ALLOCATOR_ERROR_MSG);
    *(unsigned int *)(m_wheel_flags + 176) = v28;
    v54 = this->b2;
    v52 = this->b1;
    this->m_ps_side_fric = v28;
    pulse_sum_normal::set(
      v28,
      (int)v76,
      v52,
      (phys_vec3 *)&b1_suspension_dir.y,
      v54,
      (phys_vec3 *)&b1_r_displace.y,
      (const phys_vec3 *)&v58,
      &this->m_ps_cache_list[2],
      (phys_vec3 *)&fric_ud.y);
    m_ps_side_fric = this->m_ps_side_fric;
    m_ps_side_fric->m_right_side = 0.0;
    m_ps_side_fric->m_big_dirt = 0.0;
    m_ps_side_fric->m_cfm = 0.0;
    v30 = this->m_ps_suspension;
    v68 = v59 * b1_r.w - v60 * b1_r.z;
    v69 = v60 * b1_r.y - b1_r.w * v58;
    *(float *)&ps_wheel = b1_r.z * v58 - v59 * b1_r.y;
    v72 = -v68;
    v73 = -v69;
    k_ = -*(float *)&ps_wheel;
    relative_velocity = pulse_sum_normal::get_relative_velocity(v30, (int)v76, (phys_vec3 *)&b2_r.y);
    m_wheel_state = this->m_wheel_state;
    m_hard_limit_dist = relative_velocity->y * v73 + v72 * relative_velocity->x + relative_velocity->z * k_;
    this->m_wheel_vel = m_hard_limit_dist / this->m_wheel_radius;
    if ( !m_wheel_state && this->m_acceleration_factor_k < 0.00009999999747378752
      || m_wheel_state == 1 && this->m_braking_factor_k < 0.00009999999747378752 )
    {
      pulse_sum_normal::set_pulse_sum_limits_parent_ratio(
        this->m_ps_side_fric,
        this->m_side_fric_k,
        this->m_ps_suspension);
      return;
    }
    v33 = this->m_ps_side_fric;
    v34 = (pulse_sum_wheel *)m_wheel_flags;
    v33->m_pulse_sum_min = -10000000.0;
    v33->m_pulse_sum_max = 10000000.0;
    pulse_sum_wheel_fwd = pulse_sum_constraint_solver::create_pulse_sum_wheel_fwd(psys, v34);
    m_hard_limit_dist = -this->m_pitch_stability_factor;
    v36 = this->b2;
    b2_r.y = m_hard_limit_dist * wheel_axis.y;
    v55 = v36;
    v37 = this->b1;
    b2_r.z = wheel_axis.z * m_hard_limit_dist;
    v38 = m_hard_limit_dist * wheel_axis.w;
    this->m_ps_fwd_fric = pulse_sum_wheel_fwd;
    b2_r.w = v38;
    pulse_sum_normal::set(
      pulse_sum_wheel_fwd,
      (int)v76,
      v37,
      (phys_vec3 *)&b1_suspension_dir.y,
      v55,
      (phys_vec3 *)&b1_r_displace.y,
      (const phys_vec3 *)&v72,
      &this->m_ps_cache_list[3],
      (phys_vec3 *)&b2_r.y);
    if ( this->m_wheel_state )
    {
      m_ps_fwd_fric = this->m_ps_fwd_fric;
      m_ps_fwd_fric->m_right_side = 0.0;
      m_ps_fwd_fric->m_big_dirt = 0.0;
      m_ps_fwd_fric->m_cfm = 0.0;
      v47 = this->m_ps_fwd_fric;
      *((float *)&ps_wheel + 1) = this->m_braking_factor_k * delta_t;
      v46 = *((float *)&ps_wheel + 1);
      v48 = -*((float *)&ps_wheel + 1);
    }
    else
    {
      *((float *)&ps_wheel + 1) = this->m_desired_speed_k * this->m_turning_radius_ratio_max_speed;
      m_hard_limit_dist = this->m_wheel_radius;
      v39 = m_hard_limit_dist;
      m_hard_limit_dist = this->m_acceleration_factor_k * this->m_turning_radius_ratio_accel;
      v40 = v39 * v39;
      v41 = m_hard_limit_dist * delta_t;
      m_hard_limit_dist = v40;
      m_wheel_radius = v41 / m_hard_limit_dist;
      if ( m_wheel_radius <= 0.000001 )
        m_wheel_radius = 0.0000099999997;
      v42 = this->m_ps_fwd_fric;
      m_hard_limit_dist = 1.0 / m_wheel_radius;
      v43 = *((float *)&ps_wheel + 1);
      v42->m_right_side = this->m_wheel_radius * *((float *)&ps_wheel + 1);
      v42->m_big_dirt = 0.0;
      v44 = m_hard_limit_dist;
      v42->m_cfm = m_hard_limit_dist;
      v42->m_denom = v44 + v42->m_denom;
      v45 = v43;
      v46 = 0.0;
      if ( v45 > 0.00009999999747378752 )
      {
        v47 = this->m_ps_fwd_fric;
        v47->m_pulse_sum_min = 0.0;
        v46 = 10000000.0;
LABEL_22:
        v50 = (pulse_sum_wheel *)m_wheel_flags;
        v47->m_pulse_sum_max = v46;
        pulse_sum_wheel::set_side_fwd_ratios(v50, this->m_side_fric_k, this->m_fwd_fric_k, this->m_side_fric_max);
        v51 = pulse_sum_normal::get_relative_velocity_change_dir(this->m_ps_fwd_fric, (phys_vec3 *)&b2_r.y);
        this->m_wheel_fwd = v51->y * v73 + v51->x * v72 + v51->z * k_;
        return;
      }
      v47 = this->m_ps_fwd_fric;
      if ( v45 >= -0.00009999999747378752 )
      {
        v47->m_pulse_sum_min = -10000000.0;
        v46 = 10000000.0;
        goto LABEL_22;
      }
      v48 = -10000000.0;
    }
    v47->m_pulse_sum_min = v48;
    goto LABEL_22;
  }
}

void __thiscall pulse_sum_normal::setup_vel_uni_standard(
        pulse_sum_normal *this,
        float delta_t,
        float max_penalty_restitution_vel)
{
  double v3; // st7
  int savedregs; // [esp+4h] [ebp+0h] BYREF
  float delta_tc; // [esp+Ch] [ebp+8h]
  float delta_ta; // [esp+Ch] [ebp+8h]
  float delta_tb; // [esp+Ch] [ebp+8h]

  v3 = 0.016666668;
  if ( delta_t > 0.016666668 )
    v3 = delta_t;
  delta_tc = v3;
  delta_ta = -pulse_sum_normal::get_pos(this, (int)&savedregs) / delta_tc;
  this->m_big_dirt = delta_ta;
  if ( delta_ta < 0.0 )
    this->m_big_dirt = delta_ta * 0.300000011920929;
  delta_tb = -max_penalty_restitution_vel;
  if ( delta_tb > (double)this->m_big_dirt )
    this->m_big_dirt = delta_tb;
  if ( this->m_big_dirt < 0.0 )
  {
    this->m_right_side = 0.0;
  }
  else
  {
    this->m_right_side = this->m_big_dirt;
    this->m_big_dirt = 0.0;
  }
  this->m_cfm = 0.0;
}

pulse_sum_normal *__thiscall pulse_sum_constraint_solver::create_pulse_sum_wheel_fwd(
        pulse_sum_constraint_solver *this,
        pulse_sum_wheel *psw)
{
  pulse_sum_normal *result; // eax

  if ( !psw->m_side
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\constraint_solver\\pulse_sum_constraint_solver.h",
         196,
         "psw->m_side",
         "") )
  {
    __debugbreak();
  }
  result = (pulse_sum_normal *)phys_transient_allocator::allocate(
                                 &this->m_solver_memory_allocator,
                                 160,
                                 16,
                                 0,
                                 SOLVER_MEMORY_ALLOCATOR_ERROR_MSG);
  psw->m_fwd = result;
  return result;
}

pulse_sum_wheel *__thiscall pulse_sum_constraint_solver::create_pulse_sum_wheel(pulse_sum_constraint_solver *this)
{
  pulse_sum_wheel **v2; // edi
  pulse_sum_wheel **m_last_next_ptr; // ecx

  v2 = (pulse_sum_wheel **)phys_transient_allocator::allocate(
                             &this->m_solver_memory_allocator,
                             192,
                             16,
                             0,
                             SOLVER_MEMORY_ALLOCATOR_ERROR_MSG);
  if ( !this->m_list_pulse_sum_wheel.m_last_next_ptr
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         230,
         "m_last_next_ptr",
         "") )
  {
    __debugbreak();
  }
  *v2 = 0;
  m_last_next_ptr = this->m_list_pulse_sum_wheel.m_last_next_ptr;
  ++this->m_list_pulse_sum_wheel.m_alloc_count;
  *m_last_next_ptr = (pulse_sum_wheel *)v2;
  this->m_list_pulse_sum_wheel.m_last_next_ptr = v2;
  return (pulse_sum_wheel *)v2;
}

