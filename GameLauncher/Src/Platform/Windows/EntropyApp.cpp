#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "Common/Renderer.hpp"
#include "EntropyApp.hpp"

using namespace Entropy;
Renderer* renderer;
bool isExit = false;
EntropyApp* instance;
// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);


EntropyApp::EntropyApp() {
	instance = this;
	entropyCore = new EntropyCore();
}

EntropyApp::~EntropyApp() {
}

int EntropyApp::run(int _argc, const char* const* _argv) {
	HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
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
	                      1024, // width of the window
	                      768, // height of the window
	                      nullptr, // we have no parent window, NULL
	                      nullptr, // we aren't using menus, NULL
	                      instance, // application handle
	                      nullptr); // used with multiple windows, NULL

	ShowWindow(hWnd, SW_SHOWNORMAL);
	MSG msg;
	while (!isExit) {
		if (renderer != nullptr)
			renderer->Draw();
		WaitForInputIdle(GetCurrentProcess(), 16);
		while (0 != PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	return 0;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// sort through and find what code to run for the message given
	switch (message) {
	case WM_CREATE: {
		renderer = instance->entropyCore->CreateRenderer(hWnd);
		renderer->Initialize();
	}
	break;
	case WM_SIZE: {
		RECT rc;
		GetClientRect(hWnd, &rc);
		renderer->Resize(rc.right - rc.left, rc.bottom - rc.top);
	}
	break;
		// this message is read when the window is closed
	case WM_DESTROY: {
		// close the application entirely
		PostQuitMessage(0);
		isExit = true;
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
