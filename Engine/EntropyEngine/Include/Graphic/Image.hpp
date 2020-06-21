#pragma once
#include <iostream>

namespace Entropy {
	typedef struct R8G8B8A8Unorm{
		uint8_t data[4];
	};
	
    typedef struct _Image {
        uint32_t Width;
        uint32_t Height;
        R8G8B8A8Unorm* data;
        uint32_t bitcount;
        uint32_t pitch;
        size_t  data_size;
    } Image;

    std::ostream& operator<<(std::ostream& out, const Image& image);
}


