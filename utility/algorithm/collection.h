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
			如果[first2, last2)的任意元素，在[first, last)中都存在一个元素使得f(*iterInRange1, *iterInRange2) == true
		则返回true，否则返回false
			默认情况下要求元素支持==操作，此时该函数的行为就是判断集合2是否为集合1的子集
		*/
		template< typename ForwardIterator, typename ForwardIterator2, typename Fn = equal_to<void> >
		bool includes(ForwardIterator first, ForwardIterator last, ForwardIterator2 first2, ForwardIterator2 last2, Fn f = equal_to<>())
		{
			for (; first2 != last2; ++first2)
			{
				ForwardIterator it;
				for ( it = first; it != last; ++it)
				{
					if (f(*it, *first2))//找到符合（默认==）的就退出循环
						break;
				}
				if (it == last)//如果成立说明上面的循环没有一个符合的
					break;
			}
			return first2 == last2;
		}
		
		/*
			将已排序的[first,last),[first2,last2)归并到[result, ->)中
		*/
		template< typename ForwardIterator, typename ForwardIterator2, typename ForwardIteratorOutput, typename typename Fn = less<void>>
		void merge(ForwardIterator first, ForwardIterator last, ForwardIterator2 first2, ForwardIterator2 last2, ForwardIteratorOutput result, Fn f = less<void>())
		{
			for (; first != last && first2 != last2;)
			{
				if (f(*first, *first2))
				{
					*result++ = *first++;
				}
				else
				{
					*result++ = *first2++;
				}
			}
			if (first != last)//第一部分有剩余
			{
				for (; first != last;)
				{
					*result++ = *first++;
				}
			}
			else if (first2 != last2)//第二部分有剩余
			{
				for (; first2 != last2;)
				{
					*result++ = *first2++;
				}
			}
			else
			{
				assert(false);//imposible here 
			}
		}
		/*
			按照类似于字符串比较的方式比较两个范围
		如果范围一的(默认使用小于),小于范围2，返回
		true，否则返回false
		*/
		template< typename ForwardIterator, typename ForwardIterator2, typename ForwardIteratorOutput, typename typename Fn = less<void>>
		bool lexicographical_compare(ForwardIterator first, ForwardIterator last, ForwardIterator2 first2, ForwardIterator2 last2, ForwardIteratorOutput result, Fn f = less<void>())
		{
			for (first != last && first2 != last2)
			{
				if (f(*first++, *first2++))
				{
					return true;
				}
			}
			if (first == last && first2 != last2)//集合一比完，集合二还没有完
			{
				return true;
			}
			return false;
		}
		
		
	};//namespace algorithm
};//namespace utility