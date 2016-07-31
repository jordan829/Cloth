#ifndef CSE169_VECTOR4_H
#define CSE169_VECTOR4_H

class Vector4
{
public:
	Vector4();
	void set(float, float, float, float);
	~Vector4();

	float x, y, z, w;
};

#endif