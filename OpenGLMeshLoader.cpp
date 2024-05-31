#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <iostream>
#include <cmath>
#include <string.h>
#include<time.h>
#include<mmsystem.h>
#include<vector>
#include <playsoundapi.h>
#include <Windows.h>
#include <MMSystem.h>
#include <mciapi.h>
#include <fstream>


using namespace std;
#pragma comment(lib,"winmm.lib")

#define DEG2RAD(a) (a * 0.0174532925)

//Classes
class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0, float eyeY =9, float eyeZ = 40, float centerX = 0, float centerY = 0, float centerZ = 0, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);

	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
		std::cout << "eye X:" << eye.x << std::endl;
		std::cout << "eye Y:" << eye.y << std::endl;
		std::cout << "eye z:" << eye.z << std::endl;

		std::cout << "center X:" << center.x << std::endl;
		std::cout << "center Y:" << center.y << std::endl;
		std::cout << "center z:" << center.z << std::endl;

		std::cout << "up X:" << up.x << std::endl;
		std::cout << "up Y:" << up.y << std::endl;
		std::cout << "up z:" << up.z << std::endl;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;

		/*std::cout << "eye X:" << eye.x << std::endl;
		std::cout << "eye Y:" << eye.y << std::endl;
		std::cout << "eye z:" << eye.z << std::endl;

		std::cout << "center X:" << center.x << std::endl;
		std::cout << "center Y:" << center.y << std::endl;
		std::cout << "center z:" << center.z << std::endl;

		std::cout << "up X:" << up.x << std::endl;
		std::cout << "up Y:" << up.y << std::endl;
		std::cout << "up z:" << up.z << std::endl;*/
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;

		/*std::cout << "eye X:" << eye.x << std::endl;
		std::cout << "eye Y:" << eye.y << std::endl;
		std::cout << "eye z:" << eye.z << std::endl;

		std::cout << "center X:" << center.x << std::endl;
		std::cout << "center Y:" << center.y << std::endl;
		std::cout << "center z:" << center.z << std::endl;

		std::cout << "up X:" << up.x << std::endl;
		std::cout << "up Y:" << up.y << std::endl;
		std::cout << "up z:" << up.z << std::endl;*/
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;

		/*std::cout << "eye X:" << eye.x << std::endl;
		std::cout << "eye Y:" << eye.y << std::endl;
		std::cout << "eye z:" << eye.z << std::endl;

		std::cout << "center X:" << center.x << std::endl;
		std::cout << "center Y:" << center.y << std::endl;
		std::cout << "center z:" << center.z << std::endl;

		std::cout << "up X:" << up.x << std::endl;
		std::cout << "up Y:" << up.y << std::endl;
		std::cout << "up z:" << up.z << std::endl;*/
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;

		/*std::cout << "eye X:" << eye.x << std::endl;
		std::cout << "eye Y:" << eye.y << std::endl;
		std::cout << "eye z:" << eye.z << std::endl;

		std::cout << "center X:" << center.x << std::endl;
		std::cout << "center Y:" << center.y << std::endl;
		std::cout << "center z:" << center.z << std::endl;

		std::cout << "up X:" << up.x << std::endl;
		std::cout << "up Y:" << up.y << std::endl;
		std::cout << "up z:" << up.z << std::endl;*/
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

//Global Variables
Camera camera;

//screen dimensions
int WIDTH = 1200; 
int HEIGHT = 700;

GLuint tex; //sky
GLuint tex2; //galaxy

char title[] = "Subway Surfer";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 900;

Vector Eye(0, 5, 40);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

//Player movement
float movePlayerX = 0;
float movePlayerY = 0;
float movePlayerZ = 20;

//at end of the game when he loses to keep going downward
float losePlayerY = 0;


//Arrays of objects on Earth
Vector coins [25];  
Vector trains[10];  
Vector dogPosition;
Vector jetPosition;
Vector sneakersPosition;

//Arrays of objects on Space
Vector fuel[25];  
Vector planes[10];  
Vector keyPosition;

//objects on Endgame
Vector doorPosition;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_jake;
Model_3DS model_train;
Model_3DS model_coin;
Model_3DS model_jetpack;
Model_3DS model_dog;
Model_3DS model_plane;
Model_3DS model_fuel;
Model_3DS model_key;
Model_3DS model_zombie;

Model_3DS model_boot;
Model_3DS model_tricky;
Model_3DS model_ninja;
Model_3DS model_tagbot;

// Textures
GLTexture tex_ground;


//Game Logic

//when he jumps i need the camera to jump as well => i need to check which perspective he is in
bool firstPersonPOV = false;

//health
int health = 5;

//score
int score = 0;
char* ScoreArr[20];
int scoreGain = 50;
int highScore;

//used as Z position for both health and score (to be able to move along the camera)
int scorePosZ = -50; 
//used as X position for both health and score (to be able to move along the camera)
int scorePosX = -55;
int scorePosY = 20;

//time
int minutes = 0;
int seconds = 30;
int prevTime = 0;
bool timerRunning = true;
bool timerEnd = false; //to check time



//0=earth  , 1=space , 2=end game , 3=lost after earth(jet not taken) , 4=lost after space(key not taken) , 5=win
int displayMode = 6; 

//0=jake , 1=tricky , 2=ninja , 3=tagbot
int ChosenPlayer = 0;
bool gameStarted = false;

bool sneakerTaken = false;
bool keyTaken = false;
bool jetTaken = false;
bool stop = true; //to stop camera movement in display 3,4,5 (end of game)

//ligth animation
int lightCounter = 0;
bool lightChange = false;

float lightValue = 5;
float lightStep = 1;
float lightZ = 100;


