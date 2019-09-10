#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include "Transform.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/Mesh.hpp"

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
		std::unique_ptr<Transform> m_Transform = std::make_unique<Transform>();

	protected:
		std::string m_strName;
		std::list<std::unique_ptr<BaseSceneNode>> m_Children;
	};

		std::vector<std::shared_ptr<Matial>> m_Materials;
		std::vector<std::shared_ptr<Mesh>> m_Mesh;
	protected:
		virtual void dump(std::ostream& out) const
		{
			out << "Visible: " << m_bVisible << std::endl;
			out << "Shadow: " << m_bShadow << std::endl;
			out << "Material(s): " << std::endl;
			// for (auto material : m_Materials) {
				// out << *material << std::endl;
			// }
		};

	public:

		void SetVisibility(bool visible) { m_bVisible = visible; };
		const bool Visible() { return m_bVisible; };
		void SetIfCastShadow(bool shadow) { m_bShadow = shadow; };
		const bool CastShadow() { return m_bShadow; };
		void AddSceneObjectRef(const std::shared_ptr<Material>& object) { m_Materials.push_back(object); };
	};

}
