#pragma once
#include "SceneNode.hpp"
namespace Entropy {

	class Scene {
	public:
		Scene();
		~Scene();
	protected:
		SceneEmptyNode* m_RootNode;
	};
}
