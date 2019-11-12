#include "RayTracingRenderer.hpp"
#include "Ray.hpp"
#include "RTCamera.hpp"
#include "RTMaterial.hpp"
#include "GL/glew.h"
#include "Eigen/Core"
#include "Utils.hpp"
#include <thread>
#include <mutex>
#include "Utils/Debug.hpp"
#include <Utils\ThreadPool.hpp>

using namespace Eigen;
using namespace Entropy;
void Save_bmp(unsigned char* data, int width, int height);

RayTracingRenderer::RayTracingRenderer(HWND hwnd) : hWnd(hwnd) {
}

RayTracingRenderer::~RayTracingRenderer() {
	wglDeleteContext(hRC);
	hRC = nullptr;
	ReleaseDC(hWnd, hDC);
	hDC = nullptr;
}

void RayTracingRenderer::Initialize() {
	GLuint PixelFormat; // Holds The Results After Searching For A Match
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
	if (!(hDC = GetDC(hWnd))) // Did We Get A Device Context?
	{
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return; // Return FALSE
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) // Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return; // Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) // Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return; // Return FALSE
	}
	if (!(hRC = wglCreateContext(hDC))) // Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return; // Return FALSE
	}
	if (!wglMakeCurrent(hDC, hRC)) // Try To Activate The Rendering Context
	{
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return; // Return FALSE
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	Vector3f lookfrom(13, 2, 3);
	Vector3f lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	
	camera = new RTCamera(lookfrom, lookat, Vector3f(0, 1, 0), 20,
		float(renderWidth) / float(renderHeight), aperture, dist_to_focus, 0.0, 1.0);

	memset(renderBuffer, 0, renderWidth * renderHeight * 3);

	// Hittable** list = new Hittable *[];
	//
	// list[0] = new Sphere(Vector3f(0, 0, -1), 0.5, new Lambertian(Vector3f(0.1, 0.2, 0.5)));
	// list[1] = new Sphere(Vector3f(0, -100.5, -1), 100, new Lambertian(Vector3f(0.8, 0.8, 0.0)));
	// list[2] = new Sphere(Vector3f(1, 0, -1), 0.5, new Metal(Vector3f(0.8, 0.6, 0.2), 0.3));
	// list[3] = new Sphere(Vector3f(-1, 0, -1), 0.5, new Dielectric(1.5));
	// list[4] = new Sphere(Vector3f(-1, 0, -1), -0.45, new Dielectric(1.5));
	// world = new HittableList(list, 5);
	int n = 50000;
	Hittable** list = new Hittable * [n + 1];
	list[0] = new Sphere(Vector3f(0, -1000, 0), 1000, new Lambertian(Vector3f(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float choose_mat = random_double();
			Vector3f center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - Vector3f(4, 0.2, 0)).norm() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new MovingSphere(
						center,
						center+ Vector3f(0, 0.5 * random_double(), 0),
						0.0,1.0,0.2,
						new Lambertian(Vector3f(random_double() * random_double(),
							random_double() * random_double(),
							random_double() * random_double())
						)
					);
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new Sphere(center, 0.2,
						new Metal(Vector3f(0.5 * (1 + random_double()),
							0.5 * (1 + random_double()),
							0.5 * (1 + random_double())),
							0.5 * random_double()));
				}
				else {  // glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vector3f(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vector3f(-4, 1, 0), 1.0, new Lambertian(Vector3f(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vector3f(4, 1, 0), 1.0, new Metal(Vector3f(0.7, 0.6, 0.5), 0.0));

	//world = new HittableList(list, i);
	world = new bvh_node(list, i,0,1);
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


void RayTracingRenderer::CheckTiles() {
	for (int j = 0; j < 16; j++) {
		for (int i = 0; i < 16; i++) {
			auto t = tiles[j][i];
			if (!t.isdone) return;
		}
	}
	Save_bmp(renderBuffer, renderWidth ,renderHeight);
}


void RayTracingRenderer::Draw() {
	wglMakeCurrent(hDC, hRC);
	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderWidth, renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, renderBuffer);

	glLoadIdentity();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	SwapBuffers(hDC);
	
}

void RayTracingRenderer::AwaitRenderFrame() {
}
void RayTracingRenderer::Render() {
	memset(renderBuffer, 0, renderWidth * renderHeight * 3);
	int tileWidth = renderWidth / 16;
	int tileHeight = renderHeight / 16;

	for (int j = 0; j < 16; j++) {
		for (int i = 0; i < 16; i++) {
			Tile& t = tiles[j][i];
			t.left = i * tileWidth;
			t.right = i * tileWidth + tileWidth;
			t.top = j * tileHeight;
			t.bottom = j * tileHeight + tileHeight;
			t.isdone = false;
		}
	}

	new std::thread([&]
		{
			ThreadPool pool(15);

			for (int i = 0; i < 16; ++i) {
				for (int j = 0; j < 16; ++j) {
					pool.enqueue([=] {
						auto& t = tiles[i][j];
						Log("start render tile %d %d", t.left / 16, t.top / 16);
						for (int j = t.top; j < t.bottom; j++) {
							for (int i = t.left; i < t.right; i++) {
								Vector3f col(0, 0, 0);
								for (int s = 0; s < sampleCount; s++) {
									float u = float(i + random_double()) / float(renderWidth);
									float v = float(j + random_double()) / float(renderHeight);
									Ray r = camera->get_ray(u, v);
									col += color(r, world, 0);
								}
								col /= float(sampleCount);
								col = Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

								int position = (j * renderWidth + i) * 3;
								renderBuffer[position] = 255.99 * col.x();
								renderBuffer[position + 1] = 255.99 * col.y();
								renderBuffer[position + 2] = 255.99 * col.z();
							}
						}
						Log("end render tile %d %d", t.left / 16, t.top / 16);
						t.isdone = true;
						CheckTiles();
						if (onUpdate != nullptr)
							onUpdate();
						});
				}
			}
		});

}

void Save_bmp(unsigned char* data,int width,int height)
{
	int i;
	unsigned char* copy = new unsigned char[width * height * 3];
	for (i = 0; i < width*height*3; i += 3)
	{
		//swap R and B; raw_image[i + 1] is G, so it stays where it is.
		copy[i + 0] = data[i + 2];
		copy[i + 1] = data[i + 1];
		copy[i + 2] = data[i + 0];
	}
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	const int count = 3;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = count * 8;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = width * height * 3;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	auto biBitCount = count * 8;
	auto dwBmBitsSize = ((width * biBitCount + biBitCount - 1) / biBitCount) * count * height;
	auto dwPaletteSize = 0;

	bmfHdr.bfType = 0x4D42;
	auto dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	//创建位图文件                  
	auto fh = CreateFile("RT.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return;

	DWORD dwWritten = 0;

	//     写入位图文件头              
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//     写入位图文件其余内容              
	WriteFile(fh, (LPSTR)&bi, sizeof(bi), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)copy, width * height * 3, &dwWritten, NULL);
	CloseHandle(fh);
	delete copy;
}
