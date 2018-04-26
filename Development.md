# Development.md

-  I spent the first chunk of my time setting up the ofxCv library. This library relied on the built-in ofxOpenCv to provide access to OpenCV. It required some custom setup that took a bit longer than I would have liked, but it is working smoothly now.

- Next, I did more research into facial detection, and it seemed that getting a good tracking of the face would go beyond the scope of this project. I didn't want to have a bad tracker either, so I found an existing library to detect a mesh of the face from an image based on opencv. This library is called ofxFaceTracker.

- To get the mesh working on a live webcam stream, I had to make some modifications to the webcam stream in openframeworks. I needed to flip the camera and get resizing working to make the camera properly fill the app window. This was also difficult because I needed to figure out how to modify the camera images each time before they were sent to the facial detection library so that the facial mesh would appear in the right position.

- At this point the live stream with facial detection worked, but it was incredibly laggy and wasn't going to work for a live feed. I decided to heavily customize and modify the ofxFaceTracker library to work for my project. I eventually got it working with an acceptable lag.

- Next, I created a moustache class to display and represent a moustache on the screen. I first just added a moustache at the top of the lip of the user, however, I decided to take it a step further. I wanted the image of the moustache to morph around the top of the user's lip so that it looked like a real moustache.

- This was the most difficult step so far. I had the use a lot of trigonometry to first detect which nodes made up the upper part of the lip, what angle of rotation there was so that I could rotate the moustache with the face, and then morph the provided moustache image around the line of the upper lip.
