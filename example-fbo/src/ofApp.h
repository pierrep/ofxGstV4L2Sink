#pragma once

#include "ofMain.h"
#include "ofxGstV4L2Sink.h"
#include "Particle.h"

// This openFrameworks example is designed to demonstrate how to access the
// webcam.
//
// For more information regarding this example take a look at the README.md.

class ofApp : public ofBaseApp{

    public:

        void setup();
        void update();
        void draw();
        void setupParticles();
        void updateParticles();
        void drawParticles();
        void resetParticles();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);        

        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;

        ofxGstV4L2Sink outputSink;

        ofFbo fbo;
        ofPixels pixels;

        //Particles        
        particleMode currentMode;
        vector <Particle> particles;
        vector <glm::vec3> attractPoints;
        vector <glm::vec3> attractPointsWithMovement;

        //Toggle debug info
        bool bDebug = false;       
};
