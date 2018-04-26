//
//  moustache.hpp
//  FlutterBeam
//
//  Created by Morgan Meliment on 4/23/18.
//

#ifndef moustache_hpp
#define moustache_hpp

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

using namespace ofxCv;
using namespace cv;

class Moustache {
public:
    void setMouthVertices(ofPolyline mouth);
    void setNoseVertices(ofPolyline nose);
    void draw();
    ofImage moustacheImage;
    ofImage* morphedImage = new ofImage();
private:
    ofPolyline mouthVertices;
    ofPolyline noseVertices;
    void morphMoustache(std::vector<ofPoint> points);
    int height = 30;
    int extraWidth = 15;
};

#endif /* moustache_hpp */
