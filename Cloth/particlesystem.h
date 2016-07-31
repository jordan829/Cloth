#ifndef CSE169_PARTICLESYSTEM_H
#define CSE169_PARTICLESYSTEM_H


#include <vector> 
#include <iostream>
#include "SpringDamper.h"
#include "Triangle.h"
using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(int);
	void Init();
	void update(float deltaTime, Vector3& wind, float);
	void draw();
	void createVert(Vector3, Vector3);
	void reset();
	void move(char);
	void fixTop();
	void fixBottom();
	void fixBoth();
	void freeAll();
	~ParticleSystem();

	int numParticles;
	float offset;
	vector<vector <Particle*> > cloth;
	vector<SpringDamper*> springs;
	vector<Triangle*> triangles;
};

#endif