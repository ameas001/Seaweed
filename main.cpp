// Name: Aranya Meas
// Quarter, Year: Spring 2014
// Assignment 2
//
// This file is to be modified by the student.
// main.cpp
////////////////////////////////////////////////////////////
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <GL/glut.h>

#include "const.h"
#include "color.h"
#include "vector3.h"
#include "particlesystem.h"
#include "object.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const double VIEW_LEFT = 0.0;
const double VIEW_RIGHT = WINDOW_WIDTH;
const double VIEW_BOTTOM = 0.0;
const double VIEW_TOP = WINDOW_HEIGHT;
const double VIEW_FRONT = -800;
const double VIEW_BACK = 800;

Point2D p = Point2D(400.0, 400.0);
Circle2D circle = Circle2D(p, 25.0, Color(1.0, 0.7, 0.0), true);
Player balls = Player(circle, 0.0, 0.0);

std::vector<Player> schoolFish;

Color colors(1.0, 0.5, 0.0);

int currentTime = 0;
int previousTime = 0;

std::vector<ParticleSystem*> psystems;

void GLrender();
void SchoolOfFish();
void seaweed();
void GLupdate();
void GLprocessMouse(int button, int state, int x, int y);

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Assn2 - Aranya Meas");
	glutDisplayFunc(GLrender);
	glutIdleFunc(GLupdate);
	//glutMouseFunc(GLprocessMouse);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glOrtho(VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP, VIEW_FRONT, VIEW_BACK);
	SchoolOfFish();
	seaweed();
}

//Makes my school of fish :D
void SchoolOfFish()
{
	schoolFish.push_back(Player(Circle2D(Point2D(100.0, 150.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(200.0, 150.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(150.0, 200.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(150.0, 100.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(150.0, 150.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(234.0, 150.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(150.0, 175.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(215.0, 173.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(175.0, 174.0), 5.0, ORANGE, true), 1.0, 0.5));
	schoolFish.push_back(Player(Circle2D(Point2D(170.0, 126.0), 5.0, ORANGE, true), 1.0, 0.5));
}

//Makes my seaweed :D
void seaweed()
{
	psystems.push_back(new ParticleSystemSpringMass(Vector3(190, 0, 0)));
	psystems.push_back(new ParticleSystemSpringMass(Vector3(123, 0, 0)));
	psystems.push_back(new ParticleSystemSpringMass(Vector3(321, 0, 0)));
	psystems.push_back(new ParticleSystemSpringMass(Vector3(543, 0, 0)));
	psystems.push_back(new ParticleSystemSpringMass(Vector3(654, 0, 0)));
}

//Sets the bounds for the main fish
void bounds()
{
	if(balls.c.p.y + balls.c.radius > WINDOW_HEIGHT)
	{
		balls.vy *= -1;
		balls.c.p.y = WINDOW_HEIGHT - balls.c.radius;
	}
	else if(balls.c.p.x + balls.c.radius > WINDOW_WIDTH)
	{
		balls.vx *= -1;
		balls.c.p.x = WINDOW_WIDTH - balls.c.radius;
	}
	else if(balls.c.p.x - balls.c.radius < 0)
	{
		balls.vx *= -1;
		balls.c.p.x = balls.c.radius;
	}
	else if(balls.c.p.y - balls.c.radius < 0)
	{
		balls.vy *= -1;
		balls.c.p.y = balls.c.radius;
	}
	
	/////////////////
	for(int i = 0; i < psystems.size(); ++i)
		psystems[i]->collision(balls);
}

//Sets the bounds for the school of fish
void bounds(int a)
{
	if(schoolFish[a].c.p.y + schoolFish[a].c.radius > WINDOW_HEIGHT)
	{
		schoolFish[a].vy *= -1;
		schoolFish[a].c.p.y = WINDOW_HEIGHT - schoolFish[a].c.radius;
	}
	else if(schoolFish[a].c.p.x + schoolFish[a].c.radius > WINDOW_WIDTH)
	{
		schoolFish[a].vx *= -1;
		schoolFish[a].c.p.x = WINDOW_WIDTH - schoolFish[a].c.radius;
	}
	else if(schoolFish[a].c.p.x - schoolFish[a].c.radius < 0)
	{
		schoolFish[a].vx *= -1;
		schoolFish[a].c.p.x = schoolFish[a].c.radius;
	}
	else if(schoolFish[a].c.p.y - schoolFish[a].c.radius < 0)
	{
		schoolFish[a].vy *= -1;
		schoolFish[a].c.p.y = schoolFish[a].c.radius;
	}
	
	/////////////////
	for(int i = 0; i < psystems.size(); ++i)
		psystems[i]->collision(schoolFish[a]);
}

void GLKeyboard(unsigned char key, int x, int y)
{
    if(key == 'w')
    {
        balls.up = true;
    }
    else if(key == 'a')
    {
        balls.left = true;
    }
    else if(key == 's')
    {
        balls.down = true;
    }
    else if(key == 'd')
    {
        balls.right = true;
    }
    else if(key == 'q')
    {
		balls.vy = 0;
		balls.vx = 0;
	}
}

void release(unsigned char key, int x, int y)
{
    if(key == 'w')
    {
        balls.up = false;
    }
    else if(key == 'a')
    {
        balls.left = false;
    }
    else if(key == 's')
    {
        balls.down = false;
    }
    else if(key == 'd')
    {
        balls.right = false;
    }
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	GLInit(&argc, argv);
	glutKeyboardFunc(GLKeyboard);
	glutKeyboardUpFunc(release);
	glutMainLoop();
	return 0;
}

void GLupdate()
{
	const int FRAME_RATE = 25;
	
	double dt = FRAME_RATE / 1000.0;
	updateParticleSystems(psystems, dt);
	cleanupParticleSystems(psystems);

	glutPostRedisplay();

	//sleep is not effective in capturing constant time between frames because sleep
	//doesn't consider the time it takes for context-switching. However, this reduces
	//the cpu-usage. If accurate time frames are desire, use a time accumulator
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int diffTime = currentTime - previousTime;
	previousTime = currentTime;
	usleep(1000 * std::max(FRAME_RATE - diffTime, 0));
	
	double x = 0, y = 0;
	
	if(balls.up)
	    y += 0.1;
	if(balls.down)
	    y -= 0.1;
	if(balls.left)
	    x -= 0.1;
	if(balls.right)
	    x += 0.1;	
	
    balls.updatePlayer(x, y);
    for(int i = 0; i < schoolFish.size(); ++i)
    {
		schoolFish[i].updatePlayer(0.0, 0.0);
		bounds(i);
	}
    bounds();
}

void GLrender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < psystems.size(); ++i)
		psystems[i]->render();

	balls.c.render();
	for(int i = 0; i < schoolFish.size(); ++i)
		schoolFish[i].c.render();
	glFlush();	
	glutSwapBuffers();
}
