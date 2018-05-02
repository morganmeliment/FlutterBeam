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
    void setFaceVertices(ofPolyline face);
    void draw();
    ofImage moustacheImage;
    std::string moustachePath;
    ofImage moustacheTexture;
    ofImage* morphedImage = new ofImage();
    double angleOfRotation = 0.0;
    std::vector<ofPoint> rotatePoints(std::vector<ofPoint> points);
    double getDistanceFromMouthToNose();
    double getWidthOfFace();
    int heightOffset = 15;
private:
    ofPolyline mouthVertices;
    ofPolyline noseVertices;
    ofPolyline faceVertices;
    void morphMoustache(std::vector<ofPoint> points);
    int height = 30;
    int extraWidth = 15;
};

#endif /* moustache_hpp */
