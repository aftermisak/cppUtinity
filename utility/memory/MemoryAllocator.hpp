#pragma once

#include <cstddef>

#define NOEXCEPT

namespace utility{
namespace memory{
	
	template < typename TMemoryManager >
	class MemoryAllocator
	{
		TMemoryManager _memoryManager;
	public:
		void* apply(size_t size) NOEXCEPT
		{
			assert(size > 0 && "size must greater than 0");
			return _memoryManager.getMemory( size );
		}

		void free(void* memory, size_t size) NOEXCEPT
		{
			assert(nullptr != memory && size > 0 && "...");
			_memoryManager.freeMemory( memory, size );
		}
	};

};

};

