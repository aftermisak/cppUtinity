#pragma once

#include <queue>
#include "../UnCopyable.h"
#include <memory>
#include <thread>
#include <mutex>
#include "Executor.h"
#include "Thread.h"
#include "Lock.h"
#include "Condition.h"
namespace utility{
	namespace concurrenct{
		class Executors
		{
		public:
			static shared_ptr<ExecuteService> sharedSimpleExecuteService(unsigned poolSize = 1){ return make_shared<SimpleExecuteService>(poolSize); }
			static ExecuteService* newSimpleExecuteService(unsigned poolSize = 1){ return new SimpleExecuteService(poolSize); }

		private:
			/*
				简易线程池，任务按照提交的顺序获得执行权。
			方法描述见ExecuteService
			*/
			class  SimpleExecuteService final : public ExecuteService , private UnCopyable
			{
			public:
				SimpleExecuteService(unsigned poolSize = 1);

				virtual void begin() override;
				//void execute(shared_ptr<Runnable> runnable) override;
				virtual void execute(FunRunnable f) override;
				void shutdown() override;
				void shutdownNow() override;
				virtual bool isStoped() override;
			private:
				void shutdown(bool  now = false);

				unsigned m_poolSize;
				bool m_isBegin;

				shared_ptr<Lock> m_mainLock;

				std::queue< FunRunnable > m_runnableQue;
				shared_ptr<Condition> m_queLockUniqueCondition;

				vector< unique_ptr<Thread> > m_threadVec;
				bool m_isShutDown;

				unsigned m_activeThreadNumber;
			private:
				class RunnableImp : public Runnable
				{
				public:
					RunnableImp(SimpleExecuteService * seSer) :_seSer(seSer){}
					virtual void run();
				private:
					SimpleExecuteService * _seSer;
				};

			};//end of class  SimpleExecuteService
		
		};

	};
};