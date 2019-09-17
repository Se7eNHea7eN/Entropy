#pragma once
#include "bgfx/bgfx.h"
#include "Common/Scene.hpp"

namespace Entropy {
	struct BgfxGeometry {
		std::shared_ptr<Entropy::SceneGeometryNode> geometry;
		bgfx::VertexBufferHandle vbh;
		bgfx::IndexBufferHandle ibh;
		~BgfxGeometry() {
			bgfx::destroy(vbh);
			bgfx::destroy(ibh);
			geometry.reset();
		}
	};
}
