//
//  KinectScen2.h
//  appUtilsExampleOSX
//
//  Created by vincent roudaut on mar.5/05/2015.
//
//

#pragma once

#include <ofxAppUtils.h>
#include <ofApp.h>

class KinectScene2 : public ofxFadeScene {
    
public:
    
    ofMesh mesh;
    ofCamera cam;
    
    ofxKinect * localkinect;
    ofxApp * app;
    Params * params;
    
    int t;
    
    int near= 0;
    int far = 3000;
    ofVec3f center;
    
    // set the scene name through the base class initializer
    KinectScene2(ofxKinect * _localkinect, Params* p) : ofxFadeScene("Kinect2"){
        localkinect = _localkinect;
        params = p;
        setSingleSetup(false); // call setup each time the scene is loaded
        setFade(1000, 1000); // 1 second fade in/out
        
        app = ofxGetAppPtr();
    }
    
    // scene setup
    void setup() {
        
    }
    
    // called when scene is entering, this is just a demo and this
    // implementation is not required for this class
    void updateEnter() {
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
    }
    
    // normal update
    void update() {
        localkinect->update();
        
        if(localkinect->isFrameNew()) {

        }
            t++;        
    }
    
    // called when scene is exiting, this is just a demo and this
    // implementation is not required for this class
    void updateExit() {
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
    }
    
    // draw
    void draw() {


        cam.begin();
        ofPushMatrix();
        ofEnableAlphaBlending();
        ofEnableDepthTest();
        
        if(params->getMode(1)==MANU) {
            cam.setFov(params->getFOV());
            ofRotateX(params->getROT_X());
            ofRotateY(params->getROT_Y());
            ofRotateZ(params->getROT_Z());
            float scMan = params->getSCALE();
            ofScale(scMan,scMan,scMan);
            //ofTranslate(0,0,params->getDIST());
        }
        if(params->getMode(1)==AUTO) {
        //            ofSetColor(255, 0, 0, 255*alpha);
        //            kinect.drawDepth(app->getRenderWidth()/2, app->getRenderHeight()/2);
        //ofTranslate(0,0,-1000);
        ofRotateY(t);
        float sc = sin(t/80.) + 1.2 + params->getScaledVol()*10;
        ofScale(sc,sc,sc);
        }
        
        drawPointCloud();
        
        ofDisableDepthTest();
        ofDisableAlphaBlending();
        cam.end();
        ofPopMatrix();

    }
    
    // cleanup
    void exit() {
        
    }
    
    void drawPointCloud() {
        int DIST_MIN = 100;
        int DIST_MAX = 1500;
        int w = 640;
        int h = 480;
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_POINTS);
        //mesh.setMode(OF_PRIMITIVE_LINES);
        int step = 8;
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w; x += step) {
                int dist = localkinect->getDistanceAt(x, y);
                if(dist > near && dist < far) {
                //if(dist > 0 && dist < DIST_MAX) {
                    //mesh.addColor(kinect.getColorAt(x,y));
                    //int zcolor = ( ofMap(dist, DIST_MIN, DIST_MAX, 30,100) + alpha ) / 2;
                    //mesh.addColor(ofColor(255, zcolor);
                    //ofColor alphaIR = ofColor( localkinect->getColorAt(x,y)*2, zcolor );
                    ofColor alphaIR = ofColor( localkinect->getColorAt(x,y) );
                    //alphaIR.setBrightness(50);
                    mesh.addColor(alphaIR);
                    mesh.addVertex(localkinect->getWorldCoordinateAt(x, y));
                }
            }
        }
        
        glPointSize(4);
        // the projected points are 'upside down' and 'backwards'
        ofScale(1, -1, -1);
        ofTranslate(0, 0, -2000); // center the points a bit
        //mesh.drawVertices();
        mesh.drawWireframe();
    }
    
    
};
