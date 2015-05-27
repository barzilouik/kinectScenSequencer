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
#include "ofApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_WARNING);
    kinectStart();
    
#ifdef USE_MIDI
    ofSleepMillis(300);
    midiStart();
#endif
    
#ifdef USE_AUDIO
    setupAudio();
#endif
    //secondWindow.setup("video pro", ofGetScreenWidth(), 0, 1280, 800, true);
    
    // setup for nice jaggy-less rendering
    //ofSetVerticalSync(true);
    ofBackground(0, 0, 0);

#ifdef USE_MAPPING
    // setup the render size (working area)
    //transformer.setRenderSize(600, 400);
    transformer.setRenderSize(ofGetWidth(), ofGetHeight());
    
    // turn on transform origin translation and scaling to screen size,
    // disable quad warping, and enable aspect ratio and centering when scaling
    transformer.setTransforms(true, true, false, true, true);
    
    // set the ofxApp transformer so it's automatically applied in draw()
    setTransformer(&transformer);
#endif USE_MAPPING
    
#ifdef HAVE_OFX_GUI
    // setup transform panel with transformer pointer,
    // loads settings & quad warper xml files if found
    panel.setup(&transformer);
#endif
    
    // load scenes
    particleScene = (ParticleScene*) sceneManager.add(new ParticleScene(scaledVol)); // save pointer
    //	sceneManager.add(new LineScene());
    kinectScene1 = (KinectScene1*) sceneManager.add(new KinectScene1( &kinect, &params ));
    kinectScene2 = (KinectScene2*) sceneManager.add(new KinectScene2( &kinect, &params ));
    kinectScene3 = (KinectScene3*) sceneManager.add(new KinectScene3( &kinect, &params ));
    kinectScene4 = (KinectScene4*) sceneManager.add(new KinectScene4( &kinect, &params ));
    kinectScene5 = (KinectScene5*) sceneManager.add(new KinectScene5( &kinect, &params ));
    
    sceneManager.setup(true); // true = setup all the scenes now (not on the fly)
    ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
    
    // start with a specific scene
    // set now to true in order to ignore the scene fade and change now
    sceneManager.gotoScene("Kinect1", true);
    lastScene = sceneManager.getCurrentSceneIndex();
    
    // overlap scenes when transitioning
    sceneManager.setOverlap(true);
    
    // attach scene manager to this ofxApp so it's called automatically,
    // you can also call the callbacks (update, draw, keyPressed, etc) manually
    // if you don't set it
    //
    // you can also turn off the auto sceneManager update and draw calls with:
    // setSceneManagerUpdate(false);
    // setSceneManagerDraw(false);
    //
    // the input callbacks in your scenes will be called if they are implemented
    //
    setSceneManager(&sceneManager);
    
    //angle = kinect.getCameraTiltAngle(angle);
    
    params.sceneManager = sceneManager;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    
    // the current scene is automatically updated before this function
#ifdef USE_AUDIO
    updateAudio();
#endif
    
#ifdef HAVE_OFX_GUI
    // update the transform panel when in debug mode
    if(isDebug()) {
        panel.update();
    }
#endif

#ifdef USE_MIDI
    // MIDI
    if(isDebug()) {
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofLogWarning( text.str() );
    text.str(""); // clear
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 144);
    ofLogWarning( text.str() );
    }
    
    
    int ctrl = midiMessage.control;
    switch(ctrl) {
        case 17: params.setMode(0,1); break;
        case 18: params.setMode(1,1); break;
        case 19: params.setMode(2,1); break;
        case 20: params.setMode(3,1); break;
        case 22: params.setMode(4,1); break;
            
        case 25: params.setMode(0,0); break;
        case 26: params.setMode(1,0); break;
        case 27: params.setMode(2,0); break;
        case 28: params.setMode(3,0); break;
        case 29: params.setMode(4,0); break;
            
            // rotation
        case 49: params.setROT_X( .9*params.getROT_X() + .1*ofMap(midiMessage.value, 0, 127, 0, 360) ); break;
        case 50: params.setROT_Y( .9*params.getROT_Y() + .1*ofMap(midiMessage.value, 0, 127, 0, 360) ); break;
        case 51: params.setROT_Z( .9*params.getROT_Z() + .1*ofMap(midiMessage.value, 0, 127, 0, 360) ); break;
        case 52: params.setSCALE( .9*params.getSCALE() + .1*ofMap(midiMessage.value, 0, 127, 0, 10.0f) ); break;
        case 53: params.setDIST(  .9*params.getDIST()  + .1*ofMap(midiMessage.value, 0, 127, -4000, 4000) ); break;
        case 54: params.setFOV(   .9*params.getFOV()   + .1*ofMap(midiMessage.value, 0, 127, 0.1f, 180.0f) ); break;

    }
