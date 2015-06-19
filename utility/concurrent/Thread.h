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
		两种使用方案
		1.构造，使用std::function
		2.继承并且覆盖重写run方法
		1提供的是函数式方式，2提供的是对象式方式
		不要同时使用两种方式，否则将造成错误行为
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
		//explicit Thread( shared_ptr<Runnable> runnable);废弃20150204
		/*
			函数式构造，用于使用方式1
		*/
		explicit Thread(FunRunnable fRunnable);
		/*
			默认构造，让子类正常构造,用于使用方式2
		*/
		Thread();
		/*
			开启线程
		*/
		virtual void start() final;
		/*
			开启线程并等待执行结束
		*/
		virtual void join() final;
		virtual ~Thread();

	};
};
};



#endif // if