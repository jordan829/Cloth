#include "Triangle.h"


Triangle::Triangle()
{
}

Triangle::Triangle(Particle *p1, Particle *p2, Particle *p3)
{
	density = 1.25;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

void Triangle::computeForces(Vector3 &airVelocity)
{
	// average velocity of 3 verts
	Vector3 velocity = (p1->velocity + p2->velocity + p3->velocity) / 3;

	// compute actual velocity
	Vector3 relVelocity = velocity - airVelocity;

	// compute aerodynamic force
	if (relVelocity.Mag() > 0)
	{
		normal.Cross(p2->position - p1->position, p3->position - p1->position);
		normal.Normalize();

		Vector3 v;
		v.Cross(p2->position - p1->position, p3->position - p1->position);
		float a0 = 0.5 * v.Mag();
		float a = a0 * (relVelocity.Dot(normal) / relVelocity.Mag());

		Vector3 aeroForce = (-0.5 * density * relVelocity.Mag2() * a * normal) / 3;

		// apply force if particle is not fixed in position
		if (!p1->fixed)
			p1->applyForce(aeroForce);
		if (!p2->fixed)
			p2->applyForce(aeroForce);
		if (!p3->fixed)
			p3->applyForce(aeroForce);
	}
}

Triangle::~Triangle()
{
}