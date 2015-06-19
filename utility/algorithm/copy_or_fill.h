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
			拷贝[frist,last)的数据到[first,->)
			
			返回下一个迭代器
		*/
		template<typename ForwardIteratorInput, typename ForwardIteratorOutput>
		ForwardIteratorOutput copy(ForwardIteratorInput first, ForwardIteratorInput last, ForwardIteratorOutput first2)
		{
			while (first != last)
			{
				*first2++ = *first++;
			}
			return first2;
		}

		/*
		拷贝[frist,last)的数据到[first,->)，如果满足Fun() == true

		返回下一个迭代器
		*/
		template<typename Fun, typename ForwardIteratorInput, typename ForwardIteratorOutput>
		ForwardIteratorOutput copy(ForwardIteratorInput first, ForwardIteratorInput last, ForwardIteratorOutput first2, Fun f)
		{
			while (first != last)
			{
				if (f(*first))
				{
					*first2++ = *first;
				}
				++first;
			}
			return first2;
		}
		/*
			使用value填充给定区间
		*/
		template<typename ForwardIteratorOutput>
		void fill(ForwardIteratorOutput first, ForwardIteratorOutput last, typename const  iterator_traits<ForwardIteratorOutput>::value_type& value)
		{
			while (first != last)
			{
				*first++ = value;
			}
		}

	};//namespace algorithm
};//namespace utility