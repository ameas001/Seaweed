// Name: Aranya Meas
// Quarter, Year: Spring 2014
// Assn: 2
//
// This file is to be modified by the student.
// particlesystem.cpp
////////////////////////////////////////////////////////////
// Complete implementation in ParticleSystemSpringMass 
// at the bottom of the file.
// 
#include "particlesystem.h"

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

#include "const.h"

////////////////////////
/// Useful Functions ///
////////////////////////
double randDouble(double min, double max)
{
	return rand() / static_cast<double>(RAND_MAX) * (max - min) + min;
}

/////////////////////////////////////
/// Particle Class Implementation ///
/////////////////////////////////////

Particle::Particle(const Vector3 & p, 
						const Vector3 & v, 
						const Vector3 & a, double m, 
						double t, double sz, const Color4 & c)
	: pos(p), vel(v), acc(a), mass(m), timer(t), size(sz), col(c)
{
}

void Particle::applyForce(const Vector3 & force)
{
	acc += (force / mass);
}

void Particle::applyForces(const std::vector<Vector3> & forces)
{
	Vector3 accumulator = Vector3();
	for (int i = 0; i < forces.size(); ++i)
		accumulator += forces[i];
	acc += (accumulator / mass);
}

void Particle::update(double dt)
{
	if (timer > 0.0) timer -= dt;
	vel += acc * dt;
	pos += vel * dt;
}

void Particle::render() const
{
	glColor4d(col.r, col.g, col.b, col.a);
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3d(pos.x, pos.y, pos.z);
	glEnd();
}

bool Particle::inside(Circle2D c)
{
	if(sqrt(((pos.x - c.p.x) * (pos.x - c.p.x)) + ((pos.y - c.p.y) * (pos.y - c.p.y))) <= c.radius)
		return true;
	else return false;
}

/////////////////////////////////
/// Base Class Implementation ///
/////////////////////////////////

ParticleSystem::ParticleSystem(const Vector3 & startingLocation)
	: location(startingLocation), particles()
{
}

ParticleSystem::~ParticleSystem()
{
	for (int i = 0; i < particles.size(); ++i)
		delete particles[i];
}

void ParticleSystem::update(double dt)
{
	for (int i = 0; i < particles.size(); ++i)
		particles[i]->update(dt);
}

void ParticleSystem::render() const
{
	for (int i = 0; i < particles.size(); ++i)
		particles[i]->render();
}

void ParticleSystem::cleanup()
{
	std::vector<Particle*> remaining(particles.size());
	int nsize = 0;
	for (int i = 0; i < particles.size(); ++i)
	{
		if (particles[i]->timer > 0.0)
		{
			remaining[nsize] = particles[i];
			++nsize;
		}
		else
			delete particles[i];
	}
	remaining.resize(nsize);
	particles = remaining;
}

bool ParticleSystem::isDone() const
{
	return particles.size() <= 0;
}

void ParticleSystem::collision(const Player b)
{
	for(int i = 0; i < particles.size(); ++i)
	{
		if(particles[i]->inside(b.c))
		{
			particles[i]->vel.x += 1.5 * b.vx;
			particles[i]->vel.y += 1.5 * b.vy;
		}
	}
}

void updateParticleSystems(std::vector<ParticleSystem*> & psystems, double dt)
{
	for (int i = 0; i < psystems.size(); ++i)
		psystems[i]->update(dt);
}

void cleanupParticleSystems(std::vector<ParticleSystem*> & psystems)
{
	for (int i = 0; i < psystems.size(); ++i)
		psystems[i]->cleanup();

	std::vector<ParticleSystem*> remaining(psystems.size());
	int nsize = 0;
	for (int i = 0; i < psystems.size(); ++i)
	{
		if (!psystems[i]->isDone())
		{
			remaining[nsize] = psystems[i];
			++nsize;
		}
		else
			delete psystems[i];
	}
	remaining.resize(nsize);
	psystems = remaining;
}

/////////////////////////////////////////////////
/// Sample Implementation of Particle Systems ///
/////////////////////////////////////////////////

ParticleSystemFireworks::ParticleSystemFireworks(const Vector3 & startingLocation)
	: ParticleSystem(startingLocation)
{
	init();
}

ParticleSystemFireworks::~ParticleSystemFireworks()
{
}

void ParticleSystemFireworks::init()
{
	const int NUM_PARTICLES = 100;
	particles = std::vector<Particle*>(NUM_PARTICLES);
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		double randomAngle;

		// Initialize all particles at the provided location
		Vector3 pos = location + Vector3();

		// Generate a random direction
		Vector3 vel = Vector3(0, 1, 0);
		randomAngle = randDouble(-PI / 4, PI / 4);
		vel = vel.rotate(Vector3(1, 0, 0), randomAngle);
		randomAngle = randDouble(-PI / 4, PI / 4);
		vel = vel.rotate(Vector3(0, 0, 1), randomAngle);
		double magnitude = randDouble(50, 150);
		vel *= magnitude;

		// Initialize other attributes
		Vector3 acc = Vector3();
		double mass = 1.0;
		double time = 3.0;
		double size = 2.0;
		Color4 col = Color4(0.0, 1.0, 0.0, 1.0);

		particles[i] = new Particle(pos, vel, acc, mass, time, size, col);
	}
}

