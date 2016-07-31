#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int size)
{
	// Set size of cloth
	numParticles = size;
	offset = numParticles / 2;

	// resize vector
	cloth.resize(size);
	for (int i = 0; i < size; ++i)
		cloth[i].resize(size);

	Init();
}

void ParticleSystem::Init()
{
	// Initialize and position particles
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			Particle *p = new Particle();
			p->position = Vector3(i - offset, offset, j - offset);
			this->cloth[i][j] = p;
		}
	}

	// Create spring dampers between particles
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			SpringDamper *s;

			if (i > 0)
			{
				s = new SpringDamper(this->cloth[i][j], this->cloth[i - 1][j]);
				this->springs.push_back(s);
			}

			if (j >  0)
			{
				s = new SpringDamper(this->cloth[i][j], this->cloth[i][j - 1]);
				this->springs.push_back(s);
			}

			if (i > 0 && j > 0)
			{
				s = new SpringDamper(this->cloth[i][j], this->cloth[i - 1][j - 1]);
				this->springs.push_back(s);
				s = new SpringDamper(this->cloth[i][j - 1], this->cloth[i - 1][j]);
				this->springs.push_back(s);
			}
		}
	}

	// Create triangles (2 at a time)
	for (int i = 0; i < numParticles - 1; i++)
	{
		for (int j = 0; j < numParticles - 1; j++)
		{
			Triangle *t1 = new Triangle(cloth[i][j], cloth[i + 1][j], cloth[i + 1][j + 1]);
			Triangle *t2 = new Triangle(cloth[i][j], cloth[i + 1][j + 1], cloth[i][j + 1]);
		
			triangles.push_back(t1);
			triangles.push_back(t2);
		}
	}

	// Make top and bottom row fixed in position
	fixBoth();

}

void ParticleSystem::update(float deltaTime, Vector3 &wind, float floor_pos) 
{
	Vector3 gravity(0, -9.8, 0);

	// apply gravity to particles
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			if (!cloth[i][j]->fixed)
			{
				Vector3 force = gravity*cloth[i][j]->mass;
				cloth[i][j]->applyForce(force);
			}
		}
	}

	// compute spring damper forces
	for (int i = 0; i < springs.size(); i++)
		springs[i]->computeForce();

	// compute aerodynamic forces
	for (int i = 0; i < triangles.size(); i++)
		triangles[i]->computeForces(wind);

	// update particles
	for (int i = 0; i < numParticles; i++)
		for (int j = 0; j < numParticles; j++)
			this->cloth[i][j]->update(deltaTime, floor_pos);
}

void ParticleSystem::draw()
{
	// recalculate normals
	for (int i = 0; i < triangles.size(); i++)
	{
		Vector3 norm = Vector3();
		norm.Cross(triangles[i]->p3->position - triangles[i]->p1->position, triangles[i]->p2->position - triangles[i]->p1->position);
		triangles[i]->p1->normal = norm.Normalize();
		triangles[i]->p2->normal = norm.Normalize();
		triangles[i]->p3->normal = norm.Normalize();
	}
	
	// Draw triangles (2 at a time)
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numParticles - 1; i++)
	{
		for (int j = 0; j < numParticles - 1; j++)
		{
			// get vertices
			Vector3 v0 = cloth[i][j]->position;
			Vector3 v1 = cloth[i + 1][j]->position;
			Vector3 v2 = cloth[i][j + 1]->position;
			Vector3 v3 = cloth[i + 1][j + 1]->position;

			// get normals
			Vector3 n0 = cloth[i][j]->normal;
			Vector3 n1 = cloth[i + 1][j]->normal;
			Vector3 n2 = cloth[i][j + 1]->normal;
			Vector3 n3 = cloth[i + 1][j + 1]->normal;

			// set material
			GLfloat dif[] = { 0.6f, 0.0f, 0.0f, 1.f };
			GLfloat spec[] = { 0.1f, 0.1f, 0.1f, 1.f };
			GLfloat shiny[] = { 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);


			// triangle 1
			createVert(n0, v0);
			createVert(n2, v2);
			createVert(n3, v3);

			// triangle 2
			createVert(n0, v0);
			createVert(n3, v3);
			createVert(n1, v1);
		}
	}
	glEnd();

}

void ParticleSystem::createVert(Vector3 norm, Vector3 pos)
{
	glNormal3f(norm.x, norm.y, norm.z);
	glVertex3f(pos.x, pos.y, pos.z);
}

void ParticleSystem::reset()
{
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			// Reset position, force, and velocity
			this->cloth[i][j]->position.Set(i - offset, offset, j - offset);
			this->cloth[i][j]->force.Zero();
			this->cloth[i][j]->velocity.Zero();
		}
	}
}

void ParticleSystem::move(char key)
{
	float dist = 0.25f;
	switch (key)
	{
		case 'X':
			for (int i = 0; i < numParticles; i++)
				this->cloth[i][numParticles - 1]->position += Vector3(dist, 0.0f, 0.0f);
			break;
		case 'Y':
			for (int i = 0; i < numParticles; i++)
				this->cloth[i][numParticles - 1]->position += Vector3(0.0f, dist, 0.0f);
			break;
		case 'Z':
			for (int i = 0; i < numParticles; i++)
				this->cloth[i][numParticles - 1]->position += Vector3(0.0f, 0.0f, dist);
			break;
		case 'x':
			for (int i = 0; i < numParticles; i++)
				this->cloth[i][numParticles - 1]->position -= Vector3(dist, 0.0f, 0.0f);
			break;
		case 'y':
			for (int i = 0; i < numParticles; i++)
				this->cloth[i][numParticles - 1]->position -= Vector3(0.0f, dist, 0.0f);
			break;
		case 'z':
			for (int i = 0; i < numParticles; i++)
				this->cloth[i][numParticles - 1]->position -= Vector3(0.0f, 0.0f, dist);
			break;
	}
}

void ParticleSystem::fixTop()
{
	freeAll();

	// Make top row fixed in position
	for (int i = 0; i < numParticles; i++)
		this->cloth[i][numParticles - 1]->fixed = true;
}

void ParticleSystem::fixBottom()
{
	freeAll();

	// Make bottom row fixed in position
	for (int i = 0; i < numParticles; i++)
		this->cloth[i][0]->fixed = true;
}

void ParticleSystem::fixBoth()
{
	// Make top/bottom row fixed in position
	for (int i = 0; i < numParticles; i++)
	{
		this->cloth[i][numParticles - 1]->fixed = true;
		this->cloth[i][0]->fixed = true;
	}
}

void ParticleSystem::freeAll()
{
	// free all aprticles
	for (int i = 0; i < numParticles; i++)
	{
		this->cloth[i][numParticles - 1]->fixed = false;
		this->cloth[i][0]->fixed = false;
	}
}

ParticleSystem::~ParticleSystem()
{
}