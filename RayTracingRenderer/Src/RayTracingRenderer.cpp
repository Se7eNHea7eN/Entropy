#include "RayTracingRenderer.hpp"
#include "Ray.hpp"
#include "GL/glew.h"
#include "Eigen/Core"
using namespace Eigen;
using namespace Entropy;
Entropy::RayTracingRenderer::RayTracingRenderer(HWND hwnd) : hWnd(hwnd) {
}

Entropy::RayTracingRenderer::~RayTracingRenderer() {
	wglDeleteContext(hRC);
	hRC = nullptr;
	ReleaseDC(hWnd, hDC);
	hDC = nullptr;
}

void Entropy::RayTracingRenderer::Initialize() {
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;								// Return FALSE
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;								// Return FALSE
	}
	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;								// Return FALSE
	}
	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;								// Return FALSE
	}
}

void Entropy::RayTracingRenderer::Resize(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, width, height);
}


float hit_sphere(const Vector3f& center, float radius, const Ray& r) {

	Vector3f oc = r.origin() - center;
	float a = r.direction().dot(r.direction());
	float b = 2.0 * oc.dot(r.direction());
	float c = oc.dot(oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

Vector3f color(const Ray& r) {
	float t = hit_sphere(Vector3f(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		Vector3f N = Vector3f(r.point_at_parameter(t) - Vector3f(0, 0, -1)).normalized();
		return 0.5 * Vector3f(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	Vector3f unit_direction = Vector3f(r.direction()).normalized();
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Vector3f(1.0, 1.0, 1.0) + t * Vector3f(0.5, 0.7, 1.0);
}

void Entropy::RayTracingRenderer::Draw() {
	wglMakeCurrent(hDC, hRC);
	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	

	int nx = width;
	int ny = height;
	Vector3f lower_left_corner(-1.0, -1.0, -1.0);
	Vector3f horizontal(2.0, 0.0, 0.0);
	Vector3f vertical(0.0, 2.0, 0.0);
	Vector3f origin(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glPointSize(1.0);
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			Vector3f col = color(r);
			glColor3f(col.x(), col.y(), col.z());
			glVertex3f( 2*u-1, 2*v -1,0);
		}
	}
	glEnd();

	SwapBuffers(hDC);
}

void Entropy::RayTracingRenderer::AwaitRenderFrame() {
}
