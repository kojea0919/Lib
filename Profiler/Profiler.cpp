#define _CRT_SECURE_NO_WARNINGS
#include "Profiler.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstdlib>

#define df_MAXSAMPLE 100
#define df_FileNameLen 128

void SetFileName();

typedef struct
{
	//long			_IFlag;			//프로파일의 사용 여부.

	WCHAR			_szName[64];	//샘플 이름

	LARGE_INTEGER	_liStartTime;	//샘플 실행 시간.

	__int64			_iTotalTime;	//샘플 전체 사용시간 카운터
	__int64			_iMax[2];		//최대 사용시간 카운터
	__int64			_iMin[2];		//최소 사용시간 카운터

	__int64			_iCall;			//누적 호출 횟수.

} PROFILE_SAMPLE;


class CProFile
{
public:
	PROFILE_SAMPLE *		_SAMPLE_ARR[df_MAXSAMPLE];
	int						_iCount;
	LARGE_INTEGER			_liFreq;
	WCHAR					_FileName[df_FileNameLen];


	CProFile() 
		:_iCount(0)
	{
		QueryPerformanceFrequency(&_liFreq);
		SetFileName();
	}
	~CProFile() 
	{
		for (int iCnt = 0; iCnt < _iCount; ++iCnt)
		{
			delete _SAMPLE_ARR[iCnt];
		}
	}
};

CProFile gProFile;

PROFILE_SAMPLE* FindSample(WCHAR * szName)
{
	PROFILE_SAMPLE * TargetSample;
	for (int iCnt = 0; iCnt < gProFile._iCount; ++iCnt)
	{
		TargetSample = gProFile._SAMPLE_ARR[iCnt];
		if (0==wcscmp(TargetSample->_szName, szName))
		{
			return TargetSample;
		}
	}
	return nullptr;
}


void ProfileBegin(WCHAR * szName)
{
	PROFILE_SAMPLE * Sample = FindSample(szName);

	//처음 들어온 Sample이름이면 새로 생성
	if (nullptr == Sample)
	{
		Sample = new PROFILE_SAMPLE;
		wcscpy(Sample->_szName, szName);
		Sample->_iTotalTime = 0;
		Sample->_iMax[0] = 0;
		Sample->_iMax[1] = 1;
		Sample->_iMin[0] = 0x7FFFFFFFFFFFFFFF-1;
		Sample->_iMin[1] = 0x7FFFFFFFFFFFFFFF;

		Sample->_iCall = 0;

		gProFile._SAMPLE_ARR[gProFile._iCount++] = Sample;
	}

	//Begin을 연속으로 호출했을 경우
	else if (0 != Sample->_liStartTime.QuadPart)
			return;

	QueryPerformanceCounter(&(Sample->_liStartTime));
	//Start시간저장
}

void ProfileEnd(WCHAR * szName)
{
	LARGE_INTEGER liEndTime;
	QueryPerformanceCounter(&liEndTime);

	PROFILE_SAMPLE * CurSample = FindSample(szName);

	if (0 == CurSample->_liStartTime.QuadPart)
		return;

	///////////////////////////
	if (nullptr == CurSample)//begin한적이 없는 경우
		return;				 //
	///////////////////////////


	//Sample 변수들 갱신//////
	__int64 iDiff = liEndTime.QuadPart - CurSample->_liStartTime.QuadPart;
	CurSample->_iTotalTime += iDiff;
	++CurSample->_iCall;
	CurSample->_liStartTime.QuadPart = 0;

	/////////////////////////
	//idx가 큰 쪽이 더 큰수//
	/////////////////////////

	//Max설정
	__int64 * MaxArr = CurSample->_iMax;
	__int64 * MinArr = CurSample->_iMin;


	if (MaxArr[1] < iDiff)
	{
		MaxArr[0] = MaxArr[1];
		MaxArr[1] = iDiff;
	}
	else if (MaxArr[0] < iDiff)
	{
		MaxArr[0] = iDiff;
	}

	//Min설정
	if (MinArr[0] > iDiff)
	{
 		MinArr[1] = MinArr[0];
		MinArr[0] = iDiff;
	}
	else if (MinArr[1] > iDiff)
	{
		MinArr[1] = iDiff;
	}

	if (GetAsyncKeyState(0x50) & 0x8001)
	{
		SetFileName();
		ProfileDataOutText();
	}
}

void SetFileName()
{
	WCHAR Temp[df_FileNameLen] = L"Profile_";
	WCHAR Buffer[6];
	int iCur = 7;

	/////////////////////////
	time_t T;
	struct tm TM;
	T = time(NULL);
	localtime_s(&TM, &T);
	/////////////////////////
	_itow(TM.tm_year + 1900, Buffer, 10);
	wcscpy(Temp + iCur, Buffer);
	iCur += 4;

	if (TM.tm_mon + 1 < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itow(TM.tm_mon + 1, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itow(TM.tm_mon + 1, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	if (TM.tm_mday < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itow(TM.tm_mday, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itow(TM.tm_mday, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	Temp[iCur++] = '_';

	if (TM.tm_hour < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itow(TM.tm_hour, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itow(TM.tm_hour, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	if (TM.tm_min < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itow(TM.tm_min, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itow(TM.tm_min, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 2;
	}

	if (TM.tm_sec < 10)
	{
		Temp[iCur] = '0';
		++iCur;
		_itow(TM.tm_sec, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 1;
	}
	else
	{
		_itow(TM.tm_sec, Buffer, 10);
		wcscpy(Temp + iCur, Buffer);
		iCur += 2;
	}
	Temp[iCur++] = '.';
	Temp[iCur++] = 't';
	Temp[iCur++] = 'x';
	Temp[iCur++] = 't';
	Temp[iCur] = '\0';
	wcscpy(gProFile._FileName, Temp);

}

void CheckKeyEvent()
{
	if (GetAsyncKeyState(0x5A) & 0x8001)
	{
		SetFileName();
		ProfileDataOutText();
	}
}

void ProfileDataOutText()
{
	//setlocale(LC_ALL, "");

	//char* pfname = new char[wfname.size() * 2];

	//wcstombs(pfname, wfname.c_str(), wfname.size() * 2);

	std::wofstream outFile(gProFile._FileName);
	outFile << "     TagName     |     Average     |     Min     |     Max     |    Call    " << std::endl 
		<< "=====================================================" <<std::endl;
	outFile.precision(4);

	double dFreq = gProFile._liFreq.QuadPart;

	for (int iCnt = 0; iCnt < gProFile._iCount; ++iCnt)
	{
		PROFILE_SAMPLE* CurSample = gProFile._SAMPLE_ARR[iCnt];
		
		__int64 iMax = CurSample->_iMax[1];
		__int64 iMin = CurSample->_iMin[0];
		double iAverage = (CurSample->_iTotalTime-iMax-iMin)/(double)(dFreq*(CurSample->_iCall-2));
		__int64 iCallCount = CurSample->_iCall - 2;
		
		

		outFile << std::setw(22) << std::setfill(L' ') << CurSample->_szName << "|"
			<< std::setw(14) << std::fixed << iAverage << "㎲|" << std::setw(9) <<
			iMin/ dFreq << "㎲|" << std::setw(9) << iMax/ dFreq << "㎲|" << std::setw(8) <<
			iCallCount  << std::endl;
	}
}



