#pragma
#include <string>
#include <Eigen/Core>
#include "Texture.hpp"
namespace Entropy {
	using namespace Eigen;
	template <typename T>
	struct ParameterValueMap
	{
		T Value;
		std::shared_ptr<Texture> ValueMap;

		ParameterValueMap() = default;

		ParameterValueMap(const T value) : Value(value) {};
		ParameterValueMap(const std::shared_ptr<Texture>& value) : ValueMap(value) {};

		ParameterValueMap(const ParameterValueMap<T>& rhs) = default;

		ParameterValueMap(ParameterValueMap<T>&& rhs) = default;

		ParameterValueMap& operator=(const ParameterValueMap<T>& rhs) = default;
		ParameterValueMap& operator=(ParameterValueMap<T>&& rhs) = default;
		ParameterValueMap& operator=(const std::shared_ptr<Texture>& rhs)
		{
			ValueMap = rhs;
			return *this;
		};

		~ParameterValueMap() = default;

		friend std::ostream& operator<<(std::ostream& out, const ParameterValueMap<T>& obj)
		{
			out << "Parameter Value: " << obj.Value << std::endl;
			if (obj.ValueMap) {
				out << "Parameter Map: " << *obj.ValueMap << std::endl;
			}

			return out;
		}
	};

	struct ColorRGBA {
		ColorRGBA() = default;
		ColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: r(r),
			  g(g),
			  b(b),
			  a(a) {
		}

		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 0;
	};

	
	typedef ParameterValueMap<ColorRGBA> Color;
	typedef ParameterValueMap<Vector3f> Normal;
	typedef ParameterValueMap<float>    Parameter;

	class Material : Object {
	public:
	protected:
		std::string m_Name;
		std::string m_VertexShader;
		std::string m_FragmentShader;


	public:

		Color       m_Albedo;
		Parameter   m_Metallic;
		Parameter   m_Roughness;
		Normal      m_Normal;
		Parameter   m_Specular;
		Parameter   m_AmbientOcclusion;

		std::string VertexShader() const {
			return m_VertexShader;
		}

		void SetVertexShader(const std::string& path) {
			m_VertexShader = path;
		}

		std::string FragmentShader() const {
			return m_FragmentShader;
		}

		void SetFragmentShader(const std::string& path) {
			m_FragmentShader = path;
		}
		
		void SetName(const std::string& name) { m_Name = name; };
		void SetName(std::string&& name) { m_Name = std::move(name); };
	

		friend std::ostream& operator<<(std::ostream& out, const Texture& obj);
	};
}
