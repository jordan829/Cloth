#ifndef CSE169_TRIANGLE_H
#define CSE169_TRIANGLE_H

using namespace std;

#include "particle.h"

class Triangle{

public:
	Triangle();
	Triangle(Particle *p1, Particle *p2, Particle *p3);
	void computeForces(Vector3&);
	~Triangle();

	Particle *p1;
	Particle *p2;
	Particle *p3;
	float density;
	Vector3 normal;
};

#endif