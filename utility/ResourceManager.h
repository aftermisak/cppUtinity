#pragma once
#include "UnCopyable.h"
#include "UnNewable.h"
#include <functional>
namespace utility
{
	/**
		������Դ������
		example:
			lock.lock();
			{
				utility:;LocalResGuard guard([](){
				lock.unlock();
				})
				...//other code
			}               //onlock invoke auto
			
		@param callback һ���������󣬱����ʵ������ʱ����ø÷���
	*/
	class LocalResGuard : private UnCopyable , public UnNewable
	{ 
	public:	
		explicit LocalResGuard(std::function<void()> callback): _callback(callback){}
		~LocalResGuard(){ _callback();}

	private:
		std::function<void()> _callback;
	};
	/**
		�����Ҫ���Լ����hand new operator������
	�̳в�ʹ����Ӧ������Ϊģ�����
	*/
	template<class T>
	class NewHandler
	{
	private:
		static void new_handler();
	public:
		void* operator new(std::size_t size);
	};

	template<class T>
	void* NewHandler<T>::operator new(size_t size)
	{
		auto old_handler = std::set_new_handler(NewHandler<T>::new_handler);//�ı�

		auto guard = std::shared_ptr<void>(old_handler, [](void* handler){
			std::set_new_handler( static_cast<void (*)()>( handler));//���
		});
		return ::new(size);
	}

};

