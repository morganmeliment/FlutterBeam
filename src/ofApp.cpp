#include "ofApp.h"
using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setup(640, 480);
    tracker.setup();
    moustache.moustacheImage.load("images/moustache.png");
    moustache.moustacheTexture.load("images/moustache.png");
    logo.load("images/piedPiperLogo.png");
    moustachePicker.load("images/moustachePicker.png");
    moustache.moustachePath = "images/moustache.png";
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
    //ofDrawBitmapString("Press Space To Take A Picture", 10, 20);
    
    if(tracker.getFound()) {
        //ofSetLineWidth(1);
        //tracker.draw();
        ofPolyline mouth = tracker.getImageFeature(ofxFaceTracker::Feature::OUTER_MOUTH);
        ofPolyline nose = tracker.getImageFeature(ofxFaceTracker::Feature::NOSE_BASE);
        ofPolyline face = tracker.getImageFeature(ofxFaceTracker::Feature::FACE_OUTLINE);
        moustache.setMouthVertices(mouth);
        moustache.setNoseVertices(nose);
        moustache.setFaceVertices(face);
        
        //bounds = mouth.getBoundingBox();
        
    }
    moustache.draw();
    
    logo.load("images/piedPiperLogo.png");
    int logoWidth = (ofGetWidth() * 0.2) + 20;
    int logoHeight = (logoWidth / logo.getWidth()) * logo.getHeight();
    logo.resize(logoWidth, logoHeight);
    logo.draw(ofGetWidth() - logo.getWidth(), ofGetHeight() - logo.getHeight());
    
    moustachePicker.load("images/moustachePicker.png");
    int pickerHeight = (ofGetHeight() * 0.4) + 50;
    int pickerWidth = (pickerHeight / moustachePicker.getHeight()) * moustachePicker.getWidth();
    moustachePicker.resize(pickerWidth, pickerHeight);
    moustachePicker.draw(20, (ofGetHeight() - moustachePicker.getHeight()) / 2);
    /*double baseHeight = (ofGetHeight() - moustachePicker.getHeight()) / 2;
    double divHeight = pickerHeight * 0.05;
    double circleHeight = (moustachePicker.getHeight() - 4 * divHeight) / 3;
    double leftBound = 20 + pickerWidth * 0.1;
    double rightBound = 20 + pickerWidth * 0.9;
    ofRect(leftBound, baseHeight + divHeight, 2, 2);
    ofRect(rightBound, baseHeight + divHeight, 2, 2);
    ofRect(leftBound, baseHeight + divHeight + circleHeight, 2, 2);
    ofRect(rightBound, baseHeight + divHeight + circleHeight, 2, 2);
    ofRect(leftBound, baseHeight + 2 * divHeight + circleHeight, 2, 2);
    ofRect(rightBound, baseHeight + 2 * divHeight + circleHeight, 2, 2);
    ofRect(leftBound, baseHeight + 2 * divHeight + 2 * circleHeight, 2, 2);
    ofRect(rightBound, baseHeight + 2 * divHeight + 2 * circleHeight, 2, 2);
    ofRect(leftBound, baseHeight + 3 * divHeight + 2 * circleHeight, 2, 2);
    ofRect(rightBound, baseHeight + 3 * divHeight + 2 * circleHeight, 2, 2);
    ofRect(leftBound, baseHeight + 3 * divHeight + 3 * circleHeight, 2, 2);
    ofRect(rightBound, baseHeight + 3 * divHeight + 3 * circleHeight, 2, 2);*/
}

