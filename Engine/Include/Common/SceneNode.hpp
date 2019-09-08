#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include "SceneObject.hpp"
#include "Transform.hpp"
#include "Graphic/Material.hpp"

namespace Entropy {
	class BaseSceneNode {
	public:
		BaseSceneNode() {
		};
		BaseSceneNode(const char* name) { m_strName = name; };
		BaseSceneNode(const std::string& name) { m_strName = name; };
		BaseSceneNode(const std::string&& name) { m_strName = std::move(name); };

		virtual ~BaseSceneNode() {
		};

		void AppendChild(std::unique_ptr<BaseSceneNode>&& sub_node)
		{
			m_Children.push_back(std::move(sub_node));
		}

	protected:
		std::string m_strName;
		std::list<std::unique_ptr<BaseSceneNode>> m_Children;
		std::unique_ptr<Transform> m_Transforms;
	};

	template <typename T>
	class SceneNode : public BaseSceneNode {
	protected:
		std::shared_ptr<T> m_pSceneObject;

	protected:
		virtual void dump(std::ostream& out) const {
			if (m_pSceneObject)
				out << *m_pSceneObject << std::endl;
		};

	public:
		using BaseSceneNode::BaseSceneNode;
		SceneNode() = default;
		SceneNode(const std::shared_ptr<T>& object) { m_pSceneObject = object; };
		SceneNode(const std::shared_ptr<T>&& object) { m_pSceneObject = std::move(object); };

		void AddSceneObjectRef(const std::shared_ptr<T>& object) { m_pSceneObject = object; };
		void AddSceneObjectRef(const std::shared_ptr<T>&& object) { m_pSceneObject = std::move(object); };

	};

	typedef BaseSceneNode SceneEmptyNode;


	class SceneGeometryNode : public SceneNode<SceneObjectGeometry>
	{
	protected:
		bool        m_bVisible;
		bool        m_bShadow;
		std::vector<std::shared_ptr<Material>> m_Materials;

	protected:
		virtual void dump(std::ostream& out) const
		{
			SceneNode::dump(out);
			out << "Visible: " << m_bVisible << std::endl;
			out << "Shadow: " << m_bShadow << std::endl;
			out << "Material(s): " << std::endl;
			// for (auto material : m_Materials) {
				// out << *material << std::endl;
			// }
		};

	public:
		using SceneNode::SceneNode;

		void SetVisibility(bool visible) { m_bVisible = visible; };
		const bool Visible() { return m_bVisible; };
		void SetIfCastShadow(bool shadow) { m_bShadow = shadow; };
		const bool CastShadow() { return m_bShadow; };
		using SceneNode::AddSceneObjectRef;
		void AddSceneObjectRef(const std::shared_ptr<Material>& object) { m_Materials.push_back(object); };
	};

}
