#pragma once
#include "enthandle.h"
#include <clientscript/cscr_main.h>
#include <physics/phys_local.h>
#include <qcommon/common.h>
#include "teams.h"
#include "actor_badplace.h"

enum nearestNodeHeightCheck : __int32
{                                       // XREF: ?Path_NearestNode@@YAPAUpathnode_t@@QBMPAUpathsort_t@@HMPAHHW4nearestNodeHeightCheck@@@Z/r
                                        // ?Path_NearestNodeNotCrossPlanes@@YAPAUpathnode_t@@QBMPAUpathsort_t@@HMQAY01MQAMHPAHHW4nearestNodeHeightCheck@@@Z/r
    NEAREST_NODE_DO_HEIGHT_CHECK = 0x0,
    NEAREST_NODE_DONT_DO_HEIGHT_CHECK = 0x1,
};

enum ai_stance_e : __int32
{                                       // XREF: actor_s/r
                                        // ai_stance_t/r ...
    STANCE_BAD    = 0x0,
    STANCE_STAND  = 0x1,
    STANCE_CROUCH = 0x2,
    STANCE_PRONE  = 0x4,
    STANCE_ANY    = 0x7,
};

enum nodeType : __int32
{                                       // XREF: pathnode_constant_t/r
                                        // nodespawn_t/r ...
    NODE_BADNODE             = 0x0,
    NODE_PATHNODE            = 0x1,     // XREF: .rdata:nodespawns/s
    NODE_COVER_STAND         = 0x2,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_COVER_CROUCH        = 0x3,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_COVER_CROUCH_WINDOW = 0x4,     // XREF: .rdata:nodespawns/s
    NODE_COVER_PRONE         = 0x5,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_COVER_RIGHT         = 0x6,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_COVER_LEFT          = 0x7,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_COVER_WIDE_RIGHT    = 0x8,     // XREF: .rdata:nodespawns/s
    NODE_COVER_WIDE_LEFT     = 0x9,     // XREF: .rdata:nodespawns/s
    NODE_COVER_PILLAR        = 0xA,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_CONCEALMENT_STAND   = 0xB,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_CONCEALMENT_CROUCH  = 0xC,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_CONCEALMENT_PRONE   = 0xD,     // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_REACQUIRE           = 0xE,     // XREF: .rdata:nodespawns/s
    NODE_BALCONY             = 0xF,     // XREF: .rdata:nodespawns/s
    NODE_SCRIPTED            = 0x10,    // XREF: .rdata:nodespawns/s
    NODE_NEGOTIATION_BEGIN   = 0x11,    // XREF: .rdata:nodespawns/s
    NODE_NEGOTIATION_END     = 0x12,    // XREF: .rdata:nodespawns/s
    NODE_TURRET              = 0x13,    // XREF: .rdata:nodespawns/s
                                        // .data:priorityAllowedNodes/s
    NODE_GUARD               = 0x14,    // XREF: .rdata:nodespawns/s
    NODE_NUMTYPES            = 0x15,
    NODE_DONTLINK            = 0x15,
};

struct pathnode_dynamic_t // sizeof=0x20
{                                       // XREF: pathnode_t/r
    SentientHandle pOwner;
    int iFreeTime;
    int iValidTime[3];
    int inPlayerLOSTime;
    __int16 wLinkCount;
    __int16 wOverlapCount;
    __int16 turretEntNumber;
    __int16 userCount;
};

struct pathlink_s // sizeof=0xC
{                                       // XREF: .data:pathlink_s * g_tempPathNodeLinks/r
                                        // pathlink_t/r ...
    float fDist;                        // XREF: Path_ConnectPath_0+B5/w
                                        // Path_ConnectPath_0+FB/r ...
    unsigned __int16 nodeNum;           // XREF: Path_ConnectPath_0+BB/w
                                        // Path_ConnectPath_0+100/r ...
    unsigned __int8 disconnectCount;
    unsigned __int8 negotiationLink;
    unsigned __int8 ubBadPlaceCount[4]; // XREF: Path_ConnectPath_0+C1/w
                                        // Path_ConnectPath_0+106/r ...
};

