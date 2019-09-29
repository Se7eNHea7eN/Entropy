#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Graphic/SkyBox.hpp"

using namespace Entropy;

Scene::Scene(const char* scene_name) {
	m_RootNode = std::shared_ptr<SceneNode>(new SceneNode());
	m_RootNode->Initialize();
}

Scene::~Scene() {
}

void Scene::Tick(float deltaTime) {
	if(onTick != nullptr)
		onTick(deltaTime);
}
