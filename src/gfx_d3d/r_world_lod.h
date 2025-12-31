#pragma once

struct __declspec(align(4)) GfxWorldLodChain // sizeof=0x18
{
    float origin[3];
    float lastDist;
    unsigned int firstLodInfo;
    unsigned __int16 lodInfoCount;
    // padding byte
    // padding byte
};

struct LodChainState // sizeof=0xC
{
    float m_fade;
    unsigned __int8 m_lastLevel;
    unsigned __int8 m_curLevel;
    // padding byte
    // padding byte
    const GfxWorldLodChain *m_lodChain;

    void __thiscall Init(const GfxWorldLodChain *lodChain, int localClientNum);
    void __thiscall UpdateLevel(int newLevel, int localClientNum);
    void __thiscall UpdateFade(float dt, int localClientNum);
};


void __cdecl SetFadeForSortedSurfaces(float val, int lodInfoIndex, int localClientNum);
void __cdecl R_WorldLod_Init();
void __cdecl R_WorldLod_FrameUpdate(float curTime, float *camPos, int localClientNum);
// local variable allocation has failed, the output may be wrong!
void    UpdateChain(int index, const float *inputCamPos, float dt, int localClientNum);
//double __cdecl math::Abs(const math::Dir3 *_v);

extern unsigned __int8 *s_lodVals;
