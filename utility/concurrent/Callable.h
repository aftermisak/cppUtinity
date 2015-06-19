#pragma once


#include <functional>

namespace utility{
	namespace concurrenct{

		template<typename R>
		class Callable 
		{
		public:
			virtual R call() = 0;
			virtual ~Callable(){}
		};
	};
};