struct pathnode_constant_t // sizeof=0x44
{                                       // XREF: pathnode_t/r
    nodeType type;
    unsigned __int16 spawnflags;
    unsigned __int16 targetname;        // XREF: G_FindPathNode(SpawnVar *,nodeType,int)+194/r
    unsigned __int16 script_linkName;
    unsigned __int16 script_noteworthy; // XREF: G_FindPathNode(SpawnVar *,nodeType,int)+1C6/r
    unsigned __int16 target;            // XREF: G_FindPathNode(SpawnVar *,nodeType,int)+165/r
    unsigned __int16 animscript;
    int animscriptfunc;
    float vOrigin[3];                   // XREF: G_FindPathNode(SpawnVar *,nodeType,int)+B8/r
                                        // G_FindPathNode(SpawnVar *,nodeType,int)+CF/r
    float fAngle;
    float forward[2];
    float fRadius;
    float minUseDistSq;
    __int16 wOverlapNode[2];
    __int16 wChainId;
    __int16 wChainDepth;
    __int16 wChainParent;
    unsigned __int16 totalLinkCount;
    pathlink_s *Links;
};

struct pathnode_transient_t // sizeof=0x1C
{                                       // XREF: pathnode_t/r
    int iSearchFrame;
    pathnode_t *pNextOpen;              // XREF: Path_AStarAlgorithm_CustomSearchInfo_FindCloseNode_+129/w
                                        // Path_AStarAlgorithm_CustomSearchInfo_FindCloseNode_:loc_90E13C/r ...
    pathnode_t *pPrevOpen;
    pathnode_t *pParent;
    float fCost;
    float fHeuristic;
    float costFactor;
};

struct pathnode_t // sizeof=0x80
{                                       // XREF: ?Path_UpdateLimitedDepthArcBadPlaceCount@@YAXPAUbadplace_arc_t@@HHH@Z/r
                                        // ?G_FindPathNode@@YAPAUpathnode_t@@PAUSpawnVar@@W4nodeType@@H@Z/r ...
    pathnode_constant_t constant;       // XREF: G_FindPathNode(SpawnVar *,nodeType,int)+B8/r
                                        // G_FindPathNode(SpawnVar *,nodeType,int)+CF/r ...
    pathnode_dynamic_t dynamic;
    pathnode_transient_t transient;     // XREF: Path_AStarAlgorithm_CustomSearchInfo_FindCloseNode_+129/w
                                        // Path_AStarAlgorithm_CustomSearchInfo_FindCloseNode_:loc_90E13C/r ...
};

struct pathnode_parent_t // sizeof=0x18
{                                       // XREF: .data:node_parent_world/r
    float origin_loc[3];                // XREF: get_pathnode_parent(pathnode_t const *)+17/w
                                        // get_pathnode_parent(pathnode_t const *)+27/w ...
    int entnum;                         // XREF: get_pathnode_parent(pathnode_t const *)+3F/w
    pathnode_t *m_node;                 // XREF: get_pathnode_parent(pathnode_t const *)+49/w
    pathnode_parent_t *m_next;          // XREF: get_pathnode_parent(pathnode_t const *)+53/w
};

struct pathbasenode_t // sizeof=0x10
{
    float vOrigin[3];
    unsigned int type;
};

struct pathsort_t // sizeof=0xC
{                                       // XREF: ?Sentient_NearestNode@@YIPAUpathnode_t@@PAUsentient_s@@@Z/r
                                        // ?Sentient_NearestNodeSuppressed@@YIPAUpathnode_t@@PAUsentient_s@@QAY01MQAMH@Z/r ...
    pathnode_t *node;                   // XREF: Actor_BadPlace_AttemptEscape+116/r
                                        // Actor_BadPlace_FindSafeNodeOutsideBadPlace+134/r ...
    float metric;
    float distMetric;
};

struct pathnode_tree_nodes_t // sizeof=0x8
{                                       // XREF: pathnode_tree_info_t/r
    int nodeCount;
    unsigned __int16 *nodes;
};

union pathnode_tree_info_t // sizeof=0x8
{                                       // XREF: pathnode_tree_t/r
    pathnode_tree_t *child[2];
    pathnode_tree_nodes_t s;
};

struct pathnode_tree_t // sizeof=0x10
{
    int axis;
    float dist;
    pathnode_tree_info_t u;
};

