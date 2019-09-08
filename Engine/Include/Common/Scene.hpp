#pragma once
#include <unordered_map>
#include "SceneNode.hpp"

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
		std::list<std::unique_ptr<SceneObjectGeometry>> Geometries;
		std::list<std::unique_ptr<SceneObjectCamera>> Cameras;
		std::list<std::unique_ptr<SceneObjectLight>> Lights;

		std::shared_ptr<SceneObjectCamera> MainCamera;
	protected:
		SceneEmptyNode* m_RootNode;
	};
}
