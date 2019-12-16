

#ifndef Mover_hpp
#define Mover_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Particle.hpp"


class Mover {
public:
    Mover(ofImage& blurImg);
    void setup(int attractorSize, int x, int y);
    //Mover(float x, float y);
    void applyForces(ofVec2f force);
    void update(int imageCounter);
    void display();
    void checkedges();
    ofVec2f position;
    ofVec2f velocity;
    ofVec2f acceleration;
    int index;
    float blueVal = 255;
    float greenVal = 255;
    float redVal = 255;
    ofImage &blurredImgRef;
    

private:
};

#endif /* Mover_hpp */

