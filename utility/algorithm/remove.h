#pragma once
/************************************************************************/
/*
*/
/************************************************************************/
namespace utility
{
	namespace algorithm
	{
		template<typename ForwardIterator, typename Fn = equal_to<void>>
		ForwardIterator remove(ForwardIterator first, ForwardIterator last,
			typename const iterator_traits<ForwardIterator>::value_type& value,Fn f = equal_to<>())
		{
			ForwardIterator it = first;
			for (; first != last; ++first)
			{
				if (!f(*first, value))
				{
					*it++ = std::move(*first);//舍去数据 移动赋值
				}
			}
			return it;
		}
		
	};//namespace algorithm
};//namespace utility