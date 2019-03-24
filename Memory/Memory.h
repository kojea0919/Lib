#pragma once

#include <iostream>
#include "AllocInfo.h"


using namespace std;

#define MaxLen 20

int iCount;
AllocInfo AI_Arr[MaxLen];

int iTotalAllocSize;
int iTota      lAllocCount;

#define _MemoryAlloc(Type,Size) MemoryAlloc(new Type[Size],Size,__FILE__,__LINE__);

template<typename T>
T* MemoryAlloc(T * _ptr, int _iSize,char cName[], int _iLine)
{
	AI_Arr[iCount].m_ptr = _ptr;
	AI_Arr[iCount].m_iMemorySize = sizeof(T)*_iSize;
	strcpy(AI_Arr[iCount].m_cFileName, cName);
	AI_Arr[iCount].m_iFileLine = _iLine;

	iTotalAllocSize += AI_Arr[iCount].m_iMemorySize;
	++iTotalAllocCount;

	iCount++;

	return _ptr;
}

template<typename T>
void MemoryRelease(T * _ptr)
{
	if (_ptr == NULL)
		return;

	for (int iCnt = 0; iCnt < iCount; ++iCnt)
	{
		if (AI_Arr[iCnt].m_ptr == _ptr)
		{
			delete _ptr;
			--iCount;
			AI_Arr[iCnt] = AI_Arr[iCount];
			return;
		}
	}
}

void PrintAlloc()
{
	cout << "Total Alloc Size : " << iTotalAllocSize << endl;
	cout << "Total Alloc Count : " << iTotalAllocCount << endl;

	for (int iCnt = 0; iCnt < iCount; ++iCnt)
	{
		cout << "Not ReleaseMemory : " << AI_Arr[iCnt].m_ptr << " "
			<< AI_Arr[iCnt].m_iMemorySize << " Bytes" << endl;
		cout << "File : " << AI_Arr[iCnt].m_cFileName << " : " <<
			AI_Arr[iCnt].m_iFileLine << endl;
	}
}