#pragma once
#include <bx/bx.h>
#include <tinystl/allocator.h>
#include <tinystl/vector.h>
#include <bx/filepath.h>
#include "bx/file.h"
#include <bimg/bimg.h>
#include <string>

namespace stl = tinystl;

///
void* load(const char* _filePath, uint32_t* _size = NULL);
void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);
///
void unload(void* _ptr);

bx::FileReaderI* getFileReader();
bx::FileWriterI* getFileWriter();
bx::AllocatorI* getAllocator();

bimg::ImageContainer* imageLoad(const char* _filePath, bimg::TextureFormat::Enum _dstFormat);


inline bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}