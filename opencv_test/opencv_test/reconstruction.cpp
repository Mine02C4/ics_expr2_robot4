#include "reconstruction.h"



Reconstruction::Reconstruction()
{
}


Reconstruction::~Reconstruction()
{
}

float Reconstruction::GetX(int i) 
{
	return x[i];
}

float Reconstruction::GetY(int i)
{
	return y[i];
}

float Reconstruction::GetZ(int i)
{
	return z[i];
}

void Reconstruction::Add(float rowx, float rowy, float rowz, float theta)
{
	x.push_back(rowx);
	y.push_back(rowy * cos(theta)  - rowz * sin(theta));
	z.push_back(rowy * sin(theta) + rowz * cos(theta));
}
