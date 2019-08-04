#pragma once

#include <DirectXMath.h>

struct VERTEX				// DirectX custom vertex declaration
{
	DirectX::XMFLOAT3 Position;	// position
	DirectX::XMFLOAT3 normal;	// surface normal at vertex
	float tu;
	float tv;				// texture coordinates at vertex
};