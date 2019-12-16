#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofBackground(0);
    ofHideCursor();
	a = ofRandom(20, 100);
     grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
    grabber.setup(640, 480);
    grayImage.allocate(640, 480);

    bSendSerialMessage = true;
    
    font.load("DIN.otf", 24);
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

    int baud = 9600;
    serial.setup(0, baud); //open the first device
    
    nBytesRead = 0;
    memset(onOff, 0, 2);
    //initialize white particles
    for(int i =0; i < num; i++){
      x[i] = ofRandom(-200, ofGetWidth() + 200);
      y[i] = ofRandom(-200, ofGetHeight() + 200);
      vx[i] = 0;
      vy[i] = 0;
      ax[i] = 0;
      ay[i] = 0;
    }
    //initialize circle particle test
    
      for(int i=0;i<num;i++) {
        float x = circRadius*sin(angle*i);
        float y = circRadius*cos(angle*i);
        circleX[i] = x; // Store the x co-ordinate
        circleY[i] = y; // Store the x co-ordinate
      }
    //initialize burst values (explosion)
    for(int i=0;i<num;i++) {
        float x = 1000*sin(angle*i);
        float y = 1000*cos(angle*i);
        randomOutsideX[i] = x; // Store the x co-ordinate
        randomOutsideY[i] = y; // Store the x co-ordinate
      }
    
    //initialize pupil size
    for(int i=0;i<num;i++) {
      float x = 60*sin(angle*i);
      float y = 60*cos(angle*i);
      pupilX[i] = x; // Store the x co-ordinate
      pupilY[i] = y; // Store the x co-ordinate
    }

    int xInc = 0;
    int yInc = 0;
    for (int i = 0; i < num; i++) {
        xInc += 50;
        if(xInc > ofGetWidth()) {
            xInc = 0;
            yInc += 5;
        }
        linesX[i] = xInc;
        linesY[i] = yInc;
    }
    
    
    wavePhase = 0;
    pulsePhase = 0;
    
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    soundStream.setup(settings);
    
    timerValue = 100;
    
    firstTrack.load("firstTrack.mp3");
    nightCall.load("nightcall.mp3");
    disperse.load("disperse.wav");
    disperse.setMultiPlay(false);
    staticTrack.load("static.wav");
    firstTrack.play();
    heartbeat.load("heartbeat.mp3");
    heartbeatShort.load("heartbeatShorter.wav");
    circleSound.load("force.wav");
    circleSound2.load("force2.mp3");

//    circleSound.play();

    feedbackSound.load("echo3.wav");
    feedbackSound.setMultiPlay(true);
    heartbeatShort.setMultiPlay(false);
    
    centerImage.load("eye0.png"); //CHANGE: create and put in "data"
    leftImage.load("eye2.png"); //CHANGE: create and put in "data"
    rightImage.load("eye4.png"); //CHANGE: create and put in "data"
    halfRightImage.load("eye3.png"); //CHANGE: create and put in "data"
    halfLeftImage.load("eye1.png"); //CHANGE: create and put in "data"
    noPupilImage.load("eyesnopupil.png");
    
    attractorSize = attractors.size();
    rightAttractorSize = rightAttractors.size();
    leftAttractorSize = leftAttractors.size();
    halfLeftAttractorSize = halfLeftAttractors.size();
    halfRightAttractorSize = halfRightAttractors.size();
    noPupilSize = middleNoPupil.size();

    attractorsLoc();
    mode = 2;
    
    for (int i = 0; i < 500; i++) {
        MoverMover newMover;
        newMover.setup(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 10);
        //newMover.setup(ofGetWidth()-ofRandom(0,100), ofGetHeight()-ofRandom(0,100)10);
        movers2.push_back(newMover);
    }
    
    blurredParticle.load("dot.png");
}
//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    if(grabber.isFrameNew()) {
        ofPixels pix;
        pix = grabber.getPixels();
        pix.setImageType(OF_IMAGE_GRAYSCALE);
        grayImage.setFromPixels(pix);
        brightestPoint();
    }
    
