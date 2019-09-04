#pragma once
#include <assert.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
namespace Entropy {
	class BaseSceneObject
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
	
	class SceneObjectMesh : public BaseSceneObject
	{
	protected:
		std::vector<SceneObjectIndexArray>  m_IndexArray;
		std::vector<SceneObjectVertexArray> m_VertexArray;
		PrimitiveType	m_PrimitiveType;

		bool        m_bVisible;
		bool        m_bShadow;
		bool        m_bMotionBlur;

	public:
		SceneObjectMesh(bool visible = true, bool shadow = true, bool motion_blur = true) : BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh), m_bVisible(visible), m_bShadow(shadow), m_bMotionBlur(motion_blur) {};
		SceneObjectMesh(SceneObjectMesh&& mesh)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh),
			m_IndexArray(std::move(mesh.m_IndexArray)),
			m_VertexArray(std::move(mesh.m_VertexArray)),
			m_PrimitiveType(mesh.m_PrimitiveType),
			m_bVisible(mesh.m_bVisible),
			m_bShadow(mesh.m_bShadow),
			m_bMotionBlur(mesh.m_bMotionBlur)
		{
		};
		void AddIndexArray(SceneObjectIndexArray&& array) { m_IndexArray.push_back(std::move(array)); };
		void AddVertexArray(SceneObjectVertexArray&& array) { m_VertexArray.push_back(std::move(array)); };
		void SetPrimitiveType(PrimitiveType type) { m_PrimitiveType = type; };

		friend std::ostream& operator<<(std::ostream& out, const SceneObjectMesh& obj)
		{
			out << static_cast<const BaseSceneObject&>(obj) << std::endl;
			out << "Primitive Type: " << obj.m_PrimitiveType << std::endl;
			out << "This mesh contains 0x" << obj.m_VertexArray.size() << " vertex properties." << std::endl;
			for (size_t i = 0; i < obj.m_VertexArray.size(); i++) {
				out << obj.m_VertexArray[i] << std::endl;
			}
			out << "This mesh contains 0x" << obj.m_IndexArray.size() << " index arrays." << std::endl;
			for (size_t i = 0; i < obj.m_IndexArray.size(); i++) {
				out << obj.m_IndexArray[i] << std::endl;
			}
			out << "Visible: " << obj.m_bVisible << std::endl;
			out << "Shadow: " << obj.m_bShadow << std::endl;
			out << "Motion Blur: " << obj.m_bMotionBlur << std::endl;

			return out;
		}
	};
	class SceneObjectGeometry : public BaseSceneObject
	{
	protected:
		std::vector<std::shared_ptr<SceneObjectMesh>> m_Mesh;
		bool        m_bVisible;
		bool        m_bShadow;
		bool        m_bMotionBlur;

	public:

		void SetVisibility(bool visible) { m_bVisible = visible; };
		const bool Visible() { return m_bVisible; };
		void SetIfCastShadow(bool shadow) { m_bShadow = shadow; };
		const bool CastShadow() { return m_bShadow; };
		void SetIfMotionBlur(bool motion_blur) { m_bMotionBlur = motion_blur; };
		const bool MotionBlur() { return m_bMotionBlur; };

		void AddMesh(std::shared_ptr<SceneObjectMesh>& mesh) { m_Mesh.push_back(std::move(mesh)); };

		friend std::ostream& operator<<(std::ostream& out, const SceneObjectGeometry& obj)
		{
			auto count = obj.m_Mesh.size();
			for (decltype(count) i = 0; i < count; i++) {
				out << "Mesh: " << *obj.m_Mesh[i] << std::endl;
			}

			return out;
		}
	};
}