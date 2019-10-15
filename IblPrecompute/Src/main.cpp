#include <bx/bx.h>
#include "bx/file.h"
#include <bimg/bimg.h>
#include "GL/glew.h"
#include <windows.h>
#include "GLFW/glfw3.h"
#include "shader.h"
#include <bimg/decode.h>
#include "tinystl/buffer.h"
#pragma comment(lib,"opengl32.lib")


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return
		DefWindowProc(hWnd, message, wParam, lParam);
}

extern bx::AllocatorI* getDefaultAllocator();
bx::AllocatorI* g_allocator = getDefaultAllocator();


bx::AllocatorI* getDefaultAllocator()
{
	BX_PRAGMA_DIAGNOSTIC_PUSH();
	BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
	BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
	static bx::DefaultAllocator s_allocator;
	return &s_allocator;
	BX_PRAGMA_DIAGNOSTIC_POP();
}

static void* loadMem(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
{
	if (bx::open(_reader, _filePath))
	{
		uint32_t size = (uint32_t)bx::getSize(_reader);
		void* data = BX_ALLOC(_allocator, size);
		bx::read(_reader, data, size);
		bx::close(_reader);

		if (NULL != _size)
		{
			*_size = size;
		}
		return data;
	}

	// DBG("Failed to load %s.", _filePath);
	return NULL;
}

class FileReader : public bx::FileReader
{
	typedef bx::FileReader super;

public:
	virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
	{
		return super::open(_filePath, _err);
	}
};

class FileWriter : public bx::FileWriter
{
	typedef bx::FileWriter super;

public:
	virtual bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err) override
	{
		return super::open(_filePath, _append, _err);
	}
};
static bx::FileReaderI* s_fileReader = BX_NEW(g_allocator, FileReader);
static bx::FileWriterI* s_fileWriter = BX_NEW(g_allocator, FileWriter);
bx::FileReaderI* getFileReader()
{
	return s_fileReader;
}

bx::FileWriterI* getFileWriter()
{
	return s_fileWriter;
}

bx::AllocatorI* getAllocator()
{
	if (NULL == g_allocator)
	{
		g_allocator = getDefaultAllocator();
	}

	return g_allocator;
}

bimg::ImageContainer* imageLoad(const char* _filePath, bimg::TextureFormat::Enum _dstFormat)
{
	uint32_t size = 0;
	void* data = loadMem(getFileReader(), getAllocator(), _filePath, &size);

	return bimg::imageParse(getAllocator(), data, size, _dstFormat);
}


unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


int main(int argc, char* argv[]) {
	char* sourceImagePath = "..\\Textures\\newport_loft.hdr";
#ifdef RELEASE
	if(argc < 2) {
		return 0;
	}
	sourceImagePath = argc[1];
#endif

	const int environmentTextureSize = 1024;
	const int irraianceTextureSize = 512;

	bx::FileWriter writer;
	bx::Error err;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
   // --------------------
	GLFWwindow* window = glfwCreateWindow(1, 1, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwHideWindow(window);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glewInit();

	auto hdrImage = imageLoad(sourceImagePath, bimg::TextureFormat::RGB8);

	unsigned int hdrTexture;
	glGenTextures(1, &hdrTexture);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, hdrImage->m_width, hdrImage->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, hdrImage->m_data); // note how we specify the texture's data value to be float

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	float captureProjection[16];
	bx::mtxProj(captureProjection, 90.0f, 1.0f, 0.1f, 10.0f,true);
	float captureViews[6][16];
	//右
	bx::mtxLookAt(captureViews[0], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(1.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
	//左
	bx::mtxLookAt(captureViews[1], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(-1.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
	//下
	bx::mtxLookAt(captureViews[2], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, -1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f));
	//上
	bx::mtxLookAt(captureViews[3], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f));

	//后
	bx::mtxLookAt(captureViews[4], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
	//前
	bx::mtxLookAt(captureViews[5], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f), bx::Vec3(0.0f, 1.0f, 0.0f));

	//HDR生成cubemap
	Shader equirectangularToCubemapShader("cubemap.vs", "equirectangular_to_cubemap.fs");
	unsigned int captureFBO = 0;
	glGenFramebuffers(1, &captureFBO);

	//创建环境Cubemap
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, environmentTextureSize, environmentTextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Hdr渲染到Cubemap
	equirectangularToCubemapShader.use();
	equirectangularToCubemapShader.setInt("equirectangularMap", 0);
	equirectangularToCubemapShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, environmentTextureSize, environmentTextureSize); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned char* cubePixels = new unsigned char[environmentTextureSize * environmentTextureSize * 3 * 6];

	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();

		glReadPixels(0, 0, environmentTextureSize, environmentTextureSize, GL_BGR, GL_UNSIGNED_BYTE, cubePixels + environmentTextureSize * environmentTextureSize * 3 * i);

	}

	if (bx::open(&writer, "cubemap.dds", false, &err)) {
		// bimg::imageWritePng(&writer, textureSize, textureSize, textureSize * 4, pixels, bimg::TextureFormat::RGBA8, false, &err);
		auto imageContainer = bimg::imageAlloc(getAllocator(), bimg::TextureFormat::RGBA8, environmentTextureSize, environmentTextureSize, 1, 1, true, false, cubePixels);
		bimg::imageWriteDds(&writer, *imageContainer, imageContainer->m_data, environmentTextureSize* environmentTextureSize * 3 * 6, nullptr);
		bx::close(&writer);
	}
	delete cubePixels;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//环境光卷积
	Shader convolutionShader("cubemap.vs", "irradiance_convolution.fs");
	//Framebuffer纹理
	unsigned int captureFBOTexture = 0;
	glGenTextures(1, &captureFBOTexture);
	glBindTexture(GL_TEXTURE_2D, captureFBOTexture);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, irraianceTextureSize, irraianceTextureSize, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL
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

	//开始计算卷积
	convolutionShader.use();
	convolutionShader.setInt("environmentMap", 0);
	convolutionShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	unsigned char* pixels = (unsigned char*)malloc(irraianceTextureSize * irraianceTextureSize * 3 * 6);
	glViewport(0, 0, irraianceTextureSize, irraianceTextureSize); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	for (unsigned int i = 0; i < 6; ++i)
	{
		convolutionShader.setMat4("view", captureViews[i]);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
		glReadPixels(0, 0, irraianceTextureSize, irraianceTextureSize, GL_BGR, GL_UNSIGNED_BYTE, pixels + irraianceTextureSize * irraianceTextureSize * 3 * i);
	}

	if (bx::open(&writer, "irradiance.dds", false, &err)) {
		// bimg::imageWritePng(&writer, textureSize, textureSize, textureSize * 4, pixels, bimg::TextureFormat::RGBA8, false, &err);
		auto imageContainer = bimg::imageAlloc(getAllocator(), bimg::TextureFormat::RGB8, irraianceTextureSize, irraianceTextureSize, 1, 1, true, false, pixels);
		bimg::imageWriteDds(&writer, *imageContainer, imageContainer->m_data, irraianceTextureSize * irraianceTextureSize * 3 * 6, nullptr);
		bx::close(&writer);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glfwTerminate();
	return 0;
}
