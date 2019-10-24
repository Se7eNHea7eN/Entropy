#include "EntropyApp.hpp"
#include <exception>
// #include "Utils/Debug.hpp"
#include "GenerateScene.hpp"

using namespace Entropy;


int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	// app.entropyCore->debugMode = true;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	// GenerateScene01(scene);
	GenerateScene02(scene);
	try {
		return app.run(_argc, _argv);
	}catch(std::exception e) {
		// Log(e.what());
	}
}