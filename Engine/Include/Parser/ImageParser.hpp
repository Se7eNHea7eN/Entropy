#pragma once
#include "Interface/Interface.hpp"
#include "Graphic/Image.hpp"

namespace Entropy {
	class Buffer;
	Interface ImageParser
	{
	public:
		virtual Entropy::Image Parse(const Entropy::Buffer & buf) = 0;
	};
}
