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
			�ҳ���Χ�ڣ���һ����value��ȵ�Ԫ��
		�Ҳ�������last
			Ҫ������֧�� == �����
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
			�ڸ�����Χ���ҵ�f(elemt) == true�ĵ�һ��Ԫ��
		�Ҳ�������last

			//ps:����Fn����ҪĬ�����ͣ������ɵ����߸�����Ҳ��������
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
			�ڸ�����Χ[first,last)�ڣ�Ѱ���Ƿ����Ԫ�أ���[first2,last2)�е�ĳ��Ԫ��ʹ��f(valueInRange1,valueInRange2) == true
		������ڣ����ص�һ�����ϸ������Ԫ�صĵ������������ڷ���last
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
			�ҵ���Χ�ڣ���������Ԫ����ȵ��������ȡ���ϸ�������ν��
		���ҷ���ָ�����е�һ��Ԫ�صĵ�����.
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
			�ڸ�������[first, last)���ж��Ƿ�����Ԫ�ض�ʹ�� f(value) == true,
		������ж����ϣ�����last, ������������ϣ����ص�һ�������ϵ�Ԫ�صĵ�����
		*/
		template< typename  ForwardIterator, typename Fun>
		ForwardIterator all_of(ForwardIterator first, ForwardIterator last, Fun f)
		{
			for (; first != last; ++first)
			{
				if (!f(*first))
				{
					return first;//���ص�һ�������ϵ�
				}
			}
			return last;
		}
		/*
			�ڸ�������[first, last)���ж��Ƿ����Ԫ�ض�ʹ�� f(value) == true,
		�������Ԫ�ظ��ϣ����ص�һ����Ԫ�صĵ�����, ����������ϣ�����last
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
		//	��ָ����Χ�ڣ��ҵ���һ��>=value��λ�ã�
		//�Ҳ�������last
		//	Ҫ����������,�õ��Ƕ��ַ�ʽ��������������������
		//��Ч�ʺܵͣ����������Ϊ����ʱ�临�Ӷ�
		//	Ĭ��Ҫ��Type ֧�� < ������
		//*/
		//template< typename Fun = less<>, typename ForwardIterator, typename Type >
		//ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const Type & value)
		//{
		//	ForwardIterator it;
		//	iterator_traits<ForwardIterator>::difference_type count, step;
		//	count = distance(first, last);//����
		//	Fun f;
		//	while (count > 0)//ʣ��ĸ���
		//	{
		//		it = first; step = count / 2; advance(it, step);//��itǰ������ǰfirst+ count/2�ĵط�
		//		if (f(*it, value)) //�����Ԫ��С��value,��ô���԰�ǰ���Ԫ��ȫ���ų���
		//		{
		//			first = ++it;//firstָ��first + count/2����һ��
		//			count -= step + 1;//�ų���������
		//		}
		//		else  //�����ֵ��С��value,�������Ԫ���ų��� 
		//			count = step;
		//	}
		//	return first;
		//}
		/*
			�ڸ�����Χ [first, last)�ڣ��ҵ���һ�������� f(*it, value) == true��Ԫ��
		��Ĭ�Ͼ��ǲ����� *it < value,����һ��>=value��Ԫ�أ��ĵ���������������㣬����last
			
			Ҫ���������򣬶��������ν����ú�����ν�ʵȼۡ�
			ʹ�ö��ֲ��ҷ�ʽ������������������ʹ�ø÷�����
			Ĭ������µ�����ָ�������Ҫ֧�� < ������
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
			�ڸ�����Χ [first, last)�ڣ��ҵ���һ�������� f(*it, value) == true��Ԫ��
		��Ĭ�Ͼ��ǲ����� *it <= value,����һ��>value��Ԫ�أ��ĵ���������������㣬����last
			��ϸ�� lower_bound
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
			��ָ����Χ�ڣ��ҵ���һ��==value��λ�ã�
		�Ҳ�������last
			Ҫ����������,�õ��Ƕ��ַ�ʽ��������������������
		��Ч�ʼ��ͣ����������Ϊ����ʱ�临�Ӷ�
			Ҫ��TypeĬ��֧�� < == >������������������Ӧ�еĶԳ���Ϊ
		*/
		template< typename Fun = less<void>, typename ForwardIterator>
		ForwardIterator binary_search(ForwardIterator first, ForwardIterator last, typename const iterator_traits<ForwardIterator>::value_type& value,Fun f = less<void>())
		{
			first = lower_bound<Fun>(first, last, value, f);
			
			if (first != last && *first == value)//��С�ڵĵ�һ������
			{
				return first;
			}
			else
			{
				return last;
			}
		}

		/*
			����[first, last)���Ԫ�ظ�����
		����������������ʱ�临�Ӷ�Ϊ������
		��������Ը��Ӷ�
		*/
		template<typename ForwardIterator>
		unsigned long count(ForwardIterator first, ForwardIterator last)
		{
			return (unsigned long) distance(first, last);
		}
		/*
			����[first, last)���Ԫ��  ����f(*it) == true������
		���Ը��Ӷ�
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
			��[first1, last1)������Ѱ�� ������[first2, last2)��
		������ƥ�䣬����[first1, last1)��ƥ�����е���ʼλ�ã�������
		ƥ�䷵��last1
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

