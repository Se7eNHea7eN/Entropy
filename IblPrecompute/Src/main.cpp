#include <bx/bx.h>
#include "bx/file.h"
#include <bimg/bimg.h>
#include "GL/glew.h"
#include <windows.h>
#include "GLFW/glfw3.h"
#include "shader.h"
#pragma comment(lib,"opengl32.lib")


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return
		DefWindowProc(hWnd, message, wParam, lParam);
}


int main(int _argc, const char* const* _argv) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
   // --------------------
	GLFWwindow* window = glfwCreateWindow(1, 1, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
	
	unsigned int captureFBO = 0;
	unsigned int captureFBOTexture = 0;

	glGenFramebuffers(1, &captureFBO);
	glGenTextures(1, &captureFBOTexture);
	const int textureSize = 2048;
	//
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


	// unsigned int envCubemap;
	// glGenTextures(1, &envCubemap);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	// for (unsigned int i = 0; i < 6; ++i) {
	// 	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 2048, 2048, 0, GL_RGB, GL_FLOAT, nullptr);
	// }
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glViewport(0, 0, textureSize, textureSize);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	unsigned char* pixels = (unsigned char*)malloc(textureSize * textureSize * 4);
	glReadPixels(0, 0, textureSize, textureSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels);


	bx::FileWriter writer;
	bx::Error err;
	if (bx::open(&writer, "generated.png", false, &err)) {
		bimg::imageWritePng(&writer, textureSize, textureSize, textureSize * 4, pixels, bimg::TextureFormat::RGBA8, false, &err);
		bx::close(&writer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//return 0;
}
