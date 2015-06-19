#pragma once
#include <new>
#include <memory>
using namespace std;
namespace utility
{
	namespace memory
	{
		template<typename T>
		class NewHandlerBase
		{
		public:
			
			static void*  operator new (std::size_t size) throw(std::bad_alloc)
			{
				std::new_handler oldOne = std::set_new_handler(selfNewHandler);
				std::unique_ptr<void, std::function<void(void*)>> guard(nullptr, [oldOne](void*)->void
				{
					std::set_new_handler(oldOne);
				});
				return ::operator new(size);
				
			}
			
			static void selfNewHandler()
			{
				int i = 0;
				return;
			}
		};
	};
};
