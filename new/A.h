#pragma once
class CA
{
public:
	int x;
	int y;

	CA() {}
	CA(int ix, int iy)
		:x(ix), y(iy)
	{}
	~CA() {};

public:
	void Test();
};

