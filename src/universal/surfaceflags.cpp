#include "surfaceflags.h"

int __cdecl Com_SurfaceTypeFromName(const char *name)
{
  int i; // [esp+0h] [ebp-4h]

  if ( !name )
    return -1;
  if ( !I_stricmp(name, "default") )
    return 0;
  for ( i = 0; i < 30; ++i )
  {
    if ( !I_stricmp(name, infoParms[i].name) )
      return (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & dword_E0F618[5 * i]) >> 20);
  }
  return -1;
}

const char *__cdecl Com_SurfaceTypeToName(int iTypeIndex)
{
  if ( iTypeIndex <= 0 || iTypeIndex >= 31 )
    return "default";
  if ( (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & dword_E0F618[5 * iTypeIndex - 5]) >> 20) != iTypeIndex
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\surfaceflags.cpp",
          149,
          0,
          "%s",
          "SURF_TYPEINDEX( infoParms[iTypeIndex - 1].surfaceFlags ) == iTypeIndex") )
  {
    __debugbreak();
  }
  return infoParms[iTypeIndex - 1].name;
}

bool __cdecl Com_SurfaceBurns(int iTypeIndex)
{
  bool burns; // [esp+7h] [ebp-1h]

  switch ( iTypeIndex )
  {
    case 1:
    case 3:
    case 4:
    case 7:
    case 8:
    case 10:
    case 15:
    case 21:
    case 23:
    case 24:
    case 25:
      burns = 1;
      break;
    default:
      burns = 0;
      break;
  }
  return burns;
}

bool __cdecl Com_SurfaceFloats(int iTypeIndex)
{
  return iTypeIndex == 21 || iTypeIndex > 23 && iTypeIndex <= 27;
}

double __cdecl Com_SurfaceDensity(int iTypeIndex)
{
  float density; // [esp+4h] [ebp-4h]

  switch ( iTypeIndex )
  {
    case 16:
      density = 0.3f;
      break;
    case 21:
      density = 0.5f;
      break;
    case 24:
      density = 0.25f;
      break;
    case 25:
      density = 0.6f;
      break;
    case 26:
      density = 0.2f;
      break;
    case 27:
      density = 0.1f5000001;
      break;
    default:
      density = 1.0f;
      break;
  }
  return density;
}

double __cdecl Com_SurfaceFrictionScale(int iTypeIndex)
{
  float friction; // [esp+4h] [ebp-4h]

  switch ( iTypeIndex )
  {
    case 6:
    case 10:
    case 11:
      friction = 0.9f;
      break;
    case 12:
      friction = 0.75f;
      break;
    case 14:
    case 19:
      friction = 0.8f;
      break;
    default:
      friction = 1.0f;
      break;
  }
  return friction;
}

double __cdecl Com_SurfaceBounceScale(int iTypeIndex)
{
  float bounce; // [esp+4h] [ebp-4h]

  switch ( iTypeIndex )
  {
    case 6:
    case 10:
    case 11:
      bounce = 0.3f;
      break;
    case 14:
      bounce = 0.1f;
      break;
    case 19:
      bounce = 0.2f;
      break;
    default:
      bounce = 1.0f;
      break;
  }
  return bounce;
}

