//
//  moustache.cpp
//  FlutterBeam
//
//  Created by Morgan Meliment on 4/23/18.
//
#include "Moustache.hpp"
#include <math.h>
using namespace cv;
using namespace ofxCv;

void Moustache::setMouthVertices(ofPolyline mouth) {
    mouthVertices = mouth;
}

void Moustache::setNoseVertices(ofPolyline nose) {
    noseVertices = nose;
}

std::vector<ofPoint> Moustache::rotatePoints(std::vector<ofPoint> points) {
    ofPoint firstPoint = points.at(0);
    ofPoint lastPoint = points.back();
    double x1 = firstPoint.x, y1 = firstPoint.y, x2 = lastPoint.x, y2 = lastPoint.y;
    double theta = -cvFastArctan((y2 - y1), (x2 - x1)) * ((2 * pi) / 360);
    std::vector<ofPoint> rotatedPoints;

    for (ofPoint point : points) {
        double xn = point.x - x1;
        double yn = point.y - y1;
        
        double xPrimeN = xn*cos(theta) - yn*sin(theta);
        double yPrimeN = xn*sin(theta) + yn*cos(theta);
        
        point.set(xPrimeN + x1, yPrimeN + y1);
        rotatedPoints.push_back(point);
    }
    
    angleOfRotation = theta;
    
    return rotatedPoints;
}

double getHeightAtXCoordinate(std::vector<ofPoint> points, int xCoor) {
    int i = 0;
    for (ofPoint point : points) {
        if ((point.x - points.at(0).x) > xCoor) {
            break;
        }
        i++;
    }
    
    double x = xCoor - (points.at(i - 1).x - points.at(0).x);
    double theta = -cvFastArctan((points.at(i).y - points.at(i - 1).y),
                                 (points.at(i).x - points.at(i - 1).x)) * ((2 * pi) / 360);
    
    double mHeight = (tan(theta) * x) - (points.at(i - 1).y - points.at(0).y);
    double heightDiff = points.at(i - 1).y - points.at(i).y;
    if (heightDiff < 0) {
        heightDiff = -heightDiff;
        x = (points.at(i).x - points.at(i - 1).x) - x + 3;
    } else {
        x += 3;
    }
    double sigmoidValue = (8.0/heightDiff)*x - 3.5 - (heightDiff/10.0);
    double heightMultiplier = 1 / (1 + exp(-sigmoidValue));
    std::cout << "Sigmoid: " << heightMultiplier << " : " << xCoor << std::endl;
    return mHeight * heightMultiplier;
}

void Moustache::morphMoustache(std::vector<ofPoint> points) {
    std::vector<ofPoint> rotatedPoints = rotatePoints(points);
    
    ofPoint highestPoint = rotatedPoints.at(0);

    for (ofPoint vertex : rotatedPoints) {
        if (vertex.y < highestPoint.y) {
            highestPoint = vertex;
        }
    }
    int moustacheWidth = (rotatedPoints.back().x - rotatedPoints.front().x) + 2 * extraWidth;
    int moustacheHeight = height + (rotatedPoints.front().y - highestPoint.y);
    //std::cout << height << " : " << moustacheHeight << std::endl;
    
    //std::cout << (rotatedPoints.front().y - highestPoint.y) << std::endl;
    moustacheImage.load(moustachePath);
    moustacheImage.resize(moustacheWidth, height);

    ofEnableAlphaBlending();
    ofPixels image = ofPixels();
    image.allocate(moustacheWidth + 20, moustacheHeight + 30, OF_IMAGE_COLOR_ALPHA);
    ofColor transparent = ofColor(0,0,0,0);
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            image.setColor(i, j, transparent);
        }
    }
    for (int i = 0; i < moustacheImage.getWidth(); i++) {
        for (int j = 0; j < moustacheImage.getHeight(); j++) {
            if (moustacheImage.getColor(i, j) != transparent) {
                if (i >= extraWidth && i < moustacheWidth - extraWidth) {
                    image.setColor(i + 10, j + 15 - getHeightAtXCoordinate(rotatedPoints, i - extraWidth), ofColor(0, 0, 0, 255));
                    
                    //std::cout << getHeightAtXCoordinate(rotatedPoints, i - extraWidth) << std::endl;
                } else {
                    image.setColor(i + 10, j + 15, moustacheImage.getColor(i, j));
                }
            } else {
                image.setColor(i + 10, j + 15, transparent);
            }
        }
    }
    morphedImage->setFromPixels(image);
    morphedImage->update();
    ofPushMatrix();
    ofTranslate(rotatedPoints.at(0).x - extraWidth - 10, rotatedPoints.at(0).y, 0);//move pivot to centre
    ofRotate(-angleOfRotation * (360 / (2 * pi)), 0, 0, 1);//rotate from centre
    ofPushMatrix();
    //ofTranslate(-leafImg.width/2,-leafImg.height/2,0);//move back by the centre offset
    //leafImg.draw(0,0);
    morphedImage->draw(0, -height + 5);
    ofPopMatrix();
    //ofMesh* moustache = new ofMesh(OF_PRIMITIVE_LINE_STRIP, rotatedPoints);
    //moustache->draw();
}

void Moustache::draw() {
    std::vector<ofPoint> upperLipEdge;
    if (mouthVertices.size() > 0 && noseVertices.size() > 0) {
        ofPoint leftMost = mouthVertices.getVertices().at(0);
        
        for (ofPoint vertex : mouthVertices.getVertices()) {
            if (vertex.x < leftMost.x) {
                leftMost = vertex;
            }
        }
        upperLipEdge.push_back(leftMost);
        
        bool isEdgeFinished = false;
        while (!isEdgeFinished) {
            ofPoint closestOnEdge = upperLipEdge.back();
            
            for (ofPoint vertex : mouthVertices.getVertices()) {
                if ((upperLipEdge.back().x == closestOnEdge.x
                     && upperLipEdge.back().y == closestOnEdge.y
                     && vertex.x > closestOnEdge.x)
                    || (vertex.x < closestOnEdge.x
                    && vertex.x > upperLipEdge.back().x
                    && vertex.distance(upperLipEdge.back())
                    < closestOnEdge.distance(upperLipEdge.back()))) {
                    closestOnEdge = vertex;
                }
            }
            
            if ((closestOnEdge.x == upperLipEdge.back().x
                && closestOnEdge.y == upperLipEdge.back().y)
                || std::find(upperLipEdge.begin(), upperLipEdge.end(), closestOnEdge)
                != upperLipEdge.end()) {
                isEdgeFinished = true;
            } else {
                closestOnEdge.set(closestOnEdge.x, closestOnEdge.y - 15);
                upperLipEdge.push_back(closestOnEdge);
            }
        }
        upperLipEdge.at(0).set(upperLipEdge.at(0).x, upperLipEdge.at(0).y - 15);
        while (upperLipEdge.size() >= 7) {
            upperLipEdge.erase(upperLipEdge.begin() + 1);
            upperLipEdge.erase(upperLipEdge.begin() + (upperLipEdge.size() - 2));
        }
        morphMoustache(upperLipEdge);
        
        /*
        moustacheImage.draw(upperLipEdge.at(0).x - 15, upperLipEdge.at(0).y - (height / 2),
                       -(upperLipEdge.at(0).x - upperLipEdge.back().x) + 30, height);
        
        //std::cout << upperLipEdge.at(0).x << " " << mouthVertices.size() << std::endl;
        ofMesh* moustache = new ofMesh(OF_PRIMITIVE_LINE_STRIP, upperLipEdge);
        moustache->draw();*/
    }
}
