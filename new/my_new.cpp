#include "my_new.h"
Cmy_new g_myNew;


void * operator new(size_t _size, char * _File, int _Line)
{
	void * newptr = malloc(_size);
	Cmy_new::AllocInfoSetting(newptr,_size, _File, _Line, false);
	return newptr;
}

void * operator new[](size_t _size, char * _File, int _Line)
{
	void * newptr = malloc(_size);
	Cmy_new::AllocInfoSetting(newptr, _size, _File, _Line, true);
	return newptr;
}

void operator delete(void * p)
{
	if (p == NULL)
	{
		return;
	}

	for (int iCnt = 0; iCnt < g_myNew._iCount; ++iCnt)
	{
		AllocInfo CurAl = g_myNew._AL_arr[iCnt];
		if (CurAl.m_ptr == p)
		{
			//new[]로 할당한 경우
			if (CurAl.m_bArrFlag)
			{
				std::ofstream outFile(g_myNew._FileName);
				outFile << "ARRAY  [" << CurAl.m_ptr << "] [" << CurAl.m_iMemorySize <<
					"] " << CurAl.m_cFileName << " " << CurAl.m_iFileLine << std::endl;
				outFile.close();
				return;
			}
			free(p);
			--g_myNew._iCount;
			g_myNew._AL_arr[iCnt] = g_myNew._AL_arr[g_myNew._iCount];
			return;
		}
	}

	std::ofstream outFile(g_myNew._FileName);
	outFile << "NOALLOC  [" << p  << "]" << std::endl;
	outFile.close();
}

void operator delete[](void * p)
{
	if (p == NULL)
	{
		return;
	}

	for (int iCnt = 0; iCnt < g_myNew._iCount; ++iCnt)
	{
		AllocInfo CurAl = g_myNew._AL_arr[iCnt];
		if (CurAl.m_ptr == p)
		{
			//new로 할당한 경우
			if (!CurAl.m_bArrFlag)
			{
				std::ofstream outFile(g_myNew._FileName);
				outFile << "ARRAY  [" << CurAl.m_ptr << "] [" << CurAl.m_iMemorySize <<
					"] " << CurAl.m_cFileName << " " << CurAl.m_iFileLine << std::endl;
				outFile.close();
				return;
			}
			free(p);
			--g_myNew._iCount;
			g_myNew._AL_arr[iCnt] = g_myNew._AL_arr[g_myNew._iCount];
			return;
		}
	}

	std::ofstream outFile(g_myNew._FileName);
	outFile << "NOALLOC  [" << p << "]" << std::endl;
	outFile.close();
}

void Cmy_new::AllocInfoSetting(void* _ptr,size_t _size, char * _File, int _Line, bool _Arr)
{
	AllocInfo newAlloc;
	newAlloc.m_ptr = _ptr;
	newAlloc.m_bArrFlag = _Arr;
	strcpy(newAlloc.m_cFileName, _File);
	newAlloc.m_iFileLine = _Line;
	newAlloc.m_iMemorySize = _size;

	g_myNew._AL_arr[g_myNew._iCount++] = newAlloc;
}

void Cmy_new::FileNameSetting()
{
	char Temp[dfFileName_Len] = "Alloc_";
	char Buffer[6];

	int iCur = 6;

	time_t T;
	struct tm TM;

	T = time(NULL);
	localtime_s(&TM, &T);

	_itoa(TM.tm_year + 1900, Buffer, 10);
	strcpy(Temp + iCur, Buffer);
	iCur += 4;

	if (TM.tm_mon + 1 < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itoa(TM.tm_mon + 1, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itoa(TM.tm_mon + 1, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	if (TM.tm_mday < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itoa(TM.tm_mday, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itoa(TM.tm_mday, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	Temp[iCur++] = '_';

	if (TM.tm_hour < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itoa(TM.tm_hour, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itoa(TM.tm_hour, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	if (TM.tm_min < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itoa(TM.tm_min, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itoa(TM.tm_min, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	if (TM.tm_sec < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itoa(TM.tm_sec, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itoa(TM.tm_sec, Buffer, 10);
		strcpy(Temp + iCur, Buffer);
		iCur += 2;
	}
	Temp[iCur++] = '.';
	Temp[iCur++] = 't';
	Temp[iCur++] = 'x';
	Temp[iCur++] = 't';
	Temp[iCur] = '\0';
	strcpy(_FileName, Temp);
}

void Cmy_new::operator delete(void * p, char * File, int Line)
{
}

void Cmy_new::operator delete[](void * p, char * File, int Line)
{
}