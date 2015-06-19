#pragma once
#include <cstddef>
namespace utility
{
	/**
		������Ϊ���ɶ�̬����������.
		ʹ�÷���:�̳и÷���,���Ҳ�����������д�÷���.
		��������ͻ�����ʾ����ȫ��new(::new)�������ƹ��˷�����ʹ�ñ���ͨ��
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
