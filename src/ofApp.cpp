#include "ofApp.h"
using namespace cv;
using namespace ofxCv;

/**
 * Set up all elements.
 */
void ofApp::setup(){
    cam.setup(640, 480);
    
    // Set up the facial recognition tracker.
    tracker.setup();
    
    // Set up the default image paths for our objects.
    moustache.moustacheImage.load("images/moustache.png");
    moustache.moustacheTexture.load("images/moustache.png");
    logo.load("images/piedPiperLogo.png");
    moustachePicker.load("images/moustachePicker.png");
    moustache.moustachePath = "images/moustache.png";
}

/**
 * Handle updating the camera view.
 */
void ofApp::update(){
    cam.update();

    // Check if we should update the camera view.
    if(cam.isFrameNew()) {
        tracker.update(toCv(cam));
        position = tracker.getPosition();
        scale = tracker.getScale();
        orientation = tracker.getOrientation();
        rotationMatrix = tracker.getRotationMatrix();
    }
}

/**
 * Tell elements to draw themselves to the screen.
 */
void ofApp::draw(){
    int minh = ofGetHeight();
    
    // Calculate dimensions to get a 16 by 9 aspect ratio.
    if (minh < (ofGetWidth() * 9 / 16)) {
        minh = ofGetWidth() * 9 / 16;
    }
    int minw = minh * 16 / 9;
    
    // Draw the camera so that it fills the window no matter the size and
    // maintains proportions of the camera feed.
    cam.draw(minw + ((ofGetWidth() - minw) / 2), ((ofGetHeight() - minh) / 2), -minw, minh);
    
    // Handle when a face is detected.
    if(tracker.getFound()) {
        // Get the specific features from the tracker that are important for adding
        // moustache.
        ofPolyline mouth = tracker.getImageFeature(ofxFaceTracker::Feature::OUTER_MOUTH);
        ofPolyline nose = tracker.getImageFeature(ofxFaceTracker::Feature::NOSE_BASE);
        ofPolyline face = tracker.getImageFeature(ofxFaceTracker::Feature::FACE_OUTLINE);
        
        // Update the moustache object with our new data.
        moustache.setMouthVertices(mouth);
        moustache.setNoseVertices(nose);
        moustache.setFaceVertices(face);
    }
    
    // This initializes all methods to morph the moustache then draws it to the screen.
    moustache.draw();
    
    // Handle scaling the logo to the window size (Including on resize events).
    logo.load("images/piedPiperLogo.png");
    int logoWidth = (ofGetWidth() * 0.2) + 20;
    int logoHeight = (logoWidth / logo.getWidth()) * logo.getHeight();
    logo.resize(logoWidth, logoHeight);
    logo.draw(ofGetWidth() - logo.getWidth(), ofGetHeight() - logo.getHeight());
    
    // Handle scaling the toolbar to the window size (Including on resize events).
    moustachePicker.load("images/moustachePicker.png");
    int pickerHeight = (ofGetHeight() * 0.4) + 50;
    int pickerWidth = (pickerHeight / moustachePicker.getHeight()) * moustachePicker.getWidth();
    moustachePicker.resize(pickerWidth, pickerHeight);
    moustachePicker.draw(20, (ofGetHeight() - moustachePicker.getHeight()) / 2);
}

/**
 * Generate a file path to save a screenshot to with a random file name.
 * Works on all Mac computers.
 * @return string The generated file path.
 */
string generateScreenShotFilePath() {
    ofFile screenShot = ofFile();
    string screenShotFullPath = screenShot.getAbsolutePath();
    string screenShotNewPath = "";
    
    // Find the desktop directory of the computer.
    int depth = 0;
    int charIndex = 0;
    while (depth < 3) {
        screenShotNewPath += screenShotFullPath.at(charIndex);
        
        // Check if we're entering a new directory.
        if (screenShotFullPath.at(charIndex) == '/') {
            depth++;
        }
        charIndex++;
    }
    screenShotNewPath += "Desktop/";
    
    // Generate a random name for the file and add it to the filepath.
    string screenShotName = "MoustachePic_" + std::to_string(((rand() % 100000) + 1)) + ".png";
    screenShotNewPath += screenShotName;
    
    // Return the generated filepath.
    return screenShotNewPath;
}

/**
 * Handle the 'reset' and 'take picture' methods by checking when the 'r' and 'space' keys are pressed, respectively.
 */
