#pragma once
#include <string>
#include <Eigen/Core>
#include "Texture.hpp"
#include <map>
#include <bimg/decode.h>

namespace Entropy {
	using namespace Eigen;

	struct ColorRGBA {
		ColorRGBA() = default;
		ColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: r(r),
			g(g),
			b(b),
			a(a) {
		}
		ColorRGBA(const ColorRGBA& rhs) = default;

		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 0;
	};


	struct Vec3 {
		Vec3(float x, float y, float z)
			: x(x),
			y(y),
			z(z) {
		}

		Vec3() = default;

		float x = 0;
		float y = 0;
		float z = 0;
	};

	enum ParameterType {
		Sampler,
		Vector4,
		Vector3,
		Float
	};
	
	
	struct Parameter
	{
		const void* Value;
		ParameterType type;

		Parameter() = default;

		Parameter(const void* value, ParameterType type) : Value(value) , type(type) {};

		Parameter(const Parameter& rhs) = default;

		Parameter(Parameter&& rhs) = default;

		Parameter& operator=(const Parameter& rhs) = default;
		Parameter& operator=(Parameter&& rhs) = default;
	

		~Parameter() = default;

		// friend std::ostream& operator<<(std::ostream& out, const Parameter& obj)
		// {
		// 	out << "Parameter Value: " << obj.Value << std::endl;
		// 	if (obj.ValueMap) {
		// 		out << "Parameter Map: " << *obj.ValueMap << std::endl;
		// 	}
		//
		// 	return out;
		// }
	};


	// typedef ParameterValueMap<ColorRGBA> Color;
	// typedef ParameterValueMap<Vec3> Normal;
	// typedef ParameterValueMap<float>    Parameter;
	
	class Material : Object {
	public:
	protected:
		std::string m_Name;
		std::string m_VertexShader;
		std::string m_FragmentShader;
		std::map<std::string, Parameter> parameters;
	public:

		// Color       m_Albedo;
		// ParameterValueMap<float>    m_Metallic;
		// ParameterValueMap<float>    m_Roughness;
		// Normal      m_Normal;
		// ParameterValueMap<float>    m_Specular;
		// ParameterValueMap<float>    m_AmbientOcclusion;

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

		std::map<std::string, Parameter> GetParams() {
			return parameters;
		}

		friend std::ostream& operator<<(std::ostream& out, const Texture& obj);
	};

	std::shared_ptr<bimg::ImageContainer> buildImageByFloat(float value);

	class StandardPBRMaterial : public Material {
	public:
		StandardPBRMaterial() {
			m_VertexShader = "vs_common";
			m_FragmentShader = "fs_pbr";
		}

		void SetAlbedo(ColorRGBA color) {
			auto image = std::make_shared<bimg::ImageContainer>();
			image->m_data = new ColorRGBA(color);
			image->m_size = sizeof(ColorRGBA);
			image->m_width = 1;
			image->m_height = 1;
			image->m_depth = sizeof(ColorRGBA);
			image->m_numLayers = 1;
			image->m_numMips = 0;
			image->m_hasAlpha = true;
			image->m_format = bimg::TextureFormat::RGBA8;
			auto texture = new Texture(image);
			parameters["s_albedo"] = Parameter(texture,Sampler);
		}

		void SetAlbedo(Texture* texture) {
			parameters["s_albedo"] = Parameter(texture, Sampler);
		}

		void SetNormal(Texture* texture) {
			parameters["s_normal"] = Parameter(texture, Sampler);
		}
		
		void SetMetallic(float value) {
			
			parameters["s_metallic"] = Parameter(&value, Sampler);
		}

		void SetMetallic(Texture* texture) {
			parameters["s_metallic"] = Parameter(texture, Sampler);
		}

		void SetRoughness(float value) {
			auto texture = new Texture(buildImageByFloat(value));
			parameters["s_roughness"] = Parameter(texture, Sampler);
		}

		void SetRoughness(Texture* texture) {
			parameters["s_roughness"] = Parameter(texture, Sampler);
		}


		void SetAmbientOcclusion(float value) {
			auto texture = new Texture(std::move(buildImageByFloat(value)));

			parameters["s_ao"] = Parameter(texture, Sampler);
		}

		void SetAmbientOcclusion(Texture* texture) {
			parameters["s_ao"] = Parameter(texture, Sampler);
		}
	};
}
