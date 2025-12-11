#pragma once
#include "cmd.h"

enum MapProfileTrackedValue : __int32
{                                       // XREF: ?ProfLoad_BeginTrackedValue@@YAXW4MapProfileTrackedValue@@@Z/r
    MAP_PROFILE_FILE_OPEN = 0x0,
    MAP_PROFILE_FILE_SEEK = 0x1,
    MAP_PROFILE_FILE_READ = 0x2,
    MAP_PROFILE_VALUE_MAX = 0x3,
};

struct MapProfileElement // sizeof=0x18
{                                       // XREF: MapProfileEntry/r
    unsigned __int64 ticksStart;
    unsigned __int64 ticksTotal;
    unsigned __int64 ticksSelf;
};

struct MapProfileEntry // sizeof=0x70
{                                       // XREF: mapLoadProfile_t/r
    const char *label;
    int accessCount;
    unsigned __int64 ticksStart;
    unsigned __int64 ticksTotal;
    unsigned __int64 ticksSelf;
    int indent;
    MapProfileEntry *parent;
    MapProfileElement elements[3];
};

struct MapProfileHotSpot // sizeof=0x18
{                                       // XREF: ??$_Insertion_sort1@PAUMapProfileHotSpot@@P6A_NABU1@0@ZU1@@std@@YAXPAUMapProfileHotSpot@@0P6A_NABU1@1@Z0@Z/r
    const char *label;                  // XREF: std::_Insertion_sort1<MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot>(MapProfileHotSpot *,MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot *)+38/w
    int accessCount;                    // XREF: std::_Insertion_sort1<MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot>(MapProfileHotSpot *,MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot *)+3E/w
    __int64 ticksSelf;                  // XREF: std::_Insertion_sort1<MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot>(MapProfileHotSpot *,MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot *)+44/w
    __int64 ticksFile;                  // XREF: std::_Insertion_sort1<MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot>(MapProfileHotSpot *,MapProfileHotSpot *,bool (*)(MapProfileHotSpot const &,MapProfileHotSpot const &),MapProfileHotSpot *)+50/w
};

bool __cdecl ProfLoad_IsActive();
void __cdecl ProfLoad_BeginTrackedValue(MapProfileTrackedValue type);
void __cdecl ProfLoad_BeginTrackedValueTicks(MapProfileElement *value, unsigned __int64 ticks);
void __cdecl ProfLoad_EndTrackedValue(MapProfileTrackedValue type);
void __cdecl ProfLoad_EndTrackedValueTicks(MapProfileElement *value, unsigned __int64 ticks);
void __cdecl ProfLoad_Init();
void __cdecl ProfLoad_Activate();
void __cdecl ProfLoad_Deactivate();
void    ProfLoad_Print();
MapProfileEntry * ProfLoad_CalculateSelfTicks();
int ProfLoad_PrintTree();
void __cdecl ProfLoad_GetEntryRowText(const MapProfileEntry *entry, char *rowText, unsigned int sizeofRowText);
void ProfLoad_PrintHotSpots();
bool __cdecl ProfLoad_CompareHotSpotNames(const MapProfileHotSpot *hotSpot0, const MapProfileHotSpot *hotSpot1);
bool __cdecl ProfLoad_CompareHotSpotTicks(const MapProfileHotSpot *hotSpot0, const MapProfileHotSpot *hotSpot1);
void __cdecl ProfLoad_Begin(const char *label);
MapProfileEntry *__cdecl Com_GetEntryForNewLabel(const char *label);
void __cdecl ProfLoad_End();
void __cdecl ProfLoad_DrawOverlay(rectDef_s *rect);
int    ProfLoad_DrawTree();
