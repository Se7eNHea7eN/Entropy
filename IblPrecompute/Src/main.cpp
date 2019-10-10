#include <bx/bx.h>
#include "bx/file.h"
#include <bimg/bimg.h>
#include "GL/glew.h"
#include <windows.h>
#pragma comment(lib,"opengl32.lib")


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return
		DefWindowProc(hWnd, message, wParam, lParam);
}


int main(int _argc, const char* const* _argv) {
	int width = 1024;
	int height = 1024;
	HWND hWnd = NULL;
	GLuint PixelFormat; // Holds The Results After Searching For A Match
	WNDCLASS wc; // Windows Class Structure
	DWORD dwExStyle; // Window Extended Style
	DWORD dwStyle; // Window Style
	RECT WindowRect; // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0; // Set Left Value To 0
	WindowRect.right = (long)width; // Set Right Value To Requested Width
	WindowRect.top = (long)0; // Set Top Value To 0
	WindowRect.bottom = (long)height; // Set Bottom Value To Requested Height


	HINSTANCE hInstance = GetModuleHandle(NULL); // Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WindowProc; // WndProc Handles Messages
	wc.cbClsExtra = 0; // No Extra Window Data
	wc.cbWndExtra = 0; // No Extra Window Data
	wc.hInstance = hInstance; // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Load The Arrow Pointer
	wc.hbrBackground = NULL; // No Background Required For GL
	wc.lpszMenuName = NULL; // We Don't Want A Menu
	wc.lpszClassName = "OpenGL"; // Set The Class Name

	if (!RegisterClass(&wc)) // Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}


	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW; // Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle, // Extended Style For The Window
	                            "OpenGL", // Class Name
	                            "OpenGL", // Window Title
	                            dwStyle | // Defined Window Style
	                            WS_CLIPSIBLINGS | // Required Window Style
	                            WS_CLIPCHILDREN, // Required Window Style
	                            0, 0, // Window Position
	                            WindowRect.right - WindowRect.left, // Calculate Window Width
	                            WindowRect.bottom - WindowRect.top, // Calculate Window Height
	                            NULL, // No Parent Window
	                            NULL, // No Menu
	                            hInstance, // Instance
	                            NULL))) // Dont Pass Anything To WM_CREATE
	{
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	PIXELFORMATDESCRIPTOR pfd = // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
		1, // Version Number
		PFD_DRAW_TO_WINDOW | // Format Must Support Window
		PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
		PFD_DOUBLEBUFFER, // Must Support Double Buffering
		PFD_TYPE_RGBA, // Request An RGBA Format
		32, // Select Our Color Depth
		0, 0, 0, 0, 0, 0, // Color Bits Ignored
		0, // No Alpha Buffer
		0, // Shift Bit Ignored
		0, // No Accumulation Buffer
		0, 0, 0, 0, // Accumulation Bits Ignored
		16, // 16Bit Z-Buffer (Depth Buffer)  
		0, // No Stencil Buffer
		0, // No Auxiliary Buffer
		PFD_MAIN_PLANE, // Main Drawing Layer
		0, // Reserved
		0, 0, 0 // Layer Masks Ignored
	};


	HDC hDC; // Private GDI Device Context
	HGLRC hRC; // Permanent Rendering Context
	if (!(hDC = GetDC(hWnd))) // Did We Get A Device Context?
	{
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1; // Return FALSE
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) // Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1; // Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) // Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1; // Return FALSE
	}
	if (!(hRC = wglCreateContext(hDC))) // Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1; // Return FALSE
	}
	if (!wglMakeCurrent(hDC, hRC)) // Try To Activate The Rendering Context
	{
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1; // Return FALSE
	}

	wglMakeCurrent(hDC, hRC);


	unsigned int captureFBO;
	unsigned int captureFBOTexture;

	glGenFramebuffers(1, &captureFBO);
	glGenTextures(1, &captureFBOTexture);
	const int textureSize = 2048;

	glBindTexture(GL_TEXTURE_2D, captureFBOTexture);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL
	);
	glTexParameterf(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR
	);
	glTexParameterf(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR
	);
	glTexParameterf(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
	);
	glTexParameterf(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
	);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, captureFBOTexture, 0
	);

	glBindTexture(GL_TEXTURE_2D, 0);


	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 2048, 2048, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glViewport(0, 0, textureSize, textureSize);

	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	unsigned char* pixels = (unsigned char*)malloc(textureSize * textureSize * 4);
	glReadPixels(0, 0, textureSize, textureSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels);


	bx::FileWriter writer;
	bx::Error err;
	if (bx::open(&writer, "test.png", false, &err)) {
		bimg::imageWritePng(&writer, textureSize, textureSize, textureSize * 4, pixels, bimg::TextureFormat::RGBA8, false, &err);
		bx::close(&writer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}
