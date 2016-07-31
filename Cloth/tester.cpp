////////////////////////////////////////
// tester.cpp
////////////////////////////////////////
using namespace std;

#include "tester.h"

#define WINDOWTITLE	"Cloth Simulation"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;
int jointIndex = 0;
string object;
bool drawSkel = true;
bool print = true;
bool windMode = true;
bool drawGUI = false;
Vector3 *wind;
ParticleSystem *cloth;
TwBar *set;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	TESTER->start_time = clock();
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

// Tweakbar button callback methods
void TW_CALL DeactivateGUI(void*)
{
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);
	glutPassiveMotionFunc(mousemotion);
	drawGUI = false;
}

void TW_CALL QuitApplication(void*)
{
	glFinish();
	exit(0);
}

void TW_CALL ZeroWind(void*)
{
	*wind = Vector3(0, 0, 0);
}

void TW_CALL fixTop(void*)
{
	cloth->fixTop();
}

// Buggy
void TW_CALL fixBoth(void*)
{
	cloth->fixBoth();
}

void TW_CALL freeAll(void*)
{
	cloth->freeAll();
}


////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=1024;
	WinY=768;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );
	
	// Create lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);

	GLfloat white[] = { 0.7f, 0.7f, 0.7f, 1.0 };
	GLfloat red[] = { 0.2f, 0.0f, 0.0f, 1.0 };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightfv(GL_LIGHT2, GL_AMBIENT, white);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white);
	glLightfv(GL_LIGHT3, GL_AMBIENT, white);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT3, GL_SPECULAR, white);
	
	GLfloat pos0[] = { 1, 1, -1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);

	GLfloat pos1[] = { -1, 1, 1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);

	GLfloat pos2[] = { 0, 1, 0, 0 };
	glLightfv(GL_LIGHT2, GL_POSITION, pos2);

	GLfloat pos3[] = { 0, -1, 0, 0 };
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);

	glEnable(GL_DEPTH_TEST);

	// Background color (sky blue)
	glClearColor( 0.7f, 0.9f, 1.0f, 1.0f );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));

	floor_pos = 0.0f;
	wind = new Vector3();
	cloth = new ParticleSystem(50);

	// Initialize GUI
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(WinX, WinY);

	set = TwNewBar("Settings");

	// Wind
	TwAddVarRW(set, "X velocity", TW_TYPE_FLOAT, &wind->x, "group=Wind step=1");
	TwAddVarRW(set, "Y velocity", TW_TYPE_FLOAT, &wind->y, "group=Wind step=1");
	TwAddVarRW(set, "Z velocity", TW_TYPE_FLOAT, &wind->z, "group=Wind step=1");
	TwAddButton(set, "Zero", ZeroWind, NULL, "group=Wind");

	// Set fixed rows
	TwAddButton(set, "Fix Top Row", fixTop, NULL, "group=Rows");
	TwAddButton(set, "Free Top Row", freeAll, NULL, "group=Rows");

	// Floor
	TwAddVarRW(set, "Position", TW_TYPE_FLOAT, &floor_pos, "group=Floor step=1");

	// Button to exit menu
	TwAddButton(set, "Exit Menu (ESC)", DeactivateGUI, NULL, " key=ESC ");

	// Button to quit application
	TwAddButton(set, "Quit Application", QuitApplication, NULL, "");
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {

	// Update the components in the world
	Cam.Update();

	// Oversample
	int oversample = 4;
	float deltaTime = 1 / 144.0;
	for (int i = 0; i < oversample; i++)
		cloth->update(deltaTime, *wind, floor_pos);

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.SetAspect(float(WinX)/float(WinY));
	cloth->reset();
	wind = new Vector3();
	floor_pos = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup floor
	GLfloat green[] = { 0.0f, 0.2f, 0.0f, 1.0f };
	GLfloat shiny[] = { 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);

	// Draw floor
	float floor_size = 100;
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-floor_size, floor_pos, -floor_size);
	glVertex3f(floor_size, floor_pos, -floor_size);
	glVertex3f(floor_size, floor_pos, floor_size);
	glVertex3f(-floor_size, floor_pos, floor_size);
	glEnd();

	// Setup cam
	Cam.Draw();		// Sets up projection & viewing matrices

	// Draw cloth
	glLoadIdentity();
	cloth->draw();
	
	// Draw GUI
	if (drawGUI)
		TwDraw();

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}


////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
	TwWindowSize(float(WinX), float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {

	float floorFactor = 0.25f;
	switch (key) {
		case 0x1b:		// Escape
			ActivateGUI();
			break;
		case 'q':
			Quit();
			break;
		case 'r':
			cloth->fixTop();
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::ActivateGUI()
{
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	drawGUI = true;
}

////////////////////////////////////////////////////////////////////////////////

