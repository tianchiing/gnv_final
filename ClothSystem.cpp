#include "ClothSystem.h"
#include <iostream>
using namespace std;

const float DISTANCE = 0.3;
const float MASS = 0.4f;
const float GRAV_CONST = -2.5;
const float DRAG_CONST = 0.3;
const float STRUCT_SPRING_CONST = 100;
const float SHEAR_SPRING_CONST = 40;
const float FLEX_SPRING_CONST = 40;
const float STRUCT_SPRING_LENGTH = DISTANCE;
const float SHEAR_SPRING_LENGTH = DISTANCE * 1.41412;
const float FLEX_SPRING_LENGTH = 2 * DISTANCE;
int size;

int counter = 0;
int direction = 1;

bool hasWind;

//TODO: Initialize here
ClothSystem::ClothSystem()
{
	size = 7; //when size is 8*8*8, begin to lag
	vector<Vector3f> state;
	for (unsigned k = 0; k < size; k ++) {
		for (unsigned i = 0; i < size; i ++) {
			for (unsigned j = 0; j < size; j ++) {
				state.push_back(Vector3f::ZERO);
				state.push_back(Vector3f::ZERO);
			}
		}
	}

	vector<float> ttl;
	for (unsigned k = 0; k < size; k ++) {
		for (unsigned i = 0; i < size; i ++) {
			for (unsigned j = 0; j < size; j ++) {
				ttl.push_back(0.0f);
			}
		}
	}
	
	moving = false;
	wind = false;
	hasWind = wind;
	setState(state);
	setTtl(ttl);
}

