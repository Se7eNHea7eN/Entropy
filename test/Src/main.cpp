#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
using namespace Entropy;
int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	auto cubeNode = std::unique_ptr<BaseSceneNode>(std::make_unique<SceneGeometryNode>("Cube"));
	scene->SceneGraph->AppendChild(std::move(cubeNode));
	auto _object = std::make_shared<SceneObjectGeometry>();
	return app.run(_argc, _argv);
}