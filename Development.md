# Development.md

-  I spent the first chunk of my time setting up the ofxCv library. This library relied on the built-in ofxOpenCv to provide access to OpenCV. It required some custom setup that took a bit longer than I would have liked, but it is working smoothly now.

- Next, I did more research into facial detection, and it seemed that getting a good tracking of the face would go beyond the scope of this project. I didn't want to have a bad tracker either, so I found an existing library to detect a mesh of the face from an image based on opencv. This library is called ofxFaceTracker.

- To get the mesh working on a live webcam stream, I had to make some modifications to the webcam stream in openframeworks. I needed to flip the camera and get resizing working to make the camera properly fill the app window. This was also difficult because I needed to figure out how to modify the camera images each time before they were sent to the facial detection library so that the facial mesh would appear in the right position.

- At this point the live stream with facial detection worked, but it was incredibly laggy and wasn't going to work for a live feed. I decided to heavily customize and modify the ofxFaceTracker library to work for my project. I eventually got it working with an acceptable lag.

- Next, I created a moustache class to display and represent a moustache on the screen. I first just added a moustache at the top of the lip of the user, however, I decided to take it a step further. I wanted the image of the moustache to morph around the top of the user's lip so that it looked like a real moustache.

- This was the most difficult step so far. I had the use a lot of trigonometry to first detect which nodes made up the upper part of the lip, what angle of rotation there was so that I could rotate the moustache with the face, and then morph the provided moustache image around the line of the upper lip.

- One big challenge at this point was that the moustache was blurring itself over time, and generally did not look good. I found that the solution to this was the fact that I was applying the morph modifications to the image continuously. I realized I had to reload the source image every frame to get it working properly.

- I eventually was able to create a reconstructed moustache morphed around the face, but it took many tries using different techniques to get it to display properly. This was the main timesink of the project. When I got it working, however, it still looked a bit choppy, which leads me into my next step.

- From here, I decided to apply a smoothing function to the getHeightAtPoint function to make the moustache 'flow' better. I used a modified sigmoid function which you can see an image of here: https://imgur.com/a/nUHxQ3W to smooth the height values.

- Now that the moustache flowed properly, I had to get the rotation working. This step took a long time as well because I had to calculate a new point to rotate the moustache around and I had to calculate x and y shifting values.

- At this point, the moustache was on the face, but it looked boring and cartoony. I wanted to apply real textures to it, so I decided to make one and apply it at many of the pixel coordinates in the moustache image. This did not work well, however, so I ended up scrapping it and taking a new approach.

- My new approach was to just apply textures to the base images. This worked, and it also made me realize that my code worked for any given image, not just the one I had started with.

- I decided to find a few cool moustaches and try them out. From here, I built a UI to allow the user to change moustaches in real time.

- This step took a while as well because I custom built my UI from scratch. Not only did I design the graphic, I had to write code to detect if a click landed inside of my UI, and since the UI responds dynamically to fit the size of the window, this was an arduous process.

- Once I got that working, I also wanted to add a Pied Piper logo to the window to showcase that this is related to the TV show Silicon Valley. I wrote some code to dynamically scale it to the window size.

- Finally, I wanted to let the user be able to take pictures of their moustache. This was very annoying because I had the UI elements blocking the way and I thought it would be lame to save an image with UI elements on top. I decided to take a screenshot from the app and combine it with the webcam output. This took a while as well because I had to calculate the exact areas where the UI was and replace it in a reconstruced image with pixels from a transformed webcam output. I also had to find an available filepath on the desktop and generate names for the images. It eventually worked, and the file setup should successfully save files on any mac computer.