//	for (int i = 0; i < movers.size(); i++) {
//		movers[i].update(imageCounter);
//	}
    
    if (feedbackVol <= 0) {
        feedbackVol = 0;
    }
    
    if (feedbackVol >= 0.04 && feedbackTrigger == false){
        feedbackSound.play();
        feedbackSound.setVolume(feedbackVol);
        feedbackTrigger = true;
    }
    
    else if (feedbackSound.isPlaying() == true) {
        feedbackTrigger = false;
    }
    
    feedbackVol -= 0.02;
    
    if (states == 4) {
        for (int i = 0; i < particles.size(); i++) {
            particles[i].update();
        }
    }
    if (states == 6) {
        a1.setup(smoothedX, smoothedY, 10); //CHANGE THESE VALUES
        a2.setup(ofGetMouseX() + 100, ofGetMouseY(), 10);
        timeColor -= 0.05;
    }
    if (mouseMode == true) {
        smoothedX = ofGetMouseX();
        smoothedY = ofGetMouseY();
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    if (states <= 4) {
        // base frequency of the lowest sine wave in cycles per second (hertz)
        float mappedFreq = ofMap(colorChange, 50, 350, 0.03, 0.15);
        float frequency = 160.5;
        
        // mapping frequencies from Hz into full oscillations of sin() (two pi)
        float wavePhaseStep = (frequency / outBuffer.getSampleRate()) * TWO_PI;
        float pulsePhaseStep = (0.4
                                / outBuffer.getSampleRate()) * TWO_PI;
        
        // this loop builds a buffer of audio containing 3 sine waves at different
        // frequencies, and pulses the volume of each sine wave individually. In
        // other words, 3 oscillators and 3 LFOs.
        
        for(size_t i = 0; i < outBuffer.getNumFrames(); i++) {
            
            // build up a chord out of sine waves at 3 different frequencies
            float sampleLow = sin(wavePhase);
            float sampleMid = sin(wavePhase * 1.2);
            float sampleHi = sin(wavePhase * 1.5);
            float midMap = ofMap(colorChange, 50, 350, 1.2, 1.6);
            float hiMap = ofMap(colorChange, 50, 350, 1.6, 2);

            // pulse each sample's volume
            sampleLow *= sin(pulsePhase);
            sampleMid *= sin(pulsePhase * midMap);
            sampleHi *= sin(pulsePhase);
            
            float fullSample = (sampleLow + sampleMid + sampleHi);
            
            // reduce the full sample's volume so it doesn't exceed 1
            fullSample *= mappedFreq;
            
            // write the computed sample to the left and right channels
            outBuffer.getSample(i, 0) = fullSample;
            outBuffer.getSample(i, 1) = fullSample;
            
            // get the two phase variables ready for the next sample
            wavePhase += wavePhaseStep;
            pulsePhase += pulsePhaseStep;
        }
        
        unique_lock<mutex> lock(audioMutex);
        lastBuffer = outBuffer;
    }
}
//--------------------------------------------------------------
void ofApp::brightestPoint() {
        float brightestValue = 0;
        int brightestX = 0, brightestY = 0;
        int width = grayImage.getWidth();
        int height = grayImage.getHeight();
        ofPixels& pix = grayImage.getPixels();
        for(int y = 0; y < height; y++) {
          for(int x = 0; x < width; x++) {
            int index = x+y*width;
            if(pix[index] > 40) {
                pix[index] = 255;
            } else {
                pix[index] = 0;
            }
              
            if (pix[index]>brightestValue){
                  brightestValue=pix[index];
                  brightestX=x;
                  brightestY=y;
                  brightestX = ofMap(brightestX, 0, 640, 1280, 0);
                  brightestY = ofMap(brightestY, 0, 480, 720, 0);
              }
          }
        }

    grayImage.flagImageChanged();
//    ofSetColor(255, 0, 0);
//    ofDrawEllipse(brightestX, brightestY, 10, 10);
    contourFinder.findContours(grayImage, 2, (grayImage.getWidth()*grayImage.getHeight())/2, 40, true);
    float mappedBlobX;
    float mappedBlobY;
    if (contourFinder.blobs.size() > 0) {
        mappedBlobX =ofMap(contourFinder.blobs[0].centroid.x, 0, 640, 0, 1280);
        mappedBlobY =ofMap(contourFinder.blobs[0].centroid.y, 0, 480, 0, 720);
        prevBlobX = mappedBlobX;
        prevBlobY = mappedBlobY;
    }
    if (mouseMode == false) {
      smoothedX = ofLerp(smoothedX, prevBlobX, .04);
      smoothedY = ofLerp(smoothedY, prevBlobY, .04);
    }
//        smoothedX = ofMap(smoothedX, 0, grabber.getWidth(), 0, 1024);
//        smoothedY = ofMap(smoothedY, 0, grabber.getHeight(), 0, 768);

}
//--------------------------------------------------------------
void ofApp::draw(){
//    if (onOff[1] == 1) {
//        for (int i = 0; i <= n; i++) {
//            Mover newMover;
//            newMover.setup(smoothedX, smoothedY);
//            movers.push_back(newMover);
//        }
//    }
//    for (int i=0; i<movers.size(); i++){
//        movers[i].display();
//    }
    if (debugMode == true) {
        grabber.draw(0, 0);
        contourFinder.draw(640, 0);
    }
    
    if (debugMode == false) {
        if (states == 0) {
            cout<<colorChangeIncrement<<endl;
            whiteParticles();
            if (colorChange <= 50) {
                colorChangeIncrement *= -1;
            }
            if (colorChange >= 350) {
                colorChangeIncrement *= -1;
            }
            colorChange += colorChangeIncrement;
        }
        if (states == 1 || states==2) {
            colorChange = 290;
            moveToCircle();
            if (circleSound.isPlaying() == false) {
                circleSound.play();
                circleSound.setVolume(0.4);
            }
            if (circleSound2.isPlaying() == false) {
                circleSound2.play();
                circleSound2.setVolume(0.05);
            }
            if (states == 2) {
                circleSound.setVolume(0.2);
                circleSound2.setVolume(0.02);
                if (disperse.isPlaying() == false) {
                    disperse.play();
                }
            }
        }

        if (states == 4) {
            moveToCircle();
            circleSound.stop();
            circleSound2.stop();
            for (int i = 0; i < particles.size(); i++) {
                particles[i].display();
            }
                Particle particle;                            // create the ball object
                particle.setup(smoothedX, smoothedY, 2);//up its initial state
                particles.push_back(particle);                // add it to the list
                for (int j = 0; j < particles.size(); j++) {
                    if (ofGetElapsedTimeMillis() % 50 == 0) {
                        particles.erase(particles.begin());
                    }
                }
                
            
//            if (particleExplosion){
//                Particle particle;                            // create the ball object
//                 particle.setup(smoothedX, smoothedY, 4);//up its initial state
//                 particles.push_back(particle);                // add it to the list
//                for (int j = 0; j < particles.size(); j++) {
////                   if (ofGetElapsedTimeMillis() % 200 == 0) {
////                       particles.erase(particles.begin());
////                   }
//                    particles[j].bounce();
//                   }
//
//
//            }
        }
        if (states == 5 || states == 6 || states == 7) {
            drawImage();
            cout<<imageCounter<<endl;
            imageCounter++;
            float mappedVol = ofMap(imageCounter, 900, 1100, 1, 0);
            firstTrack.setVolume(mappedVol);
            if (imageCounter >= 920) {
                if (firstTrack.isPlaying() == false) {
                    firstTrack.stop();
                }
            }
            if (imageCounter >= 920 && imageCounter <= 1300) {
                if (nightCall.isPlaying() == false) {
                    nightCall.play();
                    nightCall.setVolume(0.1);
                }
                float mappedVol = ofMap(imageCounter, 920, 1200, 0.1, 1);
                nightCall.setVolume(mappedVol);
            }
            if (states == 6) {
//                chaosSketch();
            }
        }
        if (states == 7) {
        }
        
        for (int i = 0; i < particles.size(); i++) {
            particles[i].update();
            particles[i].display();
        }
    }
}
void ofApp::chaosSketch() {
    for (int i = 0; i < movers2.size(); i++) {
//        if (timeColor < 50) {
//            movers2[i].applyForces(r.repellingForce(movers2[i]));
//        }
//        else {
        
        ofVec2f a1Force;
        a1Force.set(a1.attractionForce(movers2[i]));
        ofVec2f a2Force;
        a2Force.set(a2.attractionForce(movers2[i]));
        movers2[i].applyForces(a1Force);
        movers2[i].applyForces(a2Force);

//        }

        movers2[i].update();
        movers2[i].display();
    }
    a1.display();
    a2.display();
}

