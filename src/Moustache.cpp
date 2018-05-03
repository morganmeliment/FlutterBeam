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

/**
 * Setter function for the mouth-specific vertices from the facial detector.
 */
void Moustache::setMouthVertices(ofPolyline mouth) {
    mouthVertices = mouth;
}

/**
 * Setter function for the mouth-specific vertices from the facial detector.
 */
void Moustache::setNoseVertices(ofPolyline nose) {
    noseVertices = nose;
}

/**
 * Setter function for the mouth-specific vertices from the facial detector.
 */
void Moustache::setFaceVertices(ofPolyline face) {
    faceVertices = face;
}

/**
 * Find the angle of rotation of an edge of points and rotate them to be horizontal.
 * This function also stores the angle of rotation in the moustache class to use later.
 *
 * @return std::vector<ofPoint> The points after un-rotating them.
 */
std::vector<ofPoint> Moustache::rotatePoints(std::vector<ofPoint> points) {
    ofPoint firstPoint = points.at(0);
    ofPoint lastPoint = points.back();
    double x1 = firstPoint.x, y1 = firstPoint.y, x2 = lastPoint.x, y2 = lastPoint.y;
    
    // Calculate the angle of rotation between the first point and the last point.
    double theta = -cvFastArctan((y2 - y1), (x2 - x1)) * ((2 * pi) / 360);
    
    // Store the newly rotated points.
    std::vector<ofPoint> rotatedPoints;
    
    // Rotate each point back to remove the angle of rotation.
    for (ofPoint point : points) {
        // Calculate the differential in position relative to the first point.
        double xn = point.x - x1;
        double yn = point.y - y1;
        
        // Calculate the rotated position relative to the first point.
        double xPrimeN = xn*cos(theta) - yn*sin(theta);
        double yPrimeN = xn*sin(theta) + yn*cos(theta);
        
        // Set the new coordinates for this rotated point.
        point.set(xPrimeN + x1, yPrimeN + y1);
        
        // Add the rotated point to our vector.
        rotatedPoints.push_back(point);
    }

    // Store the angle of rotation in our moustache class.
    angleOfRotation = theta;
    
    return rotatedPoints;
}

/**
 * Find the height of the moustache at a given horizontal position along it
 *
 * @return std::vector<ofPoint> The points after un-rotating them.
 */
double getHeightAtXCoordinate(std::vector<ofPoint> points, int xCoor) {
    // Find which points the x coordinate is between.
    int i = 0;
    for (ofPoint point : points) {
        // Check if the x coordinate is between these points.
        if ((point.x - points.at(0).x) > xCoor) {
            break;
        }
        i++;
    }
    
    // Calculate the relative x position between the two points and the angle of the slope
    // between the two points.
    double x = xCoor - (points.at(i - 1).x - points.at(0).x);
    double theta = -cvFastArctan((points.at(i).y - points.at(i - 1).y),
                                 (points.at(i).x - points.at(i - 1).x)) * ((2 * pi) / 360);
    
    // Calculate the height of the line at that point and apply a sigmoid smoothing
    // function to make the moustache look less choppy.
    double mHeight = (tan(theta) * x) - (points.at(i - 1).y - points.at(0).y);
    double heightDiff = points.at(i - 1).y - points.at(i).y;
    
    // Check if we are dealing with positive or negative numbers before applying the sigmoid
    // smoothing function.
    if (heightDiff < 0) {
        heightDiff = -heightDiff;
        x = (points.at(i).x - points.at(i - 1).x) - x + 3;
    } else {
        x += 3;
    }
    
    // Calculate the factor of sigmoid smoothing.
    double sigmoidValue = (8.0/heightDiff)*x - 3.5 - (heightDiff/10.0);
    double heightMultiplier = 1 / (1 + exp(-sigmoidValue));
    
    // Return the smoothed height at the given x coordinate.
    return mHeight * heightMultiplier;
}

/**
 * Find the distance from the top of the mouth to the bottom of the nose from the
 * facial detection.
 *
 * @return double The distance in pixels from the mouth to the nose.
 */
double Moustache::getDistanceFromMouthToNose() {
    std::vector<ofPoint> nosePoints = noseVertices.getVertices();
    ofPoint lowestNosePoint = nosePoints.at(0);
    
    // Iterate through the nose points to find the lowest nose point.
    for (ofPoint point : nosePoints) {
        if (point.y > lowestNosePoint.y) {
            lowestNosePoint = point;
        }
    }
    
    std::vector<ofPoint> mouthPoints = mouthVertices.getVertices();
    ofPoint highestMouthPoint = mouthPoints.at(0);
    
    // Iterate through the mouth points to find the highest mouth point.
    for (ofPoint point : mouthPoints) {
        if (point.y < highestMouthPoint.y) {
            highestMouthPoint = point;
        }
    }
    
    // Return the absolute value of the y-distance between the points we just found.
    return abs(lowestNosePoint.y - highestMouthPoint.y);
}

/**
 * Find the width of the face from the facial detection in pixels.
 *
 * @return double The width of the face.
 */
double Moustache::getWidthOfFace() {
    std::vector<ofPoint> facePoints = faceVertices.getVertices();
    ofPoint leftMostPoint = facePoints.at(0);
    ofPoint rightMostPoint = facePoints.at(0);
    
    // Find the leftmost and rightmost points by iterating through each
    // of them.
    for (ofPoint point : facePoints) {
        if (point.x < leftMostPoint.x) {
            leftMostPoint = point;
        }
        if (point.x > rightMostPoint.x) {
            rightMostPoint = point;
        }
    }
    
    // Return the absolute value of the x-distance between the two points.
    return abs(rightMostPoint.x - leftMostPoint.x);
}