//coins,stars,key,jet rotation
float RotationAngle = 0;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void AnimateLight(int value) {

	if (lightChange) {
		
		
		if (displayMode == 1) {
			GLfloat lightIntensity[] = { 0.1, 0.1, 0.1, 1.0f };
			glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
		}
		else {
			GLfloat lightIntensity[] = { 0.5, 0.5, 0.5, 1.0f };
			glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		}
		
		GLfloat lightPosition[] = { 100.0f,100.0f,  lightZ +lightStep , 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		
		lightCounter--;
	}
	else {
		
		if (displayMode == 1) {
			
			GLfloat lightIntensity[] = { 0.7,  0.7,  0.7, 1.0f };
			glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
		}
		else {
			GLfloat lightIntensity[] = { 1, 1, 1, 1.0f };
			glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		}

		GLfloat lightPosition[] = { 0, 100.0f, 0 , 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		
		lightCounter++;
	}

	if (lightCounter==5) {
		lightChange = true;
	}
	else if (lightCounter == -5) {
		lightChange = false;
	}

	glutTimerFunc(1000, AnimateLight, 0);

	
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, camera.center.x, camera.center.y, camera.center.z, camera.up.x, camera.up.y, camera.up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspectRatio, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

//To initailize positions of objects 
void InitPositions() {

//Earthhh
	//coins
	coins[0] = Vector(-5, 0, -50);
	coins[1] = Vector(-5, 0, -100);
	coins[2] = Vector(-5, 0, -200);
	coins[3] = Vector(-5, 0, -250);
	coins[4] = Vector(-5, 0, -300);

	coins[5] = Vector(5, 0, -70);
	coins[6] = Vector(5, 0, -200);
	coins[7] = Vector(5, 0, -270);
	coins[8] = Vector(5, 0, -300);
	coins[9] = Vector(5, 0, -350);

	coins[10] = Vector(-10, 0, -60);
	coins[11] = Vector(-10, 0, -120);
	coins[12] = Vector(-10, 0, -180);
	coins[13] = Vector(-10, 0, -240);
	coins[14] = Vector(-10, 0, 300);

	coins[15] = Vector(10, 0, -80);
	coins[16] = Vector(10, 0, -160);
	coins[17] = Vector(10, 0, -240);
	coins[18] = Vector(10, 0, -320);
	coins[19] = Vector(10, 0, -400);

	//coins 3ashan khater nony
	coins[20] = Vector(0, 5, -80);
	coins[21] = Vector(0, 5, -105);
	coins[22] = Vector(0, 5, -200);
	coins[23] = Vector(0, 5, -300);
	coins[24] = Vector(0, 5, -400);


	//trains
	trains[0] = Vector(-12,0,-100);
	trains[1] = Vector(-12,0,-200);
	trains[2] = Vector(-12,0,-300);
	trains[3] = Vector(-12,0,-400);
	trains[4] = Vector(-12,0,-450);

	trains[5] = Vector(12, 0, -10);
	trains[6] = Vector(12, 0, -20);
	trains[7] = Vector(12, 0, -50);
	trains[8] = Vector(12, 0, -100);
	trains[9] = Vector(12, 0, -450);

	//dog
	dogPosition= Vector(0, 0, -150);

	//jetpack
	jetPosition = Vector(0,0,-490);

	//sneakers
	sneakersPosition= Vector(0, 0,-90);

//Space
	//fuel
	//fuel[0] = Vector(-5, 0, -50);
	//fuel[1] = Vector(-5, 0, -100);
	//fuel[2] = Vector(-5, 0, -150);
	//fuel[3] = Vector(-5, 0, -200);
	//fuel[4] = Vector(-5, 0, -250);

	//fuel[5] = Vector(5, 0, -70);
	//fuel[6] = Vector(5, 0, -140);
	//fuel[7] = Vector(5, 0, -210);
	//fuel[8] = Vector(5, 0, -280);
	//fuel[9] = Vector(5, 0, -350);

	//fuel[10] = Vector(-10, 0, -60);
	//fuel[11] = Vector(-10, 0, -120);
	//fuel[12] = Vector(-10, 0, -180);
	//fuel[13] = Vector(-10, 0, -240);
	//fuel[14] = Vector(-10, 0, 300);

	//fuel[15] = Vector(10, 0, -80);
	//fuel[16] = Vector(10, 0, -160);
	//fuel[17] = Vector(10, 0, -240);
	//fuel[18] = Vector(10, 0, -320);
	//fuel[19] = Vector(10, 0, -400);

	////row of fuel
	//fuel[20] = Vector(0, 0, -100);
	//fuel[21] = Vector(0, 0, -200);
	//fuel[22] = Vector(0, 0, -300);
	//fuel[23] = Vector(0, 0, -400);
	//fuel[24] = Vector(0, 0, -450);

	fuel[0] = Vector(-5, 0, -40);
	fuel[1] = Vector(-5, 0, -120); //
	fuel[2] = Vector(-5, 0, -180);//
	fuel[3] = Vector(-5, 0, -240);//
	fuel[4] = Vector(-5, 0, -430);

	fuel[5] = Vector(5, 0, -60);
	fuel[6] = Vector(5, 0, -160);
	fuel[7] = Vector(5, 0, -200);
	fuel[8] = Vector(5, 0, -280);
	fuel[9] = Vector(5, 0, -350);

	fuel[10] = Vector(-10, 0, -20);
	fuel[11] = Vector(-10, 0, -140);//
	fuel[12] = Vector(-10, 0, -230);//
	fuel[13] = Vector(-10, 0, -340);
	fuel[14] = Vector(-10, 0, 300);

	fuel[15] = Vector(10, 0, -80);
	fuel[16] = Vector(10, 0, -170);
	fuel[17] = Vector(10, 0, -260);
	fuel[18] = Vector(10, 0, -320);
	fuel[19] = Vector(10, 0, -400);

	//row of fuel
	fuel[20] = Vector(0, 0, -110);//
	fuel[21] = Vector(0, 0, -220);//
	fuel[22] = Vector(0, 0, -310);
	fuel[23] = Vector(0, 0, -380);//
	fuel[24] = Vector(0, 0, -470);//




	//planes
	planes[0] = Vector(-12, 0, -100);
	planes[1] = Vector(-12, 0, -200);
	planes[2] = Vector(-12, 0, -300);
	planes[3] = Vector(-12, 0, -400);
	planes[4] = Vector(-12, 0, -450);

	planes[5] = Vector(12, 0, -10);
	planes[6] = Vector(12, 0, -20);
	planes[7] = Vector(12, 0, -50);
	planes[8] = Vector(12, 0, -100);
	planes[9] = Vector(12, 0, -450);


	//key
	keyPosition = Vector(0, 0, -490);



//End game
doorPosition = Vector(0, 0, -90);



}

//=======================================================================
// Draw Objects Functions
//=======================================================================
//To draw according to chosen player
void ChoosePlayer() {
	if (ChosenPlayer == 0) {
		model_jake.Draw();
	}
	else if (ChosenPlayer == 1) {
		model_tricky.Draw();
	}
	else if (ChosenPlayer == 2) {
		model_ninja.Draw();
	}
	else if (ChosenPlayer == 3) {
		model_tagbot.Draw();
	}
	else if (ChosenPlayer == 4) {
		model_zombie.Draw();
	}
}

//Earth
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-15, 0, -500);
	glTexCoord2f(5, 0);
	glVertex3f(15, 0, -500);
	glTexCoord2f(5, 5);
	glVertex3f(15, 0, 30);
	glTexCoord2f(0, 5);
	glVertex3f(-15, 0, 30);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void drawSky() {
	//sky box (sphere)
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 800, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();

}

void drawJake() {
	glPushMatrix();
	glTranslatef(movePlayerX, movePlayerY, movePlayerZ);
	glScalef(0.5, 0.5, 0.5);
	ChoosePlayer();
	glPopMatrix();

	
}

void drawPlayerWizSneaker() {
	if (sneakerTaken == true) {
		glPushMatrix();
		glTranslatef(movePlayerX, movePlayerY, movePlayerZ);


		glPushMatrix();
		glTranslatef(0, 0.2, 0);
		glScalef(0.5, 0.5, 0.5);
		ChoosePlayer();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0, 0);
		glScalef(0.5, 0.5, 0.5);
		model_boot.Draw();
		glPopMatrix();

		glPopMatrix();
	}
	else {
		
		glPushMatrix();
		glTranslatef(movePlayerX, movePlayerY, movePlayerZ);
		glScalef(0.5, 0.5, 0.5);
		ChoosePlayer();

		glPopMatrix();
	}
}

void drawTrains() {
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		Vector position = trains[i];
		glTranslatef(position.x, position.y, position.z);
		glScalef(0.3, 0.2, 0.3);
		model_train.Draw();
		glPopMatrix();
	}
}

