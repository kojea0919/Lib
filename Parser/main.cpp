
#include <Windows.h>
#include <iostream>
#include <fstream>

#define MaxWordLen 256

using namespace std;

wifstream in("test.txt");

bool SkipNoneCommand();
//------------------------------------------
//스페이스, 탭, 엔터, 주석등을 pass하는 함수
//------------------------------------------

bool GetNextWord(WCHAR *chppBuffer, int *ipLength);
//------------------------------------------
//다음 단어의 시작 위치를 chppBuffer에 넣어주는 함수 
//------------------------------------------

bool GetValue_Int(WCHAR *szName, int *ipValue);

bool GetValue_String(WCHAR *szName, WCHAR chBuffer[]);


WCHAR * test;

int main()
{
	int iVersion;
	int iServerID;
	WCHAR String[MaxWordLen];
	int iPort;
	int iWorkerThread;
	int imaxUser;


	GetValue_String(L"ServerBindIP", String);
	GetValue_Int(L"Version", &iVersion);
	GetValue_Int(L"ServerID", &iServerID);
	GetValue_Int(L"ServerBindPort", &iPort);
	GetValue_Int(L"WorkerThread", &iWorkerThread);
	GetValue_Int(L"MaxUser", &imaxUser);

	wcout << iVersion << endl;
	wcout << iServerID << endl;
	wcout << String << endl;
	wcout << iPort << endl;
	wcout << iWorkerThread << endl;
	wcout << imaxUser << endl;

	return 0;
}

bool SkipNoneCommand()
{
	WCHAR cCursor;

	while (!in.eof())
	{
		in.get(cCursor);

		if (cCursor == '	' || cCursor == ' ' || cCursor == '\n')
		{
			continue;
		}
		else if (cCursor == '/')
		{
			in.get(cCursor);
			if (cCursor == '/')//주석 발견한 경우 (1)
			{
				in.get(cCursor);
				while(cCursor != '\n')
					in.get(cCursor);
			}

			else if (cCursor == '*')//주석 발견한 경우 (2)
			{
				in.get(cCursor);
				while (true)
				{
					in.get(cCursor);
					if (cCursor == '*')
					{
						in.get(cCursor);
						if (cCursor == '/')
							break;
					}
				}
			}
		}

		else
		{
			in.seekg(-1, ios::cur);
			//in.get(cCursor); //test용
			return true;
		}
	}
	return false;
}

bool GetNextWord(WCHAR * chppBuffer, int * ipLength)
{
	int iWordLen = 0;
	bool bFindFirst = false;

	WCHAR cWord[MaxWordLen];

	WCHAR cCur;


	//공백 or 주석 등 데이터가 아닌 것들 넘기기
	if (SkipNoneCommand())
	{
		in.get(cCur);
		while (!in.eof() && cCur != '\n' && cCur != ' '  && cCur != '\t' 
			&& cCur != '\0') 
		{
			if (cCur == '/')
			{
				in.get(cCur);
				if (cCur == '/')
					break;
				else
				{
					in.seekg(-1, ios::cur);
					cCur = '/';
				}
			}

			if (cCur == '"')
			{
				if (bFindFirst)
					break;
				in.get(cCur);
				cWord[iWordLen++] = cCur;
				bFindFirst = true;
				in.get(cCur);
				continue;
			}
			
			cWord[iWordLen++] = cCur;
			in.get(cCur);
		}
		cWord[iWordLen++] = L'\0'; 
		wcscpy_s(chppBuffer, MaxWordLen,cWord);
	
		*ipLength = iWordLen;
	}
	return true;
}

bool GetValue_Int(WCHAR * szName, int * ipValue)
{
	WCHAR chpBuff[MaxWordLen], chWord[MaxWordLen];
	int iLength;
	
	while (GetNextWord(chpBuff, &iLength))
		// szName 이라는 단어를 찾고 싶으므로 szName이라는 단어가
		//나올때 까지 반복
	{
		memset(chWord, 0, 256);
		wcscpy_s(chWord, MaxWordLen, chpBuff);
		// chWord에 NextWord(chpBuff)를 저장

		if (0 == wcscmp(szName, chWord))
			//찾고자 하는 단어인지 비교
		{
			//맞다면 등호(=)찾기
			if (GetNextWord(chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				wcscpy_s(chWord, MaxWordLen, chpBuff);

				if (0 == wcscmp(chWord, L"="))
					//등호가 맞은지 확인
				{
					//맞다면 데이터 가져오기
					if (GetNextWord(chpBuff, &iLength))
					{
						memset(chWord, 0, 256);
						wcscpy_s(chWord, MaxWordLen, chpBuff);
						*ipValue = _wtoi(chWord);
						in.seekg(0, ios::beg);
						return true;
					}
					return false;
				}
			}
			return false;
		}
	}
	return false;
}

bool GetValue_String(WCHAR * szName, WCHAR chBuffer[])
{
	WCHAR chpBuff[MaxWordLen], chWord[MaxWordLen];
	int iLength;

	test = szName;

	while (GetNextWord(chpBuff, &iLength))
		// szName 이라는 단어를 찾고 싶으므로 szName이라는 단어가
		//나올때 까지 반복
	{
		memset(chWord, 0, MaxWordLen *2);
		wcscpy_s(chWord, chpBuff);
		// chWord에 NextWord(chpBuff)를 저장

		if (0 == wcscmp(szName, chWord))
			//찾고자 하는 단어인지 비교
		{
			//맞다면 등호(=)찾기
			if (GetNextWord(chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				memcpy(chWord, chpBuff, iLength);

				if (0 == wcscmp(chWord, L"="))
					//등호가 맞은지 확인
				{
					//맞다면 데이터 가져오기
					if (GetNextWord(chpBuff, &iLength))
					{
						wcscpy_s(chBuffer, MaxWordLen, chpBuff);
						in.seekg(0, ios::beg);
						return true;
					}
					return false;
				}
			}
			return false;
		}
	}
	delete[] chpBuff;
	return false;
}