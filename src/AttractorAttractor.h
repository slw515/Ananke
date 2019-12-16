#pragma once

#define AttractorAttractor_h

#include "MoverMover.h"
#include <stdio.h>
#include "ofMain.h"

class AttractorAttractor {
public:
	AttractorAttractor();
	~AttractorAttractor(); //CHANGE?
	ofVec2f pos;
	float mass;
	void setup(float x, float y, float myMass);
	ofVec2f attractionForce(MoverMover m);
	void display(); 
};

