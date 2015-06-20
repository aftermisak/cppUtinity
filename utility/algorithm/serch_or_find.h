#pragma  once
#include <functional>
/************************************************************************/
/* 
*/
/************************************************************************/
namespace utility
{
	namespace algorithm
	{
		/*
			找出范围内，第一个与value相等的元素
		找不到返回last
			要求类型支持 == 运算符
		*/
		template< typename ForwardIterator >
		ForwardIterator find(ForwardIterator first, ForwardIterator last, typename const iterator_traits<ForwardIterator>::value_type& value
			/*typename add_lvalue_reference< typename add_const< typename remove_reference<decltype(*first)>::type >::type >::type value*/)
		{
			for (; first != last; ++first)
			{
				if (value == *first)
				{
					return first;
				}
			}
			return first;
		}

		/*
			在给定范围内找到f(elemt) == true的第一个元素
		找不到返回last

			//ps:这里Fn不需要默认类型，必须由调用者给出，也才有意义
		*/
		template< typename ForwardIterator, typename Fn>
		ForwardIterator find(ForwardIterator first, ForwardIterator last, Fn f)
		{
			for (; first != last; ++first)
			{
				if (f(*first))
				{
					return first;
				}
			}
			return first;
		}
		/*
			在给定范围[first,last)内，寻找是否存在元素，与[first2,last2)中的某个元素使得f(valueInRange1,valueInRange2) == true
		如果存在，返回第一个符合该情况的元素的迭代器，不存在返回last
		*/
		template< typename ForwardIterator , typename ForwardIterator2, typename Fn = equal_to<void> >
		ForwardIterator find(ForwardIterator first, ForwardIterator last, ForwardIterator2 first2, ForwardIterator2 last2, Fn f = equal_to<>())
		{
			for (; first != last; ++first)
			{
				ForwardIterator2 first2_copy = first2;
				
				for (; first2_copy != last2; ++first2_copy)
				{
					if (f(*first, *first2_copy))
					{
						break;
					}
				}
				if (first2_copy != last2)
				{
					return first;
				}
			}
			return first;
		}

		/*
			找到范围内，连续两个元素相等的情况，获取符合给定函数谓词
		并且返回指向其中第一个元素的迭代器.
		*/
		template<typename ForwardIterator, typename Fn = equal_to<>>
		ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, Fn f = equal_to<>())
		{
			if (first == last)
				return last;

			ForwardIterator one = first;
			ForwardIterator two = first; 
			++two;
			for (; two != last; ++one, ++two)
			{
				if (f(*one, *two))
				{
					return one;
				}
			}
			return last;
		}
		/*
			在给定区间[first, last)中判断是否所有元素都使得 f(value) == true,
		如果所有都复合，返回last, 如果并不都复合，返回第一个不符合的元素的迭代器
		*/
		template< typename  ForwardIterator, typename Fun>
		ForwardIterator all_of(ForwardIterator first, ForwardIterator last, Fun f)
		{
			for (; first != last; ++first)
			{
				if (!f(*first))
				{
					return first;//返回第一个不符合的
				}
			}
			return last;
		}
		/*
			在给定区间[first, last)中判断是否存在元素都使得 f(value) == true,
		如果存在元素复合，返回第一个该元素的迭代器, 如果都不复合，返回last
		*/
		template< typename  ForwardIterator, typename Fun>
		ForwardIterator any_of(ForwardIterator first, ForwardIterator last, Fun f)
		{
			for (; first != last; ++first)
			{
				if (f(*first))
				{
					return true; return first;
				}
			}
			return false; return last;
		}

