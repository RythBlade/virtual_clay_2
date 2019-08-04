// LCCREDIT start of LunaDX10.2
#pragma once

#include <d3d11.h>
#include <vector>
#include <dxerr.h>
#include "Utility.h"
#include "DataTypes.h"
#include "Debug.h"

class D3DSphere
{
private:
	float mRadius;
	UINT  mNumSlices;
	UINT  mNumStacks;

	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext *pDevcon;
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

public:
	D3DSphere();
	~D3DSphere();

	HRESULT init(ID3D11Device* device, ID3D11DeviceContext *pDeviceContext, float radius, UINT numSlices, UINT numStacks);
	
	void setBuffers(); // function to set the dx11 buffers before rendering - so that they aren't constantly set if the sphere is drawn multiple times
	void draw();

private:
	typedef std::vector<VERTEX> VertexList;
	typedef std::vector<DWORD> IndexList;

	void buildStacks(VertexList& vertices, IndexList& indices);
};

// end of LunaDX10.2