#include "Common/Scene.hpp"
using namespace Entropy;

void Scene::Tick(float deltaTime) {
	if(onTick != nullptr)
		onTick(deltaTime);
}
