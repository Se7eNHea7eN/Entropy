#include "RayTracingRenderer.hpp"
#include "Ray.hpp"
#include "RTCamera.hpp"
#include "RTMaterial.hpp"
#include "GL/glew.h"
#include "Eigen/Core"
#include "Utils.hpp"

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

Vector3f color(const Ray& r, Hittable* world, int depth) {
	HitRecord rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vector3f attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation.cwiseProduct(color(scattered, world, depth + 1));
		}
		else {
			return Vector3f(0, 0, 0);
		}
	}
	else {
		Vector3f unit_direction = r.direction().normalized();
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * Vector3f(1.0, 1.0, 1.0) + t * Vector3f(0.5, 0.7, 1.0);
	}
}

void RayTracingRenderer::Draw() {
	wglMakeCurrent(hDC, hRC);
	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	int nx = width;
	int ny = height ;
	int ns = 4;

	Hittable* list[4];

	list[0] = new Sphere(Vector3f(0, 0, -1), 0.5, new Lambertian(Vector3f(0.8, 0.3, 0.3)));
	list[1] = new Sphere(Vector3f(0, -100.5, -1), 100, new Lambertian(Vector3f(0.8, 0.8, 0.0)));
	list[2] = new Sphere(Vector3f(1, 0, -1), 0.5, new Metal(Vector3f(0.8, 0.6, 0.2),0.3));
	list[3] = new Sphere(Vector3f(-1, 0, -1), 0.5, new Metal(Vector3f(0.8, 0.8, 0.8),1.0));
	Hittable* world = new HittableList(list, 4);

	RTCamera camera;
	camera.setRatio(1.0 * width / height);
	glBegin(GL_POINTS);
	glPointSize(1.0);
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			Vector3f col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				Ray r = camera.get_ray(u, v);
				col += color(r, world,0);
			}
			col /= float(ns);
			col = Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			glColor3f(col.x(), col.y(), col.z());
			glVertex3f( 2*u-1, 2*v -1,0);
		}
	}
	glEnd();

	SwapBuffers(hDC);
}

void RayTracingRenderer::AwaitRenderFrame() {
}