/**
 * Morph the moustache then finally draw it to the screen.
 */
void Moustache::morphMoustache(std::vector<ofPoint> points) {
    // Get the straightened points from rotatePoints.
    std::vector<ofPoint> rotatedPoints = rotatePoints(points);
    
    ofPoint highestPoint = rotatedPoints.at(0);

    // Iterate through the points to find the highest one.
    for (ofPoint vertex : rotatedPoints) {
        if (vertex.y < highestPoint.y) {
            highestPoint = vertex;
        }
    }
    
    // Calculate the moustache width and height.
    int moustacheWidth = (rotatedPoints.back().x - rotatedPoints.front().x) + 2 * extraWidth;
    int moustacheHeight = height + (rotatedPoints.front().y - highestPoint.y);
    
    // Load the moustache image and resize it to our calculated dimensions.
    moustacheImage.load(moustachePath);
    moustacheImage.resize(moustacheWidth, height);

    // Enable working with alpha-based images.
    ofEnableAlphaBlending();
    
    // Create and set up an empty image to work with from our dimensions with transparent pixels.
    ofPixels image = ofPixels();
    image.allocate(moustacheWidth + 20, moustacheHeight + 60, OF_IMAGE_COLOR_ALPHA);
    ofColor transparent = ofColor(0,0,0,0);
    
    // Set all the pixels to be transparent.
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            image.setColor(i, j, transparent);
        }
    }
    
    // Calculate the width and height multipliers to scale the texture to the final image.
    int widthMultiplier = moustacheTexture.getWidth() / moustacheImage.getWidth();
    int heightMultiplier = moustacheTexture.getHeight() / moustacheImage.getHeight();
    
    // Reconstruct the image pixel by pixel.
    for (int i = 0; i < moustacheImage.getWidth(); i++) {
        for (int j = 0; j < moustacheImage.getHeight(); j++) {
            if (moustacheImage.getColor(i, j) != transparent) {
                if (i >= extraWidth && i < moustacheWidth - extraWidth) {
                    // Find the amount to y-axis morph the image by then set it in our final image.
                    image.setColor(i + 10, j + 45 - getHeightAtXCoordinate(rotatedPoints, i - extraWidth), moustacheImage.getColor(i, j));
                } else {
                    // Set our final image directly from the relative pixel of the original image.
                    image.setColor(i + 10, j + 45, moustacheImage.getColor(i, j));
                }
            } else {
                // Not a moustache pixel.
                image.setColor(i + 10, j + 45, transparent);
            }
        }
    }
    
    // Update the ofImage file.
    morphedImage->setFromPixels(image);
    morphedImage->setCompression(OF_COMPRESS_NONE);
    morphedImage->update();
    
    // Begin rotating and translating the image output.
    // Apply the angle of rotation we calculated earlier to rotate the moustache correctly to the face.
    ofPushMatrix();
    ofTranslate((points.at(0).x + points.back().x) / 2, (points.at(0).y + points.back().y) / 2, 0);
    ofRotate(-angleOfRotation * (360 / (2 * pi)), 0, 0, 1);
    
    // Scale the image to account for distance from the camera.
    ofScale((1 + (getWidthOfFace() / 200)) / 2, (1 + (getDistanceFromMouthToNose() / 17)) / 2);
    
    // Draw the final image.
    morphedImage->draw(-(morphedImage->getWidth()/2),
                       -(morphedImage->getHeight()/2)
                       - heightOffset / ((1 + (getDistanceFromMouthToNose() / 17)) / 2));
    
    // End the transformation effect for future images.
    ofPopMatrix();
}

/**
 * Prepare the moustache to be drawn by morphMoustache.
 */
void Moustache::draw() {
    // Reconstruct an edge of points representing the upper lip.
    std::vector<ofPoint> upperLipEdge;
    
    // Check if the facial detection gave us what we need.
    if (mouthVertices.size() > 0 && noseVertices.size() > 0) {
        ofPoint leftMost = mouthVertices.getVertices().at(0);
        
        // Find the leftmost part of our lip edge.
        for (ofPoint vertex : mouthVertices.getVertices()) {
            if (vertex.x < leftMost.x) {
                leftMost = vertex;
            }
        }
        upperLipEdge.push_back(leftMost);
        
        // Find the rest of our lip edge until we get to the farthest node.
        bool isEdgeFinished = false;
        while (!isEdgeFinished) {
            ofPoint closestOnEdge = upperLipEdge.back();
            
            // Check each vertex point to see if it matches our criteria for being
            // on the upper lip edge.
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
            
            // Check if we've found the end of the lip edge yet.
            if ((closestOnEdge.x == upperLipEdge.back().x
                && closestOnEdge.y == upperLipEdge.back().y)
                || std::find(upperLipEdge.begin(), upperLipEdge.end(), closestOnEdge)
                != upperLipEdge.end()) {
                isEdgeFinished = true;
            } else {
                closestOnEdge.set(closestOnEdge.x, closestOnEdge.y);
                upperLipEdge.push_back(closestOnEdge);
            }
        }

        // Smooth the upper lip edge to remove outlier nodes.
        while (upperLipEdge.size() >= 7) {
            upperLipEdge.erase(upperLipEdge.begin() + 1);
            upperLipEdge.erase(upperLipEdge.begin() + (upperLipEdge.size() - 2));
        }
        
        // Morph and draw the moustache around our detected upper lip edge.
        morphMoustache(upperLipEdge);
    }
}
