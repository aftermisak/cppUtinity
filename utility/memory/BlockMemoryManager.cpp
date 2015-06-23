#include "BlockMemoryManager.h"
#include "MemoryUnit.h"
#include <new>
#include <cassert>
using namespace std;
using namespace utility::memory;

const size_t BlockMemoryManager::memory_block_size = 8 * MB;

void* BlockMemoryManager::getMemory(size_t size) NOEXCEPT
{
	if (size > memory_block_size){ return nullptr; }

	//find available memory from block
	for (auto& memoryBlock : this->_memoryBlockList)
	{
		void * memory = memoryBlock->get(size);
		if (nullptr != memory)
			return memory;
	}
	
	//try to create new block
	std::shared_ptr<BlockMemoryManager::MemoryBlock> newMemoryBlock = nullptr;
	for (size_t sizeOfMemory = memory_block_size; sizeOfMemory > 1 * KB && newMemoryBlock == nullptr; sizeOfMemory /= 2)
	{
		newMemoryBlock = createMemoryBlock(sizeOfMemory);
	}
	//try failed
	if (newMemoryBlock == nullptr){ return nullptr; }

	//try succeed
	_memoryBlockList.push_back(newMemoryBlock);
	return newMemoryBlock->get(size);
}

void BlockMemoryManager::freeMemory(void* memory, size_t size) NOEXCEPT
{
	//find available memory from block
	for (auto& memoryBlock : this->_memoryBlockList)
	{
		if (memoryBlock->isContainAddress(memory))
		{
			memoryBlock->free(memory, size);
			break;
		}
	}
}

std::shared_ptr<BlockMemoryManager::MemoryBlock> BlockMemoryManager::createMemoryBlock(size_t size)
{
	void* resource = malloc(size);
	if (nullptr == resource)
	{
		return nullptr;
	}

	auto oldNewHandler = set_new_handler(nullptr);
	auto result = new BlockMemoryManager::MemoryBlock(resource, size);
	set_new_handler(oldNewHandler);
	if (nullptr == result)
	{
		free(resource);
		return nullptr;
	}
	return std::shared_ptr<BlockMemoryManager::MemoryBlock>(result);
}

BlockMemoryManager::MemoryBlock::MemoryBlock(void* resource, size_t size):
_address(resource), _capacity(size), _sizeOfUsed( 0 ),
_sizeOfFreed(0), _isUnapplyableWaitForAllFree(false)
{
	
}
bool BlockMemoryManager::MemoryBlock::isContainAddress(void * address)
{
	return this->_address <= address && (char *)this->_address + this->_capacity >= (char *)address;
}
void BlockMemoryManager::MemoryBlock::free(void * address, size_t size)
{
	assert(isContainAddress(address) && "address is not in this block!");
	_sizeOfFreed += size;
	if (this->_isUnapplyableWaitForAllFree && _sizeOfUsed == _sizeOfFreed)
	{
		this->refresh();
	}
}

void* BlockMemoryManager::MemoryBlock::get(size_t size)
{
	if (this->_isUnapplyableWaitForAllFree || this->_capacity - this->_sizeOfUsed < size)
	{
		this->_isUnapplyableWaitForAllFree = true;
		return nullptr;
	}
	void* result = (char*)this->_address + this->_sizeOfUsed;
	this->_sizeOfUsed += size;
	return result;
}

void BlockMemoryManager::MemoryBlock::refresh()
{
	this->_sizeOfUsed = 0;
	this->_sizeOfFreed = 0;
	this->_isUnapplyableWaitForAllFree = false;
}