#include "FreeListMemoryManager.h"
#include <cassert>
#include <new>
#include <cassert>
using namespace std;
const size_t FreeListMemoryManager::APPLY_UNIT = 8;//8 Byte

const size_t FreeListMemoryManager::MAX_APPLYABLE_UNIT_COUNT = 1024;

FreeListMemoryManager::FreeListMemoryManager()
{
	_memoryListArray = new (std::nothrow) (MemoryNode[MAX_APPLYABLE_UNIT_COUNT]);
	assert(nullptr != _memoryListArray && "...");
}


void* FreeListMemoryManager::getMemory(size_t size) NOEXCEPT
{
	assert(size != 0 && "memory apply must greater than 0");

	size_t realApplySize = calculateRealApplySize(size, APPLY_UNIT);
	size_t indexOfList = realApplySize / APPLY_UNIT - 1;

	auto& _memoryListHead = _memoryListArray[indexOfList];
	auto firstFreeNode = _memoryListHead._next;
	if (nullptr != firstFreeNode)  //有可用的已释放内存
	{
		_memoryListHead._next = firstFreeNode->_next;
		return (void*)firstFreeNode;
	}
	
	//没有可用的已释放内存,则申请新的内存
	void* newAppliedMemory = malloc(realApplySize);
	return newAppliedMemory;
}
void FreeListMemoryManager::freeMemory(void* memory, size_t size) NOEXCEPT
{
	size_t realApplySize = calculateRealApplySize(size, APPLY_UNIT);
	size_t indexOfList = realApplySize / APPLY_UNIT - 1;

	auto& memoryListHead = _memoryListArray[indexOfList];
	if (nullptr == memoryListHead._next)
	{
		auto freedMemory = (MemoryNode*)memory;
		memoryListHead._next = freedMemory;
		freedMemory->_next = nullptr;
		return;
	}

	auto _memoryListNode = memoryListHead._next;
	while (_memoryListNode->_next != nullptr)
	{ 
		_memoryListNode = _memoryListNode->_next;
	}
	auto freedMemory = (MemoryNode*)memory;
	_memoryListNode->_next = freedMemory;
	freedMemory->_next = nullptr;
}