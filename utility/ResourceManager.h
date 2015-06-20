#pragma once
#include "UnCopyable.h"
#include "UnNewable.h"
#include <functional>
namespace utility
{
	/**
		本地资源帮助类
		example:
			lock.lock();
			{
				utility:;LocalResGuard guard([](){
				lock.unlock();
				})
				...//other code
			}               //onlock invoke auto
			
		@param callback 一个函数对象，本类的实例析构时会调用该方法
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
		如果需要有自己类的hand new operator方法。
	继承并使用相应的类作为模板参数
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
		auto old_handler = std::set_new_handler(NewHandler<T>::new_handler);//改变

		auto guard = std::shared_ptr<void>(old_handler, [](void* handler){
			std::set_new_handler( static_cast<void (*)()>( handler));//设回
		});
		return ::new(size);
	}

};