Vector3f clothHelper(vector<Vector3f> &state, int i, int j) {
	return Vector3f(0, 0, 0);
	Vector3f position = state[2 * (i * size + j)];
	Vector3f velocity = state[2 * (i * size + j) + 1];
	Vector3f delta;
	// structural spring
	Vector3f struct_spring = Vector3f(0, 0, 0);
	if (i > 0) {
		delta = position - state[2 * ((i - 1) * size + j)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (i < size - 1) {
		delta = position - state[2 * ((i + 1) * size + j)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (j > 0) {
		delta = position - state[2 * (size * i + j - 1)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (j < size - 1) {
		delta = position - state[2 * (size * i + j + 1)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}

	// shear spring
	Vector3f shear_spring = Vector3f::ZERO;
	if (i > 0) {
		if (j > 0) {
			delta = position - state[2 * (size * (i - 1) + j - 1)];		
			shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
		}
		if (j < size - 1) {
			delta = position - state[2 * (size * (i - 1) + j + 1)];
			shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
		}
	}
	if (i < size - 1) {
		if (j > 0) {
			delta = position - state[2 * (size * (i + 1) + j - 1)];
			shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
		}
		if (j < size - 1) {
			delta = position - state[2 * (size * (i + 1) + j + 1)];
			shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
		}
	}

	// flex spring
	Vector3f flex_spring = Vector3f(0, 0, 0);
	if (i > 1) {
		delta = position - state[2 * ((i - 2) * size + j)];
		flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	}
	if (i < size - 2) {
		delta = position - state[2 * ((i + 2) * size + j)];
		flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	}
	if (j > 1) {
		delta = position - state[2 * (size * i + j - 2)];
		flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	}
	if (j < size - 2) {
		delta = position - state[2 * (size * i + j + 2)];
		flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	}
	
	// other forces
	Vector3f G = MASS * Vector3f(0, GRAV_CONST, 0);
	Vector3f D = - DRAG_CONST * velocity;

	Vector3f W = Vector3f(0, 0, 0);
	if (hasWind) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		W = Vector3f(r1 - 0.5, r2 - 0.5, r3 - r2 + 0.5);
	}

	return (struct_spring + shear_spring + flex_spring + G + D + W)/MASS;
	//return W;
}

Vector3f smokeHelper(vector<Vector3f> &state, int i, int j, int k) {

	Vector3f position = state[2 * (i * size + j + k * size * size)];
	Vector3f velocity = state[2 * (i * size + j + k * size * size) + 1];
	Vector3f delta;
	// structural spring
	Vector3f struct_spring = Vector3f(0, 0, 0);
	if (i > 0) {
		delta = position - state[2 * ((i - 1) * size + j + k * size * size)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (i < size - 1) {
		delta = position - state[2 * ((i + 1) * size + j + k * size * size)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (j > 0) {
		delta = position - state[2 * (size * i + j + k * size * size - 1)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (j < size - 1) {
		delta = position - state[2 * (size * i + j + k * size * size + 1)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (k > 0) {
		delta = position - state[2 * (size * i + j + (k - 1) * size * size)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}
	if (k < size - 1) {
		delta = position - state[2 * (size * i + j + (k + 1) * size * size)];
		struct_spring += - STRUCT_SPRING_CONST * (delta.abs() - STRUCT_SPRING_LENGTH) * delta.normalized();
	}

	//// shear spring
	//Vector3f shear_spring = Vector3f::ZERO;
	//if (i > 0) {
	//	if (j > 0) {
	//		delta = position - state[2 * (size * (i - 1) + j - 1)];		
	//		shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
	//	}
	//	if (j < size - 1) {
	//		delta = position - state[2 * (size * (i - 1) + j + 1)];
	//		shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
	//	}
	//}
	//if (i < size - 1) {
	//	if (j > 0) {
	//		delta = position - state[2 * (size * (i + 1) + j - 1)];
	//		shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
	//	}
	//	if (j < size - 1) {
	//		delta = position - state[2 * (size * (i + 1) + j + 1)];
	//		shear_spring += - SHEAR_SPRING_CONST * (delta.abs() - SHEAR_SPRING_LENGTH) * delta.normalized();
	//	}
	//}

	//// flex spring
	//Vector3f flex_spring = Vector3f(0, 0, 0);
	//if (i > 1) {
	//	delta = position - state[2 * ((i - 2) * size + j)];
	//	flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	//}
	//if (i < size - 2) {
	//	delta = position - state[2 * ((i + 2) * size + j)];
	//	flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	//}
	//if (j > 1) {
	//	delta = position - state[2 * (size * i + j - 2)];
	//	flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	//}
	//if (j < size - 2) {
	//	delta = position - state[2 * (size * i + j + 2)];
	//	flex_spring += - FLEX_SPRING_CONST * (delta.abs() - FLEX_SPRING_LENGTH) * delta.normalized();
	//}
	
	// other forces
	Vector3f G = MASS * Vector3f(0, - 0.01 * GRAV_CONST, 0);
	Vector3f D = - DRAG_CONST * velocity;

	Vector3f W = Vector3f(0, 0, 0);
	if (1) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		W = Vector3f(r1 - 0.5, r2 - 0.4, r3 - r2);
	}

	return W;
	//return (struct_spring + G + D + W)/MASS;
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state, vector<float> ttl)
{
	vector<Vector3f> f;
	/*for (int i = 0; i < size; i ++) {
		for (int j = 0; j < size; j ++) {
			if (moving && ((i == 0 && j == 0) || (i == size - 1 && j == 0))) {
				f.push_back(Vector3f(state[2 * (size * i + j) + 1].x(), state[2 * (size * i + j) + 1].y(), state[2 * (size * i + j) + 1].z() + direction));
			}else {
				f.push_back(state[2 * (size * i + j) + 1]);
			}
			if ((i == 0 && j == 0) || (i == size - 1 && j == 0)) {
				f.push_back(Vector3f(0, 0, 0));
			} else {
				f.push_back(clothHelper(state, i, j));
			}
		}
	}
	if (moving) counter += direction;
	if (counter > 150 || counter < -150) {
		direction = - direction;
	}
	hasWind = wind;*/
	counter += direction;

	//cout << state[0] << endl;
	for (int k = 0; k < getState().size()/(2 * size * size); k ++) {
		for (int i = 0; i < size; i ++) {
			for (int j = 0; j < size; j ++) {
				f.push_back(state[2 * (size * i + j + k * size * size) + 1]);
				f.push_back(smokeHelper(state, i, j, k)); //add ttl. motion should be relevant to ttl.
			}
		}
	}
	
	return f;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
	for (int k = 0; k < getState().size()/(2 * size * size); k ++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				// draw lines up and right
				
				Vector3f pos = getState()[2 * (i * size + j + k * size * size)];
				glPushMatrix();
				glTranslatef(pos[0], pos[1], pos[2] );
				glutSolidSphere(0.025f,10.0f,10.0f);
				glPopMatrix();
			}
		}
	}
	
}

