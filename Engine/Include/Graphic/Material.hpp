#pragma once
#include <string>
#include <Eigen/Core>
#include "Texture.hpp"
#include <map>
#include <bimg/decode.h>
#include "Utils/Utils.hpp"

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


	struct Vec4 {
		Vec4(float x, float y, float z, float w)
			: x(x),
			  y(y),
			  z(z),
			  w(w) {
		}

		Vec4() = default;

		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
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
		uint32_t num = 0;
		ParameterType type;
		int index = 0;
		Parameter() = default;

		Parameter(const void* value, ParameterType type,int index = 0) : Value(value) , type(type), index(index){};
		Parameter(const void* value, uint32_t num, ParameterType type) : Value(value), type(type), num(num) {};

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

		bimg::ImageContainer* buildImageByFloat(float value);
		bimg::ImageContainer* buildImageByColor(ColorRGBA value);

	public:

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

		void SetParam(std::string name, Parameter param) {
			parameters[name] = param;
		}
		
		friend std::ostream& operator<<(std::ostream& out, const Texture& obj);
	};


	class StandardPBRMaterial : public Material {
	protected:
		std::vector<Vec4> u_params;
	protected:
		void setParams() {
			parameters["u_params"] = Parameter(&u_params[0], 1, Vector4);
		}
	public:
		StandardPBRMaterial() {
			m_VertexShader = "vs_common";
			m_FragmentShader = "fs_pbr";
			u_params.push_back(Vec4(-1,-1,0,0));
			setParams();
		}

		void SetAlbedo(ColorRGBA color) {
			parameters["s_albedo"] = Parameter(new Texture(buildImageByColor(color)),Sampler,0);
		}

		void SetAlbedo(Texture* texture) {
			parameters["s_albedo"] = Parameter(texture, Sampler, 0);
		}

		void SetNormal(Texture* texture) {
			parameters["s_normal"] = Parameter(texture, Sampler, 1);

			u_params[0].x = 1;
			setParams();
		}
		
		void SetMetallic(float value) {
			SetMetallic(new Texture(buildImageByFloat(value)));
		}

		void SetMetallic(Texture* texture) {
			parameters["s_metallic"] = Parameter(texture, Sampler, 2);
		}

		void SetRoughness(float value) {
			parameters["s_roughness"] = Parameter(new Texture(buildImageByFloat(value)), Sampler, 3);
		}

		void SetRoughness(Texture* texture) {
			parameters["s_roughness"] = Parameter(texture, Sampler, 3);
		}

		void SetAmbientOcclusion(float value) {
			parameters["s_ao"] = Parameter(new Texture(buildImageByFloat(value)), Sampler, 4);
		}

		void SetAmbientOcclusion(Texture* texture) {
			parameters["s_ao"] = Parameter(texture, Sampler, 4);
		}


		void SetEmissive(ColorRGBA value) {
			SetEmissive(new Texture(buildImageByColor(value)));
		}

		void SetEmissive(Texture* texture) {
			parameters["s_emissive"] = Parameter(texture, Sampler, 5);
			u_params[0].y = 1;
			setParams();
		}

		void SetIrradianceMap(Texture* texture) {
			parameters["s_irradianceMap"] = Parameter(texture, Sampler, 6);
			u_params[0].z = 1;
			setParams();
		}
	};
}
