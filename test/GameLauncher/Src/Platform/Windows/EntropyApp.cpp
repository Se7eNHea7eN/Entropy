 #include "bx/Thread.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "Renderer/Renderer.hpp"
#include "EntropyApp.hpp"
#include <ctime>
#include <cstdint>
#include "Core/EntropyCore.hpp"
using namespace Entropy;
bool isExit = false;

struct MainThreadEntry
{
	int m_argc;
	const char* const* m_argv;

	static int32_t threadFunc(bx::Thread* _thread, void* _userData);
};
int32_t MainThreadEntry::threadFunc(bx::Thread* /*_thread*/, void* _userData)
{
	Renderer* renderer = EntropyCore::GetInstance()->GetRenderer();
	renderer->Initialize();
	
	auto lastTime = GetTickCount64();
	while (!isExit) {
		auto thisTime = GetTickCount64();
		EntropyCore::GetInstance()->Tick((thisTime - lastTime) / 1000.0);
		lastTime = thisTime;
		if (renderer != nullptr)
			renderer->Draw();
	}
	return 0;
}

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);


EntropyApp::EntropyApp() {

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
	                      0, // x-position of the window
	                      0, // y-position of the window
	                      1920, // width of the window
	                      1080, // height of the window
	                      nullptr, // we have no parent window, NULL
	                      nullptr, // we aren't using menus, NULL
	                      instance, // application handle
	                      nullptr); // used with multiple windows, NULL

	ShowWindow(hWnd, SW_SHOWNORMAL);

	EntropyInitConfig config;
	config.hwnd = hWnd;
	EntropyCore::Init(config);

	MainThreadEntry mte;
	mte.m_argc = _argc;
	mte.m_argv = _argv;
	//
	// bx::Thread thread;
	// thread.init(mte.threadFunc, &mte);
	
	MSG msg;
	auto lastTime = GetTickCount64();

	while (!isExit) {
		// renderer->AwaitRenderFrame();
		// WaitForInputIdle(GetCurrentProcess(), 16);
		auto thisTime = GetTickCount64();
		entropyCore->Tick((thisTime - lastTime) / 1000.0);
		lastTime = thisTime;
		EntropyCore::GetInstance()->GetRenderer()->Draw();
		while (0 != PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	EntropyCore::Release();
	return 0;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// sort through and find what code to run for the message given
	switch (message) {
	case WM_CREATE: {
	
	}
	break;
	case WM_SIZE: {
		RECT rc;
		GetClientRect(hWnd, &rc);
		EntropyCore::GetInstance()->GetRenderer()->Resize(rc.right - rc.left, rc.bottom - rc.top);
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