void drawCoins() {
	for (int i = 0; i < 25; i++) {
		glPushMatrix();
		Vector position = coins[i];
		glTranslatef(position.x, position.y, position.z);
		glScalef(10, 10, 10);
		glRotated(RotationAngle, 0, 1, 0);
		model_coin.Draw();
		glPopMatrix();
	}
}

void drawJetpack() {
	glPushMatrix();
	Vector position = jetPosition;
	glTranslatef(position.x, position.y, position.z);
	glScalef(20, 20, 20);
	glRotated(RotationAngle, 0, 1, 0);
	glRotatef(180.f, 0, 1, 0);
	model_jetpack.Draw();
	glPopMatrix();
}

void drawDog() {
	glPushMatrix();
	Vector position = dogPosition;
	glTranslatef(position.x, position.y, position.z);
	glRotatef(180.f, 0, 1, 0);
	//glScalef(0.5, 0.5, 0.5);
	model_dog.Draw();
	glPopMatrix();
}

void RenderGroundFarHome()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-15, 0, -560);
	glTexCoord2f(5, 0);
	glVertex3f(15, 0, -560);
	glTexCoord2f(5, 5);
	glVertex3f(15, 0, -600);
	glTexCoord2f(0, 5);
	glVertex3f(-15, 0, -600);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	// Draw house Model (fi door aho wink wink)
	glPushMatrix();
	glTranslatef(0, 0, -600);
	glRotatef(90.f, 1, 0, 0);
	glScaled(1.5,1.5,1.5);
	model_house.Draw();
	glPopMatrix();
}

void drawSneaker() {
	glPushMatrix();
	Vector position = sneakersPosition;
	glTranslatef(position.x, position.y, position.z);
	glRotated(RotationAngle, 0, 1, 0);
	glScalef(1, 1, 1);
	model_boot.Draw();
	glPopMatrix();
}


//Space
void drawUniverse() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 600, 100, 100);
	gluDeleteQuadric(qobj);

	glPopMatrix();
}

void drawSpacePlayer() {
	glPushMatrix();
	glTranslatef(movePlayerX, movePlayerY, movePlayerZ); //for both jake and jet to move together
	glRotatef(270.f, 1, 0, 0);

	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	ChoosePlayer();
	glPopMatrix();

	glPushMatrix();  //to make jet attached to jake
	glTranslatef(0, 2.5, 0.5);
	glScalef(10, 10, 10);
	glRotatef(180.f, 0, 1, 0);
	model_jetpack.Draw();  
	glPopMatrix();

	

	glPopMatrix();
}

void drawPlanes() {
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		Vector position = planes[i];
		glTranslatef(position.x, position.y, position.z);
		glScalef(0.5, 0.5, 0.5);
		glRotatef(180.f, 0, 1, 0);
		model_plane.Draw();
		glPopMatrix();
	}
}

void drawFuel() {
	for (int i = 0; i < 25; i++) {
		glPushMatrix();
		Vector position = fuel[i];
		glTranslatef(position.x, position.y, position.z);
		glScalef(5, 5, 5);
		glRotated(RotationAngle, 0, 1, 0);
		model_fuel.Draw();
		glPopMatrix();
	}
}

