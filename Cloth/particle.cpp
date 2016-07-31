#include "particle.h"

Particle::Particle()
{
	mass = 20.0;
	position = *new Vector3();
	velocity = *new Vector3();
	force = *new Vector3();
	fixed = false;
	normal = *new Vector3();
}

Particle::Particle(float x, float y, float z)
{
	mass = 20.0;
	position = *new Vector3(x, y, z);
	velocity = *new Vector3();
	force = *new Vector3();
	fixed = false;
}

void Particle::update(float deltaTime, float floor_pos)
{
	Vector3 Accel = (1.0 / mass) * force;

	velocity += Accel*deltaTime;
	position += velocity*deltaTime;

	// Detect collision with floor
	if (position.y <= floor_pos)
	{
		//position.y = floor_pos + 0.05;
		//velocity.Zero();

		float elasticity = 0.2f;
		float friction = 0.5f;
		position.y = (floor_pos + 0.1) - position.y;
		velocity.y = -elasticity * velocity.y;
		velocity.x = (1 - friction)* velocity.x;
		velocity.z = (1 - friction)* velocity.z;
	}

	force.Zero();
}

void Particle::applyForce(Vector3 &f)		
{ 
	force += f; 
}

Particle::~Particle()
{}