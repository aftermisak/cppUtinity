#pragma once
#include "UnCopyable.h"
#include <mutex>
using namespace std;

namespace utility
{

	/**
		����ģʽ�����̰߳�ȫ����ģ��
		ʹ�÷�ʽ���̳и��࣬���뽫ʵ������Ӧ�汾
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
		����ģʽ�������̰߳�ȫ������汾�ĵ��������ܹ��ھ�̬��ʼ��ʱ������,
	��Ϊ��̬��ʼ��ʱ��������ʵ������һ����ɡ�
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
		����ģʽ�̰߳�ȫ����ģ��
		ʹ�÷�ʽ���̳и��࣬���뽫ʵ������Ӧ�汾,����汾�ĵ��������ܹ��ھ�̬��ʼ��ʱ������,
	��Ϊ��̬��ʼ��ʱͬ�����ź���û�б���ʼ��
	*/
	template<class T>
	class SingletonTS : private UnCopyable
	{
	public:
		static T* getInstance();
	private:
		static std::mutex instanceMutex;
	};
	template<class T> std::mutex SingletonTS<T>::instanceMutex;//Ĭ�Ϲ���

	template<class T>
	T* SingletonTS<T>::getInstance()	
	{
		static T * instance = nullptr;
		if( instance == nullptr )
		{
			//��
			lock_guard<std::mutex> lg(SingletonTS<T>::instanceMutex);
			if(instance == nullptr)
			{
				instance = new T;
			}
		}
		return instance;
	}
};
