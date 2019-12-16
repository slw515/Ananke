#pragma once
#define MoverMover_h
#include <stdio.h>
#include "ofMain.h"

class MoverMover
{
public:
	MoverMover();
	~MoverMover();
	ofVec2f pos, vel, acc;
	float mass;

	void setup(float x, float y, float myMass);
	void applyForces(ofVec2f & force);
	void update();
	void display();
	void checkedges();
};

