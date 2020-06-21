#pragma once
#include <list>
#include <memory>
#include <string>
#include "Interface/IRuntimeModule.hpp"


namespace Entropy {
	class Component;
	class Transform;
	class SceneNode :public std::enable_shared_from_this<SceneNode> , public IRuntimeModule {
	public:
		SceneNode();;
		SceneNode(const char* name);;
		SceneNode(const std::string& name);;
		SceneNode(const std::string&& name);;

		virtual ~SceneNode();;

		void AddChild(std::shared_ptr<SceneNode>&& sub_node);

		void AddComponent(std::shared_ptr<Component>&& c);

		Transform* GetTransform() const {
			return m_Transform.get();
		}

		std::shared_ptr<SceneNode> SharedPtr() {
			return shared_from_this();
		}

		std::shared_ptr<SceneNode> GetParent() {
			return m_Parent;
		}


		int Initialize() override;
		void Finalize() override;


		void Tick(float deltaTime) override;
		std::string GetName() {
			return m_strName;
		}
		std::list<std::shared_ptr<SceneNode>> GetChildren() {
			return m_Children;
		}
	protected:
		std::string m_strName;
		std::unique_ptr<Transform> m_Transform ;

		std::shared_ptr<SceneNode> m_Parent;
		std::list<std::shared_ptr<SceneNode>> m_Children;

		std::list<std::shared_ptr<Component>> m_Components;

	protected:

		void setParent(std::shared_ptr<SceneNode> p) {
			m_Parent = p;
		}
	};

}
