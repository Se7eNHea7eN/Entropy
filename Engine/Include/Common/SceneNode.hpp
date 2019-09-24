#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <string>


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

		std::shared_ptr<Transform> GetTransform() const {
			return m_Transform;
		}

		virtual void Tick(float deltaTime);;
	protected:

		std::string m_strName;
		std::shared_ptr<Transform> m_Transform;

		std::shared_ptr<SceneNode> m_Parent;
		std::list<std::shared_ptr<SceneNode>> m_Children;

		std::list<std::shared_ptr<Component>> m_Components;

	protected:

		void setParent(std::shared_ptr<SceneNode> p) {
			m_Parent = p;
		}
	};
	typedef SceneNode SceneEmptyNode;
	

}
