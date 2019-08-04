#include "d3d_application.h"
#include "settings/window_settings.h"
#include "settings/directx_settings.h"
#include "macros.h"
 
namespace virtual_clay
{
    /*----------------------------------------------------------------------------\
     *						Initialisation and Clean up                   		  |
     *----------------------------------------------------------------------------*/

     /*
      Constructor

      Parameter list
         eventBroker:		the event broker for the application
         inputHandler:		the input handler for the application
      */
    D3DApplication::D3DApplication(/*EventBroker *eventBroker, InputHandler *inputHandler, FrameController *frameController*/ )
		: pSwapchain( nullptr )
		, pDev( nullptr )
		, pDevcon( nullptr )
		, pBackbuffer( nullptr )
		, pDepthStencil( nullptr )
		, pDepthStencilView( nullptr )
		, pBlend( nullptr )
    {
        //pEventBroker = eventBroker;
        //pInputHandler = inputHandler;
        //pFrameController = frameController;

        crashed = false;
        m_allowDirectCompute = true;

        //pEventBroker->addEvent(EVENT_D3DCRASHED);
        //
        //performance.init(&stringLayer, frameController);
        //particleInterface.initialiseInterface(&stringLayer, inputHandler, &particleManager, &particleRenderer, &mToolManager);
    }

    /*
     Destructor
     */
    D3DApplication::~D3DApplication()
    {
    }

    /*
     Initialises Direct3D

     Parameter list
        hWnd:		handle of window
     */
    HRESULT D3DApplication::initD3D( HWND hWnd )
    {
		HRESULT result = S_OK;

        // create a struct to hold information about the swap chain
        DXGI_SWAP_CHAIN_DESC scd;

        // clear out the struct for use
        ZeroMemory( &scd, sizeof( DXGI_SWAP_CHAIN_DESC ) );

        // fill the swap chain description struct
        scd.BufferCount = 1;								// On use one backbuffer
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// use 32-bit colour
        scd.BufferDesc.Width = g_defaultWindowWidth; //SCREEN_WIDTH;				// set the back buffer width
        scd.BufferDesc.Height = g_defaultWindowWidth;//SCREEN_HEIGHT;				// set the back buffer height
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how the swap chain is to be used
        scd.OutputWindow = hWnd;							// the window to be used
        scd.SampleDesc.Count = gSampleDescCount;// D3D_SAMPLE_DESC_COUNT;		// how many multisamples - anti-aliasing
        scd.SampleDesc.Quality = gSampleDescQuality;// D3D_SAMPLE_DESC_QUALITY;
        scd.Windowed = TRUE;								// windowed/full-screen mode
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// allow full-screen switching
        scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = 0;

        // for this to work we need a directX 11 or 10.1 device so check for these
        D3D_FEATURE_LEVEL levelsWanted[] =
        {
			D3D_FEATURE_LEVEL_11_0,
            //D3D_FEATURE_LEVEL_10_1,
        };

        D3D_FEATURE_LEVEL *acceptable = NULL;

        // create a device, device context and swap chaing using the scd struct
		result = D3D11CreateDeviceAndSwapChain(
            NULL,
            D3D_DRIVER_TYPE_HARDWARE, // we don't want to use the software reference device so only specify the hardware device
            NULL,
            NULL,
            levelsWanted,
            sizeof( levelsWanted ) / sizeof( levelsWanted[ 0 ] ),
            D3D11_SDK_VERSION,
            &scd,
            &pSwapchain,
            &pDev,
            acceptable,
            &pDevcon );

		if (FAILED(result))
		{
			return result;
		}

        // now we have a device, check it supports directx 11
		result = checkFeatureSupport();

		if (FAILED(result))
		{
			return result;
		}

        // get the address of the back buffer
        ID3D11Texture2D *pBackBuffer;

        // the first back buffer defined in the swap chain using ID3D11Texture2D COM object.
		result = pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);

		if (FAILED(result))
		{
			return result;
		}

        // use the back buffer address to create the render target
		result = pDev->CreateRenderTargetView( pBackBuffer, NULL, &pBackbuffer );

		if (FAILED(result))
		{
			return result;
		}

        // we're done with the texture COM object so we can release it (this doesn't release the actual back 
        // buffer - just the COM object accessing it
        SafeRelease( pBackBuffer );

