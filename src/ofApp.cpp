#include "ofApp.h"
using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setup(640, 480);
    tracker.setup();
    moustache.moustacheImage.load("images/uglyStache.png");
    moustache.moustacheTexture.load("images/uglyStache.png");
    logo.load("images/piedPiperLogo.png");
    moustache.moustachePath = "images/uglyStache.png";
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();

    if(cam.isFrameNew()) {
        tracker.update(toCv(cam));
        position = tracker.getPosition();
        scale = tracker.getScale();
        orientation = tracker.getOrientation();
        rotationMatrix = tracker.getRotationMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    int minh = ofGetHeight();
    
    if (minh < (ofGetWidth() * 9 / 16)) {
        minh = ofGetWidth() * 9 / 16;
    }
    int minw = minh * 16 / 9;
    cam.draw(minw + ((ofGetWidth() - minw) / 2), ((ofGetHeight() - minh) / 2), -minw, minh);
    //ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    
    if(tracker.getFound()) {
        //ofSetLineWidth(1);
        //tracker.draw();
        ofPolyline mouth = tracker.getImageFeature(ofxFaceTracker::Feature::OUTER_MOUTH);
        ofPolyline nose = tracker.getImageFeature(ofxFaceTracker::Feature::NOSE_BASE);
        moustache.setMouthVertices(mouth);
        moustache.setNoseVertices(nose);
        
        //bounds = mouth.getBoundingBox();
        
    }
    moustache.draw();
    
    logo.load("images/piedPiperLogo.png");
    int logoWidth = (ofGetWidth() * 0.2) + 20;
    int logoHeight = (logoWidth / logo.getWidth()) * logo.getHeight();
    logo.resize(logoWidth, logoHeight);
    logo.draw(ofGetWidth() - logo.getWidth(), ofGetHeight() - logo.getHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r') {
        tracker.reset();
    }
}
