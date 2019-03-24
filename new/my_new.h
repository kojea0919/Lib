#pragma once
#include <cstring>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include <fstream>


#define dfMax_Len 30
#define dfName_Len 128
#define dfFileName_Len 30

struct AllocInfo {
	void *	m_ptr;
	int		m_iMemorySize;
	char	m_cFileName[dfName_Len];
	int		m_iFileLine;

	bool	m_bArrFlag;
};

class Cmy_new 
{
public:
	Cmy_new()
		:_iCount(0)
	{
		FileNameSetting();
		std::ofstream outFile(_FileName);
		outFile.close();
	}
	~Cmy_new()
	{
		std::ofstream outFile(_FileName);
		for (int iCnt = 0; iCnt < _iCount; ++iCnt)
		{
			AllocInfo CurAl = _AL_arr[iCnt];
			outFile << "LEAK  [" << CurAl.m_ptr << "] [" << CurAl.m_iMemorySize <<
				"] " << CurAl.m_cFileName << " " << CurAl.m_iFileLine << std::endl;
		}
		outFile.close();
	}

private:
	static void AllocInfoSetting(void* _ptr, size_t _size, char * _File, int _Line,bool _Arr);
	void FileNameSetting();

public:
	friend void * operator new(size_t _size, char * _File, int _Line);
	friend void * operator new[](size_t _size, char * _File, int _Line);

	void operator delete(void * p, char * File, int Line);
	void operator delete[](void * p, char * File, int Line);

	friend void operator delete(void * p);
	friend void operator delete[](void * p);
public:
	AllocInfo	_AL_arr[dfMax_Len];
	char		_FileName[dfFileName_Len];
	int			_iCount;

};