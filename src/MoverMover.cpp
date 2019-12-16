#include "MoverMover.h"

MoverMover::MoverMover()
{
}


MoverMover::~MoverMover()
{
}

void MoverMover::setup(float x, float y, float myMass) {
	pos.set(x, y);
	vel.set(0, 0);
	acc.set(0, 0);
	mass = myMass; 
}

void MoverMover::applyForces(ofVec2f & force) {
	ofVec2f f = force / mass;
	acc += f;
}

void MoverMover::update() {
	vel += acc;
	pos += vel;
	acc *= 0; 
	//vel *= 0.97; //constrain speed
}

void MoverMover::display() {
	ofFill();
	ofSetColor(255, 255, 255);
	ofDrawEllipse(pos.x, pos.y, 5, 5);
}

void MoverMover::checkedges() {
	if (pos.x >= ofGetWidth()) {
		pos.x = ofGetWidth();
		vel.x *= -1;
	}
	else if (pos.x <= 0) {
		vel.x *= -1;
		pos.x = 0;
	}
	if (pos.y >= ofGetHeight()) {
		vel.y *= -1;
		pos.y = ofGetHeight();
	}
	if (pos.y <= 0) {
		vel.y *= -1;
		pos.y = 0;
	}
}