#pragma once
#include "Common.hpp"

namespace Entropy {
	class EntropyCore;
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

		void SetEngine(EntropyCore* engine);
	};

}
