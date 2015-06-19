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
			���[first2, last2)������Ԫ�أ���[first, last)�ж�����һ��Ԫ��ʹ��f(*iterInRange1, *iterInRange2) == true
		�򷵻�true�����򷵻�false
			Ĭ�������Ҫ��Ԫ��֧��==��������ʱ�ú�������Ϊ�����жϼ���2�Ƿ�Ϊ����1���Ӽ�
		*/
		template< typename ForwardIterator, typename ForwardIterator2, typename Fn = equal_to<void> >
		bool includes(ForwardIterator first, ForwardIterator last, ForwardIterator2 first2, ForwardIterator2 last2, Fn f = equal_to<>())
		{
			for (; first2 != last2; ++first2)
			{
				ForwardIterator it;
				for ( it = first; it != last; ++it)
				{
					if (f(*it, *first2))//�ҵ����ϣ�Ĭ��==���ľ��˳�ѭ��
						break;
				}
				if (it == last)//�������˵�������ѭ��û��һ�����ϵ�
					break;
			}
			return first2 == last2;
		}
		
		/*
			���������[first,last),[first2,last2)�鲢��[result, ->)��
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
			if (first != last)//��һ������ʣ��
			{
				for (; first != last;)
				{
					*result++ = *first++;
				}
			}
			else if (first2 != last2)//�ڶ�������ʣ��
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
			�����������ַ����Ƚϵķ�ʽ�Ƚ�������Χ
		�����Χһ��(Ĭ��ʹ��С��),С�ڷ�Χ2������
		true�����򷵻�false
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
			if (first == last && first2 != last2)//����һ���꣬���϶���û����
			{
				return true;
			}
			return false;
		}
		
		
	};//namespace algorithm
};//namespace utility