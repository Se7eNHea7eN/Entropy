#include "Common/Scene.hpp"
using namespace Entropy;

void Scene::Tick() {
	if(onTick != nullptr)
		onTick();
}
