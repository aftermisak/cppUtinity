#include "LockAndConditionImp.h"
#include <cassert>
#include <stdexcept>
#include <exception>
#include <ctime>
#include "../ConstsAndToolMethods.h"
#include "LockGuard.h"
using namespace std;
using namespace utility::concurrenct;

/**************************SimpleLock Implement*************************************/
SimpleLock::SimpleLock():
	m_lock( this->m_mutex )
{
	m_lock.unlock();
}
SimpleLock::~SimpleLock()
{
	
}
void SimpleLock::lock() 
{
	m_lock.lock();
}

bool SimpleLock::lock( unsigned ms )
{
	return m_lock.try_lock_for( std::chrono::milliseconds( ms ) );
}

void SimpleLock::unlock() 
{
	m_lock.unlock();
}
shared_ptr<Condition> SimpleLock::newSharedPtrCondition()
{
	return shared_ptr<Condition>( new ConditionForSimpleLock( &this->m_lock ) );
}
Condition* SimpleLock::newCondition()
{
	return  new ConditionForSimpleLock(&this->m_lock);
}

/**************************ReentrantLock Implement*********************************/
ReentrantLock::ReentrantLock():
	m_lock( this->m_mutex )
{
	m_lock.unlock();
}
ReentrantLock::~ReentrantLock()
{
	
}
void ReentrantLock::lock() 
{
	m_lock.lock();
}

bool ReentrantLock::lock( unsigned ms )
{
	return m_lock.try_lock_for( std::chrono::milliseconds( ms ) );

}

void ReentrantLock::unlock() 
{
	m_lock.unlock();
}
shared_ptr<Condition> ReentrantLock::newSharedPtrCondition()
{
	return shared_ptr<Condition>( new ConditionForReentrantLock( &this->m_lock ) );
}
Condition* ReentrantLock::newCondition()
{
	return new ConditionForReentrantLock(&this->m_lock);
}
/**************************SimpleCondition Implement*************************************/
SimpleLock::ConditionForSimpleLock::ConditionForSimpleLock( std::unique_lock<std::timed_mutex>* lock ):
	m_lock( lock )
{

}
void SimpleLock::ConditionForSimpleLock::wait()
{
	m_condition.wait( *m_lock );
}
bool SimpleLock::ConditionForSimpleLock::wait(unsigned ms)
{
    
	return m_condition.wait_for(*m_lock, std::chrono::milliseconds(ms)) == std::cv_status::no_timeout;
}
void SimpleLock::ConditionForSimpleLock::notify() 
{
	m_condition.notify_one();
}
void SimpleLock::ConditionForSimpleLock::notifyAll()
{
	m_condition.notify_all();
}


/*****************************************ConditionForReentrantLock Implement***************/
ReentrantLock::ConditionForReentrantLock::ConditionForReentrantLock( std::unique_lock<std::recursive_timed_mutex>* lock ):
	m_lock( lock )
{

}

void ReentrantLock::ConditionForReentrantLock::wait()
{
	m_condition.wait( *m_lock );
}
bool ReentrantLock::ConditionForReentrantLock::wait(unsigned ms)
{
	return m_condition.wait_for(*m_lock, std::chrono::milliseconds(ms)) == std::cv_status::no_timeout;
}
void ReentrantLock::ConditionForReentrantLock::notify() 
{
	m_condition.notify_one();
}
void ReentrantLock::ConditionForReentrantLock::notifyAll()
{
	m_condition.notify_all();
}


/****************************************ReadWriteLockImp Implement*******************************************************************/

