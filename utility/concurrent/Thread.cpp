#include "Thread.h"
#include <functional>
#include <thread>
#include <stdexcept>
using namespace utility::concurrenct;


//Thread::Thread( shared_ptr<Runnable> runnable ):
//	Thread([runnable]() -> void{
//		runnable->run();})
//{
//
//}
Thread::Thread(FunRunnable fRunnable):
	m_fRunnable( fRunnable ),
	m_isStarted( false )
{

}
Thread::Thread():
Thread([](){})//Î¯ÍÐ¹¹Ôì
{

}

Thread::~Thread()
{

}

void Thread::start()
{
	if( !m_isStarted )
	{
		m_isStarted = true;
		thread( std::bind(&Thread::invoke, this) ).detach();
	}
	else
	{
		//can not start again
	}
}
void Thread::join()
{
	if( !m_isStarted )
	{
		m_isStarted = true;
		thread( std::bind(&Thread::invoke, this) ).join();
	}
	else
	{
		//can not start again
	}
}

void Thread::invoke()
{
	this->run();
	m_fRunnable();
}