        // prepare the depth stencil - create the depth texture
        D3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory( &descDepth, sizeof( descDepth ) );
        descDepth.Width = g_defaultWindowWidth;// SCREEN_WIDTH;
        descDepth.Height = g_defaultWindowHeight;// SCREEN_HEIGHT;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = gSampleDescCount;// D3D_SAMPLE_DESC_COUNT;
        descDepth.SampleDesc.Quality = gSampleDescQuality;// D3D_SAMPLE_DESC_QUALITY;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;

		result = pDev->CreateTexture2D( &descDepth, NULL, &pDepthStencil );

		if (FAILED(result))
		{
			return result;
		}

        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory( &descDSV, sizeof( descDSV ) );
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;

		result = pDev->CreateDepthStencilView( pDepthStencil, &descDSV, &pDepthStencilView );

		if (FAILED(result))
		{
			return result;
		}

        // set the render target as the back buffer, using the depth stencil view
        pDevcon->OMSetRenderTargets( 1, &pBackbuffer, pDepthStencilView );

        D3D11_BLEND_DESC desc;
        ZeroMemory( &desc, sizeof( desc ) );

        desc.AlphaToCoverageEnable = false;
        desc.IndependentBlendEnable = true;

        D3D11_RENDER_TARGET_BLEND_DESC targetDesc;
        ZeroMemory( &targetDesc, sizeof( targetDesc ) );

        desc.RenderTarget[ 0 ] = targetDesc;
        desc.RenderTarget[ 0 ].BlendEnable = true;
        desc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        desc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;

		result = pDev->CreateBlendState( &desc, &pBlend );

		if (FAILED(result))
		{
			return result;
		}

        // text rendering requires alpha blending
        float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        pDevcon->OMSetBlendState( pBlend, blendFactors, 0xffffffff );

        // set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

        viewport.TopLeftX = D3D_VIEWPORT_TOPLEFT_X;
        viewport.TopLeftY = D3D_VIEWPORT_TOPLEFT_Y;
        viewport.Width = D3D_VIEWPORT_WIDTH;
        viewport.Height = D3D_VIEWPORT_HEIGHT;
        viewport.MinDepth = D3D_VIEWPORT_MIN_DEPTH;
        viewport.MaxDepth = D3D_VIEWPORT_MAX_DEPTH;

        pDevcon->RSSetViewports( 1, &viewport );

		result = initPipeline();
		
		if (FAILED(result))
		{
			return result;
		}

		result = initGraphics();

		if (FAILED(result))
		{
			return result;
		}

