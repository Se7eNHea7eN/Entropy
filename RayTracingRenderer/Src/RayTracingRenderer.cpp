#include "RayTracingRenderer.hpp"
#include "Ray.hpp"
#include "Bvh.hpp"
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

Hittable* createScene() {
	int n = 50000;
	Hittable** list = new Hittable * [n + 1];
	RTTexture* checker = new noise_texture(4);
	list[0] = new Sphere(Vector3f(0, -1000, 0), 1000, new Lambertian(checker));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float choose_mat = random_double();
			Vector3f center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - Vector3f(4, 0.2, 0)).norm() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new MovingSphere(
						center,
						center + Vector3f(0, 0.5 * random_double(), 0),
						0.0, 1.0, 0.2,
						new Lambertian(new ConstantTexture(Vector3f(random_double() * random_double(),
							random_double() * random_double(),
							random_double() * random_double()))
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
	list[i++] = new Sphere(Vector3f(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(Vector3f(0.4, 0.2, 0.1))));
	list[i++] = new Sphere(Vector3f(4, 1, 0), 1.0, new Metal(Vector3f(0.7, 0.6, 0.5), 0.0));

	return new BvhNode(list, i, 0, 1);
}

Hittable* createLightScene() {
	RTTexture* pertext = new noise_texture(4);
	Hittable** list = new Hittable * [4];
	list[0] = new Sphere(Vector3f(0, -1000, 0), 1000, new Lambertian(pertext));
	list[1] = new Sphere(Vector3f(0, 2, 0), 2, new Lambertian(pertext));
	list[2] = new Sphere(Vector3f(0, 7, 0), 2,
		new DiffuseLight(new ConstantTexture(Vector3f(4, 4, 4))));
	list[3] = new XYRect(3, 5, 1, 3, -2,
		new DiffuseLight(new ConstantTexture(Vector3f(4, 4, 4))));
	return new HittableList(list, 4);
}

void createCornellBox(Hittable** world, Hittable** lightShapes, RTCamera** camera,float aspect) {
	Hittable** list = new Hittable * [100];
	int i = 0;
	RTMaterial* red = new Lambertian(new ConstantTexture(Vector3f(0.65, 0.05, 0.05)));
	RTMaterial* white = new Lambertian(new ConstantTexture(Vector3f(0.73, 0.73, 0.73)));
	RTMaterial* green = new Lambertian(new ConstantTexture(Vector3f(0.12, 0.45, 0.15)));
	RTMaterial* light = new DiffuseLight(new ConstantTexture(Vector3f(15, 15, 15)));
	RTMaterial* blue = new Lambertian(new ConstantTexture(Vector3f(0.07, 0.09, 0.96)));

	list[i++] = new flip_normals(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new flip_normals(new XZRect(213, 343, 227, 332, 554, light));
	list[i++] = new flip_normals(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new XYRect(0, 555, 0, 555, 555, blue));
	auto glass = new Dielectric(1.5);
	list[i++] = new Sphere(Vector3f(190, 90, 190), 90, glass);
	//list[i++] = new translate(new rotate_y(new Box(Vector3f(0, 0, 0), Vector3f(165, 165, 165), white),-18), Vector3f(130, 0, 65));

	auto aluminum = new Metal(Vector3f(0.8, 0.85, 0.88), 0.0);
	list[i++] = new translate(new rotate_y(new Box(Vector3f(0, 0, 0), Vector3f(165, 330, 165), aluminum),15), Vector3f(265, 0, 295));
	*world = new HittableList(list, i);

	Hittable **a = new Hittable*[2];

	auto light_shape = new XZRect(213, 343, 227, 332, 554, 0);
	auto glass_sphere = new Sphere(Vector3f(190, 90, 190), 90, 0);
	a[0] = light_shape;
	a[1] = glass_sphere;

	*lightShapes = new HittableList(a, 2);

	Vector3f lookfrom(278, 278, -800);
	Vector3f lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;

	*camera = new RTCamera(lookfrom, lookat, Vector3f(0, 1, 0), vfov,
		aspect, aperture, dist_to_focus, 0.0, 1.0);
}

void createFinalScene(Hittable** world, Hittable** lightShapes, RTCamera** camera, float aspect) {
	int nb = 20;
	Hittable** list = new Hittable * [30];
	Hittable** Boxlist = new Hittable * [10000];
	Hittable** Boxlist2 = new Hittable * [10000];
	RTMaterial* white = new Lambertian(new ConstantTexture(Vector3f(0.73, 0.73, 0.73)));
	RTMaterial* ground = new Lambertian(new ConstantTexture(Vector3f(0.48, 0.83, 0.53)));
	int b = 0;
	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < nb; j++) {
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (random_double() + 0.01);
			float z1 = z0 + w;
			Boxlist[b++] = new Box(Vector3f(x0, y0, z0), Vector3f(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new BvhNode(Boxlist, b, 0, 1);
	RTMaterial* light = new DiffuseLight(new ConstantTexture(Vector3f(15, 15, 15)));
	list[l++] = new flip_normals(new XZRect(123, 423, 147, 412, 554, light));
	Vector3f center(400, 400, 200);
	list[l++] = new MovingSphere(center, center + Vector3f(30, 0, 0),
		0, 1, 50, new Lambertian(new ConstantTexture(Vector3f(0.7, 0.3, 0.1))));
	list[l++] = new Sphere(Vector3f(260, 150, 45), 50, new Dielectric(1.5));
	list[l++] = new Sphere(Vector3f(0, 150, 145), 50,
		new Metal(Vector3f(0.8, 0.8, 0.9), 10.0));
	Hittable* boundary = new Sphere(Vector3f(360, 150, 145), 70, new Dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2,
		new ConstantTexture(Vector3f(0.2, 0.4, 0.9)));
	boundary = new Sphere(Vector3f(0, 0, 0), 5000, new Dielectric(1.5));
	list[l++] = new constant_medium(boundary, 0.0001,
		new ConstantTexture(Vector3f(1.0, 1.0, 1.0)));
	int nx, ny, nn;
	//unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	//RTMaterial* emat = new Lambertian(new image_texture(tex_data, nx, ny));
	//list[l++] = new Sphere(Vector3f(400, 200, 400), 100, emat);
	RTTexture* pertext = new noise_texture(0.1);
	list[l++] = new Sphere(Vector3f(220, 280, 300), 80, new Lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		Boxlist2[j] = new Sphere(
			Vector3f(165 * random_double(), 165 * random_double(), 165 * random_double()),
			10, white);
	}
	list[l++] = new translate(new rotate_y(
		new BvhNode(Boxlist2, ns, 0.0, 1.0), 15), Vector3f(-100, 270, 395));
	*world = new HittableList(list, l);

	Vector3f lookfrom(400, 400, -800);
	Vector3f lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;

	*camera = new RTCamera(lookfrom, lookat, Vector3f(0, 1, 0), vfov,
		aspect, aperture, dist_to_focus, 0.0, 1.0);
	*lightShapes = new XZRect(123, 423, 147, 412, 554, nullptr);
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

	memset(renderBuffer, 0, renderWidth * renderHeight * 3);

	createCornellBox(&world,&lightShapes, &camera, float(renderWidth) / float(renderHeight));
	//createFinalScene(&world, &lightShapes, &camera, float(renderWidth) / float(renderHeight));
}

void Entropy::RayTracingRenderer::Resize(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, width, height);
}

Vector3f color(const Ray& r, Hittable* world, Hittable* light_shape, int depth) {
	HitRecord hrec;
	if (world->hit(r, 0.001, FLT_MAX, hrec)) {
		scatter_record srec;
		Vector3f emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);

		if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
			if (srec.is_specular) {
				return srec.attenuation.cwiseProduct(color(srec.specular_ray, world, light_shape, depth + 1));
			}
			else {
				hittable_pdf plight(light_shape, hrec.p);
				mixture_pdf p(&plight, srec.pdf_ptr.get());
				Ray scattered = Ray(hrec.p, p.generate(), r.time());
				float pdf_val = p.value(scattered.direction());
				return emitted + srec.attenuation.cwiseProduct(hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * color(scattered, world, light_shape, depth + 1) / pdf_val);
			}
		}
		else {
			return emitted;
		}
	}
	return Vector3f(0, 0, 0);
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
			ThreadPool pool(16);

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
									col += de_nan(color(r, world, lightShapes,0));
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
