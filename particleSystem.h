#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>

using namespace std;

class ParticleSystem
{
public:

	ParticleSystem(int numParticles=0);

	int m_numParticles;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state, vector<float> ttl) = 0;
	
	// getter method for the system's state
	vector<Vector3f> getState(){ return m_vVecState; };
	
	// setter method for the system's state
	void setState(const vector<Vector3f>  & newState) { 
		m_vVecState = newState;	
	};

	// getter method for the system's ttl
	vector<float> getTtl(){ return m_vVecTtl; };
	
	// setter method for the system's ttl
	void setTtl(const vector<float>  & newState) { 
		m_vVecTtl = newState;	
	};
	
	virtual void draw() = 0;
	
protected:

	vector<Vector3f> m_vVecState;
	vector<float> m_vVecTtl;
};

#endif
