
#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"

class Particle {
public:
	Particle();
	void setup(float x, float y, int mode);
	//Mover(float x, float y);
	void applyForces(ofVec2f & force);
	void update();
	void display();
	void checkedges();
    void bounce(); 
	float velocityChange;
	ofVec2f position;
	int currentMode;
	float angle;
	float angularAcceleration;
    float greenVal, blueVal;
	float angularVelocity;
	ofVec2f velocity;
	ofVec2f acceleration;
	float alpha;
	int counterBackground;

};

#endif /* Mover_hpp */
