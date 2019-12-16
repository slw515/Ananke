#include "AttractorAttractor.h"

AttractorAttractor::AttractorAttractor()
{
}


AttractorAttractor::~AttractorAttractor()
{
}

void AttractorAttractor::setup(float x, float y, float myMass) {
	pos.set(x, y);
	mass = myMass; 
}

ofVec2f AttractorAttractor::attractionForce(MoverMover m) {
	ofVec2f force = pos - m.pos;
	float distance = force.length();
	distance = ofClamp(distance, 2, 8);//CHANGE? 
	force.normalize();
	distance *= distance;
	float forceMag = (mass*m.mass) / (distance);
	force *= forceMag;
	return force;
}

void AttractorAttractor::display() {
	ofSetColor(255, 255, 0);
	ofFill(); 
	ofDrawEllipse(pos.x, pos.y, 50, 50);
}
