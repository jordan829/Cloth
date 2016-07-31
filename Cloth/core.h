////////////////////////////////////////
// core.h
////////////////////////////////////////

#ifndef CSE169_CORE_H
#define CSE169_CORE_H

////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define M_PI	3.14159f
#include <windows.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

//#include <GLUT/glut.h>

void drawAxis(float size);
void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax, float r, float g, float b, float a);

////////////////////////////////////////////////////////////////////////////////

/*
This file just has a bunch of common stuff used by all objects. It mainly just
includes GL and some other standard headers.
*/

#endif