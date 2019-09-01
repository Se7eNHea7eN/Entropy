// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "Renderer.hpp"
#include "EntropyCore.hpp"
#include <thread>
using namespace Entropy;
EntropyCore entropy;
Renderer* renderer;
std::thread* renderThread;
bool isExit = false;
void appThreadEntrance() {
	while (!isExit) {
		if (renderer != nullptr)
			renderer->draw();
	}
	delete renderer;
	renderer = nullptr;
}
// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine,
                   int nCmdShow) {
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("Entropy");

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(WS_EX_ACCEPTFILES,
	                      _T("Entropy"), // name of the window class
	                      _T("Entropy"), // title of the window
	                      WS_OVERLAPPEDWINDOW | WS_VISIBLE, // window style
	                      300, // x-position of the window
	                      300, // y-position of the window
	                      500, // width of the window
	                      400, // height of the window
	                      nullptr, // we have no parent window, NULL
	                      nullptr, // we aren't using menus, NULL
	                      hInstance, // application handle
	                      nullptr); // used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);
	// enter the main loop:

	// this struct holds Windows event messages
	// renderThread = new std::thread(appThreadEntrance);

	MSG msg;
	while(!isExit) {
		if (renderer != nullptr)
			renderer->draw();
		WaitForInputIdle(GetCurrentProcess(), 16);
		while (0 != PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	// wait for the next message in the queue, store the result in 'msg'
	// while (GetMessage(&msg, nullptr, 0, 0)) {
	// 	// translate keystroke messages into the right format
	// 	TranslateMessage(&msg);
	//
	// 	// send the message to the WindowProc function
	// 	DispatchMessage(&msg);
	// 	if (renderer != nullptr)
	// 		renderer->draw();
	// }
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// sort through and find what code to run for the message given
	switch (message) {
	case WM_CREATE: {
		renderer = entropy.CreateRenderer(hWnd);
		renderer->initialize();
	}
	break;
	case WM_SIZE: {
		RECT rc;
		GetClientRect(hWnd, &rc);
		renderer->resize(rc.right - rc.left, rc.bottom - rc.top);
	}
	break;
	case WM_PAINT: {
		
	}
	break;
		// this message is read when the window is closed
	case WM_DESTROY: {
		// close the application entirely
		PostQuitMessage(0);
		isExit = true;
		if(renderThread != nullptr) {
			renderThread->join();
			renderThread = nullptr;
		}
		return 0;
	}
	break;
	case WM_DISPLAYCHANGE:
		// InvalidateRect(hWnd, nullptr, false);
		break;
	}

	// Handle any messages the switch statement didn't
	return
		DefWindowProc(hWnd, message, wParam, lParam);
}

