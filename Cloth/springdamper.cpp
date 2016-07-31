#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle *r1, Particle *r2)
{
	this->r1 = r1;
	this->r2 = r2;

	springConstant = 30000;
	dampingFactor = 500;
	Vector3 rest = this->r1->position - this->r2->position;
	restLength = rest.Mag();
}

void SpringDamper::computeForce()
{
	Vector3 distance = Vector3(r2->position - r1->position);
	float dist = distance.Mag();
	Vector3 e = distance.Normalize();
	
	float v1 = e.Dot(r1->velocity);
	float v2 = e.Dot(r2->velocity);
	float displacement = restLength - dist;
	
	float springForce = -springConstant*displacement;
	float dampForce = -dampingFactor*(v1 - v2);
	float sdForce = springForce + dampForce;

	// Apply forces only to particles not fixed
	if (!r1->fixed)
		r1->applyForce(sdForce*e);
	if (!r2->fixed)
		r2->applyForce(-1.0 * sdForce*e);
}

SpringDamper::~SpringDamper()
{
}