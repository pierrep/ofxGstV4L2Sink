#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(100, 100, 100);

    // desired camera resolution
    camWidth = 640;
    camHeight = 480;

    // get a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(camWidth, camHeight);

    virtualCamera.setup(camWidth,camHeight);
}


//--------------------------------------------------------------
void ofApp::update(){

    vidGrabber.update();

    if(vidGrabber.isFrameNew()){                 
        virtualCamera.update(vidGrabber.getPixels());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    vidGrabber.draw(0, 0);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    virtualCamera.close();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