//--------------------------------------------------------------
void ofApp::drawImage() {
    if (states < 8) {
        if (imageCounter < 1700) {
            moveToCircle();
            if (movers.size() < 800) {
                Mover newnewMover(blurredParticle);
                newnewMover.setup(noPupilSize, smoothedX, smoothedY);
                movers.push_back(newnewMover);
            }
    //        if(imageCounter < 900) {
                for (int i = movers.size() - 1; i >= 0; i--) {
                    for (int j = middleNoPupil.size() - 1; j >= 0; j--) {
                        if (movers[i].index == middleNoPupil[j].index) {
                            ofVec2f force = middleNoPupil[j].attract(movers[i]);
                            movers[i].applyForces(force);
                        }
                    }
                    movers[i].update(imageCounter);
                    movers[i].display();
                }
        }
        
//
//        if (imageCounter > 900 && imageCounter < 1100) {
//            for (int i = movers.size() - 1; i >= 0; i--) {
//                for (int j = attractors.size() - 1; j >= 0; j--) {
//                    if (movers[i].index == attractors[j].index) {
//                        ofVec2f force = attractors[j].attract(movers[i]);
//                        movers[i].applyForces(force);
//                    }
//                }
//                movers[i].update(imageCounter);
//                movers[i].display();
//            }
//        }

        if (imageCounter > 1200) {
            if (imageCounter > 1200) {
                if (imageCounter > 1220) {
                    if (staticTrack.isPlaying() == false) {
                        staticTrack.play();
                    }
                }
                if (dramMusicVol < 0) {
                    nightCall.stop();
                }
                if (heartbeat.isPlaying() == false) {
                    heartbeat.play();
                    heartbeat.setVolume(0.1);
                }
                heartbeatVol = ofMap(imageCounter, 1900, 2300, 0.2, 0.65);
                heartbeat.setVolume(heartbeatVol);

                dramMusicVol = ofMap(imageCounter, 1600, 2100, 1, 0);
                nightCall.setVolume(dramMusicVol);
                staticVol = ofMap(imageCounter, 1220, 2550, 0.2, 1);
                staticTrack.setVolume(staticVol);
            }
            //right
            if (imageCounter > 1700) {
                
            if (!endNow) {
             if (eyepos == 0) {
                for (int j = rightAttractors.size() - 1; j >= 0; j-= 12) {

                    if (endNow) {
                        rightAttractors[j].fall();
                        rightAttractors[j].display(eyePulse);
                        nightCall.stop();
                    }
                    rightAttractors[j].display(eyePulse);
                }

                 if (ofGetElapsedTimeMillis() % 30 == 0) {
                     eyepos = 1;
                     goingRight = false;
                     goingLeft = true;
                 }
            }

            //half right
            if (eyepos == 1) {
                    for (int j = halfRightAttractors.size() - 1; j >= 0; j-=12) {
                        if (goingRight){
                            if (states == 7) {
                                halfRightAttractors[j].fall();
                                halfRightAttractors[j].display(eyePulse);
                                nightCall.stop();
                             }
                        if (ofGetElapsedTimeMillis() % 30 == 0) {
                            halfRightAttractors[j].display(eyePulse);
                            eyepos = 0;
                        }
                        }

                        else if (goingLeft){
                            if (endNow) {
                                halfRightAttractors[j].fall();
                                halfRightAttractors[j].display(eyePulse);
                                nightCall.stop();
                             }
                        if (ofGetElapsedTimeMillis() % 40 == 0) {

                            halfRightAttractors[j].display(eyePulse);
                            eyepos = 2;
                        }
                        }


                    }
                }

            //center
            else if (eyepos == 2) {
                    for (int j = attractors.size() - 1; j >= 0; j-=12) {
                        if (goingRight){
                            if (endNow) {
                                attractors[j].fall();
                                attractors[j].display(eyePulse);
                                nightCall.stop();
                             }
                        if (ofGetElapsedTimeMillis() % 20 == 0) {
                            attractors[j].display(eyePulse);
                            eyepos = 1;
                        }
                        }

                        if (goingLeft){
                            if (endNow) {
                                attractors[j].fall();
                                attractors[j].display(eyePulse);
                                nightCall.stop();
                             }
                            if (ofGetElapsedTimeMillis() % 30 == 0) {
                                attractors[j].display(eyePulse);
                                eyepos = 3;
                            }
                        }
                    }
                }

            //half left
            else if (eyepos == 3) {
                for (int j = halfLeftAttractors.size() - 1; j >= 0; j-= 12) {
                    if (goingRight){
                        if (endNow) {
                            halfLeftAttractors[j].fall();
                            halfLeftAttractors[j].display(eyePulse);
                            nightCall.stop();
                        }
                    if (ofGetElapsedTimeMillis() % 20 == 0) {

                        halfLeftAttractors[j].display(eyePulse);
                        eyepos = 2;
                    }
                    }

                    if (goingLeft){
                        if (endNow) {
                            halfLeftAttractors[j].fall();
                            halfLeftAttractors[j].display(eyePulse);
                            nightCall.stop();
                        }
                    if (ofGetElapsedTimeMillis() % 30 == 0) {

                        halfLeftAttractors[j].display(eyePulse);
                        eyepos = 4;
                    }
                    }

                }
            }

            //left
            else if (eyepos == 4) {
                for (int j = leftAttractors.size() - 1; j >= 0; j-= 12) {
                    if (endNow) {
                        leftAttractors[j].fall();
                        leftAttractors[j].display(eyePulse);
                        nightCall.stop();
                    }
                    leftAttractors[j].display(eyePulse);
                }
                if (ofGetElapsedTimeMillis() % 40 == 0) {
                    eyepos = 3;
                    goingRight = true;
                    goingLeft = false;
                }
            }
        }
            }

            if (endNow == true) {
                for (int j = attractors.size() - 1; j >= 0; j-=12) {
                    attractors[j].fall();
                    attractors[j].display(eyePulse);
                }
                staticTrack.stop();
                heartbeat.stop();
                if (heartbeatPlay == true) {
                    heartbeatShort.play();
                    heartbeatShort.setVolume(1);

                }
            }
        }
        }
}
//--------------------------------------------------------------
void ofApp::attractorsLoc() {
    //img.loadPixels(); CHANGE
    int centerIndex = 0;
    int leftIndex = 0;
    int rightIndex = 0;
    int halfRightIndex = 0;
    int halfLeftIndex = 0;
    int noPupilIndex = 0;

    ofPixels& centerPix = centerImage.getPixels(); //CHECK IF DONE RIGHT
    ofPixels& rightPix = rightImage.getPixels(); //CHECK IF DONE RIGHT
    ofPixels& leftPix = leftImage.getPixels(); //CHECK IF DONE RIGHT
    ofPixels& halfLeftPix = halfLeftImage.getPixels(); //CHECK IF DONE RIGHT
    ofPixels& halfRightPix = halfRightImage.getPixels(); //CHECK IF DONE RIGHT
    ofPixels& noPupilPix = noPupilImage.getPixels(); //CHECK IF DONE RIGHT
    
    for (int y = 0; y < 800; y = y + 1) {
        for (int x = 0; x < 1280; x = x + 1) {
            int loc = x + y * 1280;
            float r = noPupilPix.getColor(x, y).getBrightness();
            if (r < 220) {
                attractor newAttractor(blurredParticle);
                newAttractor.setup(x, y, noPupilIndex);
                middleNoPupil.push_back(newAttractor);
                noPupilIndex++;
            }
        }
    }

    for (int y = 0; y < 800; y = y + 1) {
        for (int x = 0; x < 1280; x = x + 1) {
            int loc = x + y * 1280;
            float r = centerPix.getColor(x, y).getBrightness();
            if (r < 220) {
                attractor newAttractor(blurredParticle);
                newAttractor.setup(x, y, centerIndex);
                attractors.push_back(newAttractor);
                centerIndex++;
            }
        }
    }
    
    for (int y = 0; y < 800; y = y + 1) {
        for (int x = 0; x < 1280; x = x + 1) {
            int loc = x + y * 1280;
            float r = rightPix.getColor(x, y).getBrightness();
            if (r < 220) {
                attractor newAttractor(blurredParticle);
                newAttractor.setup(x, y, rightIndex);
                rightAttractors.push_back(newAttractor);
                rightIndex++;
            }
        }
    }
    
    for (int y = 0; y < 800; y = y + 1) {
        for (int x = 0; x < 1280; x = x + 1) {
            int loc = x + y * 1280;
            float r = leftPix.getColor(x, y).getBrightness();
            if (r < 220) {
                attractor newAttractor(blurredParticle);
                newAttractor.setup(x, y, leftIndex);
                leftAttractors.push_back(newAttractor);
                leftIndex++;
            }
        }
    }
    
    for (int y = 0; y < 800; y = y + 1) {
        for (int x = 0; x < 1280; x = x + 1) {
            int loc = x + y * 1280;
            float r = halfLeftPix.getColor(x, y).getBrightness();
            if (r < 220) {
                attractor newAttractor(blurredParticle);
                newAttractor.setup(x, y, halfLeftIndex);
                halfLeftAttractors.push_back(newAttractor);
                halfLeftIndex++;
            }
        }
    }
    for (int y = 0; y < 800; y = y + 1) {
        for (int x = 0; x < 1280; x = x + 1) {
            int loc = x + y * 1280;
            float r = halfRightPix.getColor(x, y).getBrightness();
            if (r < 220) {
                attractor newAttractor(blurredParticle);
                newAttractor.setup(x, y, halfRightIndex);
                halfRightAttractors.push_back(newAttractor);
                halfRightIndex++;
            }
        }
    }
    noPupilSize = middleNoPupil.size();
    rightAttractorSize = rightAttractors.size();
    leftAttractorSize = leftAttractors.size();
    halfRightAttractorSize = halfRightAttractors.size();
    halfLeftAttractorSize = halfLeftAttractors.size();
    attractorSize = attractors.size(); //CHANGE? CHECK IF ATTRACTORS PROPERLY SET
}

