#pragma once
#include <functional>
using namespace std;

namespace utility{
	namespace concurrenct{
		class Runnable
		{
		public:
			/*
				运行runable
				@exception no throw
			*/
			virtual void run() = 0;
			virtual ~Runnable(){}
		};

		typedef std::function<void()> FunRunnable;
	};
};