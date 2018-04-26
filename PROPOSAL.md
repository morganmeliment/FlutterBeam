# Final Project Proposal

For my final project, I want to build a system that can recognize faces from a live webcam feed, then overlay features onto the faces in real-time. For example, one option the project could provide is to have a mustache overlay. This project has two main components – detecting faces, and then deciding where to overlay the filter in a reasonable amount of time. I assume lag will be one of the biggest issues since I'd like this project to work in real time.

# Libraries

I want to use OpenCV to do the majority of the image recognition. (https://opencv.org/) It has powerful tools for doing facial recognition and it seems to be a popular choice for this kind of problem. I hope it will be able to recognize faces quickly, because I'd like to improve the project to support things like rotating faces if I have time. I shouldn't need any other libraries to make this work because openFrameworks already supports webcam usage.

Edit: I have also decided to use ofxFaceTracker to provide a mesh of the face that I am tracking. This will speed up development and allow me to focus on other features of the tracker.

# Process

I'm going to start the project by setting up an interface with a webcam feed that I can pull a constant stream of images from. Then, I want to get started with training and setting up the OpenCV facial recognition. I found this guide https://www.superdatascience.com/opencv-face-recognition/ which seems to be a helpful start. The guide uses python, but it shouldn't be too difficult to translate to C++. Once I can recognize where faces are, I want to have a filter layer on top of the webcam feed. I'll move my filters to the correct location on the screen to combine the images. If I have multiple filter options, I'll set up a basic interface for selecting which one to use. All of this will run in a windowed application from openFrameworks.
