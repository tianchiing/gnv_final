
#include "pendulumSystem.h"
#include<iostream>
using namespace std;

const float REST_LENGTH = 0.25; // the initial distance between two particles
const float SPRING_CONST = 5.0;
const float DRAG_CONST = 0.1;
const float MASS = 1.0;
const float GRAV_CONST = -1.0;

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	vector<Vector3f> state;
	
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		// for this system, we care about the position and the velocity
		state.push_back(Vector3f(0.5*i + 2, 0.5*i + 2, 0.5*i + 2));
		state.push_back(Vector3f(0, 0, 0));
	}
	
	setState(state);
}

Vector3f forceHelper(vector<Vector3f>& state, int i) {
	Vector3f x = state[2 * i];
	Vector3f v = state[2 * i + 1];
	Vector3f gravity = MASS * Vector3f(0, GRAV_CONST, 0);
	Vector3f drag = -DRAG_CONST * v;
	Vector3f spring = Vector3f::ZERO;
	if (i > 0) {
		Vector3f diff = x - state[2 * i - 2];
		spring += -SPRING_CONST * (diff.abs() - REST_LENGTH) * diff.normalized();
	}
	if (i < state.size()/2 - 1) {
		Vector3f diff = x - state[2 * i + 2];
		spring += -SPRING_CONST * (diff.abs() - REST_LENGTH) * diff.normalized();
	}
	return (gravity + drag + spring) / MASS;
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state, vector<float> ttl)
{
	vector<Vector3f> f;
	// YOUR CODE HERE
	f.push_back(state[1]);
	f.push_back(Vector3f(0, 0, 0));
	for (unsigned i = 1; i < state.size()/2; i ++) {
		Vector3f v = state[i * 2 + 1];
		f.push_back(v);
		f.push_back(forceHelper(state, i));
	}

	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = getState()[2*i];//  position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
