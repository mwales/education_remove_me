#pragma once

class Airplane
{
public:
	Airplane(int x);

	virtual ~Airplane();

	void fly();
protected:
	int tailNumber;
};