void drawKey() {
	glPushMatrix();
	Vector position = keyPosition;
	glTranslatef(position.x, position.y, position.z);
	glScalef(0.8, 0.8, 0.8);
	glRotated(RotationAngle, 0, 1, 0);
	glRotatef(90.f, 0, 0, 1);
	glRotatef(90.f, 1, 0, 0);
	model_key.Draw();
	glPopMatrix();
}

//End Game
void drawDoor() {

	// Draw house Model (fi door aho wink wink)
	glPushMatrix();
	Vector position = doorPosition;
	glTranslatef(position.x, position.y, position.z);
	glRotatef(90.f, 1, 0, 0);
	model_house.Draw();
	glPopMatrix();

}

void drawPlayerWizKey() {
	if (keyTaken == true) {
		glPushMatrix();
		glTranslatef(movePlayerX, movePlayerY, movePlayerZ);


		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		ChoosePlayer();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(2.5, 3, 0);
		glScalef(0.1, 0.1, 0.1);
		glRotatef(90.f, 0, 0, 1);
		glRotatef(90.f, 1, 0, 0);
		model_key.Draw();
		glPopMatrix();

		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(movePlayerX, movePlayerY, movePlayerZ);
		glScalef(0.5, 0.5, 0.5);
		ChoosePlayer();
		glPopMatrix();
	}
}

void RenderGroundEnd()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-15, 0, -100);
	glTexCoord2f(5, 0);
	glVertex3f(15, 0, -100);
	glTexCoord2f(5, 5);
	glVertex3f(15, 0, 35);
	glTexCoord2f(0, 5);
	glVertex3f(-15, 0, 35);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	
}

//Lose
void drawDeadPlayer() {
	glPushMatrix();
	glTranslatef(0, losePlayerY, camera.eye.z - 30); //losePlayerY to keep descending 
	if (ChosenPlayer == 4) {
		glScalef(20, 20, 20);
	}
	else {
		glScalef(0.5, 0.5, 0.5);
	}
	glRotatef(180, 0,1,0);
	ChoosePlayer();
	glPopMatrix();
}

void decsend(int value) {
	losePlayerY = losePlayerY - 0.05;
	glutTimerFunc(1000, decsend, 0);


}


//Prints
void printScore(int x, int y,int z, char* string)
{
	glColor3f(0, 0, 0); 
	sprintf((char*)ScoreArr, "Score: %d", score);

	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3f(x, y,z);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}



	glColor3f(0.0f, 0.0f, 0.0f); //Black
	glRasterPos3f(scorePosX -1, scorePosY-3, scorePosZ);
	std::string label = "High Score : " + std::to_string(highScore);
	for (char c : label) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.


}

