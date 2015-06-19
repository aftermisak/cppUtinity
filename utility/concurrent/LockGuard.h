#pragma once
#include "../UnNewable.h"
#include "../UnCopyable.h"
#include "Lock.h"

namespace utility{
	namespace concurrenct{
		class LockGuard : public UnNewable, public UnCopyable
		{
		public:
			LockGuard(Lock& aLock) :_lock(aLock){ _lock.lock(); }
			~LockGuard(){ _lock.unlock(); }
		private:
			Lock& _lock;
		};
	}
};