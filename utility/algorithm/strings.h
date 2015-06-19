#pragma once

#include <string>
using namespace std;

namespace utility
{
	namespace algorithm
	{
		/*
			字典顺序比较字符串，但是忽略大小写
		*/
		int strCompareIgnoreCase(const string& one, const string& two);
	};
};


