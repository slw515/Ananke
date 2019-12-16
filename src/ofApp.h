#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "Mover.hpp"
#include "attractor.hpp"
#include "AttractorAttractor.h"
#include "MoverMover.h"
#include "Repeller.h"
#include "Particle.hpp"
#include "ofxPS3EyeGrabber.h"

//#define NUMMOVERS 300

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void brightestPoint();
    void whiteParticles();
    void moveToCircle();
    void drawLines();
    void chaosSketch();

    
    void audioOut(ofSoundBuffer &outBuffer);
    void drawImage();


    void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
    
    int n = 1;
    float distance;
    float a;

    vector <Particle> particles;

    ofxCv::FlowFarneback fb;
    
    //Mover movers[NUMMOVERS];
    
        ofTrueTypeFont        font;
    
    bool bSendSerialMessage;            // a flag for sending serial
    unsigned char inByte;                     // data from serial, we will be trying to read 2
    int nBytesRead;                    // how much did we read?
    int onOff[2];

    ofSerial            serial;
    
    ofxCvContourFinder contourFinder;
    ofVideoGrabber grabber;
    ofxCvGrayscaleImage grayImage; // grayscale depth image

    float smoothedX, smoothedY;
    
    bool isMaking = true;
    //below code for white particles function
    float num = 5000;
    float vx[5000];
    float vy[5000];
    float x[5000];
    float y[5000];
    float ax[5000];
    float ay[5000];
    float pupilsX[5000];
    float pupilsY[5000];
    float magnetism = 10.0;
    float radius = 1.7;
    float velChange = 0.8;
    
    int timer = 0;
    
    bool radiateOut = false;
    
    float colorChange = 300;
    float colorChangeIncrement = 9.0;
    
    float states = -1;
    int pupilAlpha = 255;
    float circRadius = 150; // Size of the circle
    float angle= TWO_PI/5000; // Calculate the angle each point is on
    float circleX[5000];
    float circleY[5000];
    //build explosion code
    float randomOutsideX[5000];
    float randomOutsideY[5000];
    float pupilX[5000];
    float pupilY[5000];

    //drawing the lines with points code
    float linesX[5000];
    float linesY[5000];

    double wavePhase;
    double pulsePhase;

    std::mutex audioMutex;
    ofSoundStream soundStream;
    ofSoundBuffer lastBuffer;
    ofPolyline waveform;
    float rms;
    
    int timerValue;
    float feedbackVol;
    float heartbeatVol;
    ofSoundPlayer circleSound2;
    ofSoundPlayer staticTrack;
    ofSoundPlayer heartbeat;
    ofSoundPlayer firstTrack;
    ofSoundPlayer circleSound;
    ofSoundPlayer nightCall;
    ofSoundPlayer feedbackSound;
    ofSoundPlayer disperse;
    ofSoundPlayer heartbeatShort;

    
    bool feedbackTrigger = false;
    bool heartbeatPlay = false;
    vector <Mover> movers;

    //ATTRACTORS
    vector <attractor> attractors;
    vector <attractor> leftAttractors;
    vector <attractor> rightAttractors;
    vector <attractor> halfLeftAttractors;
    vector <attractor> halfRightAttractors;
    vector <attractor> middleNoPupil;

    ofImage centerImage;
    ofImage noPupilImage;
    ofImage rightImage;
    ofImage leftImage;
    ofImage halfRightImage;
    ofImage halfLeftImage;

    void attractorsLoc();
    
    int attractorSize;
    int noPupilSize;
    int leftAttractorSize;
    int rightAttractorSize;
    int halfLeftAttractorSize;
    int halfRightAttractorSize;

    int mode = 2;
    bool debugMode = false;
    bool mouseMode = false;
    
    vector <MoverMover> movers2;
    AttractorAttractor a1;
    AttractorAttractor a2;
    Repeller r;

    float timeWind;
    float timeColor = 200;
    float dramMusicVol = 1;
    float staticVol;
    int imageCounter = 0;
    int eyepos = 2;
    int eyePulse = 255;
    int eyePulseIncrement = 7;
    int eyePulseTotal = 0;
    int reverseEye = 1;
    
    bool goingLeft = false;
    bool goingRight = true;
    
    //states 4
    int particleStayCounter;
    bool particleExplosion = false;
    
    //final gravity
    bool endNow = false;
    float prevBlobX;
    float prevBlobY;
    int pupilGreen = 255;
    int pupilBlue = 255;
    
    ofImage blurredParticle;
    int heartBeatCounter;
};

