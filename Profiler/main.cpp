//#define ProfileMode
#include <iostream>
#include <Windows.h>
#include "Profiler.h"
#include <crtdbg.h>
using namespace std;


#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)


int wmain(int argc, WCHAR * argv[])
{
	while (1)
	{
		Profile_Begin(L"test1");
		wcout << L"test" << endl;
		Profile_Begin(L"test1");

		Profile_End(L"test1");

		Profile_Begin(L"test1");
		for (int i = 0; i < 1000; ++i)
			for (int i = 0; i < 1000; ++i);
		Profile_End(L"test1");


		Profile_Begin(L"test1");
		for (int i = 0; i < 10000; ++i)
			for (int i = 0; i < 10000; ++i);
		Profile_End(L"test1");


		Profile_Begin(L"test1");
		wcout << L"test" << endl;
		wcout << L"test" << endl;
		wcout << L"test" << endl;
		wcout << L"test" << endl;
		Profile_End(L"test1");

	}
	return 0;
}