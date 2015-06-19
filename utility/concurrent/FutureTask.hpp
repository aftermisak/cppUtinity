#pragma once

#include "Future.h"
#include "LockAndConditionImp.h"
#include "Callable.h"
#include "../ConstsAndToolMethods.h"
namespace utility{
	namespace concurrenct{
		
		template<class T>  class FutureTask final : public RunnableFuture<T>
		{
		public:
			FutureTask(std::function<T()> fCallable);

			virtual void wait() override final;
			virtual bool wait( unsigned ms ) override final;
			virtual T get() override final;

			virtual void run() override final;
		private:
			enum class EState
			{
				e_non_begin,
				e_running,
				e_over
			};
			EState _state;
			shared_ptr<Lock> _lock;
			shared_ptr<Condition> _condition;
			std::function<T()> _fCallable;
			T _result;
		};

		template<class T>
		FutureTask<T>::FutureTask(std::function<T()> fCallable) :
			_state(EState::e_non_begin),
			_lock(new ReentrantLock()),
			_condition(_lock->newSharedPtrCondition()),
			_fCallable(fCallable)
		{

		}
		template<class T> 
		void  FutureTask<T>::wait()
		{
			LockGuard lg(*_lock);
			while( this->_state != EState::e_over)
			{
				_condition->wait();
			}
		}

		template<class T> 
		bool FutureTask<T>::wait( unsigned ms )
		{
			unsigned remainMs = ms;
			
			while( this->_state != EState::e_over)
			{
				unsigned msBeginWait = (unsigned)getApplicationMilliSeconds();
				if(!_condition->wait(ms))//超时
				{
					return false;
				}
				else//未超时，减去已经等待的事件
				{
					unsigned msWait = (unsigned)getApplicationMilliSeconds() - msBeginWait;
					if( msWait >= remainMs )//可能发生的情况，也判定为已经超时
					{
						return false;
					}
					else
					{
						remainMs -= msWait;
					}
				}
			}
			return true;
		}

		template<class T> 
		T FutureTask<T>::get()
		{
			this->wait();
			return _result;
		}

		template<class T> 
		void FutureTask<T>::run() throw()
		{
			{
				LockGuard lg(*_lock);
				this->_state = EState::e_running;
				this->_condition->notifyAll();
			}
			_result = _fCallable();
			{
				LockGuard lg(*_lock);
				this->_state = EState::e_over;
				this->_condition->notifyAll();
			}
		}

	};
};