//--------------------------------------------------------------
void ofApp::drawLines() {
    for(int i=0; i<num; i++){
      float distance = ofDist(linesX[i], linesY[i], x[i], y[i]);
      float xPositive = x[i] - smoothedX;
      float yPositive = y[i] - smoothedY;
      float circx = circleX[i] + smoothedX;
      float circy = circleY[i] + smoothedY;
      float mouseDist = ofDist(smoothedX, smoothedY, x[i], y[i]);

      magnetism = 300;

      if (distance > 20){
        ax[i] = magnetism * (linesX[i] - x[i]) / (distance * distance);
        ay[i] = magnetism * (linesY[i] - y[i]) / (distance * distance);
      }
        
      else if (distance < 20) {
        ax[i] = 0.2;
        ay[i] = 0.2;
      }

        if (mouseDist < 90){
            //hardcoded movement of circle points
          ax[i] = magnetism * (circx - x[i]) / (distance * distance);
          ay[i] = magnetism * (circy - y[i]) / (distance * distance);
        }

          vx[i] += ax[i];
          vy[i] += ay[i];
          
          vx[i] = vx[i] * velChange;
          vy[i] = vy[i] * velChange;
          
          x[i] += vx[i];
          y[i] += vy[i];
          
          float distanceParticle = ofDist(0,0,vx[i],vy[i]);
    
          float r = ofMap(distance, 0, colorChange, 255, 50);
          float g = ofMap(distance, 0,colorChange, 255, 154);
          float b = ofMap(distance, 0,colorChange, 255, 128);
          ofSetColor(255, 255, 255);
          ofDrawEllipse(x[i],y[i],radius,radius);
    }
}

