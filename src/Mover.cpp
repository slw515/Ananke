#include "Mover.hpp"

Mover::Mover(ofImage& blurImg) : blurredImgRef(blurImg) {}
   


void Mover::setup(int attractorSize, int x, int y){
    position.set(x, y);
    velocity.set(ofRandom(-1, 1), ofRandom(-1, 1));
    acceleration.set(0, 0);
    index = int(ofRandom(attractorSize)); //CHANGE
}

void Mover::applyForces(ofVec2f force) {
    ofVec2f f = force;
    acceleration = f;
}

void Mover::update(int imageCounter) {
    velocity += acceleration;
    position += velocity;
    velocity *= 0.94; //constrain speed
    acceleration *= 0;
    if (imageCounter > 600 && imageCounter < 850) {
        greenVal = ofMap(imageCounter, 700, 850, 255, 0);
        blueVal = ofMap(imageCounter, 700, 850, 255, 0);
    }
//    else if (imageCounter > 900) {
//        greenVal = 0;
//        blueVal = 0;
//        redVal = ofMap(imageCounter, 900, 1000, 255, 0);
//    }
}

void Mover::display() {
    ofSetColor(redVal, greenVal, blueVal);
    ofFill();
//    ofDrawEllipse(position.x, position.y, 5, 5);
    blurredImgRef.draw(position.x, position.y, 14, 14);
    
}

void Mover::checkedges() {
    if (position.x >= ofGetWidth()) {
        position.x = ofGetWidth();
        velocity.x *= -1;
    }
    else if (position.x <= 0) {
        velocity.x *= -1;
        position.x = 0;
    }
    if (position.y >= ofGetHeight()) {
        velocity.y *= -1;
        position.y = ofGetHeight();
    }
    if (position.y <= 0) {
        velocity.y *= -1;
        position.y = 0;
    }
}
