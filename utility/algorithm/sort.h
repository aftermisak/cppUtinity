#pragma once
/************************************************************************/
/*
*/
/************************************************************************/
namespace utility
{
	namespace algorithm
	{
		template<typename RandomAccessIter, typename Fn = less<void>>
		void nth_element(RandomAccessIter first, RandomAccessIter inrange, RandomAccessIter last, Fn f = less<>())
		{
			std::nth_element(first, inrange, last, f);
		}
	};//namespace algorithm
};//namespace utility