//--------------------------------------------------------------
void ofApp::whiteParticles() {
    ofSetColor(0, 0, 0, 20);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    for(int i=0; i<num; i++){
      float distance = ofDist(smoothedX, smoothedY, x[i], y[i]);
     
      if (distance > 3){
        ax[i] = magnetism * (smoothedX - x[i]) / (distance * distance);
        ay[i] = magnetism * (smoothedY - y[i]) / (distance * distance);
      }
      if (distance < 3) {
        x[i] = ofRandom(ofGetWidth());
        y[i] = ofRandom(ofGetHeight());
      }
          vx[i] += ax[i];
          vy[i] += ay[i];
          
          vx[i] = vx[i] * velChange;
          vy[i] = vy[i] * velChange;
          
          x[i] += vx[i];
          y[i] += vy[i];
          
          float distanceParticle = ofDist(0,0,vx[i],vy[i]);
    
          float r = ofMap(distance, 0, colorChange, 255, 50);
          float g = ofMap(distance, 0,colorChange, 255, 154);
          float b = ofMap(distance, 0,colorChange, 255, 128);
          ofSetColor(r, g, b);
          ofDrawEllipse(x[i],y[i],radius,radius);
    }
}
//--------------------------------------------------------------
void ofApp::moveToCircle() {
    if (states == 1) {
        for(int i=0; i<num; i++){
          float circx = circleX[i] + smoothedX;
          float circy = circleY[i] + smoothedY;
          
          float distance = ofDist(circx, circy, x[i], y[i]);
          magnetism = 40;
          if (distance > 3){
              //hardcoded movement of circle points
            ax[i] = magnetism * (circx - x[i]) / (distance * distance);
            ay[i] = magnetism * (circy - y[i]) / (distance * distance);
          }
              vx[i] += ax[i];
              vy[i] += ay[i];
              
              vx[i] = vx[i] * velChange;
              vy[i] = vy[i] * velChange;
              
              x[i] += vx[i];
              y[i] += vy[i];
              
              float distanceParticle = ofDist(0,0,vx[i],vy[i]);
        
              float r = ofMap(distance, 0, colorChange, 255, 50);
              float g = ofMap(distance, 0,colorChange, 255, 154);
              float b = ofMap(distance, 0,colorChange, 255, 128);
              ofSetColor(r, g, b);
              ofDrawEllipse(x[i],y[i],radius,radius);
        }
   }
    else if (states == 2) {
    for(int i=0; i<num; i++){
      float circx = randomOutsideX[i] + smoothedX;
      float circy = randomOutsideY[i] + smoothedY;

      float distance = ofDist(circx, circy, x[i], y[i]);
      magnetism = 450;
      if (distance > 100){
          //hardcoded movement of circle points
        ax[i] = magnetism * (circx - x[i]) / (distance * distance);
        ay[i] = magnetism * (circy - y[i]) / (distance * distance);
      }
        if (distance < 100) {
            x[i] = circx;
            y[i] = circy;
        }
      
          vx[i] += ax[i];
          vy[i] += ay[i];

          vx[i] = vx[i];
          vy[i] = vy[i];

          x[i] += vx[i];
          y[i] += vy[i];

          float distanceParticle = ofDist(0,0,vx[i],vy[i]);

          float r = ofMap(distance, 0, 600, 255, 200);
          float g = ofMap(distance, 0, 600, 255, 200);
          float b = ofMap(distance, 0, 600, 255, 200);
          ofSetColor(r, g, b);
          ofDrawEllipse(x[i],y[i],radius,radius);
        }
    }
    if (states == 4) {
        for (int i = 0; i < num; i++) {
            x[i] = ofRandom(400, 800);
            y[i] = ofRandom(150, 450);
        }
    }
     if (states > 4) {
         for(int i=0; i<num; i++){
             if (imageCounter > 600 && imageCounter < 850) {
                 pupilGreen = ofMap(imageCounter, 700, 850, 255, 0);
                 pupilBlue = ofMap(imageCounter, 700, 850, 255, 0);
             }
             float circx, circy;
             if (imageCounter < 1170) {
                circx = pupilX[i] + 635;
                circy = pupilY[i] + 346;
             }
             if (imageCounter > 1170) {
                 float constrainedX = ofMap(smoothedX, 200, 1080, 580, 690);
                 float constrainedY= ofMap(smoothedY, 150, 570, 310, 370);
                 circx = pupilX[i] + constrainedX;
                 circy = pupilY[i] + constrainedY;
             }
           float distance = ofDist(circx, circy, x[i], y[i]);
           magnetism = 30;
             if (imageCounter > 600) {
               if (distance > 5){
                   //hardcoded movement of circle points
                 ax[i] = magnetism * (circx - x[i]) / (distance * distance);
                 ay[i] = magnetism * (circy - y[i]) / (distance * distance);
                 pupilAlpha = 0;
               }
               else {
                   pupilAlpha = 255;
               }
             }
             else {
                 if (distance > 7){
                     //hardcoded movement of circle points
                   ax[i] = magnetism * (circx - x[i]) / (distance * distance);
                   ay[i] = magnetism * (circy - y[i]) / (distance * distance);
                   pupilAlpha = 0;
                 }
                 else {
                     pupilAlpha = ofMap(imageCounter, 400, 600, 0, 255);
                 }
                 
             }
               vx[i] += ax[i];
               vy[i] += ay[i];
               
               vx[i] = vx[i] * velChange;
               vy[i] = vy[i] * velChange;
               
               x[i] += vx[i];
               y[i] += vy[i];
               
               float distanceParticle = ofDist(0,0,vx[i],vy[i]);
         
               float r = ofMap(distance, 0, colorChange, 255, 50);
               float g = ofMap(distance, 0,colorChange, 255, 154);
               float b = ofMap(distance, 0,colorChange, 255, 128);
               ofSetColor(r, pupilGreen, pupilBlue, pupilAlpha);
//               ofDrawEllipse(x[i],y[i],5,5);
                blurredParticle.draw(x[i], y[i], 10, 10);

         }
    }
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}    

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

