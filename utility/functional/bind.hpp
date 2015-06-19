#pragma  once

namespace utility
{
	namespace functional
	{
		//test code invoker,  for prepare to bind

		template <typename Fn, typename... Args>
		auto invoke(Fn f, Args&&... args) -> decltype(f(std::forward<Args>(args)...))
		{
			return f(std::forward<Args>(args)...);
		}

		template<typename Fn, typename T, typename... Args>
		auto invoke(Fn f, T* obj, Args&&... args) ->decltype((obj->*f)(std::forward<Args>(args)...))
		{
			return (obj->*f)(std::forward<Args>(args)...);
		}

		template<typename Fn, typename T, typename... Args>
		auto invoke(Fn f, T& obj, Args&&... args) ->decltype((obj.*f)(std::forward<Args>(args)...))
		{
			return (obj.*f)(std::forward<Args>(args)...);
		}

		/**********************************************bind**********************************************************************/
		//template< typename Fn, typename... Args >
		//auto bind(Fn f, Args&&... args) -> std::function< decltype(f(std::forward<Args>(args)...))() >
		//{
		//	return [f, ]()->decltype(f(std::forward<Args>(args)...))
		//	{
		//		return f(std::forward<Args>(args)...);
		//	};
		//}

	};
};