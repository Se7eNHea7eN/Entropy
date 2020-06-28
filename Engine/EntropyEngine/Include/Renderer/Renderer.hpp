#pragma once
#include <memory>
#include <string>
namespace Entropy {
	class EntropyCore;
	class RHITexture2D;
	class Texture;


	class Renderer {
				
	protected:
		int width;
		int height;
		EntropyCore* engine;
	public:
		Renderer() = default;
		virtual ~Renderer() = default;
		virtual void Initialize() = 0;
		virtual void Resize(int w, int h) = 0;
		virtual void Draw() = 0;
		virtual void AwaitRenderFrame() = 0;
		void SetEngine(EntropyCore* engine);

		virtual std::shared_ptr<RHITexture2D> CreateRHITexture2D(const Entropy::Texture* pTexture) = 0;
		virtual std::shared_ptr<RHITexture2D> CreateRHITexture2DFromFile(const std::string filePath) = 0;
	};

}
