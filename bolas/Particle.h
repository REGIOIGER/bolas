#pragma once

#include <math.h>
#include "Vector3D.cpp"

class Particle
{
public:
Particle();
~Particle();


Vector3D<double> position;
Vector3D<double> velocity;

double mass;
double radius;
int color;

double particleTime;

inline Vector3D<double> GetPosition(double simTime) const
{
	return position + (simTime - particleTime) * velocity;
}

};

