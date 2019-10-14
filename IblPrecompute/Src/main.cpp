#include <bx/bx.h>
#include "bx/file.h"
#include <bimg/bimg.h>
#include "GL/glew.h"
#include <windows.h>
#include "GLFW/glfw3.h"
#include "shader.h"
#include <bimg/decode.h>
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


int main(int _argc, const char* const* _argv) {

	bx::FileWriter writer;
	bx::Error err;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
   // --------------------
	GLFWwindow* window = glfwCreateWindow(512, 512, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glewInit();

	auto hdrImage = imageLoad("Textures/newport_loft.hdr", bimg::TextureFormat::RGBA8);

	unsigned int hdrTexture;
	glGenTextures(1, &hdrTexture);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, hdrImage->m_width, hdrImage->m_height, 0, GL_RGBA, GL_BYTE, hdrImage->m_data); // note how we specify the texture's data value to be float

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const int textureSize = 2048;
	
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

	Shader shader("Shaders\glsl\src\cubemap.vs", "Shaders\glsl\src\equirectangular_to_cubemap.fs");
	auto error = glGetError();

	unsigned int captureFBO = 0;
	unsigned int captureFBOTexture = 0;

	glGenFramebuffers(1, &captureFBO);
	glGenTextures(1, &captureFBOTexture);
	glGenFramebuffers(1, &captureFBO);
	glGenTextures(1, &captureFBOTexture);
	glBindTexture(GL_TEXTURE_2D, captureFBOTexture);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, textureSize, textureSize, 0,
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


	shader.use();
	shader.setInt("equirectangularMap", 0);
	shader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	unsigned char* pixels = (unsigned char*)malloc(textureSize * textureSize * 4);
	glViewport(0, 0, textureSize, textureSize); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.setMat4("view", captureViews[i]);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
		glReadPixels(0, 0, textureSize, textureSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		char buffer[128]{};
		sprintf_s(buffer,128,"cube_%d.png", i);

		if (bx::open(&writer, buffer, false, &err)) {
			bimg::imageWritePng(&writer, textureSize, textureSize, textureSize * 4, pixels, bimg::TextureFormat::RGBA8, false, &err);
			bx::close(&writer);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glfwTerminate();
	return 0;
}
