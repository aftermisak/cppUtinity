#ifndef _THREAD_H_
#define _THREAD_H_
#include <memory>
#include <functional>
#include "../UnCopyable.h"
#include "Runnable.h"
using namespace std;

namespace utility{
namespace concurrenct{
	/*
		Thread
		����ʹ�÷���
		1.���죬ʹ��std::function
		2.�̳в��Ҹ�����дrun����
		1�ṩ���Ǻ���ʽ��ʽ��2�ṩ���Ƕ���ʽ��ʽ
		��Ҫͬʱʹ�����ַ�ʽ��������ɴ�����Ϊ
	*/
	class Thread : public Runnable , private UnCopyable
	{
	private:
		bool m_isStarted;
		FunRunnable m_fRunnable;
		void invoke();
	protected:
		virtual void run() override { };
	public:
		//explicit Thread( shared_ptr<Runnable> runnable);����20150204
		/*
			����ʽ���죬����ʹ�÷�ʽ1
		*/
		explicit Thread(FunRunnable fRunnable);
		/*
			Ĭ�Ϲ��죬��������������,����ʹ�÷�ʽ2
		*/
		Thread();
		/*
			�����߳�
		*/
		virtual void start() final;
		/*
			�����̲߳��ȴ�ִ�н���
		*/
		virtual void join() final;
		virtual ~Thread();

	};
};
};



#endif // if