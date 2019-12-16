#include "Repeller.h";

Repeller::Repeller()
{
}


Repeller::~Repeller()
{
}

void Repeller::setup(float x, float y, float _mass) {
	pos.set(x, y);
	mass = _mass;
}

ofVec2f Repeller::repellingForce(MoverMover m) {
	ofVec2f force = pos - m.pos; //CHANGE: VECTOR SUBTRACTION OK? 
	float distance = force.length();
	force.normalize();
	distance *= distance;
	distance = ofClamp(distance, 5, 20); //CHANGE? 
	float forceMag = (mass*m.mass) / (distance*distance);
	float newForceMag = (forceMag * -1);
	force *= newForceMag; //CHANGE: double check
	return force;
}

void Repeller::display() {
	ofFill(); 
	ofSetColor(0, 0, 255);
	ofDrawEllipse(pos.x, pos.y, 10, 10);
}
