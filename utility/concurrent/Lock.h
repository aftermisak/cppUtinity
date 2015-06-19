#pragma  once
#include <memory>
using namespace std;
#include "Condition.h"
namespace utility{
	namespace concurrenct{
		class Lock
		{
		public:
			virtual ~Lock(){}
			/*
				阻塞，直到获取到锁
			*/
			virtual void lock() = 0;
			/*
				阻塞，直到获取锁或者超时
				@param ms 等待时间，毫秒为单位
				@return  成功获取锁返回 true, 超时返回false
			*/
			virtual bool lock( unsigned ms ) = 0;
			/*
				解锁,用于解除Look::look获取的锁定
			*/
			virtual void unlock() = 0;

			/*
				创建一个和当前锁相关的条件变量
			*/
			virtual shared_ptr<Condition> newSharedPtrCondition() = 0;

			/*
				创建一个和当前锁相关的条件变量，
			分配与堆内存。
			*/
			virtual Condition* newCondition() = 0;
		};

		/*
			读写锁,
			同一个该对象的写锁会排斥读锁和写锁
			同一个该对象的读锁会排斥写锁，但是不会排斥读锁
			ps: 由于读锁和写锁，将作为读写锁的内部数据，在多线程情况下，智能指针的拷贝是不安全的，所以
		这里返回的是原始指针,两个get方法需要保证实现的正确性
		*/
		class ReadWriteLock
		{
		public:
			virtual Lock* getReadLock() = 0;
			virtual Lock* getWriteLock() = 0;
			virtual ~ReadWriteLock(){}
		};
};
};
