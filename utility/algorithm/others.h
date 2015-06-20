#pragma  once
/************************************************************************/
/* 
*/
/************************************************************************/
namespace utility
{
	namespace algorithm
	{
		/*
			对给定区间的所有元素调用f non-member函数
		*/
		template<typename ForwardIterator, typename Fn, typename... Args>
		Fn for_each(ForwardIterator first, ForwardIterator last, Fn f, Args&&... args)
		{
			for (; first != last; ++first)
			{
				f(*first,std::forward<Args>(args)...);
			}
			return f;
		}
		/*
			对给定区间的所有元素调用f member函数
		*/
		template<typename ForwardIterator, typename Fn, typename... Args>
		Fn for_each_call_mem(ForwardIterator first, ForwardIterator last, Fn f, Args&&... args)
		{
			for (; first != last; ++first)
			{
				(*(first).*f)(std::forward<Args>(args)...);
			}
			return f;
		}
	};//namespace algorithm
};//namespace utility