void ofApp::keyPressed(int key){
    if (key == 'r') {
        // Reset the facial recognition tracker. (Sometimes it stops working fully)
        tracker.reset();
    } else if (key == ' ') {
        // Handle taking a picture and saving it to the desktop.
        
        // Create a temporary file to check if the filepath is available.
        ofFile tempFile = ofFile(generateScreenShotFilePath());
        
        // Find an available filepath.
        while (tempFile.exists()) {
            tempFile = ofFile(generateScreenShotFilePath());
        }

        // Start the creation of the image file by taking a screenshot of the app.
        ofImage screenShotImage = ofImage();
        screenShotImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        
        // Next, get the webcam output and resize it the same way that it is in the draw function.
        ofPixels cameraPixels = cam.getPixels();
        cameraPixels.mirror(false, true);
        int minh = ofGetHeight();
        
        // Calculate dimensions for a 16 by 9 aspect ratio.
        if (minh < (ofGetWidth() * 9 / 16)) {
            minh = ofGetWidth() * 9 / 16;
        }
        int minw = minh * 16 / 9;
        
        // Calculate the offset to match the webcam output to the screenshot.
        int iOffset = abs((ofGetWidth() - minw) / 2);
        int jOffset = abs((ofGetHeight() - minh) / 2);
        
        // Resize the camera output to line up with the screenshot.
        cameraPixels.resize(minw, minh);
        
        // Create a new image meshing these two together. The webcam output is used to cover up
        // the UI elements from the screenshot.
        ofImage cameraPicture = ofImage();
        cameraPicture.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
        cameraPicture.setFromPixels(cameraPixels);
        cameraPicture.update();
        
        // Calculate dimensions of the picker on screen.
        int pickerHeight = (ofGetHeight() * 0.4) + 50;
        int pickerWidth = (pickerHeight / moustachePicker.getHeight()) * moustachePicker.getWidth();

        // Start the reconstruction of the final image.
        ofImage finalImage = ofImage();
        finalImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
        
        // Calculate dimensions of the logo on screen.
        int logoWidth = (ofGetWidth() * 0.2) + 20;
        int logoHeight = (logoWidth / logo.getWidth()) * logo.getHeight();
        
        // Reconstruct the final images from our two composite images.
        for (int i = 0; i < finalImage.getWidth(); i++) {
            for (int j = 0; j < finalImage.getHeight(); j++) {
                if ((i >= 20 && i <= 20 + pickerWidth) && (j >= (ofGetHeight() - pickerHeight) / 2 && j <= ofGetHeight() - (ofGetHeight() - pickerHeight) / 2)) {
                    // Get and set the color from the shifted webcam output.
                    finalImage.setColor(i, j, cameraPicture.getColor(i + iOffset, j + jOffset));
                } else if (i >= ofGetWidth() - logoWidth && j >= ofGetHeight() - logoHeight) {
                    // Get and set the color from the shifted webcam output.
                    finalImage.setColor(i, j, cameraPicture.getColor(i + iOffset, j + jOffset));
                } else {
                    // Get and set the color from the screenshot.
                    finalImage.setColor(i, j, screenShotImage.getColor(i, j));
                }
            }
        }
        finalImage.update();
        
        // Save the reconstructed image to the unique filepath we found earlier.
        finalImage.save(tempFile.path());
    }
}

/**
 * Handle when the user clicks button 1 on the toolbar.
 */
void ofApp::button1Action() {
    moustache.moustacheImage.load("images/colorStache.png");
    moustache.moustacheTexture.load("images/colorStache.png");
    moustache.moustachePath = "images/colorStache.png";
    moustache.heightOffset = 15;
}

/**
 * Handle when the user clicks button 2 on the toolbar.
 */
void ofApp::button2Action() {
    moustache.moustacheImage.load("images/moustache.png");
    moustache.moustacheTexture.load("images/moustache.png");
    moustache.moustachePath = "images/moustache.png";
    moustache.heightOffset = 15;
}

/**
 * Handle when the user clicks button 3 on the toolbar.
 */
void ofApp::button3Action() {
    moustache.moustacheImage.load("images/uglyStache.png");
    moustache.moustacheTexture.load("images/uglyStache.png");
    moustache.moustachePath = "images/uglyStache.png";
    moustache.heightOffset = 20;
}

/**
 * Detect button clicks for our UI elements. Custom built for this project.
 */
void ofApp::mouseReleased(int x, int y, int button) {
    // Calculate the picker dimensions.
    int pickerHeight = (ofGetHeight() * 0.4) + 50;
    int pickerWidth = (pickerHeight / moustachePicker.getHeight()) * moustachePicker.getWidth();
    
    // Get standard aspects of the image to make it easier to calculate clickable bounds later.
    double baseHeight = (ofGetHeight() - moustachePicker.getHeight()) / 2;
    double divHeight = pickerHeight * 0.05;
    double circleHeight = (moustachePicker.getHeight() - 4 * divHeight) / 3;
    double leftBound = 20 + pickerWidth * 0.1;
    double rightBound = 20 + pickerWidth * 0.9;
    
    // Check if the mouse click was within the bounds we specified.
    if (leftBound < x && rightBound > x) {
        if (baseHeight + divHeight <= y
            && y <= baseHeight + divHeight + circleHeight) {
            button1Action();
        } else if (baseHeight + 2 * divHeight + circleHeight <= y
                   && y <= baseHeight + 2 * divHeight + 2 * circleHeight) {
            button2Action();
        } else if (baseHeight + 3 * divHeight + 2 * circleHeight <= y
                   && y <= baseHeight + 3 * divHeight + 3 * circleHeight) {
            button3Action();
        }
    }
}