SimpleReadWriteLock::SimpleReadWriteLock():
	_readLock( new LockImp(this, LockImp::LockType::e_read) ),
	_writeLock( new LockImp(this, LockImp::LockType::e_write) ),
	//_rwState( RWState::e_none ),
	_readerCount( 0 ),
	_writerCount( 0 ),
	_realWriteLock( new ReentrantLock() ),
	_dataLock( new ReentrantLock() ),
	_dataLockCon( _dataLock->newSharedPtrCondition() ),
	_writeTryCount( 0 )
{

}
void SimpleReadWriteLock::readLock()
{
	bool isFirstReaderNow = false;
	{
		LockGuard lg(*_dataLock);
		while( _writerCount > 0 || _writeTryCount > 0)//如果写着数量大于0，或者有写等待 让路等待
		{
			_dataLockCon->wait();
		}
		bool isFirstReaderNow = 0 == _readerCount++;  //当前是不是第一个读者
	}//_dataLock在此解锁

	if( isFirstReaderNow )//如果是第一个读者，需要将写锁锁上
	{
		_readLock->lock(); //将写锁锁定,致使不能获取写锁
	}
	
}
void SimpleReadWriteLock::writeLock()
{
	{
		LockGuard lg(*_dataLock);//数据锁定
		++_writeTryCount;      //写等待数量+1
	}
	_realWriteLock->lock(); //加锁

	{
		LockGuard lg(*_dataLock);;//数据锁定
		--_writeTryCount;  //写等待数据-1
		++_writerCount;  //写入者数量记录+1
		//_dataLockCon->notifyAll();//唤醒可能等待的读线程
		//_dataLockCon->notify();
	}
}


bool SimpleReadWriteLock::readLock(unsigned ms)
{
	unsigned remainMs = ms;
	unsigned msBeginWait = (unsigned)getApplicationMilliSeconds();
	LockGuard lg(*_dataLock);   //读写锁的数据锁定
	while( _writerCount > 0 || _writeTryCount > 0 )//如果写着数量大于0  或者写尝试数量大于0，让锁等待
	{
		if(!_dataLockCon->wait( remainMs )) //超时
		{
			return false;
		}
		else//未超时，但是需要减去等待了多久时间，以便继续等待的使用
		{
			
			unsigned msWait = (unsigned)getApplicationMilliSeconds() - msBeginWait;
			if( msWait >= remainMs )//可能发生的情况，也判定为已经超时
			{
				return false;
			}
			else
			{
				remainMs -= msWait;
			}
		}
	}
	if( 0 == _readerCount++ )//读人数记录,并且如果是第一个读者，就把写锁加上
	{
		_readLock->lock(); //将写锁锁定,致使不能获取写锁
	}
	return true;
}
bool SimpleReadWriteLock::writeLock( unsigned ms )
{
	{
		LockGuard lg(*_dataLock);
		++_writeTryCount;
	}
	bool result = _realWriteLock->lock( ms );

	{//是否等待成功都需要将写尝试数递减
		LockGuard lg(*_dataLock);
		--_writeTryCount;
		if( result )//加锁申请成功
		{
			++_writerCount;  //写入人数记录
		}
		else
		{
			_dataLockCon->notifyAll();  //只有没加锁成功的情况，才有必要唤醒等待
		}
	}
	return result;
}

void SimpleReadWriteLock::readUnlock()
{
	bool isLastReader = false;

	{
		LockGuard lg(*_dataLock);//读写锁的数据锁定
		isLastReader = 0 ==  --_readerCount;
	}//
	if( isLastReader ) //如果最后一个读者离开，需要将写锁解锁,致使写锁可以被获取
	{
		_realWriteLock->unlock();
	}
}
void SimpleReadWriteLock::writeUnlock()
{
	{
		LockGuard lg(*_dataLock);;//读写锁的数据锁定
		--_writerCount;//记录减少
		_dataLockCon->notifyAll();//唤醒可能等待的线程
	}//

	_realWriteLock->unlock();//解锁
	
}
SimpleReadWriteLock::LockImp::LockImp( SimpleReadWriteLock * readWriteLockImp ,LockType lockType):
_lockType( lockType ),
_realLock( readWriteLockImp )
{

}
void SimpleReadWriteLock::LockImp::lock()
{
	if(_lockType == LockType::e_read)
	{
		_realLock->readLock();
	}
	else
	{
		_realLock->writeLock();
	}
}
bool SimpleReadWriteLock::LockImp::lock( unsigned ms )
{
	if(_lockType == LockType::e_read)
	{
		return _realLock->readLock( ms );
	}
	else
	{
		return _realLock->writeLock( ms );
	}
}
void SimpleReadWriteLock::LockImp::unlock()
{
	if(_lockType == LockType::e_read)
	{
		_realLock->readUnlock();
	}
	else
	{
		_realLock->writeUnlock();
	}
}
shared_ptr<Condition> SimpleReadWriteLock::LockImp::newSharedPtrCondition()
{
	throw logic_error("not support");
}
Condition* SimpleReadWriteLock::LockImp::newCondition()
{
	throw logic_error("not support");
}