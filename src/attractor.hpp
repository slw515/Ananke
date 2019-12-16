#pragma once

#define attractor_h

#include <stdio.h>
#include "ofMain.h"
#include "Mover.hpp"
#include "Particle.hpp"


class attractor{
public:
    attractor(ofImage& blurImg);
    ofImage &blurredImgRef;

    //~attractor(); //CHANGE?
    ofVec2f pos;
    int index;
    ofVec2f velocity;
    ofVec2f acceleration;
    void setup(float x, float y, int i);
    void fall();
    

    ofVec2f attract(Mover m);
    void display(int eyeState);
    float movement = 0;
    float movementIncrement = ofRandom(-1, 1);
    
    float strengthDivider = 80;
    float howRed = 255;

};
