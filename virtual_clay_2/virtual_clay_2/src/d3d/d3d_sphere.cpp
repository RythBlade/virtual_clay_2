// LCCREDIT start of LunaDX10.2
#include "d3d_sphere.h"
#include "macros.h"
 
namespace virtual_clay
{
	D3DSphere::D3DSphere()
		: mNumVertices( 0 )
		, mNumFaces( 0 )
		, md3dDevice( 0 )
		, mVB( 0 )
		, mIB( 0 )
		, mRadius( 0.0f )
		, mNumSlices( 0 )
		, mNumStacks( 0 )
	{
	}

	D3DSphere::~D3DSphere()
	{
		// ensure the COM objects are all cleaned up
		SafeRelease( mVB );
		SafeRelease( mIB );
	}

	HRESULT D3DSphere::init( ID3D11Device* device, ID3D11DeviceContext* pDeviceContext, float radius, UINT numSlices, UINT numStacks )
	{
		md3dDevice = device;
		pDevcon = pDeviceContext;

		mRadius = radius;
		mNumSlices = numSlices;
		mNumStacks = numStacks;

		std::vector<VERTEX> vertices;
		std::vector<DWORD> indices;

		buildStacks( vertices, indices );

		mNumVertices = static_cast< UINT >( vertices.size() );
		mNumFaces = static_cast< UINT >( indices.size() / 3 );

		//----------------------------- create the vertex buffer---------------------------
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA defaultData;

		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		ZeroMemory( &defaultData, sizeof( defaultData ) );

		defaultData.pSysMem = &vertices[ 0 ];

		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = sizeof( VERTEX ) * mNumVertices;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		HRESULT result = device->CreateBuffer( &bufferDesc, &defaultData, &mVB );

		if ( FAILED( result ) )
		{
			return result;
		}

		//----------------------------- create the index buffer----------------------------
		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		ZeroMemory( &defaultData, sizeof( defaultData ) );

		defaultData.pSysMem = &indices[ 0 ];

		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = sizeof( DWORD ) * mNumFaces * 3;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		result = device->CreateBuffer( &bufferDesc, &defaultData, &mIB );

		if ( FAILED( result ) )
		{
			return result;
		}

		return S_OK;
	}

	void D3DSphere::setBuffers()
	{
		UINT stride = sizeof( VERTEX );
		UINT offset = 0;
		pDevcon->IASetVertexBuffers( 0, 1, &mVB, &stride, &offset );
		pDevcon->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
	}

	void D3DSphere::draw()
	{
		pDevcon->DrawIndexed( mNumFaces * 3, 0, 0 );
	}

	void D3DSphere::buildStacks( VertexList& vertices, IndexList& indices )
	{
		float phiStep = DirectX::XM_PI / mNumStacks;

		// do not count the poles as rings
		UINT numRings = mNumStacks - 1;

		// Compute vertices for each stack ring.
		for ( UINT i = 1; i <= numRings; ++i )
		{
			float phi = i * phiStep;

			// vertices of ring
			float thetaStep = 2.0f * DirectX::XM_PI / mNumSlices;
			for ( UINT j = 0; j <= mNumSlices; ++j )
			{
				float theta = j * thetaStep;

				VERTEX v;

				// spherical to cartesian
				v.Position.x = mRadius * sinf( phi ) * cosf( theta );
				v.Position.y = mRadius * cosf( phi );
				v.Position.z = mRadius * sinf( phi ) * sinf( theta );
				
				DirectX::XMVECTOR vectorToNormalise = DirectX::XMLoadFloat3( &v.Position );

				DirectX::XMStoreFloat3( &v.normal, vectorToNormalise );

				vertices.push_back( v );
			}
		}

		// poles: note that there will be texture coordinate distortion

		VERTEX poles[] =
		{
			{DirectX::XMFLOAT3( 0.0f, -mRadius, 0.0f ), DirectX::XMFLOAT3( 0.0f, -1.0f, 0.0f ), 0.0f, 1.0f},
			{DirectX::XMFLOAT3( 0.0f, mRadius, 0.0f ), DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ), 0.0f, 0.0f}
		};

		vertices.push_back( poles[ 0 ] );
		vertices.push_back( poles[ 1 ] );

		UINT northPoleIndex = (UINT )vertices.size() - 1;
		UINT southPoleIndex = (UINT )vertices.size() - 2;

		UINT numRingVertices = mNumSlices + 1;

		// Compute indices for inner stacks (not connected to poles).
		for ( UINT i = 0; i < mNumStacks - 2; ++i )
		{
			for ( UINT j = 0; j < mNumSlices; ++j )
			{
				indices.push_back( i * numRingVertices + j );
				indices.push_back( i * numRingVertices + j + 1 );
				indices.push_back( ( i + 1 ) * numRingVertices + j );

				indices.push_back( ( i + 1 ) * numRingVertices + j );
				indices.push_back( i * numRingVertices + j + 1 );
				indices.push_back( ( i + 1 ) * numRingVertices + j + 1 );
			}
		}

		// Compute indices for top stack.  The top stack was written 
		// first to the vertex buffer.
		for ( UINT i = 0; i < mNumSlices; ++i )
		{
			indices.push_back( northPoleIndex );
			indices.push_back( i + 1 );
			indices.push_back( i );
		}

		// Compute indices for bottom stack.  The bottom stack was written
		// last to the vertex buffer, so we need to offset to the index
		// of first vertex in the last ring.
		UINT baseIndex = ( numRings - 1 ) * numRingVertices;
		for ( UINT i = 0; i < mNumSlices; ++i )
		{
			indices.push_back( southPoleIndex );
			indices.push_back( baseIndex + i );
			indices.push_back( baseIndex + i + 1 );
		}
	}
}

// end of LunaDX10.2