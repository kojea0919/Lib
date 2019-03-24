#pragma once

#include <Windows.h>


#ifdef ProfileMode
#define Profile_Begin(X) ProfileBegin(X)
#define Profile_End(X) ProfileEnd(X)

#else
#define Profile_Begin(X)
#define Profile_End(X)
#endif

void ProfileBegin(WCHAR * szName);
//////////////////////////
//시간 check 시작 지점 설정
//////////////////////////

void ProfileEnd(WCHAR * szName);
//////////////////////////
//시간 check 끝 nt();
/////////////////////////////
//p가 눌리면 아래 함수를 호출
/////////////////////////////지점 설정
//////////////////////////


void ProfileDataOutText();
////////////////////////////////////////
//Profiling 된 데이터를 Text파일로 생성
////////////////////////////////////////

