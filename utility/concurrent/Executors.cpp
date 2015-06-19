#include "Executors.h"
#include "Thread.h"
#include <cassert>
#include "LockAndConditionImp.h"
#include "LockGuard.h"
using namespace utility::concurrenct;

Executors::SimpleExecuteService::SimpleExecuteService(unsigned poolSize) :
m_poolSize(poolSize),
m_activeThreadNumber(poolSize),
m_runnableQue(),
m_threadVec(),
m_isBegin(false),
m_isShutDown(false),
m_mainLock(shared_ptr<Lock>(new SimpleLock())),
m_queLockUniqueCondition(this->m_mainLock->newSharedPtrCondition())
{
	if (poolSize < 1)
		assert(false);
	m_threadVec.reserve(poolSize);
	for (unsigned i = 0; i < m_poolSize; i++)
	{
		shared_ptr<Runnable> ri = make_shared<RunnableImp>(this);
		m_threadVec.push_back(unique_ptr<Thread>(new Thread([ri]()->void{
			ri->run();
		})));
	}
}
void Executors::SimpleExecuteService::begin()
{

	if (m_isBegin)
	{
		return;
	}
	m_isBegin = true;
	for (unsigned i = 0; i < m_threadVec.size(); i++)
	{
		m_threadVec[i]->start();
	}

}
void Executors::SimpleExecuteService::RunnableImp::run()
{
	while (true)
	{
		{//����̳߳��Ѿ��رղ��Ҷ����Ѿ����ˣ��˳��߳�
			LockGuard lg(*this->_seSer->m_mainLock);
			if (_seSer->m_isShutDown && _seSer->m_runnableQue.size() == 0)
			{
				break;
			}
		}
		FunRunnable aRunnable = []()->void{};
		{//ͬ������
			LockGuard lg(*this->_seSer->m_mainLock);
			/*	�����û��������ȴ��������ѵĿ�����ֻ��һ�֣������������������С�
			��ʱ�Ļ���˵��û�������������У���ʹ��whilie-waitģ�ͣ���Ϊ��ÿ�ε���
			��ѭ��ȥ����̳߳��Ƿ�ر�
			*/
			if (_seSer->m_runnableQue.size() == 0)
			{
				this->_seSer->m_queLockUniqueCondition->wait(1000);
			}
			if (_seSer->m_runnableQue.size() > 0)
			{
				aRunnable = _seSer->m_runnableQue.front();
				_seSer->m_runnableQue.pop();
			}
		}
		//ִ��
		aRunnable();
	}

	LockGuard lg(*this->_seSer->m_mainLock);
	--_seSer->m_activeThreadNumber;//�߳̽�������¼
}


//void Executors::SimpleExecuteService::execute(shared_ptr<Runnable> runnable)
//{
//	this->execute([runnable]()->void{
//		runnable->run();
//	});
//}
void Executors::SimpleExecuteService::execute(FunRunnable f)
{
	LockGuard lg(*this->m_mainLock);
	if (m_isShutDown)
	{
		return;
	}
	m_runnableQue.push(f);
	this->m_queLockUniqueCondition->notifyAll();
}
void Executors::SimpleExecuteService::shutdown(bool  now)
{
	LockGuard lg(*this->m_mainLock);
	if (m_isShutDown)
	{
		return;
	}
	this->m_isShutDown = true;
	if (now)
	{
		while (this->m_runnableQue.size() > 0)
		{
			this->m_runnableQue.pop();
		}
	}
}

void Executors::SimpleExecuteService::shutdownNow()
{
	this->shutdown(true);
}
void Executors::SimpleExecuteService::shutdown()
{
	this->shutdown(false);
}

bool Executors::SimpleExecuteService::isStoped()
{
	LockGuard lg(*this->m_mainLock);
	if (m_activeThreadNumber == 0 && m_isShutDown)
	{
		return true;
	}
	return false;
}