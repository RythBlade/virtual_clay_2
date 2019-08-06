// LCCREDIT start of 3DGraph1
#include <iostream>
#include "d3d_camera.h"
#include "settings/window_settings.h"

/*----------------------------------------------------------------------------\
 *								Camera defaults                  			  |
 *----------------------------------------------------------------------------*/
#define DEFAULT_CAMERA_FOV				DirectX::XM_PIDIV4
#define DEFAULT_CAMERA_NEAR_PLANE		0.01f
#define DEFAULT_CAMERA_FAR_PLANE		1000.0f


namespace virtual_clay
{
	/*----------------------------------------------------------------------------\
	 *						Initialisation and Clean up                   		  |
	 *----------------------------------------------------------------------------*/

	 /*
	   Constructs and initialises a camera object
	   */
	D3DCamera::D3DCamera()
	{
		m_cameraPos = DirectX::XMFLOAT3( 0.0f, 0.0f, -1.0f );
		m_cameraTarget = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
		m_cameraUp = DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f );

		m_horizontalRotation = 0.0f;

		// make sure we are looking down the z axis
		m_verticalRotation = DirectX::XM_PIDIV2;
		m_radius = 1.0f;

		m_nearPlane = DEFAULT_CAMERA_NEAR_PLANE;
		m_farPlane = DEFAULT_CAMERA_FAR_PLANE;
		m_fieldOfView = DEFAULT_CAMERA_FOV;
	}

	/*
		Destructs a camera object
	 */
	D3DCamera::~D3DCamera()
	{
	}

	/*----------------------------------------------------------------------------\
	 *						Transformation pipeline code						  |
	 *----------------------------------------------------------------------------*/

	 /*
	   Sets the view and projection transformation matrices.
	   */
	void D3DCamera::setMatrices()
	{
		setViewMatrix();
		setProjectionMatrix();
	}

	/*
	  Sets the view transformation matrix.
	  */
	void D3DCamera::setViewMatrix()
	{
		DirectX::FXMVECTOR cameraPos = DirectX::XMLoadFloat3( &m_cameraPos );
		DirectX::FXMVECTOR cameraTarget = DirectX::XMLoadFloat3( &m_cameraTarget );
		DirectX::FXMVECTOR cameraUp = DirectX::XMLoadFloat3( &m_cameraUp );

		m_matView = DirectX::XMMatrixLookAtLH( cameraPos, cameraTarget, cameraUp );
	}


	/*
	  Sets the projection transformation matrix.
	  */
	void D3DCamera::setProjectionMatrix()
	{
		m_matProj = DirectX::XMMatrixPerspectiveFovLH( m_fieldOfView, g_defaultWindowWidth / g_defaultWindowWidth, m_nearPlane, m_farPlane );
	}

	/*----------------------------------------------------------------------------\
	 *							Camera controls                   				  |
	 *----------------------------------------------------------------------------*/

	 /*
	  sets the horizontal rotation of the camera

	  Parameter list
		 rotation:		to horizontal rotation of the camera
	  */
	void D3DCamera::setHorizontalRotation( float rotation )
	{
		m_horizontalRotation = fmod( rotation, 2.0f * DirectX::XM_PI );

		updateCameraVectors();
	}

	/*
	 sets the vertical rotation of the camera

	 Parameter list
		rotation:		to vertical rotation of the camera
	 */
	void D3DCamera::setVerticalRotation( float rotation )
	{
		m_verticalRotation = rotation;

		if ( m_verticalRotation > DirectX::XM_PI )
		{
			m_verticalRotation = DirectX::XM_PI;
		}
		else if ( m_verticalRotation < 0.0f )
		{
			m_verticalRotation = 0.0f;
		}

		updateCameraVectors();
	}

	void D3DCamera::setRadius( float radius )
	{
		if ( radius != 0 )
		{
			m_radius = radius;

			if ( m_radius < 0.0f )
			{
				m_radius = 0.0f;
			}

			updateCameraVectors();
		}
	}

	/*
	 recalculates and updates the camera vectors
	 */
	void D3DCamera::updateCameraVectors()
	{
		DirectX::XMFLOAT3 forward( sinf( m_horizontalRotation ), cosf( m_verticalRotation ), cosf( m_horizontalRotation ) );

		DirectX::XMVECTOR vectorToNormalise = DirectX::XMLoadFloat3( &forward );

		vectorToNormalise = DirectX::XMVector3Normalize( vectorToNormalise );

		DirectX::XMStoreFloat3( &forward, vectorToNormalise );

		m_cameraPos.x = m_radius * forward.x;
		m_cameraPos.y = m_radius * forward.y;
		m_cameraPos.z = m_radius * forward.z;
	}

	// end of 3DGraph1

}