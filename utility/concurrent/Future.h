#pragma once
namespace utility{
	namespace concurrenct{
		//interface
		template<typename T>
		class Future
		{
		public:
			//�ȴ����������
			virtual void wait() = 0;

			//�ȴ�ms���룬������ɷ���true,��ʱ����false
			virtual bool wait( unsigned ms ) = 0;

			//�ȴ���ȡ���
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