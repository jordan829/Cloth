#ifndef CSE169_PARTICLE_H
#define CSE169_PARTICLE_H

#include "vector3.h"
#include "matrix34.h"

class Particle
{
public:
	Particle();
	Particle(float, float, float);
	void update(float, float);
	void applyForce(Vector3&);
	~Particle();

	float mass;
	bool fixed;
	Vector3 normal;
	Vector3 position;
	Vector3 velocity;
	Vector3 force;
};

#endif