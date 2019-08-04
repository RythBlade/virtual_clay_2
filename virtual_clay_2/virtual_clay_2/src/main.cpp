/*----------------------------------------------------------------------------\
*                               Includes		                    		  |
*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <windowsx.h>
//#include "D3DApplication.h"
//#include "AppInfo.h"
//#include "FrameController.h"

#include <winerror.h>

#include "settings/window_settings.h"

#include "settings/directx_settings.h"

#include "d3d/d3d_application.h"

/*----------------------------------------------------------------------------\
*                               Globals		                    			  |
*----------------------------------------------------------------------------*/

//EventBroker broker;
//InputHandler inputHandler;
//FrameController frameController;
virtual_clay::D3DApplication d3dApp/*( &broker, &inputHandler, &frameController )*/;

/*----------------------------------------------------------------------------\
*                               Prototypes		                    		  |
*----------------------------------------------------------------------------*/

// the WindowProc function prototype
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );


/*----------------------------------------------------------------------------\
*                       Main Entry and Windows API Calls					  |
*----------------------------------------------------------------------------*/

/*
Main entry point for the application
*/
int WINAPI WinMain( HINSTANCE hInstance,
    HINSTANCE, // unused
    LPSTR, // unused
    int nCmdShow )
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = virtual_clay::g_className;//WIN_API_CLASS_NAME;

    RegisterClassEx( &wc );

    // set the required client area and adjust rectangle to account for the window border to give the window rectangle.
    RECT wr = { 0, 0, virtual_clay::g_defaultWindowWidth, virtual_clay::g_defaultWindowHeight };
    AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, FALSE );

    hWnd = CreateWindowEx(
        NULL,
        virtual_clay::g_className, //WIN_API_CLASS_NAME,
        virtual_clay::g_windowTitle, //WIN_API_WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        virtual_clay::g_defaultWindowPosX, //SCREEN_POS_X,
        virtual_clay::g_defaultWindowPosY, //SCREEN_POS_Y,
        wr.right - wr.left,		// calculate the window size from the adjusted value
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL );

    ShowWindow( hWnd, nCmdShow );

    // set up and initialise Direct3D
    if ( SUCCEEDED( d3dApp.initD3D( hWnd ) ) )
    {
        // enter the message loop
        MSG msg;
        
        while ( TRUE )
        {
            if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );

                // if the application should close - break the message loop
                if ( msg.message == WM_QUIT )
                {
                    break;
                }
            }

            // break the message loop if the d3d application fails
            if ( d3dApp.getCrashed() )
            {
                // we need top clean up if the application fails so only break the message loop, don't exit the function
                break;
            }

            // run a frame of the d3d application
            if ( FAILED( d3dApp.run() ) )
            {
                // break the message loop if something failed on the frame
                // we need top clean up if the application fails so only break the message loop, don't exit the function
                break;
            }
            
            //// update the input handler - this must be the last thing called for the frame as it resets the mouse deltas and
            //// rotates the button states to cater for non-consistent WinApi input events.
            //inputHandler.updateInputHandler();
            //
            //// run the frame controller to ensure the application is running at the desired frame rate settings
            //frameController.frameEnded();
        }
    }

    // clean up DirectX and COM
    d3dApp.cleanD3D();
    UnregisterClass( virtual_clay::g_className/*WIN_API_CLASS_NAME*/, wc.hInstance );

    return 0;
}

/*
Handles messages sent to this window from the WinAPI
*/
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message )
    {
    case WM_KEYDOWN:
        //inputHandler.sendKeyboardEvent( wParam, WM_KEYDOWN );
        break;

    case WM_KEYUP:
        //inputHandler.sendKeyboardEvent( wParam, WM_KEYUP );
        break;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_XBUTTONDOWN:
        SetCapture( hWnd );
        //inputHandler.sendMouseEvent( wParam, lParam, message );
        break;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_XBUTTONUP:
        //inputHandler.sendMouseEvent( wParam, lParam, message );
        ReleaseCapture();
        break;

    case WM_MOUSEMOVE:
        //inputHandler.sendMouseEvent( wParam, lParam, message );
        break;

    case WM_MOUSEWHEEL:
        //inputHandler.sendMouseWheelEvent( wParam );
        break;

    case WM_DESTROY:
    {
        PostQuitMessage( 0 );
        return 0;
    }
    }

    return DefWindowProc( hWnd, message, wParam, lParam );
}