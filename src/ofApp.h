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
    
        ofVideoGrabber cam;
        Moustache moustache;
        ofRectangle bounds;
        ofImage logo;
        ofImage moustachePicker;
		
        ofxFaceTracker tracker;
        ofVec2f position;
        float scale;
        ofVec3f orientation;
        ofMatrix4x4 rotationMatrix;
    
        Mat translation, rotation;
        ofMatrix4x4 pose;
};
