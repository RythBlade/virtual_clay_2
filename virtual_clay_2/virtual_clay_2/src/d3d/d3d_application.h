#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <WTypesbase.h>
#include "d3d_sphere.h"

//#include <d3d11.h>
//#include <d3dx11.h>
//#include <d3dx10.h>		// directx 11 extends 10, so we'llo need some of the library from 10
//#include "Debug.h"
//#include "Utility.h"
//#include "D3DWorldTransform.h"
//#include "AppInfo.h"
//#include "EventBroker.h"
//#include "EventNames.h"
//#include "InputHandler.h"
//#include "D3DCamera.h"
//#include "ParticleManager.h"
//#include "FrameController.h"
//#include "D3DStringLayer.h"
//#include "DataTypes.h"
//#include "Performance.h"
//#include "StringTable.h"
//#include "D3DSphere.h"
//#include "ParticleManagerInterface.h"
//#include "ParticleManagerRenderer.h"
//#include "ToolManager.h"

//#if defined(DEBUG) | defined(_DEBUG) // when in debug mode, define a simple debug macro to output the HRESULT
//	#ifndef HR
//		#define HR(x)									\
//				{												\
//					HRESULT hResult = S_OK;						\
//					hResult = x;								\
//																\
//					if(FAILED(hResult))							\
//					{											\
//					}											\
//				}
//	#endif
//
//				/*
//				DXTrace(								\
//							__FILE__,							\
//							__LINE__,							\
//							hResult,							\
//							DEBUG_ERROR_MSG,					\
//							true);								\
//				*/
//#else
//	#ifndef HR // when not debug, replace debug output with the original call
//		#define HR(x) (x)
//	#endif
//#endif

//#define HRReturn(x)				\
//{								\
//	HRESULT hresult = S_OK;		\
//	hresult = x;				\
//	HR(hresult)					\
//								\
//	if(FAILED(hresult))			\
//	{							\
//		return hresult;			\
//	}							\
//}


// macro to perform the debug hr check as well as fire the application crashed event and set the flag
// also returns the HRESULT back to the calling function.
//#define HrCheck(x)												\
//{																\
//    HRESULT hr = S_OK;											\
//	hr = x;														\
//	if(FAILED(hr))												\
//	{															\
//		crashed = true;											\
//	}															\
//																\
//	HRReturn(hr);												\
//}

namespace virtual_clay
{
    class D3DApplication
    {
    public:
        D3DApplication(/*EventBroker *eventBroker, InputHandler *inputHandler, FrameController *frameController*/ );
        ~D3DApplication();

        HRESULT initD3D( HWND hWnd );			// sets up and initialises Direct3D
        HRESULT cleanD3D();					// closes Direct3D and releases the memory

        HRESULT run();						// runs one frame of the application

        // data accessors
        inline bool getCrashed() { return crashed; }
        //inline InputHandler *getInputHandler() { return pInputHandler; }

    private:
        HRESULT initPipeline();				// loads and prepares the shaders
        HRESULT initGraphics();				// creates the shape to render

        HRESULT update();					// performs update routines for the frame
        void handleCameraInput();			// handles the camera input
        void handleInput();					// handles other input
        HRESULT renderFrame();				// renders the frame
        HRESULT render3DWorld();			// renders all of the 3D world objects
        HRESULT	render2DText();				// renders all of the 2D text

        HRESULT checkFeatureSupport();		// checks whether the current hardware can run the required DirectX 11 features.
		
    private:
        // DX 11 API variables
        IDXGISwapChain *pSwapchain;						// pointer to the swap chain interface for this app
        
        ID3D11Device *pDev;								// pointer to a directX 11 device interface for this app
        ID3D11DeviceContext *pDevcon;					// pointer to the directx 11 device context for this app
        
        ID3D11RenderTargetView *pBackbuffer;			// pointer to the back buffer for this app
        
        ID3D11Texture2D* pDepthStencil;					// the texture used as the depth buffer for the rendering context
        ID3D11DepthStencilView* pDepthStencilView;		// the depth stencil view
        
        ID3D11BlendState *pBlend;						// pointer to a blend state interface
        
                                                        /*				DX 11 Shader variables			*/
        //ID3D11VertexShader *p3DVS;						// the standard vertex shader used for rendering 3D objects
        //ID3D11PixelShader *p3DPS;						// the standard pixel shader used for rendering 3D objects
        //ID3D11Buffer *pConstantBuffer;					// the buffer to contain the world-view-projection matrices
        //ID3D11Buffer *pLightingConstantBuffer;			// the buffer to contain the lighting details
        //
        //ID3D11VertexShader *pTextVS;					// the vertex shader used for rendering text
        //ID3D11PixelShader *pTextPS;						// the pixel shader used for rendering text
        //
        //                                                /*				DX 11 Vertex layout				*/
        //ID3D11InputLayout *pLayout;						// the input layout object - used to describe the vertex format	

        /*				Local state variables			*/
        bool crashed;									// flag to show when the system crash event has been thrown

        /*			Supporting operational classes		*/
        //EventBroker *pEventBroker;						// the event broker for the application
        //InputHandler *pInputHandler;					// pointer to an input handler
        //FrameController *pFrameController;				// the frame controller for the application

        /*				User interface objects			*/
        //Performance performance;						// object which displays the performance statistics for the application
        //D3DStringLayer stringLayer;						// the string layer which outputs strings to the screen
        //ParticleManagerInterface particleInterface;		// the particle manager interface
        //ParticleManagerRenderer particleRenderer;		// the rendering object for the particle manager

        /*				Game world objects				*/
        //ParticleManager particleManager;				// the particle manager
        D3DSphere mSphere;								// class which create and maintains the geometry of a sphere
        //D3DCamera camera;								// the camera object
        //ToolManager mToolManager;

        bool m_allowDirectCompute;						// flag to show if the hardware supports DirectCompute
    };
}
