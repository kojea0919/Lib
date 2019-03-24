#pragma once
class CB
{
public:
	int x;
	int y;

	CB() {}
	CB(int ix, int iy)
		:x(ix), y(iy)
	{}
	~CB() {};

public:
	void Test();
};

