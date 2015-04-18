#include "TimeStepper.hpp"
#include <iostream>

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> states = particleSystem->getState();
	vector<float> ttl = particleSystem->getTtl();
	vector<Vector3f> f = particleSystem->evalF(states, ttl);
	vector<Vector3f> result;
	vector<float> newttl;
	for (unsigned i = 0; i < states.size(); i ++) {
		Vector3f temp = states[i] + f[i] * stepSize;
		if (ttl[i/2] >= 5) {
			temp = Vector3f::ZERO;
		}
		//cout << ttl[i/2] << endl;
		result.push_back(temp);
	}
	for (unsigned i = 0; i < ttl.size(); i ++) {
		//float random = 1?0:(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) > 0.2);
		float temp = ttl[i] + stepSize * 2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // make it random here
		if (temp >= 5.05) temp = 0;
		newttl.push_back(temp); 
	}
	particleSystem->setState(result);
	particleSystem->setTtl(newttl);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> states = particleSystem->getState();
	vector<float> ttl = particleSystem->getTtl();
	vector<Vector3f> f = particleSystem->evalF(states, ttl);
	vector<Vector3f> a;
	vector<Vector3f> b;
	for (unsigned i = 0; i < states.size(); i ++) {
		a.push_back(states[i] + f[i] * stepSize);
	}
	vector<Vector3f> g = particleSystem->evalF(a, ttl);
	for (unsigned i = 0; i < states.size(); i ++) {
		b.push_back(states[i] + (f[i] + g[i]) * stepSize * 0.5);
	}
	particleSystem->setState(b);
}