void printHealthBar() {
	
	// Draw the "Lives" label
	glColor3f(0.0f, 0.0f, 0.0f); //Black
	glRasterPos3f(scorePosX+15, scorePosY,scorePosZ);
	std::string label = "Lives : " + std::to_string(health);
	for (char c : label) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//Animation 
void AnimateObjects(int value) {

	RotationAngle = RotationAngle + 10;
	glutTimerFunc(100, AnimateObjects, 0);


}


//High Score
bool writeHighScore(int score) {
	if (score > highScore) {

		std::ofstream file("highscore.txt");
		if (!file) {
			return false;
		}
		file << score << std::endl;
		file.close();
		return true;
	}
	return false;
}

int readHighScore() {
	int scores = 0;
	std::ifstream file("highscore.txt");
	if (!file) {
		return scores;
	}
	file >> scores;
	file >> highScore;
	file.close();
	return scores;
}


//=======================================================================
// Display Function
//=======================================================================


void displayEarth() {

	RenderGround();
	drawPlayerWizSneaker();
    drawTrains();
	drawCoins();
	drawJetpack();
	drawDog();
	drawSky();
	drawSneaker();
	RenderGroundFarHome();

}

void displaySpace() {
	
	 drawUniverse();
	 drawPlanes();
	 drawSpacePlayer();
	 drawFuel();
	 drawKey();

	
	

}

void displayEndGame() {
	RenderGroundEnd();
	drawPlayerWizKey();
	drawSky();
	drawDoor();
}

void displayChoosePlayer() {
	drawSky();
	RenderGround();

	//jake
	glPushMatrix();
	glTranslatef(-10, 0, 10);
	glRotated(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	model_jake.Draw();
	glPopMatrix();

	//tricky
	glPushMatrix();
	glTranslatef(-3, 0, 10);
	glRotated(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	model_tricky.Draw();
	glPopMatrix();

	//ninja
	glPushMatrix();
	glTranslatef(3, 0, 10);
	glRotated(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	model_ninja.Draw();
	glPopMatrix();

	//tagbot
	glPushMatrix();
	glTranslatef(10, 0, 10);
	glRotated(180, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	model_tagbot.Draw();
	glPopMatrix();

	//game over text
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos3f(-3, 10, camera.eye.z - 30);
	std::string scoreText = "Choose Your Player";

	for (char c : scoreText) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture
}

void displayWin() {
	stop = true;

	//reset camera to be at the center
	camera.eye.x = 0;
	camera.center.x = 0;

	//background
	drawSky();

	glColor3f(0.0f, 0.0f, 0.0f);
	if (highScore < score) {
		writeHighScore(score);
		glRasterPos3f(-10, 5, camera.eye.z-30);
		std::string scoreText = "CONGRATULATIONS!YOU REACHED NEW HIGH SCORE, Your Score: " + std::to_string(score);
		//std::cout << scoreText << std::endl;
		for (char c : scoreText) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
	
		

	}
	else {
		glRasterPos3f(-10, 5, camera.eye.z - 30);
		std::string scoreText = "CONGRATULATIONS! You reached home , Your Score: " + std::to_string(score);
		//std::cout << scoreText << std::endl;
		for (char c : scoreText) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
		}

	}
	

	
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void displayLoseOne() {
	//to stop camera automatic movement
	stop = true; 

	//reset camera to be at the center
	camera.eye.x = 0;
	camera.center.x = 0;

	//draw 
	drawSky();
	drawDeadPlayer();

	//for jake to go down gradually
	glutTimerFunc(100, decsend, 0); //for jake to keep going downward

	//game over text
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos3f(-10, 5, camera.eye.z-30);
	std::string scoreText = "Game Over! You lost your way , Your Score: " + std::to_string(score);

	for (char c : scoreText) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void displayLoseTwo() {

	//to stop camera automatic movement
	stop = true;

	//reset camera to be at the center
	camera.eye.x = 0;
	camera.center.x = 0;
	
	//draw 
	drawUniverse();
	drawDeadPlayer();

	//for jake to go down gradually
	glutTimerFunc(100, decsend, 0); //for jake to keep going downward

	//game over text
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos3f(-10, 5, camera.eye.z - 30);
	std::string scoreText = "Game Over! You lost your way , Your Score: " + std::to_string(score);

	for (char c : scoreText) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupCamera();

	/*GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);*/

	if (displayMode == 0 || displayMode == 1 || displayMode == 2) {
		printScore(scorePosX, scorePosY, scorePosZ, (char*)ScoreArr);
		printHealthBar();
	}

	if (displayMode == 0) {
     	displayEarth();
	}
	else if (displayMode == 1) {
		displaySpace();
	}
	else if (displayMode == 2) {
		displayEndGame();
	}
	else if (displayMode == 3) {
		displayLoseOne();
	}
	else if (displayMode == 4) {
		displayLoseTwo();
	}
	else if (displayMode == 5) {
		displayWin();
		
	}
	else if (displayMode == 6) {
		displayChoosePlayer();
	}



	glutSwapBuffers();
}

//= ======================================================================
// Logic 
//=======================================================================

//sound 
void playBackgroundSound(int value) {
	sndPlaySound(TEXT("themesong.wav"), SND_ASYNC | SND_LOOP | SND_FILENAME);
}

void updateScorePosition(int value) {
	
	scorePosZ = scorePosZ - 5;

	glutPostRedisplay();
	glutTimerFunc(1000, updateScorePosition, 0);
}

void Timer(int value) {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - prevTime;

	if (deltaTime >= 1000) {
		if (timerRunning) {
			if (minutes == 0 && seconds == 0) {
				// Timer has reached zero
				timerRunning = false;
				timerEnd = true;
			}
			else if (seconds == 0) {
				// Decrease minutes and set seconds to 59
				minutes--;
				seconds = 59;
			}
			else {
				// Decrease seconds
				seconds--;
			}
		}

		// Update the previous time
		prevTime = currentTime;
	}

	// Redraw the scene
	glutPostRedisplay();

	// Register the timer function again
	glutTimerFunc(10, Timer, 0);
}

void firstPerson() {
	if (!firstPersonPOV) {
		camera.eye.y = camera.eye.y - 4;
		camera.eye.z = camera.eye.z - 22;

		camera.center.y = camera.center.y - 4;
		camera.center.z = camera.center.z - 22;

		scorePosZ = scorePosZ - 22;
		scorePosY = scorePosY - 4;
		firstPersonPOV = true;

	}


	glutPostRedisplay();
	
}

void thirdPerson() {
	if (firstPersonPOV) {
		camera.eye.y = camera.eye.y + 4;
		camera.eye.z = camera.eye.z + 22;

		camera.center.y = camera.center.y + 4;
		camera.center.z = camera.center.z + 22;

		scorePosZ = scorePosZ + 22;
		scorePosY = scorePosY + 4;

		firstPersonPOV = false;
	}

	glutPostRedisplay();

}

void checkEndGame(int value) {

	if (displayMode == 0) {
		if (movePlayerZ <= -490 && jetTaken == false || health == 0) {
			PlaySound(TEXT("lostGround.wav"), NULL, SND_ASYNC | SND_FILENAME);
			displayMode = 3;

			//to display zombie jake
			if (ChosenPlayer == 0) {
				ChosenPlayer = 4;
			}
		}
	}

	if (displayMode == 1) {
		if (movePlayerZ <= -490 && keyTaken == false || health == 0) {
			PlaySound(TEXT("lostGround.wav"), NULL, SND_ASYNC | SND_FILENAME);
			displayMode = 4;
			if (ChosenPlayer == 0) {
				ChosenPlayer = 4;
			}
		}

	}

	if (displayMode == 2) {
		if (movePlayerZ < -90 && (movePlayerX < -5 || movePlayerX > 5)) {
			PlaySound(TEXT("lostGround.wav"), NULL, SND_ASYNC | SND_FILENAME);
			displayMode = 3;
			if (ChosenPlayer == 0) {
				ChosenPlayer = 4;
			}
		}
		else if (movePlayerZ < -90 && keyTaken == true) {
			PlaySound(TEXT("victory.wav"), NULL, SND_ASYNC | SND_FILENAME);
			displayMode = 5;
		}


	}

	//glutTimerFunc(100, checkEndGame, 0);

}

//while going from one display mode to another as everything drawn from z=0 till z=-500 so i need to reset everything
void resetScene() {
	movePlayerX = 0;
	movePlayerY = 0;
	movePlayerZ = 20;

	camera.eye = Vector3f(0, 9, 40);
	camera.center = Vector3f(0, 0, 0);
	camera.up = Vector3f(0, 1, 0);

	scorePosZ = -50;

	if (firstPersonPOV) {
		camera.eye.y = camera.eye.y - 4;
		camera.eye.z = camera.eye.z - 22;

		camera.center.y = camera.center.y - 4;
		camera.center.z = camera.center.z - 22;

		scorePosZ = scorePosZ - 22;
	}

}

//to let him down to ground when he jumps "gradually"
void restoreJump(int value) {
	if (movePlayerY > 0) {
		movePlayerY = movePlayerY - 0.5;

		if (firstPersonPOV) {
			camera.eye.y = camera.eye.y - 0.5;
			camera.center.y = camera.center.y - 0.5;
		}
		glutTimerFunc(10, restoreJump, 0);
	}
	

}

//when to player collides with an obstacle supposed to rebound depending on his direction (player,camera)
void rebound(int direction) {
	int smallRebound = 6;
	int bigRebound = 9;
	
	if (direction == 0) { //forward --> rebound backward

		/*movePlayerZ = movePlayerZ + 6;
		camera.eye.z = camera.eye.z + 6;
		camera.center.z = camera.center.z + 6;*/

		if (movePlayerX - 6 >= -20 && movePlayerX - 6 <= -15) {
			movePlayerX = movePlayerX + bigRebound;
			camera.eye.x = camera.eye.x + bigRebound;
			camera.center.x = camera.center.x + bigRebound;

			scorePosX = scorePosX + bigRebound;
		}
		else if (movePlayerX - 6 >= -15 && movePlayerX - 6 <= -11) {
			movePlayerX = movePlayerX + smallRebound;
			camera.eye.x = camera.eye.x + smallRebound;
			camera.center.x = camera.center.x + smallRebound;

			scorePosX = scorePosX + smallRebound;

			
		}
		else if (movePlayerX + 6 <= 20 && movePlayerX + 6 >= 15){
			movePlayerX = movePlayerX - bigRebound;
			camera.eye.x = camera.eye.x - bigRebound;
			camera.center.x = camera.center.x - bigRebound;

			scorePosX = scorePosX - bigRebound;
		}
		else {
			movePlayerX = movePlayerX - smallRebound;
			camera.eye.x = camera.eye.x - smallRebound;
			camera.center.x = camera.center.x - smallRebound;

			scorePosX = scorePosX - smallRebound;
		}
	}
	if (direction == 1) { //right --> rebound left
		
		movePlayerX = movePlayerX - smallRebound;
		camera.eye.x = camera.eye.x - smallRebound;
		camera.center.x = camera.center.x - smallRebound;

		scorePosX = scorePosX - smallRebound;
		
	}
	if (direction == 2) { //left --> rebound right
		movePlayerX = movePlayerX + smallRebound;
		camera.eye.x = camera.eye.x + smallRebound;
		camera.center.x = camera.center.x + smallRebound;

		scorePosX = scorePosX + smallRebound;
				
	}
	glutPostRedisplay();
}
void checkCollision(int direction);
void avoidObstacle(int jump) {
	//the jump =the forward translate(for the dog smaller than for the train for example)
	movePlayerY = 7;
	movePlayerZ = movePlayerZ - jump;
	camera.eye.z = camera.eye.z - jump;
	camera.center.z = camera.center.z - jump;
	scorePosZ = scorePosZ - jump; //never forget akhona el score fi ay 7araka lel camera

	if (firstPersonPOV) {
		camera.eye.y = camera.eye.y + 7;
		camera.center.y = camera.center.y + 7;
	}

	checkEndGame(0);
	checkCollision(0);
	glutTimerFunc(10, restoreJump, 0); //nenzel bs bera7a
}

void collCollisionEarth() {
	float distance = 2;
	float distanceX = 1;

	for (int i = 0; i < 25; i++) {
		Vector collectable = coins[i];

		/*std::cout << "collectable X:" << collectable.x << std::endl;
		std::cout << "collectable Y:" << collectable.y << std::endl;
		std::cout << "collectable Z:" << collectable.z << std::endl;

		std::cout << "player X:" << movePlayerX << std::endl;
		std::cout << "player Y:" << movePlayerY << std::endl;
		std::cout << "player Z:" << movePlayerZ << std::endl;
		std::cout << "____________________________________________________________________________________"  << std::endl;*/


		if (movePlayerX - distanceX <= collectable.x && movePlayerX + distanceX >= collectable.x
			&& movePlayerZ - distance <= collectable.z && movePlayerZ + distance >= collectable.z
			&& movePlayerY - distance <= collectable.y && movePlayerY + distance >= collectable.y) {

			
			coins[i] = Vector(100,100,100);
			score = score + scoreGain;
			//sound
			PlaySound(TEXT("coin.wav"), NULL, SND_ASYNC | SND_FILENAME);
			glutTimerFunc(1000, playBackgroundSound, 0);

		}
	}
}

void collCollisionSpace() {
	float distance = 2;

	for (int i = 0; i < 25; i++) {
		Vector collectable = fuel[i];

		/*	std::cout << "collectable X:" << collectable.x << std::endl;
			std::cout << "collectable Y:" << collectable.y << std::endl;
			std::cout << "collectable Z:" << collectable.z << std::endl;

			std::cout << "player X:" << movePlayerX << std::endl;
			std::cout << "player Y:" << movePlayerY << std::endl;
			std::cout << "player Z:" << movePlayerZ << std::endl;
			std::cout << "____________________________________________________________________________________"  << std::endl;*/


		if (movePlayerX - distance <= collectable.x && movePlayerX + distance >= collectable.x
			&& movePlayerZ - distance <= collectable.z && movePlayerZ + distance >= collectable.z
			&& movePlayerY - distance <= collectable.y && movePlayerY + distance >= collectable.y) {

			/*std::cout << "collided" << std::endl;*/
			fuel[i] = Vector(100, 100, 100);
			/*std::cout << fuel[i].x << std::endl;
			std::cout << fuel[i].y << std::endl;
			std::cout << fuel[i].z << std::endl;*/
			score = score + scoreGain;

			//sound
			PlaySound(TEXT("starCollectable.wav"), NULL, SND_ASYNC | SND_FILENAME);
			glutTimerFunc(1000, playBackgroundSound, 0);


		}
	}
}

void jetCollision() {
	float distance = 2;

	if (movePlayerX - distance <= jetPosition.x && movePlayerX + distance >= jetPosition.x
		&& movePlayerZ - distance <= jetPosition.z && movePlayerZ + distance >= jetPosition.z
		&& movePlayerY - distance <= jetPosition.y && movePlayerY + distance >= jetPosition.y) {
		jetPosition = Vector(0, 0, 0);
		resetScene();
		jetTaken = true;
		displayMode = 1;
		//sound
		PlaySound(TEXT("jetpack.wav"), NULL, SND_ASYNC | SND_FILENAME);
		glutTimerFunc(1000, playBackgroundSound, 0);


	}



	
}

void keyCollision() {
	float distance = 2;

	if (movePlayerX - distance <= keyPosition.x && movePlayerX + distance >= keyPosition.x
		&& movePlayerZ - distance <= keyPosition.z && movePlayerZ + distance >= keyPosition.z
		&& movePlayerY - distance <= keyPosition.y && movePlayerY + distance >= keyPosition.y) {
		keyPosition = Vector(0, 0, 0);
		resetScene();
		keyTaken = true;
		displayMode = 2;
		//sound
		PlaySound(TEXT("jetpack.wav"), NULL, SND_ASYNC | SND_FILENAME);
		glutTimerFunc(1000, playBackgroundSound, 0);

	}
}

void disableSneaker(int value) {
	sneakerTaken = false;
}

void sneakerCollision() {
	float distance = 2;

	if (movePlayerX - distance <= sneakersPosition.x && movePlayerX + distance >= sneakersPosition.x
		&& movePlayerZ - distance <= sneakersPosition.z && movePlayerZ + distance >= sneakersPosition.z
		&& movePlayerY - distance <= sneakersPosition.y && movePlayerY + distance >= sneakersPosition.y) {
		sneakersPosition = Vector(0, 0, 0);
		sneakerTaken = true;
		glutTimerFunc(10000, disableSneaker, 0);

		//sound
		PlaySound(TEXT("jetpack.wav"), NULL, SND_ASYNC | SND_FILENAME);
		glutTimerFunc(1000, playBackgroundSound, 0);

	}
}



void obsCollisionEarth(int direction) {
	float distance = 5; //x,y for train
	float distanceZ = 8; //z for train
	float distanceD = 3; //x,y,z for dog

	//Train
	for (int i = 0; i < 10; i++) {
		Vector obstacle = trains[i];

		/*	std::cout << "collectable X:" << collectable.x << std::endl;
			std::cout << "collectable Y:" << collectable.y << std::endl;
			std::cout << "collectable Z:" << collectable.z << std::endl;

			std::cout << "player X:" << movePlayerX << std::endl;
			std::cout << "player Y:" << movePlayerY << std::endl;
			std::cout << "player Z:" << movePlayerZ << std::endl;
			std::cout << "____________________________________________________________________________________"  << std::endl;*/


		if (movePlayerX - distance <= obstacle.x && movePlayerX + distance >= obstacle.x
			&& movePlayerZ - distanceZ <= obstacle.z && movePlayerZ + distanceZ >= obstacle.z
			&& movePlayerY - distance <= obstacle.y && movePlayerY + distance >= obstacle.y) {

			if (sneakerTaken) {  
				avoidObstacle(20);
			}
			else {
				health--;
				rebound(direction);

				//sound
				PlaySound(TEXT("obstacle.wav"), NULL, SND_ASYNC | SND_FILENAME);
				if (health != 0) {
					glutTimerFunc(1000, playBackgroundSound, 0);
				}
			}

			
		}
	}

	//Dog
	if (movePlayerX - distanceD <= dogPosition.x && movePlayerX + distanceD >= dogPosition.x
		&& movePlayerZ - distanceD <= dogPosition.z && movePlayerZ + distanceD >= dogPosition.z
		&& movePlayerY - distanceD <= dogPosition.y && movePlayerY + distanceD >= dogPosition.y) {

		if (sneakerTaken) {
			avoidObstacle(10);
		}
		else {
			health--;
			rebound(direction);

			//sound
			PlaySound(TEXT("dog.wav"), NULL, SND_ASYNC | SND_FILENAME);
			if (health != 0) {
				glutTimerFunc(1000, playBackgroundSound, 0);
			}

		}  //avoidObstacle(7);
	}
}

void obsCollisionSpace(int direction) {
	float distance = 8;
	float distanceW = 5;

	//Plane
	for (int i = 0; i < 10; i++) {
		Vector obstacle = planes[i];

		/*	std::cout << "collectable X:" << collectable.x << std::endl;
			std::cout << "collectable Y:" << collectable.y << std::endl;
			std::cout << "collectable Z:" << collectable.z << std::endl;

			std::cout << "player X:" << movePlayerX << std::endl;
			std::cout << "player Y:" << movePlayerY << std::endl;
			std::cout << "player Z:" << movePlayerZ << std::endl;
			std::cout << "____________________________________________________________________________________"  << std::endl;*/


		if (movePlayerX - distanceW <= obstacle.x && movePlayerX + distanceW >= obstacle.x
			&& movePlayerZ - distance <= obstacle.z && movePlayerZ + distance >= obstacle.z
			&& movePlayerY - distance <= obstacle.y && movePlayerY + distance >= obstacle.y) {
			health--;
			rebound(direction);
			PlaySound(TEXT("obstacle.wav"), NULL, SND_ASYNC | SND_FILENAME);
			if (health != 0) { 
				glutTimerFunc(1000, playBackgroundSound, 0);
			}

			//avoidObstacle(10);
		}
	}
}

void checkCollision(int direction) {
	if (displayMode == 0) {
		collCollisionEarth();
		obsCollisionEarth(direction);
		jetCollision();
		sneakerCollision();
	}
	else if (displayMode == 1) {
		collCollisionSpace();
		obsCollisionSpace(direction);
		keyCollision();
	}
	else if (displayMode == 2) {
		//check for collision with door to end the game
		//khalas done at end game
	}
}



void moveForward(int value) {
	
	if (!stop) {
		
		camera.eye.z = camera.eye.z - 1;
		camera.center.z = camera.center.z - 1;
		movePlayerZ = movePlayerZ - 1;
		scorePosZ = scorePosZ - 1;

		/*std::cout << "eye X:" << camera.eye.x << std::endl;
		std::cout << "eye Y:" << camera.eye.y << std::endl;
		std::cout << "eye z:" << camera.eye.z << std::endl;

		std::cout << "center X:" << camera.center.x << std::endl;
		std::cout << "center Y:" << camera.center.y << std::endl;
		std::cout << "center z:" << camera.center.z << std::endl;

		std::cout << "up X:" << camera.up.x << std::endl;
		std::cout << "up Y:" << camera.up.y << std::endl;
		std::cout << "up z:" << camera.up.z << std::endl;

		std::cout << "player X:" << movePlayerX << std::endl;
		std::cout << "player Y:" << movePlayerY << std::endl;
		std::cout << "player Z:" << movePlayerZ << std::endl;*/

		checkCollision(0);
		checkEndGame(0);
		glutPostRedisplay();
		

	}
	glutTimerFunc(100, moveForward, 0);
	
	
}

void moveLeft() {
	if (movePlayerX > -14) {
		movePlayerX = movePlayerX - 1;
		camera.eye.x = camera.eye.x - 1;
		camera.center.x = camera.center.x - 1;

		scorePosX = scorePosX - 1;
	}
	checkCollision(2);
	checkEndGame(0);
	glutPostRedisplay();
}

void moveRight() {
	if (movePlayerX < 14) {
		movePlayerX = movePlayerX + 1;
		camera.eye.x = camera.eye.x + 1;
		camera.center.x = camera.center.x + 1;

		scorePosX = scorePosX + 1;
	}
	checkCollision(1);
	checkEndGame(0);
	glutPostRedisplay();

}




//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	float dis = 0.1;
	float a = 1.0;

	switch (button)
	{
	case 'w':
		camera.moveY(dis);
		break;
	case 's':
		camera.moveY(-dis);
		break;
	case 'a':
		camera.moveX(dis);
		break;
	case 'd':
		camera.moveX(-dis);
		break;
	case 'q':
		camera.moveZ(dis);
		break;
	case 'e':
		camera.moveZ(-dis);
		break;
	case 'r':
		camera.rotateY(-a);
		break;
	case 'l':
		camera.rotateY(a);
		break;
	case 'm':
		camera.rotateX(a);
		break;
	case 'n':
		camera.rotateX(-a);
		break;
	case 'f':
		firstPerson();
		break;
	case 't':
		thirdPerson();
		break;
	case ' ' :
		avoidObstacle(30);
		break;

}
	glutPostRedisplay();
}


void Special(int key, int x, int y) {
	

	switch (key) {
	case GLUT_KEY_UP:
		//camera.rotateX(a);
		moveForward(0);
		break;
	case GLUT_KEY_DOWN:
		//camera.rotateX(-a);
		avoidObstacle(20);
		break;
	case GLUT_KEY_LEFT:
		//camera.rotateY(a);
		moveLeft();
		break;
	case GLUT_KEY_RIGHT:
		//camera.rotateY(-a);
		moveRight();
		break;
	}

	glutPostRedisplay();
}
//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, camera.center.x, camera.center.y, camera.center.z, camera.up.x, camera.up.y, camera.up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================

void myMouse(int button, int state, int x, int y)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		
	{
		if (gameStarted == false) {

			if (x > 0 && x < 330) {
				ChosenPlayer = 0;
			}
			else if (x > 331 && x < 650) {
				ChosenPlayer = 1;
			}
			else if (x > 651 && x < 850) {
				ChosenPlayer = 2;
			}
			else if (x > 851 && x < 1200) {
				ChosenPlayer = 3;
			}

			gameStarted = true;
			stop = false;
			displayMode = 0;

		}
		else {
			movePlayerY = 7;

			if (firstPersonPOV) {
				camera.eye.y = camera.eye.y + 7;
				camera.center.y = camera.center.y + 7;
			}
			glutTimerFunc(10, restoreJump, 0);
		}
	}

	
	checkCollision(0);

}




//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, camera.center.x, camera.center.y, camera.center.z, camera.up.x, camera.up.y, camera.up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	model_jake.Load("Models/jake/Jake.3ds");
	model_train.Load("Models/Subway/Subway.3ds");
	model_jetpack.Load("Models/Jetpack/Jetpack.3ds");
	model_coin.Load("Models/Coin/Coin.3ds");
	model_dog.Load("Models/dog/Dog.3ds");
	model_plane.Load("Models/Plane/airplane.3ds");
	model_fuel.Load("Models/ScoreBooster/ScoreBooster.3ds");
	model_key.Load("Models/Key_B_02.3ds/Key_B_02.3ds");
	model_zombie.Load("Models/ZombieJake/ZombieJake.3ds");
	
	model_boot.Load("Models/Sneakers/Super Sneakers.3ds");
	model_tricky.Load("Models/Tricky/Tricky.3ds");
	model_ninja.Load("Models/ninja/ninja.3ds");
	model_tagbot.Load("Models/Tagbot/Tagbot.3ds");


	// Loading texture files
	tex_ground.Load("Textures/railway.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	loadBMP(&tex2, "Textures/Galaxy.bmp", true);
}




//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(30, 30);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutMotionFunc(myMotion);

	glutSpecialFunc(Special);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	//new code
	InitPositions();
	glutTimerFunc(100, moveForward, 0);
	glutTimerFunc(10, Timer, 0);
	glutTimerFunc(100, AnimateObjects, 0);
	glutTimerFunc(100, AnimateLight, 0);
	//glutTimerFunc(10000, updateScorePosition, 0);
	//glutTimerFunc(100, checkEndGame, 0);
	highScore = readHighScore();
	
	//new code

	

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	playBackgroundSound(0);

	glutMainLoop();
}