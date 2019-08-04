#pragma once
#include <wtypes.h>

namespace virtual_clay
{
    UINT const gSampleDescCount = 1;
    UINT const gSampleDescQuality = 0;

#define D3D_VIEWPORT_TOPLEFT_X			0
#define D3D_VIEWPORT_TOPLEFT_Y			0
#define D3D_VIEWPORT_WIDTH				g_defaultWindowWidth
#define D3D_VIEWPORT_HEIGHT				g_defaultWindowHeight
#define D3D_VIEWPORT_MIN_DEPTH			0.0f
#define D3D_VIEWPORT_MAX_DEPTH			1.0f
}