#pragma once
#include <functional>
#include <memory>

namespace Entropy {
	class SkyBox;
	class Camera;
	class SceneNode;
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

		SceneNode* GetRootNode() {
			return m_RootNode.get();
		}
	protected:
		std::shared_ptr<SceneNode> m_RootNode;
		std::function<void(float)> onTick;

		std::shared_ptr<SkyBox> m_skybox;
	public:
		void SetSkybox(const std::shared_ptr<SkyBox>& sky_box) {
			m_skybox = sky_box;
		}

		std::shared_ptr<SkyBox> GetSkybox() const {
			return m_skybox;
		}
	};
}
