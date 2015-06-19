#pragma  once

#define  NOEXCEPT
#include <vector>
using namespace std;
class FreeListMemoryManager
{
private:
	union MemoryNode
	{
		MemoryNode* _next;
		void* _clientMemory;
		MemoryNode() :_next(nullptr){}
	}; 
	const static size_t APPLY_UNIT;
	const static size_t MAX_APPLYABLE_UNIT_COUNT;


private:
	MemoryNode* _memoryListArray;
	inline static size_t calculateRealApplySize(size_t size, size_t unit){
		return ((size - 1) / unit + 1) * unit;
	}
public:
	FreeListMemoryManager();
	//return memory address, nullptr if memory is not enough
	void* getMemory(size_t size) NOEXCEPT;
	void freeMemory(void* memory, size_t size) NOEXCEPT;
};