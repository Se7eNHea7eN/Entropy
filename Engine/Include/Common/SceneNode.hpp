#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>


namespace Entropy {
	class Component;
	class Transform;
	class SceneNode {
	public:
		SceneNode();;
		SceneNode(const char* name);;
		SceneNode(const std::string& name);;
		SceneNode(const std::string&& name);;

		virtual ~SceneNode();;

		void AppendChild(std::shared_ptr<SceneNode>&& sub_node);

		void AppendComponent(std::shared_ptr<Component>&& c);

		Transform* GetTransform() const {
			return m_Transform.get();
		}

		virtual void Tick(float deltaTime);;
	protected:

		std::string m_strName;
		std::unique_ptr<Transform> m_Transform;

		std::shared_ptr<SceneNode> m_Parent;
		std::vector<std::shared_ptr<SceneNode>> m_Children;

		std::vector<std::shared_ptr<Component>> m_Components;

	protected:

		void setParent(std::shared_ptr<SceneNode> p) {
			m_Parent = p;
		}
	};
	typedef SceneNode SceneEmptyNode;
	

}