void ParticleSystemFireworks::update(double dt)
{
	// Reset by zeroing out the acceleration vector
	for (int i = 0; i < particles.size(); ++i)
	{
		particles[i]->acc = Vector3();
	}
	// Apply force of gravity to all particles
	for (int i = 0; i < particles.size(); ++i)
	{
		particles[i]->applyForce(Vector3(0.0, -98, 0.0));
	}
	ParticleSystem::update(dt);
}

void ParticleSystemFireworks::render() const
{
	ParticleSystem::render();
}

void ParticleSystemFireworks::cleanup()
{
	ParticleSystem::cleanup();
}

bool ParticleSystemFireworks::isDone() const
{
	return ParticleSystem::isDone();
}

////////////////////////////////////////
/// Spring-Mass Class Implementation ///
////////////////////////////////////////

// Spring Joint Constructor
ParticleSystemSpringMass::SpringJoint::SpringJoint(Particle* p1, Particle* p2, double k, double d, double l)
	: particle1(p1), particle2(p2), stiffness(k), length(l), damp(d)
{
}

// Spring Joint function to calculate force
Vector3 ParticleSystemSpringMass::SpringJoint::calculateSpringForce() const
{
	
	return (particle1->pos - particle2->pos).normalized() * stiffness *
	 (((particle1->pos - particle2->pos).magnitude() - length) +
	  (particle1->vel - particle2->vel).dot((particle1->pos - particle2->pos).normalized()) * damp) * -1;
}

// Spring Joint render function
void  ParticleSystemSpringMass::SpringJoint::render() const
{
  glColor4d(0.0, 0.5, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(particle1->pos.x, particle1->pos.y, particle1->pos.z);
	glVertex3d(particle2->pos.x, particle2->pos.y, particle2->pos.z);
	glEnd();
}

// ParticleSystemSpringMass Constructor
ParticleSystemSpringMass::ParticleSystemSpringMass(const Vector3 & startingLocation)
	: ParticleSystem(startingLocation), springConnections()
{
	init();
}

// ParticleSystemSpringMass Destructor
ParticleSystemSpringMass::~ParticleSystemSpringMass()
{
}

// ParticleSystemSpringMass initialization function
void ParticleSystemSpringMass::init()
{
	int w, h;
	w = 5;
	h = 15;
	int NUM_PARTICLES = w * h;
	particles = std::vector<Particle*>(NUM_PARTICLES);
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		// Initialize all particles at the provided location
		
		Vector3 pos = location;
		int j = i / w;
		int k = i % w;
		pos += Vector3(20 * k, 30 * j, 0);

		// Initialize other attributes
		Vector3 vel = Vector3();
		Vector3 acc = Vector3();
		double mass = 1.0;
		double time = 3.0;
		double size = 2.0;
		Color4 col = Color4(0.0, 1.0, 0.0, 1.0);

		particles[i] = new Particle(pos, vel, acc, mass, time, size, col);
	}
	
	int m, n;
	for(m = 0; m < h; m++)
	{
		if(m + 1 < h)
		{
		  for(int o = 0; o < w; o++)
		    springConnections.push_back(SpringJoint(particles[w * m + o], particles[w * (m + 1) + o], 110.0, 0.1, 28.0));
		}
	  for(n = 0; n + 1 < w; n++)
	    springConnections.push_back(SpringJoint(particles[w * m + n], particles[w * m + n + 1], 110.0, 0.1, 18.0));
	}
}

// ParticleSystemSpringMass update function
void ParticleSystemSpringMass::update(double dt)
{
	for(int i = 0; i < particles.size(); ++i)
	{
		particles[i]->acc = Vector3(0.0, 0.0, 0.0);
		if(i == 0 || i == 4 || i == 2)
		{
			particles[i]->vel = Vector3(0.0, 0.0, 0.0);
			particles[i]->mass = 9999999;
		}
		else			
			particles[i]->applyForce(Vector3(0.0, 9.8, 0.0));
	}
	Vector3 springForce;
	for(int i = 0; i < springConnections.size(); ++i)
	{
			springForce = springConnections[i].calculateSpringForce();
			springConnections[i].particle1->applyForce(springForce);
			springConnections[i].particle2->applyForce(springForce * -1);
	}
	ParticleSystem::update(dt);
}

// ParticleSystemSpringMass render function
void ParticleSystemSpringMass::render() const
{
	for(int i = 0; i < springConnections.size(); i++)
		springConnections[i].render();
	ParticleSystem::render();
}

// ParticleSystemSpringMass cleanup function
void ParticleSystemSpringMass::cleanup()
{
	return;
}

// ParticleSystemSpringMass isDone function
bool ParticleSystemSpringMass::isDone() const
{
	return false;
}

void ParticleSystemSpringMass::collision(const Player b)
{
	ParticleSystem::collision(b);
}
