#pragma once
#include "../Common/Component.hpp"
#include <vector>

namespace Entropy {
	class Material;
	class Mesh;
	class StaticMeshComponent : public Component {
	public:
		static std::vector<std::shared_ptr<StaticMeshComponent>> AllStaticMeshComponents;
	public:
		StaticMeshComponent();
		~StaticMeshComponent() override;
		int Initialize() override;
		void Finalize() override;
	protected:
		virtual void dump(std::ostream& out) const
		{
			// out << "Visible: " << m_bVisible << std::endl;
			// out << "Shadow: " << m_bShadow << std::endl;
			// out << "Material(s): " << std::endl;
			// for (auto material : m_Materials) {
				// out << *material << std::endl;
			// }
		};
		std::vector<std::shared_ptr<Material>> m_Materials;
		std::vector<std::shared_ptr<Mesh>> m_Mesh;
		bool enableLighting = true;
	public:
		bool EnableLighting() const {
			return enableLighting;
		}

		void SetEnableLighting(bool enable_lighting) {
			enableLighting = enable_lighting;
		}

		std::vector<std::shared_ptr<Material>>& GetMaterials() {
			return m_Materials;
		}

		std::vector<std::shared_ptr<Mesh>>& GetMeshes() {
			return m_Mesh;
		}

		void AddMesh(const std::shared_ptr<Mesh>& m) {
			m_Mesh.push_back(m);
		}

		void AddMaterial(const std::shared_ptr<Material>& m) {
			m_Materials.push_back(m);
		}
		// void SetVisibility(bool visible) { m_bVisible = visible; };
		// const bool Visible() { return m_bVisible; };
		// void SetIfCastShadow(bool shadow) { m_bShadow = shadow; };
		// const bool CastShadow() { return m_bShadow; };
	};
}