#endif
    
    //if(!params.autoMode) {
        params.setScaledVol(&scaledVol);
        params.setVolHistory(&volHistory);

    //}

      params.update();

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    //secondWindow.begin();
    
    // the current scene is automatically drawn before this function
    
    // show the render area edges with a white rect
    if(isDebug()) {
        ofNoFill();
        ofSetColor(255);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
        ofFill();
        
        drawAudio();
    }
    
    //secondWindow.end();
    
    // drop out of the auto transform space back to OF screen space
    transformer.pop();
    
#ifdef HAVE_OFX_GUI
    // draw the transform panel when in debug mode
    if(isDebug()) {
        panel.draw();
    }
#endif
    
    // draw current scene info using the ofxBitmapString stream interface
    // to ofDrawBitmapString
    ofSetColor(200);
    ofxBitmapString(12, ofGetHeight()-8)
    << "Current Scene: " << sceneManager.getCurrentSceneIndex()
    << " " << sceneManager.getCurrentSceneName() << endl;
    
    // go back to the auto transform space
    //
    // this is actually done automatically if the transformer is set but
    // included here for completeness
    transformer.push();
    
    // the warp editor is drawn automatically after this function
}



///////////// AUDIO ////////////////////
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    //    smoothedVol *= 0.93;
    //    smoothedVol += 0.07 * curVol;
    smoothedVol *= 0.80;
    smoothedVol += 0.20 * curVol;
    bufferCounter++;
    
}

void ofApp::setupAudio(){
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    soundStream.listDevices();
    soundStream.setDeviceID(0);
    
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    soundStream.start();
    
}

//--------------------------------------------------------------
void ofApp::updateAudio(){
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
}