		///*
		//	在指定范围内，找到第一个>=value的位置，
		//找不到返回last
		//	要求序列有序,用的是二分方式，如果不是随机迭代器，
		//其效率很低，随机迭代器为对数时间复杂度
		//	默认要求Type 支持 < 操作符
		//*/
		//template< typename Fun = less<>, typename ForwardIterator, typename Type >
		//ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const Type & value)
		//{
		//	ForwardIterator it;
		//	iterator_traits<ForwardIterator>::difference_type count, step;
		//	count = distance(first, last);//个数
		//	Fun f;
		//	while (count > 0)//剩余的个数
		//	{
		//		it = first; step = count / 2; advance(it, step);//让it前进到当前first+ count/2的地方
		//		if (f(*it, value)) //如果该元素小于value,那么可以把前面的元素全部排除掉
		//		{
		//			first = ++it;//first指向first + count/2的下一个
		//			count -= step + 1;//排除掉的数量
		//		}
		//		else  //如果该值不小于value,将后面的元素排除掉 
		//			count = step;
		//	}
		//	return first;
		//}
		/*
			在给定范围 [first, last)内，找到第一个不满足 f(*it, value) == true的元素
		（默认就是不满足 *it < value,即第一个>=value的元素）的迭代器，如果都满足，返回last
			
			要求序列有序，而且排序的谓词与该函数的谓词等价。
			使用二分查找方式，不建议非随机迭代器使用该方法。
			默认情况下迭代器指向的类型要支持 < 操作符
		*/
		template< typename Fun = less<>, typename ForwardIterator>
		ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, typename const iterator_traits<ForwardIterator>::value_type& value,Fun f = less<>())
		{
			while( first != last )
			{
				typename iterator_traits<ForwardIterator>::difference_type step = distance(first, last) / 2;
				ForwardIterator mid = first; advance(mid, step);
				if(f(*mid , value))
				{
					advance(mid, 1);
					first = mid;
				}
				else
				{
					last = mid;
				}
			}
			return first;//or last
		}
		/*
			在给定范围 [first, last)内，找到第一个不满足 f(*it, value) == true的元素
		（默认就是不满足 *it <= value,即第一个>value的元素）的迭代器，如果都满足，返回last
			详细见 lower_bound
		*/
		template< typename Fun = less_equal<>, typename ForwardIterator>
		ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, typename const iterator_traits<ForwardIterator>::value_type& value, Fun f = less_equal<>())
		{
			//while (first != last)
			//{
			//	typename iterator_traits<ForwardIterator>::difference_type step = distance(first, last) / 2;
			//	ForwardIterator mid = first; advance(mid, step);
			//	if ( f(*mid, value) )
			//	{
			//		first = mid + 1;
			//	}
			//	else
			//	{
			//		last = mid;
			//	}
			//}
			//return first;//or last
			return lower_bound(first, last, value, less_equal<>());//simple implement
		}
		/*
			在指定范围内，找到第一个==value的位置，
		找不到返回last
			要求序列有序,用的是二分方式，如果不是随机迭代器，
		其效率极低，随机迭代器为对数时间复杂度
			要求Type默认支持 < == >三个操作符，并且有应有的对称行为
		*/
		template< typename Fun = less<void>, typename ForwardIterator>
		ForwardIterator binary_search(ForwardIterator first, ForwardIterator last, typename const iterator_traits<ForwardIterator>::value_type& value,Fun f = less<void>())
		{
			first = lower_bound<Fun>(first, last, value, f);
			
			if (first != last && *first == value)//不小于的第一个等于
			{
				return first;
			}
			else
			{
				return last;
			}
		}

		/*
			返回[first, last)间的元素个数。
		如果是随机迭代器，时间复杂度为常量，
		否则非线性复杂度
		*/
		template<typename ForwardIterator>
		unsigned long count(ForwardIterator first, ForwardIterator last)
		{
			return (unsigned long) distance(first, last);
		}
		/*
			返回[first, last)间的元素  满足f(*it) == true个数。
		线性复杂度
		*/
		template<typename ForwardIterator, typename Fn >
		unsigned long count(ForwardIterator first, ForwardIterator last, Fn f)
		{
			unsigned long ret = 0;
			while (first != last)
			{
				if (f(*first++))
				{
					++ret;
				}
			}
			return ret;
		}
		/*
			在[first1, last1)序列中寻找 子序列[first2, last2)，
		若存在匹配，返回[first1, last1)中匹配序列的起始位置，不存在
		匹配返回last1
		*/
		template <class ForwardIterator1, class ForwardIterator2, class Fn = equal_to<>>
		ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2, ForwardIterator2 last2,
			Fn f = equal_to<>())
		{
			for (; first1 !+ last1; ++first1)
			{
				ForwardIterator1 first1_copy = first1;
				ForwardIterator2 first2_copy = first2;
				
				while (*first1_copy++ == *first2_copy++)
				{
					if (first2_copy == last2) return first1_copy;
					if (first1_copy == last1) return last1;
				}
			}
		}
	};
};

