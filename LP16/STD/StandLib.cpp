#include "stdafx.h"
#include <iostream>
#include <cstring>

extern "C"
{
	void _stdcall showstr(char * str)
	{
		setlocale(0, "");
		std::cout << str << std::endl;
	}

	void _stdcall showrc(int is)
	{
		if (is == 0)
			std::cout << "false" << std::endl;
		else
			std::cout << "true" << std::endl;
	}

	void _stdcall shownum(int intnum)
	{
		std::cout << intnum << std::endl;
	}
	int _stdcall arsum(int a, int d, int n)
	{
		int sum = 0;
		sum = n*(2 * a + (n - 1)*d) / 2;
		return sum;
	}

	int _stdcall geomsum(int a, int d, int n)
	{
		int sum = 1;
		sum = a*(1 - pow(d, n)) / (1 - d);
		return sum;
	}
	bool _stdcall issubstr(char* str, char* sub)
	{
		char *ptrstrstr = strstr(str, sub);
		if (ptrstrstr == NULL)
			return false;
		return true;
	}

	int _stdcall getstrlen(char* str)
	{
		return strlen(str);
	}

}
