
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
	m_numParticles = 1;
	vector<Vector3f> vec;
	vec.push_back(Vector3f(0.5f, 0.5f, 0.5f));
	setState(vec);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state, vector<float> ttl)
{
	vector<Vector3f> f;

	// YOUR CODE HERE
	for (unsigned i = 0; i < state.size(); i ++) {
		f.push_back(Vector3f(state[i].y() * -1, state[i].x(), 0));
	}

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		Vector3f pos = getState()[0];//YOUR PARTICLE POSITION
	  glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
}
