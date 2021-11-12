#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    ofSetFrameRate(30);

    camWidth = 1280;  // try to grab at this size.
    camHeight = 720;

    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth, camHeight);

    outputSink.setup(camWidth,camHeight,4);

    fbo.allocate(camWidth,camHeight,GL_RGB);
    pixels.allocate(camWidth,camHeight,3);

    setupParticles();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100, 100, 100);
    vidGrabber.update();

    updateParticles();
    fbo.readToPixels(pixels);
    //pixels.mirror(false,true);
    if(vidGrabber.isFrameNew()){                 
        //outputSink.update(vidGrabber.getPixels());
        outputSink.update(pixels);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);

    fbo.begin();
        vidGrabber.draw(0, 0);
        drawParticles();
    fbo.end();

    fbo.draw(0,0);

    if(bDebug) {
        ofSetColor(255);
        ofDrawBitmapString(ofToString(ofGetFrameRate()),20,ofGetHeight()-20);
    }
}

//--------------------------------------------------------------
void ofApp::resetParticles(){

    //these are the attraction points used in the forth demo
    attractPoints.clear();
    for(int i = 0; i < 4; i++){
        attractPoints.push_back( glm::vec3( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) , 0) );
    }

    attractPointsWithMovement = attractPoints;

    for(unsigned int i = 0; i < particles.size(); i++){
        particles[i].setMode(currentMode);
        particles[i].setAttractPoints(&attractPointsWithMovement);
        particles[i].reset();
    }
}

//--------------------------------------------------------------
void ofApp::setupParticles()
{
    int numParticles = 1500;
    particles.assign(numParticles, Particle());
    currentMode = PARTICLE_MODE_ATTRACT;

    resetParticles();
}

//--------------------------------------------------------------
void ofApp::updateParticles()
{
    if(currentMode == PARTICLE_MODE_NONE) return;

    for(unsigned int i = 0; i < particles.size(); i++){
        particles[i].setMode(currentMode);
        particles[i].update();
    }

    //lets add a bit of movement to the attract points
    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles()
{
    if(currentMode == PARTICLE_MODE_NONE) return;

    for(unsigned int i = 0; i < particles.size(); i++){
        particles[i].draw();
    }

    if(bDebug) {
        ofSetColor(190);
        if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
            for(unsigned int i = 0; i < attractPoints.size(); i++){
                ofNoFill();
                ofDrawCircle(attractPointsWithMovement[i], 10);
                ofFill();
                ofDrawCircle(attractPointsWithMovement[i], 4);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 's' || key == 'S'){
        vidGrabber.videoSettings();
    }
    if( key == '1'){
        currentMode = PARTICLE_MODE_ATTRACT;
    }
    if( key == '2'){
        currentMode = PARTICLE_MODE_REPEL;
    }
    if( key == '3'){
        currentMode = PARTICLE_MODE_NEAREST_POINTS;
    }
    if( key == '4'){
        currentMode = PARTICLE_MODE_NOISE;
        resetParticles();
    }

    if( key == ' ' ){
        currentMode = PARTICLE_MODE_NONE;
    }
    if( key == 'd') {
        bDebug = !bDebug;
    }
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
