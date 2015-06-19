#pragma once
/**
	don't use any class in this file, this is some testCode
*/
#include <memory>
using namespace std;
#include "Runnable.h"
namespace utility{
namespace concurrenct{
	class  Executor
	{
	public:
		//提交runnable 对象式
		//virtual void execute( shared_ptr<Runnable> runnable ) = 0;

		//提交runable 函数式
		virtual void execute( FunRunnable f ) = 0;

		virtual ~Executor(){}
	};
	class ExecuteService : public Executor
	{
	public:
		//开启线程池
		virtual void begin() = 0;


		//关闭线程池，该方法不会影响等待队列中的任务
		virtual void shutdown() = 0;



		//关闭线程池，该方法会将等待队列清空，只会将当前正在运行的任务执行完成
		virtual void shutdownNow() = 0;



		//若线程池关闭，任务队列完成，并且所有线程已经关闭，返回true，否则，返回false
		virtual bool isStoped() = 0;


		virtual ~ExecuteService(){}
	};
};
};
