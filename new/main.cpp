#include <iostream>
#include "my_new.h"
#include "A.h"


using namespace std;


#define new new(__FILE__,__LINE__)

int main()
{
	CA a;
	a.Test();
	return 0;
}

