#pragma once
#include <cstddef>
namespace utility
{
	/**
		将类标记为不可动态创建的类型.
		使用方法:继承该方法,并且不再子类中重写该方法.
		但是如果客户端显示调用全局new(::new)，可以绕过此方法，使得编译通过
	*/
	class UnNewable
	{
	public:
		static void* operator new(std::size_t size) = delete;
		static void operator delete(void* ptr) = delete;
		static void* operator new[](std::size_t size) = delete;
		static void operator delete[](void* ptr) = delete;
	//private:
	//	static void* operator new(std::size_t size);
	//	static void operator delete(void* ptr);
	//	static void* operator new[](std::size_t size);
	//	static void operator delete[](void* ptr);
		
	};
};
