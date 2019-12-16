//
//  attractor.cpp
//  opticalFlow_squares
//
//  Created by Steven Wyks on 12/4/19.
//

#include "attractor.hpp"

attractor::attractor(ofImage& blurImg) : blurredImgRef(blurImg) {

}


//attractor::~attractor()
//{}

void attractor::setup(float x, float y, int i) {
    pos.set(x, y);
    index = i;
    velocity.set(ofRandom(-1, 1), ofRandom(-1, 1));
    acceleration.set(0, 0);
}

ofVec2f attractor::attract(Mover m) {
    ofVec2f mouse;
    mouse.set(ofGetMouseX(), ofGetMouseY());
    ofVec2f f = pos - m.position;
    float d = f.length();
    d = ofClamp(d, 5, 25);
    float strength = strengthDivider / (d*d);
    f.normalize();
    f *= strength;
    return f;
}

void attractor::display(int eyeState) {
    ofFill();
    ofSetColor(eyeState, 0, 0);
    
    blurredImgRef.draw(pos.x + ofRandom(-10, 10), pos.y + ofRandom(-10, 10), 10, 10);
//    cout<<movement<<endl;
    if (movement >= 15) {
        movementIncrement *= -1;
    }
    if (movement <= -15) {
        movementIncrement *= -1;
    }
    movement += movementIncrement;
}

void attractor::fall() {
    ofVec2f gravity = ofVec2f(0, ofRandom(0.15, 0.8));
    velocity += gravity;
    pos += velocity;
    velocity *= 1.15; //constrain speed
    acceleration *= 0;
}
