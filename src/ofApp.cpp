#include "ofApp.h"
using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setup(640, 480);
    tracker.setup();
    moustache.moustacheImage.load("images/moustache.png");
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
    //std::cout << minh << std::endl;
    cam.draw(minw + ((ofGetWidth() - minw) / 2), ((ofGetHeight() - minh) / 2), -minw, minh);
    //cam.draw(0, 0);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    
    if(tracker.getFound()) {
        //ofSetLineWidth(1);
        //tracker.draw();
        ofPolyline mouth = tracker.getImageFeature(ofxFaceTracker::Feature::OUTER_MOUTH);
        ofPolyline nose = tracker.getImageFeature(ofxFaceTracker::Feature::NOSE_BASE);
        moustache.setMouthVertices(mouth);
        moustache.setNoseVertices(nose);
        
        bounds = mouth.getBoundingBox();
        
        
        /*ofSetupScreenOrtho(minw, minh, -1000, 1000);
        ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
        
        ofPushMatrix();
        ofScale(5,5,5);
        tracker.getObjectMesh().drawWireframe();
        ofPopMatrix();
        
        applyMatrix(rotationMatrix);
        ofScale(5,5,5);
        tracker.getObjectMesh().drawWireframe();*/
    }
    moustache.draw();
    //
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r') {
        tracker.reset();
    }
}
