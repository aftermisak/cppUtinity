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
			�Ը������������Ԫ�ص���f non-member����
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
			�Ը������������Ԫ�ص���f member����
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