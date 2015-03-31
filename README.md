# Skin Detector


Skin detector that trains a gaussian mixture models using threholds in the x,y and depth to select the background and foreground model.

After training the models, they can be used to classify pixels in a image by their probability of being in the foreground model. 

The idea of this detector comes from the following paper:

Jones, M.J., Rehg, J.M.: Statistical color models with application to skin detection. IJCV 46(1), 81–96 (2002)


Dependencies
-----

Make sure to have the following packages installed:
 * Qt 4.8
 * Openni 2.x
 * OpenCV
 * CMake (to build the project)

Building
-----

To compile the project you may use CMake with the following commands

    mkdir build
    cd build
    cmake ..
    make
    
Running
-----
Go to the bin folder that was created when the project was build and run the program

    skin_detector
   

Once the program starts, it will show you a control window with all the possible options. You may start by initializing the camera (make sure it is conected first) and capture a set of images with the record and stop button. This can also be achieve if the space bar is pressed to start the capturing or to stopped.

After capturing the set, it can be reviewed clicking the view record button. If you are satisfied with the current set, you may click the train button to train the gaussian models. 

Once the models are trained you may save the current trained model or capture new images to test them with the current model. After you click the test button it will show the resulting images, which you can do a binary threshold and see in white the pieces detected as skin.

If necessary the models can be loaded to ommit the training step that requires a large amount of time.

Notes
----
This program was tested in a system with Ubuntu 12.04
