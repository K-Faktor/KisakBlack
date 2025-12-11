#pragma once

//const cpose_t *__cdecl Ragdoll_HandleBody(const cpose_t *pose);
unsigned short __cdecl Ragdoll_HandleBody(const struct cpose_t *pose);
struct BoneOrientation *__cdecl Ragdoll_BodyBoneOrientations(struct RagdollBody *body);
struct BoneOrientation *__cdecl Ragdoll_BodyPrevBoneOrientations(struct RagdollBody *body);
void __cdecl Ragdoll_DoControllers(const cpose_t *pose, const struct DObj *obj, int *partBits);
