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
//�ð� check ���� ���� ����
//////////////////////////

void ProfileEnd(WCHAR * szName);
//////////////////////////
//�ð� check �� nt();
/////////////////////////////
//p�� ������ �Ʒ� �Լ��� ȣ��
/////////////////////////////���� ����
//////////////////////////


void ProfileDataOutText();
////////////////////////////////////////
//Profiling �� �����͸� Text���Ϸ� ����
////////////////////////////////////////