void __cdecl Path_GetType(pathnode_t *node);
void __cdecl Scr_SetPathnodeField(unsigned int entnum, unsigned int offset);
void __cdecl Path_ReadOnly(int offset);
void __cdecl Scr_GetPathnodeField(unsigned int entnum, unsigned int offset);
void __cdecl Path_CallFunctionForNodes(scriptInstance_t inst, void (__cdecl *function)(scriptInstance_t, pathnode_t *));
void __cdecl GScr_SetDynamicPathnodeField(pathnode_t *node, unsigned int index);
void    parented_pathnode_list_update(gentity_s *gent, const phys_mat44 *mat);
const pathnode_parent_t *__cdecl get_pathnode_parent(const pathnode_t *node);
void __cdecl G_UpdateTrackExtraNodes();
void __cdecl G_DropPathNodeToFloor(unsigned int nodeIndex);
// local variable allocation has failed, the output may be wrong!
void    node_droptofloor(pathnode_t *node);
void __cdecl setup_pathnode_parent(pathnode_t *node, int entnum, const float *origin_loc);
bool __cdecl is_moving_entity(gentity_s *gent);
void __cdecl G_InitPathBaseNode(pathbasenode_t *pbnode, const pathnode_t *pnode);
void __cdecl G_DropPathnodesToFloor();
void __cdecl G_SpawnPathnodeDynamic(SpawnVar *spawnVar);
void __cdecl G_ParsePathnodeScriptFields(pathnode_t *node, SpawnVar *spawnVar);
void __cdecl G_ParsePathnodeScriptField(const char *key, char *value, pathnode_t *node);
void __cdecl G_SetPathnodeScriptVariable(const char *key, char *value, pathnode_t *ent);
void __cdecl PathNode_UpdateStringField(
                const char *destKey,
                unsigned __int16 *destScrString,
                const char *key,
                char *value);
void __cdecl PathNode_UpdateFloatField(const char *destKey, float *destFloat, const char *key, const char *value);
void __cdecl PathNode_OriginMatches(const char *value, const float *nodeOrigin);
void __cdecl GScr_AddFieldsForPathnode();
void __cdecl Scr_FreePathnode(pathnode_t *node);
void __cdecl Scr_AddPathnode(pathnode_t *node);
void __cdecl Scr_GetNode();
void __cdecl Scr_GetNodeArray();
void __cdecl Scr_GetAllNodes();
pathnode_t *__cdecl Scr_GetPathnode(unsigned int index, scriptInstance_t inst);
bool __cdecl Path_CompareNodesIncreasing(const pathsort_t *ps1, const pathsort_t *ps2);
unsigned int __cdecl Path_ConvertNodeToIndex(const pathnode_t *node);
pathnode_t *__cdecl Path_ConvertIndexToNode(unsigned int index);
void __cdecl Path_Init(int restart);
void __cdecl Path_Shutdown();
void __cdecl PathNode_ClearStringField(unsigned __int16 *destScrString);
void __cdecl Path_NodesInCylinder_process(pathnode_t *pnode);
void __cdecl Path_MovingNodesInCylinder();
int __cdecl Path_NodesInCylinder(
                const float *origin,
                float maxDist,
                float maxHeight,
                pathsort_t *nodes,
                int maxNodes,
                int typeFlags);
void __cdecl Path_NodesInCylinder_r(pathnode_tree_t *tree);
int __cdecl Path_NodesInRadius(const float *origin, float maxDist, pathsort_t *nodes, int maxNodes, int typeFlags);
int __cdecl Path_IsDynamicBlockingEntity(gentity_s *ent);
bool __cdecl Path_IsBadPlaceLink(unsigned int nodeNumFrom, unsigned int nodeNumTo, team_t eTeam);
void __cdecl Path_AutoDisconnectPaths();
void __cdecl Path_PreSpawnInitPaths();
unsigned int Path_InitLinkCounts();
unsigned int Path_InitNodesDynamic();
void __cdecl Path_InitNodeDynamic(pathnode_t *loadNode);
void __cdecl Path_InitPaths();
int Path_InitLinkInfoArray();
void __cdecl Path_CheckSpawnExitNodesConnectivity();
void __cdecl Path_DrawDebugNoLinks(const pathnode_t *node, const float (*color)[4], int duration);
void __cdecl Path_DrawDebugNode(const float *cameraPos, const pathnode_t *node);
double __cdecl Path_GetDebugStringScale(const float *cameraPos, const float *origin);
void __cdecl Path_DrawDebugNodeBox(const pathnode_t *node);
void __cdecl Path_DrawDebugNearestNode(const float *vOrigin);
void __cdecl Path_DrawDebugFindPath(const float *vOrigin);
void __cdecl Path_DrawVisData();
void __cdecl Path_Update();
void __cdecl Path_DrawDebug();
void __cdecl Path_DrawDebugLink(const pathnode_t *node, int i, bool bShowAll);
void __cdecl Path_DrawDebugClaimedNodes(const float *origin, int numNodes);
bool __cdecl Path_NodesVisible(const pathnode_t *node0, const pathnode_t *node1);
int __cdecl NodeVisCacheEntry(int i, int j);
bool __cdecl Path_ExpandedNodeVisible(const pathnode_t *node0, const pathnode_t *node1);
int __cdecl ExpandedNodeVisCacheEntry(int i, int j);
pathnode_t *__cdecl Path_NearestNode(
                const float *vOrigin,
                pathsort_t *nodes,
                int typeFlags,
                float fMaxDist,
                int *returnCount,
                int maxNodes,
                nearestNodeHeightCheck heightCheck);
