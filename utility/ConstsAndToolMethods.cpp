#include "ConstsAndToolMethods.h"
#include <ctime>

double getApplicationSeconds()
{
	return std::clock() * 1.0 / CLOCKS_PER_SEC;
}

double getApplicationMilliSeconds()
{
	return getApplicationSeconds() * 1000.0;
}