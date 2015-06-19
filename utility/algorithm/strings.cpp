#include "strings.h"
#include <algorithm>
#include <cctype>
using namespace utility::algorithm;

int utility::algorithm::strCompareIgnoreCase(const string& one, const string& two)
{
	return std::lexicographical_compare(one.begin(), one.end(), two.begin(), two.end(), [](char a, char b)->bool
	{
		a = std::tolower(a);
		b = std::tolower(b);
		return a < b;
	});
}
