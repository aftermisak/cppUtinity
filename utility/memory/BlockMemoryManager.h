#pragma once
#include <cstddef>
#include <list>
#include <memory>
#define NOEXCEPT
using namespace std;
namespace utility{
	namespace memory{
		
		class BlockMemoryManager
		{
		private:
			class MemoryBlock
			{
			private:
				void* _address;
				size_t _capacity;
				size_t _sizeOfUsed;
				size_t _sizeOfFreed;
				bool _isUnapplyableWaitForAllFree;
			public:
				MemoryBlock(void* resource, size_t size);
				bool isContainAddress(void * address);
				void free(void * address, size_t size);
				void* get(size_t size);
			private:
				void refresh();
			};
			const static size_t memory_block_size;

			std::list<std::shared_ptr<MemoryBlock>> _memoryBlockList;
		public:
			//return memory address, nullptr if memory is not enough
			void* getMemory(size_t size) NOEXCEPT;
			void freeMemory(void* memory, size_t size) NOEXCEPT;
		private:
			//create memory block , return nullptr if memory is not enough
			static std::shared_ptr<MemoryBlock> createMemoryBlock(size_t size);
		};

	};

};
