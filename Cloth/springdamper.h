#ifndef CSE169_SPRINGDAMPER_H
#define CSE169_SPRINGDAMPER_H

#include "Particle.h""
class SpringDamper
{
public:
	SpringDamper();
	SpringDamper(Particle *r1, Particle *r2);
	void computeForce();
	~SpringDamper();

	float springConstant;
	float dampingFactor;
	Particle *r1, *r2;
	float restLength;	
};

#endif