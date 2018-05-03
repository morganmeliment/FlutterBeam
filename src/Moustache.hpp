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
    // Setter functions.
    void setMouthVertices(ofPolyline mouth);
    void setNoseVertices(ofPolyline nose);
    void setFaceVertices(ofPolyline face);
    
    // Morph and draw public functions.
    void draw();
    std::vector<ofPoint> rotatePoints(std::vector<ofPoint> points);
    double getDistanceFromMouthToNose();
    double getWidthOfFace();
    
    // Resources for drawing the moustache that can be modified.
    ofImage moustacheImage;
    std::string moustachePath;
    ofImage moustacheTexture;
    ofImage* morphedImage = new ofImage();
    double angleOfRotation = 0.0;
    int heightOffset = 15;
private:
    // Facial recognition data.
    ofPolyline mouthVertices;
    ofPolyline noseVertices;
    ofPolyline faceVertices;
    
    // Constants.
    int height = 30;
    int extraWidth = 15;
    
    void morphMoustache(std::vector<ofPoint> points);
};

#endif /* moustache_hpp */
