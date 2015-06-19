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
			����[frist,last)�����ݵ�[first,->)
			
			������һ��������
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
		����[frist,last)�����ݵ�[first,->)���������Fun() == true

		������һ��������
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
			ʹ��value����������
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