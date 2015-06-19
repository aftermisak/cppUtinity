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
	return m_condition.wait_for(*m_lock, std::chrono::milliseconds(ms)) == std::_Cv_status::no_timeout;
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
	return m_condition.wait_for(*m_lock, std::chrono::milliseconds(ms)) == std::_Cv_status::no_timeout;
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
		while( _writerCount > 0 || _writeTryCount > 0)//���д����������0��������д�ȴ� ��·�ȴ�
		{
			_dataLockCon->wait();
		}
		bool isFirstReaderNow = 0 == _readerCount++;  //��ǰ�ǲ��ǵ�һ������
	}//_dataLock�ڴ˽���

	if( isFirstReaderNow )//����ǵ�һ�����ߣ���Ҫ��д������
	{
		_readLock->lock(); //��д������,��ʹ���ܻ�ȡд��
	}
	
}
void SimpleReadWriteLock::writeLock()
{
	{
		LockGuard lg(*_dataLock);//��������
		++_writeTryCount;      //д�ȴ�����+1
	}
	_realWriteLock->lock(); //����

	{
		LockGuard lg(*_dataLock);;//��������
		--_writeTryCount;  //д�ȴ�����-1
		++_writerCount;  //д����������¼+1
		//_dataLockCon->notifyAll();//���ѿ��ܵȴ��Ķ��߳�
		//_dataLockCon->notify();
	}
}


bool SimpleReadWriteLock::readLock(unsigned ms)
{
	unsigned remainMs = ms;
	unsigned msBeginWait = (unsigned)getApplicationMilliSeconds();
	LockGuard lg(*_dataLock);   //��д������������
	while( _writerCount > 0 || _writeTryCount > 0 )//���д����������0  ����д������������0�������ȴ�
	{
		if(!_dataLockCon->wait( remainMs )) //��ʱ
		{
			return false;
		}
		else//δ��ʱ��������Ҫ��ȥ�ȴ��˶��ʱ�䣬�Ա�����ȴ���ʹ��
		{
			
			unsigned msWait = (unsigned)getApplicationMilliSeconds() - msBeginWait;
			if( msWait >= remainMs )//���ܷ����������Ҳ�ж�Ϊ�Ѿ���ʱ
			{
				return false;
			}
			else
			{
				remainMs -= msWait;
			}
		}
	}
	if( 0 == _readerCount++ )//��������¼,��������ǵ�һ�����ߣ��Ͱ�д������
	{
		_readLock->lock(); //��д������,��ʹ���ܻ�ȡд��
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

	{//�Ƿ�ȴ��ɹ�����Ҫ��д�������ݼ�
		LockGuard lg(*_dataLock);
		--_writeTryCount;
		if( result )//��������ɹ�
		{
			++_writerCount;  //д��������¼
		}
		else
		{
			_dataLockCon->notifyAll();  //ֻ��û�����ɹ�����������б�Ҫ���ѵȴ�
		}
	}
	return result;
}

void SimpleReadWriteLock::readUnlock()
{
	bool isLastReader = false;

	{
		LockGuard lg(*_dataLock);//��д������������
		isLastReader = 0 ==  --_readerCount;
	}//
	if( isLastReader ) //������һ�������뿪����Ҫ��д������,��ʹд�����Ա���ȡ
	{
		_realWriteLock->unlock();
	}
}
void SimpleReadWriteLock::writeUnlock()
{
	{
		LockGuard lg(*_dataLock);;//��д������������
		--_writerCount;//��¼����
		_dataLockCon->notifyAll();//���ѿ��ܵȴ����߳�
	}//

	_realWriteLock->unlock();//����
	
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