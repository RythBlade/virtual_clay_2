// LCCREDIT start of 3DGraph1
#pragma once

#include <windows.h>
#include <DirectXMath.h>

namespace virtual_clay
{
	class D3DCamera
	{
	private:
		DirectX::XMMATRIX m_matView; 		// view matrix
		DirectX::XMMATRIX m_matProj; 		// projection matrix
		DirectX::XMFLOAT3 m_cameraPos;		// vector representing the position of the camera.
		DirectX::XMFLOAT3 m_cameraTarget;	// vector representing the position of the camera's target.
		DirectX::XMFLOAT3 m_cameraUp;		// vector representing the up direction for the camera.

		float m_horizontalRotation;		// the horizontal rotation of the camera
		float m_verticalRotation;		// the vertical rotation of the camera
		float m_radius;					// the radius of the camera

		float m_nearPlane;				// the near clipping plane
		float m_farPlane;				// the far clipping plane
		float m_fieldOfView;			// field of view

	public:
		D3DCamera();
		~D3DCamera();

		void setMatrices();							// Sets the view and projection transformation matrices

		void setHorizontalRotation( float rotation );	// sets the horizontal rotation of the camera
		void setVerticalRotation( float rotation );	// sets the vertical rotation of the camera
		void setRadius( float radius );				// sets the camera radius

		// data accessors
		inline DirectX::XMMATRIX* getViewMat() { return &m_matView; }				// Gets the view matrix
		inline DirectX::XMMATRIX* getProjMat() { return &m_matProj; }				// Gets the projection matrix
		inline DirectX::XMFLOAT3* getCameraPos() { return &m_cameraPos; }				// returns the camera position vector.
		inline DirectX::XMFLOAT3* getCameraTarget() { return &m_cameraTarget; }		// returns the camera position vector.

		inline float getHorizontalRotation() { return m_horizontalRotation; }	// Gets the horzontal rotation
		inline float getVerticalRotation() { return m_verticalRotation; }		// Gets the vertical rotation
		inline float getRadius() { return m_radius; }							// Gets the radius of the camera

		inline float getNearClipPlane() { return m_nearPlane; }					// Gets the near clipping plane
		inline float getFarClipPlane() { return m_farPlane; }					// Gets the near clipping plane
		inline float getFieldOfView() { return m_fieldOfView; }					// Gets the near clipping plane

		inline void setNearClipPlane( float value ) { m_nearPlane = value; }		// Gets the near clipping plane
		inline void setFarClipPlane( float value ) { m_farPlane = value; }		// Gets the near clipping plane
		inline void setFieldOfView( float value ) { m_fieldOfView = value; }		// Gets the near clipping plane

	private:
		void setViewMatrix();			// Sets the view transformation matrix
		void setProjectionMatrix();		// Sets the projection transformation matrix

		void updateCameraVectors();		// recalculates and updates the camera vectors
	};
	// end of 3DGraph1
}
