#pragma once

#include "Lock.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "../UnCopyable.h"
#include "../UnNewable.h"
namespace utility{
	namespace concurrenct{

		//可设置时间锁
		class SimpleLock : public Lock ,private UnCopyable
		{
		private:
			std::timed_mutex m_mutex;
			std::unique_lock<std::timed_mutex> m_lock;
		public:
			SimpleLock();
			~SimpleLock();
			virtual void lock() override;
			virtual bool lock( unsigned ms ) override;
			virtual void unlock() override;
			virtual shared_ptr<Condition> newSharedPtrCondition() override;
			virtual Condition* newCondition() override;
		private:
			class ConditionForSimpleLock : public Condition
			{
			private:
				std::unique_lock<std::timed_mutex>* m_lock;
				//std::condition_variable can only take std::mutex,so use std::condition_variable_any
				std::condition_variable_any m_condition;
			public:
				ConditionForSimpleLock( std::unique_lock<std::timed_mutex>* lock );
				virtual void wait() override;
				virtual bool wait(unsigned ms) override;
				virtual void notify() override;
				virtual void notifyAll() override;
			};
		};
		
		//可设置时间可重入锁
		class ReentrantLock : public Lock, private UnCopyable
		{
		private:
			std::recursive_timed_mutex m_mutex;
			std::unique_lock<std::recursive_timed_mutex> m_lock;
		public:
			ReentrantLock();
			~ReentrantLock();
			virtual void lock() override;
			virtual bool lock( unsigned ms ) override;
			virtual void unlock() override;
			virtual shared_ptr<Condition> newSharedPtrCondition() override;
			virtual Condition* newCondition() override;
		private:
			class ConditionForReentrantLock : public Condition
			{
			private:
				std::unique_lock<std::recursive_timed_mutex>* m_lock;
				//std::condition_variable can only take std::mutex,so use std::condition_variable_any
				std::condition_variable_any m_condition;
			public:
				ConditionForReentrantLock( std::unique_lock<std::recursive_timed_mutex>* lock );
				virtual void wait() override;
				virtual bool wait(unsigned ms) override;
				virtual void notify() override;
				virtual void notifyAll() override;
			};
		};

		//template <typename LockPointer>
		//class LockGuard : public UnNewable//, private UnCopyable
		//{
		//public:
		//	explicit LockGuard(LockPointer aLock) :m_theLock(aLock){ aLock->lock(); }
		//	/*
		//		move construction
		//	*/
		//	explicit LockGuard(LockGuard&& rhs){ m_theLock = rhs.m_theLock; }
		//	/*
		//		move assignment
		//	*/
		//	LockGuard& operator=(LockGuard&& rhs){ m_theLock = rhs.m_theLock; return *this; }

		//	~LockGuard(){ m_theLock->unlock(); }
		//private:
		//	//不可拷贝
		//	LockGuard(const LockGuard& rhs) = delete;
		//	//不可pass by value
		//	LockGuard& operator=(const LockGuard& rhs) = delete;
		//private:
		//	LockPointer m_theLock;
		//};
		////template<>
		////class LockGuard< shared_ptr<Lock> > : public UnNewable, private UnCopyable
		////{
		//////public:
		//////	explicit LockGuard(shared_ptr<Lock> aLock) :m_theLock(aLock){ aLock->lock(); }
		//////	~LockGuard(){ m_theLock->unlock(); }
		//////private:
		//////	shared_ptr<Lock> m_theLock;
		////};

		//template<typename LockPointer>
		//LockGuard<LockPointer> make_lockguard(LockPointer lockPointer)
		//{
		//	//LockGuard<LockPointer>(lockPointer) lg;
		//	//return lg;
		//	return LockGuard<LockPointer>(lockPointer);
		//}
		
		/*
			基础 读写锁实现。
			该类是写锁优先的实现，一旦发生写锁申请，在当前写锁获取到申请或者超时之前(也就是存在写锁申请时),
		后发生的读锁都会被阻塞。如果写入过于频繁或者 即便不频繁但写入时间花费太久，都可能造成读锁饥饿
			该类不保证公平性，后到的锁申请可能会先获得，无论是读锁，还是写锁之间的公平都不提供。
		*/
		class SimpleReadWriteLock : public ReadWriteLock, private UnCopyable
		{
		private:
			enum class RWState
			{
				e_read,
				e_write,
				e_none
			};
			
			class LockImp : public Lock
			{
			public:
				virtual void lock() override;
				virtual bool lock( unsigned ms ) override;
				virtual void unlock() override;
				/*
					读写锁不支持该操作
					该方法只会抛出 logic_error
				*/
				virtual shared_ptr<Condition> newSharedPtrCondition() override;
				/*
					读写锁不支持该操作
					该方法只会抛出 logic_error
				*/
				virtual Condition* newCondition() override;

				typedef  RWState LockType;
				LockImp(SimpleReadWriteLock * readWriteLockImp, LockType lockType);
			private:
				LockType _lockType;
				SimpleReadWriteLock * _realLock;
			};
			
		public:
			explicit SimpleReadWriteLock();
			virtual Lock* getReadLock() override  { return _readLock.get(); }
			virtual Lock* getWriteLock() override { return _writeLock.get(); }
		private:

			void readLock();
			void writeLock();

			bool readLock(unsigned ms);
			bool writeLock( unsigned ms );

			void readUnlock();
			void writeUnlock();

		private:
			unsigned _readerCount;
			unsigned _writerCount;
			
			unsigned _writeTryCount;

			unique_ptr<Lock> _readLock;
			unique_ptr<Lock> _writeLock;

			shared_ptr<Lock> _realWriteLock;
			shared_ptr<Lock> _dataLock;
			shared_ptr<Condition> _dataLockCon;

		};
	};
};
