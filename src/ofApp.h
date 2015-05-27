/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxAppUtils.h"
#include "ofxKinect.h"
#include "ofxMidi.h"
#include "ofxSecondWindow.h"
#include "Params.h"

// optional ofxGUI control panel for transformer,
// uncomment this if you're using ofxGui in this project
#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
#include "ofxGui.h"
#include "ofxTransformPanel.h"
#endif

//#define USE_MIDI
//#define USE_MAPPING

#define AUTO 0
#define MANU 1

// we only specify a pointer to the Particle Scene here, so tell the compiler
// that it exists as a class and we can include the class header in testApp.cpp
class ParticleScene;
class KinectScene1;
class KinectScene2;
class KinectScene3;
class KinectScene4;
class KinectScene5;

class Params;
/// make sure you check out main.cpp for how to run this app with app utils!
///
/// controls:
///     - L & R arrows to change scenes (note: wait for 2s transition)
///     - D arrow to change to "no scene" & U arrow to return to last scene
///     - 'd' key to toggle debug mode and enable control panel & warp editor
///     - 'm' key to mirror the render area on the x axis
///     - 'n' key to mirror the render area on the y axis
///     - 'a' key to enable aspect ratio scaling
///     - 'c' key to enable auto centering, only applied if apsect ratio scaling
///           is on and the quad warper is off
///     - 'q' key to toggle to quad warper
///     - 'f' to toggle fullscreen mode
///     - '-' & '=': remove/add particles when the particle scene is running
///     - 'o' to toggle scene overlap
///
class ofApp : public ofxApp, public ofxMidiListener { // <-- inherits ofxApp instead of ofBaseApp
    
public:
    
    
    ofApp()  {}
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void windowResized(int w, int h);
    
    // rendering transformer
    ofxTransformer transformer;
    
    // handles the scenes
    ofxSceneManager sceneManager;
    int lastScene;
    
    // keep a pointer to the Particle Scene, so we can modify it more easily
    ParticleScene *particleScene;
    KinectScene1 *kinectScene1;
    KinectScene2 *kinectScene2;
    KinectScene3 *kinectScene3;
    KinectScene4 *kinectScene4;
    KinectScene5 *kinectScene5;
    
    ofxKinect kinect;
    void      kinectStart();
    
    // MIDI
    void newMidiMessage(ofxMidiMessage& msg);
    void midiStart();
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    stringstream text;
    
    // Separate renderer window
    ofxSecondWindow secondWindow;
    
#ifdef HAVE_OFX_GUI
    ofxTransformPanel panel;
#endif
    
    // AUDIO
#define USE_AUDIO
    float smoothedVol;
    float scaledVol;
    ofSoundStream soundStream;
    void audioIn(float * input, int bufferSize, int nChannels);
    void setupAudio();
    void updateAudio();
    void drawAudio();
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    // Others
    int mode;
    float ROT_X, ROT_Y, ROT_Z;
    float FOV_DEG;
    float PARAM_DIST=1000;
    int step;
    
    Params params;
    
    int angle;
};


