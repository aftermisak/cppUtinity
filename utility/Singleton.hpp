#pragma once
#include "UnCopyable.h"
#include <mutex>
using namespace std;

namespace utility
{

	/**
		懒惰模式，非线程安全单例模板
		使用方式，继承该类，编译将实例化相应版本
	*/
	template<class T>
	class Singleton : private UnCopyable
	{
	public:
		static T* getInstance();
		
	};
	template<class T>
	T* Singleton<T>::getInstance()
	{
		static T instance;
		return &instance;
	}
	/*
		饥饿模式单例，线程安全，这个版本的单例，不能够在静态初始化时被调用,
	因为静态初始化时，单例的实例化不一定完成。
	*/
	template<class T>
	class SingletonHTS : private UnCopyable
	{
	public:
		static T* getInstance();
	private:
		static T _instance;
	};
	template<class T> T SingletonHTS<T>::_instance;
	template<class T>
	T* SingletonHTS<T>::getInstance()
	{
		return &_instance;
	}

	
	/**
		懒惰模式线程安全单例模板
		使用方式，继承该类，编译将实例化相应版本,这个版本的单例，不能够在静态初始化时被调用,
	因为静态初始化时同步的信号量没有被初始化
	*/
	template<class T>
	class SingletonTS : private UnCopyable
	{
	public:
		static T* getInstance();
	private:
		static std::mutex instanceMutex;
	};
	template<class T> std::mutex SingletonTS<T>::instanceMutex;//默认构造

	template<class T>
	T* SingletonTS<T>::getInstance()	
	{
		static T * instance = nullptr;
		if( instance == nullptr )
		{
			//锁
			lock_guard<std::mutex> lg(SingletonTS<T>::instanceMutex);
			if(instance == nullptr)
			{
				instance = new T;
			}
		}
		return instance;
	}
};
