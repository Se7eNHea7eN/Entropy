#include "Common/Scene.hpp"
using namespace Entropy;

Scene::Scene(const char* scene_name) {
	m_RootNode = std::make_unique<SceneEmptyNode>();
}

void Scene::Tick(float deltaTime) {
	if(onTick != nullptr)
		onTick(deltaTime);
}