        return S_OK;
    }

    /*
     Checks whether the hardware supports the compute shader
     */
    HRESULT D3DApplication::checkFeatureSupport()
    {
		HRESULT result = S_OK;

        D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS options;

		result = pDev->CheckFeatureSupport(
			D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS,
			&options,
			sizeof( D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS ) );

		if ( FAILED( result ) )
		{
			return result;
		}

        if ( !options.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x )
        {
#define COMPUTE_NOT_SUPPORTED			"DirectCompute is not supported on your hardware! Running the simulation without DirectCompute may be very slow! Would you like to run the simulation without DirectCompute?"
#define UNSUPPORTED_HARDWARE			"Unsupported HW"
            int returned = MessageBox( NULL, COMPUTE_NOT_SUPPORTED, UNSUPPORTED_HARDWARE, MB_YESNO | MB_ICONERROR );

            // if the user decides to continue with no compute acceleration, make sure we switch it off
            if ( returned == IDYES )
            {
                m_allowDirectCompute = false;
            }
            else
            {
                // something went wrong on the message box, or the user doesn't want to continue. 
                // In this instance we quite the application be returning E_FAIL
                return E_FAIL;
            }
        }

        return S_OK;
    }

    /*
     Initialises the rendering pipeline
     */
    HRESULT D3DApplication::initPipeline()
    {
        //// load and compile the two shaders
        //ID3DBlob *VS, *PS, *textVS, *textPS;
        //
        //HrCheck( CompileShaderFromFile( L".\\Resource Files\\Generic3DShader.hlsl", "VShader", "vs_4_0", &VS ) );
        //HrCheck( CompileShaderFromFile( L".\\Resource Files\\Generic3DShader.hlsl", "PShader", "ps_4_0", &PS ) );
        //
        //HrCheck( CompileShaderFromFile( L".\\Resource Files\\TextShaders.hlsl", "TextVertexShader", "vs_4_0", &textVS ) );
        //HrCheck( CompileShaderFromFile( L".\\Resource Files\\TextShaders.hlsl", "TextPixelShader", "ps_4_0", &textPS ) );
        //
        //// encapsulate both shaders into shader objects
        //HrCheck( pDev->CreateVertexShader( VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &p3DVS ) );
        //HrCheck( pDev->CreatePixelShader( PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &p3DPS ) );
        //
        //HrCheck( pDev->CreateVertexShader( textVS->GetBufferPointer(), textVS->GetBufferSize(), NULL, &pTextVS ) );
        //HrCheck( pDev->CreatePixelShader( textPS->GetBufferPointer(), textPS->GetBufferSize(), NULL, &pTextPS ) );
        //
        //// create the input layout object
        //D3D11_INPUT_ELEMENT_DESC ied[] =
        //{
        //    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //};
        //
        //HrCheck( pDev->CreateInputLayout( ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout ) );
        //pDevcon->IASetInputLayout( pLayout );
        //
        //SafeRelease( VS );
        //SafeRelease( PS );
        //SafeRelease( textVS );
        //SafeRelease( textPS );

        return S_OK;
    }

    /*
     Initialises the graphics
     */
    HRESULT D3DApplication::initGraphics()
    {
        //// initialise the sphere geometry
        //HrCheck( mSphere.init( pDev, pDevcon, 1.0f, 10, 10 ) );
        //
        //HrCheck( particleManager.init( pDev, pDevcon, &mToolManager, m_allowDirectCompute ) );
        //
        //// initialise the text layer
        //HrCheck( stringLayer.initFont11( pDev, SCREEN_WIDTH, SCREEN_HEIGHT ) );
        //
        //HrCheck( particleRenderer.init( &particleManager, pDev, pDevcon, &camera, m_allowDirectCompute ) );
        //
        //HrCheck( mToolManager.init( pDev, pDevcon, &camera, pInputHandler ) );
        //
        //// initiaise the camera position
        //camera.setRadius( 10.0f );
        //camera.setMatrices();
        //
        //D3D11_BUFFER_DESC bufferDesc;
        //
        ////----------------------------- create the constants buffer ------------------------
        //ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
        //
        //bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        //bufferDesc.ByteWidth = sizeof( VertexShaderConstantBuffer );
        //bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        //bufferDesc.CPUAccessFlags = 0;
        //bufferDesc.MiscFlags = 0;
        //
        //HrCheck( pDev->CreateBuffer( &bufferDesc, NULL, &pConstantBuffer ) );
        //
        //bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        //bufferDesc.ByteWidth = sizeof( LightConstantBuffer );
        //bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        //bufferDesc.CPUAccessFlags = 0;
        //bufferDesc.MiscFlags = 0;
        //
        //HrCheck( pDev->CreateBuffer( &bufferDesc, NULL, &pLightingConstantBuffer ) );

        return S_OK;
    }

    /*
     Cleans up Direct3D and COM

     Parameter list
        hWnd:		handle of window
     */
    HRESULT D3DApplication::cleanD3D()
    {
        // direct x can't close properly when in full screen mode - so ensure everything is windowed
		HRESULT result = pSwapchain->SetFullscreenState( FALSE, NULL );	// switch to windowed mode

		if ( FAILED( result ) )
		{
			return result;
		}

        // close and release all existing COM objects
        SafeRelease( pBlend );
        //SafeRelease( pConstantBuffer );
        //SafeRelease( pLightingConstantBuffer );
        SafeRelease( pDepthStencil );
        SafeRelease( pDepthStencilView );
        //SafeRelease( pLayout );
        //SafeRelease( p3DVS );
        //SafeRelease( pTextVS );
        //SafeRelease( p3DPS );
        //SafeRelease( pTextPS );
        SafeRelease( pSwapchain );
        SafeRelease( pBackbuffer );
        SafeRelease( pDev );
        SafeRelease( pDevcon );

        return S_OK;
    }

    /*----------------------------------------------------------------------------\
     *									Update                   				  |
     *----------------------------------------------------------------------------*/

    HRESULT D3DApplication::run()
    {
        if ( crashed )
        {
            return E_FAIL;
        }

        if ( FAILED( update() ) )
        {
            return E_FAIL;
        }

        if ( FAILED( renderFrame() ) )
        {
            return E_FAIL;
        }

        return S_OK;
    }

    /*
     Runs the update for the frame.
     */
    HRESULT D3DApplication::update()
    {
        handleCameraInput();
        handleInput();
        //particleInterface.updateInterface();
        //
        //mToolManager.updateManager();
        //
        //particleManager.updateParticles();
        //
        //// update the performance monitor
        //performance.update();

        return S_OK;
    }

    void D3DApplication::handleInput()
    {
//         if ( ( *pInputHandler->getKeyboardMap() )[ KEY_P ] == KEY_NEWLY_PRESSED )
//         {
//             performance.setShowPerformanceStats( !performance.getShowPerformanceStats() );
//         }
// 
//         if ( ( *pInputHandler->getKeyboardMap() )[ KEY_F ] == KEY_NEWLY_PRESSED )
//         {
//             pFrameController->setFastAsPossible( !pFrameController->getFastAsPossible() );
//         }
// 
//         if ( ( *pInputHandler->getKeyboardMap() )[ KEY_B ] == KEY_HELD )
//         {
//             performance.resetBestRate();
//         }
// 
//         if ( ( *pInputHandler->getKeyboardMap() )[ KEY_H ] == KEY_HELD )
//         {
//             if ( ( *pInputHandler->getKeyboardMap() )[ KEY_ADD ] == KEY_HELD )
//             {
//                 pFrameController->setFrameRate( pFrameController->getFrameRate() + 1 );
//             }
//             else if ( ( *pInputHandler->getKeyboardMap() )[ KEY_SUBTRACT ] == KEY_HELD )
//             {
//                 pFrameController->setFrameRate( pFrameController->getFrameRate() - 1 );
//             }
//         }
    }

    void D3DApplication::handleCameraInput()
    {
//         if ( ( *pInputHandler->getMouseButtonMap() )[ MOUSE_LBUTTON ] == KEY_HELD )
//         {
//             D3DXVECTOR3 *delta = pInputHandler->getMousePosDelta();
// 
//             if ( delta->x != 0 )
//             {
//                 camera.setHorizontalRotation( camera.getHorizontalRotation() + MOUSE_X_SCROLL_SPEED * delta->x );
//             }
// 
//             if ( delta->y != 0 )
//             {
//                 camera.setVerticalRotation( camera.getVerticalRotation() + MOUSE_Y_SCROLL_SPEED * delta->y );
//             }
//         }
// 
//         float mouseWheelDelta = pInputHandler->getMouseWheelDelta();
// 
//         if ( mouseWheelDelta != 0 )
//         {
//             camera.setRadius( camera.getRadius() + MOUSE_WHEEL_SCROLL_SPEED * mouseWheelDelta );
//         }
    }

    /*----------------------------------------------------------------------------\
     *								Rendering                   				  |
     *----------------------------------------------------------------------------*/

     /*
      Function used to render a single frame
      */
    HRESULT D3DApplication::renderFrame()
    {
        // clear the screen and depth buffer
        float clearColour[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        pDevcon->ClearRenderTargetView( pBackbuffer, clearColour );// D3DXCOLOR( D3D_CLEAR_COLOUR ) );
        pDevcon->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

        // render the 3D world first
        HRESULT result = render3DWorld();

		if ( FAILED( result ) )
		{
			return result;
		}

        // render the 2D text next - so that it is on top of the 3D objects
		result = render2DText();

		if ( FAILED( result ) )
		{
			return result;
		}

        // switch the back buffer and front buffer
		result = pSwapchain->Present( 0, 0 );

		if ( FAILED( result ) )
		{
			return result;
		}

        return S_OK;
    }

    HRESULT D3DApplication::render3DWorld()
    {
        //HrCheck( particleRenderer.render() );
        //
        //HrCheck( mToolManager.render() );

        return S_OK;
    }

    HRESULT	D3DApplication::render2DText()
    {
        // set the shader objects to be the text shaders.
        //pDevcon->VSSetShader( pTextVS, 0, 0 );
        //pDevcon->PSSetShader( pTextPS, 0, 0 );
        //
        //HrCheck( stringLayer.drawStrings( pDev, pDevcon ) );

        return S_OK;
    }
}