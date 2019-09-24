#pragma once
#include <unordered_map>
#include "SceneNode.hpp"
#include <functional>
#include "Camera.hpp"

namespace Entropy {

	class Scene {
	public:

		Scene(const char* scene_name);
		~Scene();

		// std::unique_ptr<SceneNode> SceneGraph;
		// std::list<std::shared_ptr<SceneGeometryNode>> Geometries;
		// std::list<std::shared_ptr<Camera>> Cameras;
		// std::list<std::shared_ptr<SceneObjectLight>> Lights;
		std::shared_ptr<Camera> MainCamera;
		void Tick(float deltaTime);
		void SetOnTick(std::function<void(float)> onTick) {
			this->onTick = onTick;
		}

		SceneEmptyNode* GetRootNode() {
			return m_RootNode.get();
		}
	protected:
		std::shared_ptr<SceneEmptyNode> m_RootNode;
		std::function<void(float)> onTick;
	};
}