string generateScreenShotFilePath() {
    ofFile screenShot = ofFile();
    string screenShotFullPath = screenShot.getAbsolutePath();
    string screenShotNewPath = "";
    
    int depth = 0;
    int charIndex = 0;
    while (depth < 3) {
        screenShotNewPath += screenShotFullPath.at(charIndex);
        
        if (screenShotFullPath.at(charIndex) == '/') {
            depth++;
        }
        charIndex++;
    }
    screenShotNewPath += "Desktop/";
    
    string screenShotName = "MoustachePic_" + std::to_string(((rand() % 100000) + 1)) + ".png";
    screenShotNewPath += screenShotName;
    
    return screenShotNewPath;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r') {
        tracker.reset();
    } else if (key == ' ') {
        ofFile tempFile = ofFile(generateScreenShotFilePath());
        
        while (tempFile.exists()) {
            tempFile = ofFile(generateScreenShotFilePath());
        }

        ofImage screenShotImage = ofImage();
        screenShotImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        ofPixels cameraPixels = cam.getPixels();
        cameraPixels.mirror(false, true);
        int minh = ofGetHeight();
        
        if (minh < (ofGetWidth() * 9 / 16)) {
            minh = ofGetWidth() * 9 / 16;
        }
        int minw = minh * 16 / 9;
        
        int iOffset = abs((ofGetWidth() - minw) / 2);
        int jOffset = abs((ofGetHeight() - minh) / 2);
        cameraPixels.resize(minw, minh);
        ofImage cameraPicture = ofImage();
        cameraPicture.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
        cameraPicture.setFromPixels(cameraPixels);
        cameraPicture.update();
        
        int pickerHeight = (ofGetHeight() * 0.4) + 50;
        int pickerWidth = (pickerHeight / moustachePicker.getHeight()) * moustachePicker.getWidth();

        ofImage finalImage = ofImage();
        finalImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
        
        int logoWidth = (ofGetWidth() * 0.2) + 20;
        int logoHeight = (logoWidth / logo.getWidth()) * logo.getHeight();
        
        for (int i = 0; i < finalImage.getWidth(); i++) {
            for (int j = 0; j < finalImage.getHeight(); j++) {
                if ((i >= 20 && i <= 20 + pickerWidth) && (j >= (ofGetHeight() - pickerHeight) / 2 && j <= ofGetHeight() - (ofGetHeight() - pickerHeight) / 2)) {
                    finalImage.setColor(i, j, cameraPicture.getColor(i + iOffset, j + jOffset));
                } else if (i >= ofGetWidth() - logoWidth && j >= ofGetHeight() - logoHeight) {
                    finalImage.setColor(i, j, cameraPicture.getColor(i + iOffset, j + jOffset));
                } else {
                    finalImage.setColor(i, j, screenShotImage.getColor(i, j));
                }
            }
        }
        finalImage.update();
        
        finalImage.save(tempFile.path());
        //tempFile.create();
    }
}

void ofApp::mouseReleased(int x, int y, int button) {
    int pickerHeight = (ofGetHeight() * 0.4) + 50;
    int pickerWidth = (pickerHeight / moustachePicker.getHeight()) * moustachePicker.getWidth();
    double baseHeight = (ofGetHeight() - moustachePicker.getHeight()) / 2;
    double divHeight = pickerHeight * 0.05;
    double circleHeight = (moustachePicker.getHeight() - 4 * divHeight) / 3;
    double leftBound = 20 + pickerWidth * 0.1;
    double rightBound = 20 + pickerWidth * 0.9;
    if (leftBound < x && rightBound > x) {
        if (baseHeight + divHeight <= y
            && y <= baseHeight + divHeight + circleHeight) {
            moustache.moustacheImage.load("images/colorStache.png");
            moustache.moustacheTexture.load("images/colorStache.png");
            moustache.moustachePath = "images/colorStache.png";
            moustache.heightOffset = 15;
        } else if (baseHeight + 2 * divHeight + circleHeight <= y
                   && y <= baseHeight + 2 * divHeight + 2 * circleHeight) {
            moustache.moustacheImage.load("images/moustache.png");
            moustache.moustacheTexture.load("images/moustache.png");
            moustache.moustachePath = "images/moustache.png";
            moustache.heightOffset = 15;
        } else if (baseHeight + 3 * divHeight + 2 * circleHeight <= y
                   && y <= baseHeight + 3 * divHeight + 3 * circleHeight) {
            moustache.moustacheImage.load("images/uglyStache.png");
            moustache.moustacheTexture.load("images/uglyStache.png");
            moustache.moustachePath = "images/uglyStache.png";
            moustache.heightOffset = 20;
        }
    }
}
