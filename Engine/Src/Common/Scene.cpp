#include "Common/Scene.hpp"
using namespace Entropy;

Scene::Scene(const char* scene_name) {
	m_RootNode = std::shared_ptr<SceneEmptyNode>(new SceneEmptyNode());
	m_RootNode->Initialize();
}

Scene::~Scene() {
}

void Scene::Tick(float deltaTime) {
	if(onTick != nullptr)
		onTick(deltaTime);
}
