#pragma once
#define Repeller_h

#include "MoverMover.h"
#include <stdio.h>
#include "ofMain.h"

class Repeller
{
public:
	Repeller();
	~Repeller();
	float mass; 
	ofVec2f pos; 
	void setup(float x, float y, float _mass);
	ofVec2f repellingForce(MoverMover m);
	void display(); 
};

