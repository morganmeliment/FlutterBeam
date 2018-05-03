#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "Moustache.hpp"
#include <cstdlib>

using namespace ofxCv;
using namespace cv;

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
        void mouseReleased(int x, int y, int button);
    
        // Actions to handle UI events.
        void button1Action();
        void button2Action();
        void button3Action();
    
        // UI and camera elements.
        ofVideoGrabber cam;
        Moustache moustache;
        ofRectangle bounds;
        ofImage logo;
        ofImage moustachePicker;
		
        // Facial recognition elements.
        ofxFaceTracker tracker;
        ofVec2f position;
        float scale;
        ofVec3f orientation;
        ofMatrix4x4 rotationMatrix;
        Mat translation, rotation;
        ofMatrix4x4 pose;
};