pathnode_t *__cdecl Path_NearestNodeNotCrossPlanes(
                const float *vOrigin,
                pathsort_t *nodes,
                int typeFlags,
                float fMaxDist,
                float (*vNormal)[2],
                float *fDist,
                int iPlaneCount,
                int *returnCount,
                int maxNodes,
                nearestNodeHeightCheck heightCheck);
bool __cdecl Path_CanClaimNode(const pathnode_t *node, sentient_s *claimer);
sentient_s *__cdecl Path_GetNodeOwner(const pathnode_t *node);
bool __cdecl Path_CanStealPriorityNode(const pathnode_t *node, sentient_s *claimer);
bool __cdecl Path_CanStealNode(const pathnode_t *node, sentient_s *claimer);
void __cdecl Path_ClaimNode(pathnode_t *node, sentient_s *claimer);
void __cdecl Path_ClaimNodeInternal(pathnode_t *node, sentient_s *claimer);
void __cdecl Path_MarkNodeOverlap(pathnode_t *node);
void __cdecl Path_RevokeClaim(pathnode_t *node, sentient_s *pNewClaimer);
void __cdecl Path_ForceClaimNode(pathnode_t *node, sentient_s *claimer);
void __cdecl Path_RelinquishNodeNow(sentient_s *claimer);
void __cdecl Path_RelinquishNodeSoon(sentient_s *claimer);
void __cdecl Path_RelinquishNode(sentient_s *claimer, int timeUntilRelinquished);
void __cdecl Path_MarkNodeInvalid(pathnode_t *node, team_t eTeam);
ai_stance_e __cdecl Path_AllowedStancesForNode(pathnode_t *node);
void __cdecl Path_ValidateNode(pathnode_t *node);
void __cdecl Path_ValidateAllNodes();
void __cdecl Path_CheckLinkLeaks();
void __cdecl Path_CheckUserCountLeaks();
void __cdecl Path_ConnectPathsForEntity(gentity_s *ent);
void __cdecl Path_ConnectPath(pathnode_t *node, int toNodeNum);
void __cdecl Path_DisconnectPathsForEntity(gentity_s *ent);
void __cdecl Path_DisconnectPath(gentity_s *ent, pathnode_t *node, pathlink_s *link);
void    Path_UpdateArcBadPlaceCount(badplace_arc_t *arc, int teamflags, int delta);
void __cdecl Path_UpdateBadPlaceCountForLink(pathlink_s *link, int teamflags, int delta);
void __cdecl Path_CheckForInwardLinks(
                pathnode_t *node,
                int currDepth,
                int maxDepth,
                float fRadiusSqrd,
                float fHeightSqrd,
                float fMaxRadiusSqrd,
                float fMaxHeightSqrd,
                float *centroid,
                badplace_arc_t *arc,
                float *side0,
                float *side1,
                int teamflags,
                int delta);
void    Path_UpdateLimitedDepthArcBadPlaceCount(
                badplace_arc_t *arc,
                int teamflags,
                int delta,
                int depth);
void __cdecl Path_UpdateBrushBadPlaceCount(gentity_s *brushEnt, int teamflags, int delta);
int __cdecl Path_IsNodeInArc(
                pathnode_t *pNode,
                const float *origin,
                float radius,
                float angle0,
                float angle1,
                float halfHeight);
bool __cdecl IsNodeEnabled(const pathnode_t *pNode);
pathnode_t *__cdecl G_FindPathNode(SpawnVar *spawnVar, nodeType type, int gameId);
void __cdecl G_ProcessPathnodeCommand(const RadiantCommand *command, SpawnVar *spawnVar);
void __cdecl G_ClearSelectedPathNode();
