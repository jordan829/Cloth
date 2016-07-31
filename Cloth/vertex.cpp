#include "vertex.h"

Vertex::Vertex()
{
	position = Vector3(0, 0, 0);
	normal = Vector3(0, 0, 0);
}

void Vertex::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Vertex::setNormal(float x, float y, float z)
{
	normal.x = x;
	normal.y = y;
	normal.z = z;
}

Vertex::~Vertex()
{}