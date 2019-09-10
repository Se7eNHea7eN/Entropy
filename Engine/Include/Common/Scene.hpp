#pragma once
#include <unordered_map>
#include "SceneNode.hpp"
#include <functional>
#include "Camera.hpp"

namespace Entropy {

	class Scene {
	public:
	public:
		Scene(const char* scene_name) :
			SceneGraph(new BaseSceneNode(scene_name))
		{
		}
		~Scene() = default;

		std::unique_ptr<BaseSceneNode> SceneGraph;
		std::list<std::shared_ptr<SceneGeometryNode>> Geometries;
		std::list<std::shared_ptr<SceneObjectCamera>> Cameras;
		// std::list<std::shared_ptr<SceneObjectLight>> Lights;

		std::shared_ptr<SceneObjectCamera> MainCamera;

		void Tick();
		void SetOnTick(std::function<void()> onTick) {
			this->onTick = onTick;
		}
	protected:
		SceneEmptyNode* m_RootNode;
		std::function<void()> onTick;
	};
}
