#pragma once
#include <assert.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Graphic/Mesh.hpp"
#include "Object.hpp"

namespace Entropy {
	class BaseSceneObject : Object
	{
		protected:

			BaseSceneObject() {};
			virtual ~BaseSceneObject() {};

		private:
			// can not be copied
			BaseSceneObject(BaseSceneObject& obj) = delete;
			BaseSceneObject& operator=(BaseSceneObject& obj) = delete;

		public:
		
		friend std::ostream& operator<<(std::ostream& out, const BaseSceneObject& obj)
		{
			out << "SceneObject" << std::endl;
			out << "-----------" << std::endl;

			return out;
		}
	};
	
	
	class SceneObjectGeometry : public BaseSceneObject
	{
	protected:
		std::vector<std::shared_ptr<Mesh>> m_Mesh;
		bool        m_bVisible;
		bool        m_bShadow;

	public:

		void SetVisibility(bool visible) { m_bVisible = visible; };
		const bool Visible() { return m_bVisible; };
		void SetIfCastShadow(bool shadow) { m_bShadow = shadow; };
		const bool CastShadow() { return m_bShadow; };

		void AddMesh(std::shared_ptr<Mesh>& mesh) { m_Mesh.push_back(std::move(mesh)); };

		friend std::ostream& operator<<(std::ostream& out, const SceneObjectGeometry& obj)
		{
			auto count = obj.m_Mesh.size();
			for (decltype(count) i = 0; i < count; i++) {
				// out << "Mesh: " << *obj.m_Mesh[i] << std::endl;
			}

			return out;
		}
	};


}
