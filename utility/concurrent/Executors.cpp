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
		{//如果线程池已经关闭并且队列已经空了，退出线程
			LockGuard lg(*this->_seSer->m_mainLock);
			if (_seSer->m_isShutDown && _seSer->m_runnableQue.size() == 0)
			{
				break;
			}
		}
		FunRunnable aRunnable = []()->void{};
		{//同步区域
			LockGuard lg(*this->_seSer->m_mainLock);
			/*	如果中没有任务，则等待，被唤醒的可能性只有一种，就是有新任务加入队列。
			超时的话，说明没有新任务加入队列，不使用whilie-wait模型，是为了每次到外
			层循环去检查线程池是否关闭
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
		//执行
		aRunnable();
	}

	LockGuard lg(*this->_seSer->m_mainLock);
	--_seSer->m_activeThreadNumber;//线程结束，记录
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