//--------------------------------------------------------------
void ofApp::drawAudio(){
    
    ofSetColor(225);
    ofDrawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
    ofDrawBitmapString("press 's' to unpause the audio\n'e' to pause the audio", 31, 92);
    
    ofNoFill();
    
    // draw the left channel:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Left Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < left.size(); i++){
        ofVertex(i*2, 100 -left[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    // draw the right channel:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 370, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Right Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < right.size(); i++){
        ofVertex(i*2, 100 -right[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    // draw the average volume:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(565, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofRect(0, 0, 400, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofCircle(200, 200, scaledVol * 190.0f);
    
    //lets draw the volume history as a graph
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i, 400);
        
        ofVertex(i, 400 - volHistory[i] * 70);
        
        if( i == volHistory.size() -1 ) ofVertex(i, 400);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    drawCounter++;
    
    ofSetColor(225);
    string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount());
    ofDrawBitmapString(reportString, 32, 589);
    
}


//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    // make a copy of the latest message
    //ofLogWarning("msg midi");
    midiMessage = msg;
}


void ofApp::midiStart() {
    midiIn.listPorts(); // via instance
    midiIn.openPort(2);
    //midiIn.openPort("IAC Pure Data In");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    // don't ignore sysex, timing, & active sense messages,these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    // add testApp as a listener
    midiIn.addListener(this);
    // print received messages to the console
    midiIn.setVerbose(true);
}

void ofApp::kinectStart() {
    kinect.setRegistration(true);
    //kinect.init(false, false);
    kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    if(!kinect.isConnected()) { ofSleepMillis(300); };
    kinect.open();		// opens first available kinect
    
    if(kinect.isConnected()) {
        ofLogNotice() << "Kinect connected >>>>>";
    }
}



// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    ofLogWarning("code = " + ofToString(key));
    
    switch (key) {
            
        case '1':
        case '&':
            sceneManager.gotoScene(0);
            break;
        case '2':
        case 233:
            sceneManager.gotoScene(1);
            break;
        case '3':
        case 34:
            sceneManager.gotoScene(2);
            break;
        case '4':
        case 39:
            sceneManager.gotoScene(3);
            break;
        case '5':
        case 40:
            sceneManager.gotoScene(4);
            break;
        case '6':
        case 167:
            sceneManager.gotoScene(5);
            break;
            
        case '7':
        case 232:
            if(sceneManager.getCurrentScene() == kinectScene4) {
                kinectScene4->getRollCamRef()->setRandomPos(270);
                ofLogNotice("rollcam rnd pos");
            }
            break;
        case '8':
        case 33:
            if(sceneManager.getCurrentScene() == kinectScene4) {
                kinectScene4->getRollCamRef()->setRandomScale(0.5, 1.5);
                ofLogNotice("rollcam rnd scale");
            }
            break;
        case '9':
        case 231:
            if(sceneManager.getCurrentScene() == kinectScene4) {
                kinectScene4->getRollCamRef()->setPos(0, 0, 0);
                kinectScene4->getRollCamRef()->setScale(1);
                ofLogNotice("rollcam far");
            }
            break;
        case '0':
        case 224:
            if(sceneManager.getCurrentScene() == kinectScene4) {
                kinectScene4->getRollCamRef()->setPos(0, 0, 0);
                ofLogNotice("rollcam reset");
            }
            break;
            /*
             // rollcam
             if (key=='u') {//All Random.
             rollCam.setRandomScale(0.5, 1.5);
             rollCam.setRandomPos(270);
             }
             if (key=='i') {//Random rotate.
             rollCam.setRandomPos(270);
             }
            
             if (key=='7') {//Inputting optional rotate.
             rollCam.setPos(0, 0, 0);
             }
             if (key=='8') {//Random distance.
             rollCam.setRandomScale(0.5, 1.5);
             }
             if (key=='9') {//Inputting optional distance.
             rollCam.setScale(1);
             }
            */
            
        case 'd':
            bDebug = !bDebug;
            break;
            
        case 'a':
            transformer.setAspect(!transformer.getAspect());
            break;
            
        case 'c':
            transformer.setCentering(!transformer.getCentering());
            break;
            
        case 'm':
            transformer.setMirrorX(!transformer.getMirrorX());
            break;
            
        case 'n':
            transformer.setMirrorY(!transformer.getMirrorY());
            break;
            
        case 'q':
            transformer.setWarp(!transformer.getWarp());
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case OF_KEY_LEFT:
            sceneManager.prevScene();
            break;
            
        case OF_KEY_RIGHT:
            sceneManager.nextScene();
            break;
            
//        case OF_KEY_DOWN:
//            if(sceneManager.getCurrentScene()) { // returns NULL if no scene selected
//                lastScene = sceneManager.getCurrentSceneIndex();
//            }
//            sceneManager.noScene();
//            break;
//            
//        case OF_KEY_UP:
//            sceneManager.gotoScene(lastScene);
//            break;
            
        case '-':
            if(sceneManager.getCurrentScene() == particleScene) {
                particleScene->removeOneParticle();
            }
            break;
            
        case '=':
            if(sceneManager.getCurrentScene() == particleScene) {
                particleScene->addOneParticle();
            }
            break;
            
        case 'o':
            sceneManager.setOverlap(!sceneManager.getOverlap());
            break;
            
        case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }
    
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    
    // transformer.setNewScreenSize() is automatically called if the transformer is set
    
}
