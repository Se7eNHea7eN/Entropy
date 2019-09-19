#pragma once
#include <bx/bx.h>
#include <tinystl/allocator.h>
#include <tinystl/vector.h>
#include <bx/filepath.h>
#include "bx/file.h"
#include <bimg/bimg.h>

namespace stl = tinystl;

///
void* load(const char* _filePath, uint32_t* _size = NULL);
void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);
///
void unload(void* _ptr);


bx::FileReaderI* getFileReader();
bx::FileWriterI* getFileWriter();
bx::AllocatorI* getAllocator();

