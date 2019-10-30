#include "RayTracingRenderer.hpp"
#include "Ray.hpp"
#include "RTCamera.hpp"
#include "GL/glew.h"
#include "Eigen/Core"
#include <random>

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

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator =
		std::bind(distribution, generator);
	return rand_generator();
}

float hit_sphere(const Vector3f& center, float radius, const Ray& r) {

	Vector3f oc = r.origin() - center;
	float a = r.direction().squaredNorm();
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
Vector3f random_in_unit_sphere() {
	Vector3f p;
	do {
		p = 2.0 * Vector3f(random_double(), random_double(), random_double()) - Vector3f(1, 1, 1);
	} while (p.norm() >= 1.0);
	return p;
}
Vector3f color(const Ray& r, Hittable* world) {
	HitRecord rec;
	if (world->hit(r, 0.0001, FLT_MAX, rec)) {
		Vector3f target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * color(Ray(rec.p, target - rec.p), world);
	}
	else {
		Vector3f unit_direction = r.direction().normalized();
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * Vector3f(1.0, 1.0, 1.0) + t * Vector3f(0.5, 0.7, 1.0);
	}
}

void Entropy::RayTracingRenderer::Draw() {
	wglMakeCurrent(hDC, hRC);
	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	

	int nx = width;
	int ny = height;
	int ns = 1;



	Hittable* list[2];
	list[0] = new Sphere(Vector3f(0, 0, -1), 0.5);
	list[1] = new Sphere(Vector3f(0, -100.5, -1), 100);
	Hittable* world = new HittableList(list, 2);

	RTCamera camera;
	glBegin(GL_POINTS);
	glPointSize(1.0);
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			Vector3f col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				Ray r = camera.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(ns);
			
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			glColor3f(col.x(), col.y(), col.z());
			glVertex3f( 2*u-1, 2*v -1,0);
		}
	}
	glEnd();

	SwapBuffers(hDC);
}

void Entropy::RayTracingRenderer::AwaitRenderFrame() {
}
