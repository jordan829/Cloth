#ifndef CSE169_VERTEX_H
#define CSE169_VERTEX_H

#include "vector3.h"

class Vertex {

public:
	Vector3 position;
	Vector3 normal;

	Vertex();
	void setPosition(float, float, float);
	void setNormal(float, float, float);
	~Vertex();
};

#endif