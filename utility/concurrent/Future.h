#pragma once
namespace utility{
	namespace concurrenct{
		//interface
		template<typename T>
		class Future
		{
		public:
			//等待至任务完成
			virtual void wait() = 0;

			//等待ms毫秒，任务完成返回true,超时返回false
			virtual bool wait( unsigned ms ) = 0;

			//等待获取结果
			virtual T get() = 0;

			virtual ~Future(){}
		};
		//interface
		template<typename T>
		class RunnableFuture : public Runnable, public Future<T>
		{
		public:
			virtual ~RunnableFuture(){}
		};
	};
};