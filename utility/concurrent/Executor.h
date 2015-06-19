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
		//�ύrunnable ����ʽ
		//virtual void execute( shared_ptr<Runnable> runnable ) = 0;

		//�ύrunable ����ʽ
		virtual void execute( FunRunnable f ) = 0;

		virtual ~Executor(){}
	};
	class ExecuteService : public Executor
	{
	public:
		//�����̳߳�
		virtual void begin() = 0;


		//�ر��̳߳أ��÷�������Ӱ��ȴ������е�����
		virtual void shutdown() = 0;



		//�ر��̳߳أ��÷����Ὣ�ȴ�������գ�ֻ�Ὣ��ǰ�������е�����ִ�����
		virtual void shutdownNow() = 0;



		//���̳߳عرգ����������ɣ����������߳��Ѿ��رգ�����true�����򣬷���false
		virtual bool isStoped() = 0;


		virtual ~ExecuteService(){}
	};
};
};