void ofApp::keyPressed(int key){
    if (key == '>'){
        timerValue += 5; // do something else
    }
    else if (key == '<'){
        timerValue -= 5; // do something else
    }
    
     else if (key == '-'){
         if (colorChangeIncrement < 0) {
             colorChangeIncrement += 0.5; // do something else
         }
         else if (colorChangeIncrement > 0) {
             colorChangeIncrement -= 0.5; // do something else
         }
     }
     else if (key == '+'){
        if (colorChangeIncrement > 0) {
            colorChangeIncrement += 0.5; // do something else
        }
        else if (colorChangeIncrement < 0) {
            colorChangeIncrement -= 0.5; // do something else
        }
     }
    else if (key == 's'){
        states++;
    }
    else if (key == 'q'){
        states--;
    }
    else if (key == 'a'){
        feedbackVol += 0.1;
    }
    else if (key == 'd'){
        debugMode = !debugMode;
    }
    else if (key == '1') {
        mode = 1;
    }
    else if (key == '2') {
        mode = 2;
    }
    else if (key == 'm') {
        mouseMode = !mouseMode;
    }
    else if (key == 'p'){
        for (int i = 0; i <= n; i++) {
            Particle particle;
            particle.setup(smoothedX, smoothedY, 4);//up its initial stat
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            particle.setup(smoothedX, smoothedY, 4);//up its initial state
            particles.push_back(particle);
            for (int j = 0; j < particles.size(); j++) {
                if (ofGetElapsedTimeMillis() % 50 == 0) {
                    particles.erase(particles.begin());
                }
            }
        }
    }
    
    else if (key == 'e'){
        endNow = true; 
    }
    else if (key == 'h') {
        heartbeatPlay = !heartbeatPlay;
    }
}
