#include "Particle.hpp"

Particle::Particle() {}

void Particle::setup(float x, float y, int mode) {
	angle = 0;
	alpha = 255;
    greenVal = 255;
    blueVal = 255;
	angularAcceleration = 0;
	angularVelocity = 0;
	position.set(x, y);
	velocity.set(ofRandom(-10, 10), ofRandom(-10, 10));
	acceleration.set(0, 0);
	velocityChange = 0.1;
	currentMode = mode;
	//ofSetBackgroundAuto(false);
}

void Particle::applyForces(ofVec2f & force) {
	auto f = force;
	acceleration += f;
}

void Particle::update() {
	//CHANGING MODES: increase/decrease velocity
	if (currentMode == 1) {
		velocityChange = 0.1;
	}

	if (currentMode == 2) {
		velocityChange = 0.8;
	}

	if (currentMode == 3) {
		velocityChange = 1.5;
	}
    
    if (currentMode == 4){
        velocityChange = 1;
        bounce();
    }
    
	velocity += acceleration;
	position += velocity;

	angularAcceleration = acceleration.x / 50.;
	angularVelocity += angularAcceleration;
	angularVelocity = ofClamp(angularVelocity, -1., .1);
	angle += angularVelocity;
	velocity *= velocityChange;
	angularVelocity *= .9;
	acceleration *= 0;
    alpha -= 5;
    greenVal -= 10;
    blueVal -= 10;
}

void Particle::display() {
	counterBackground++;


        ofSetColor(255, 255, 255, alpha);
        ofFill();
        ofDrawEllipse(position.x, position.y, 5, 5);

    
    if (currentMode == 4){
        greenVal -= 10;
        ofSetColor(255, greenVal, greenVal);
        ofDrawEllipse(position.x, position.y, 5, 5);
    }
}

void Particle::checkedges() {
	if (position.x >= ofGetWidth()) {
//		position.x = ofGetWidth();
		velocity.x *= -1;
	}
	else if (position.x <= 0) {
		velocity.x *= -1;
//		position.x = 0;
	}
	if (position.y >= ofGetHeight()) {
		velocity.y *= -1;
//		position.y = ofGetHeight();
	}
	if (position.y <= 0) {
		velocity.y *= -1;
//		position.y = 0;
	}
}

void Particle::bounce() {

}
