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

#include <ofxAppUtils.h>
#include <ofApp.h>
#include "ofxVboParticles.h"
#include "ofxRollingCam.h"

class KinectScene4 : public ofxFadeScene {
    
public:
    
    ofCamera cam;
    ofxRollingCam rollCam;
    vector <ofVec3f> pos;
    
    #define MAX_PART 10
    ofxVboParticles* partix[MAX_PART];
    int part_idx;
    
    ofxKinect * localkinect;
    ofxApp * app;
    Params * params;
    
    int t;
    
    int h=480;
    int w=640;
    int step = 5;
    ofVec3f center;
    
    int near= 0;
    int far = 3000;
    
    // set the scene name through the base class initializer
    KinectScene4(ofxKinect * _localkinect, Params* p) : ofxFadeScene("Kinect4"){
        localkinect = _localkinect;
        params = p;
        setSingleSetup(false); // call setup each time the scene is loaded
        setFade(1000, 1000); // 1 second fade in/out
        
        app = ofxGetAppPtr();
        
        // partciles
        part_idx = 0;
        for(int i = 0; i<MAX_PART; i++) {
            partix[i] = new ofxVboParticles(w*h/(step*step), 5);
        }
        
        // rolling Cam
        rollCam.setup( .05, .05f );
        float r= 1200; //ofGetHeight()*0.8;
        for (int i=0; i<300; i++) {
            ofVec3f newPos;
            //newPos.set(ofRandomf()*r,ofRandomf()*r,ofRandomf()*r);
            newPos.set(0,ofRandomf()*r,ofRandomf()*r);

            pos.push_back(newPos);
        }
        
    }
    
    // scene setup
    void setup() {
        rollCam.setPos(0,0,0);
        rollCam.setCamSpeed(.2);
        rollCam.setScale(1);
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
        rollCam.update();   //rollCam's rotate update.

        
        // Update partciles
        partix[part_idx]->friction = 0.05;
        partix[part_idx]->numParticles = 0;
        
        partix[part_idx]->positions.clear();
        partix[part_idx]->velocitys.clear();
        partix[part_idx]->colors.clear();
        partix[part_idx]->forces.clear();
        
        for(int i = 0; i<MAX_PART; i++) {
            //partix[i]->positions[i].x += (100 -left[i]*180.0f) * 10;
            //if( i != part_idx )partix[i]->update();
            if( i != part_idx ) {
                partix[i]->update();
                //partix[i]->colors->setFade
            }
        }
        
        ofMesh dummy;
        
        // crete new partciles
        if(localkinect->isFrameNew()) {
            for(int y = 0; y < h; y += step) {
                for(int x = 0; x < w; x += step) {
                    float dist = localkinect->getDistanceAt(x, y);
                        if(dist > near && dist < far) {
                        
                        ofPoint p = localkinect->getWorldCoordinateAt(x, y);
                        /*
                         int part_idx_prec = ( part_idx - 1 ) % MAX_PART;
                         ofPoint pprec, diff;
                         if(partix[part_idx_prec] != NULL) {
                         ofPoint pprec = partix[part_idx_prec]->positions[x/step + y*w/step];
                         ofPoint diff = p - pprec;
                         //ofLogNotice("partprec "+ofToString(diff.length()) );
                         }
                         else ofLogNotice("NULL partix, idx = "+ofToString(part_idx_prec) );
                         
                         ofColor partColor;
                         if( diff.length() > 10 ) partColor = (255,128);
                         if( diff.length() <= 10 ) partColor = (50,128);
                         */
                        // ofLogNotice("diff = "+ofToString(diff.length()));
                        partix[part_idx]->addParticle(p,
                                                      //ofVec3f(ofRandom(20) - 10, ofRandom(20) - 10, ofRandom(20) - 10), // velocity
                                                      ofVec3f(0,params->getScaledVol()*10,params->getScaledVol()*20), // velocity
                                                      //kinect.getColorAt(x,y) );
                                                      ofColor(255)//, p.z / 15.)
                                                      //ofColor( p.z, 128 )
                                                      //partColor
                                                      //ofColor(255, 128*alpha)
                                                      );
                        dummy.addVertex(localkinect->getWorldCoordinateAt(x, y));
                        
                    }
                }
            }
        }
        part_idx++;
        if(part_idx >= MAX_PART) part_idx=0;
        center = dummy.getCentroid();
    }
    
    // called when scene is exiting, this is just a demo and this
    // implementation is not required for this class
    void updateExit() {
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
    }
    
    // draw
    void draw() {
        //ofEnableBlendMode(OF_BLENDMODE_ADD);

        ofPushMatrix();
        rollCam.begin();
        ofEnableAlphaBlending();
        ofEnableDepthTest();
        //            ofSetColor(255, 0, 0, 255*alpha);
        //            kinect.drawDepth(app->getRenderWidth()/2, app->getRenderHeight()/2);
        
        ofTranslate(center);
        ofTranslate(0,0,params->getDIST());
        
        if(params->getMode(3)==MANU) {
            //rollCam.setFov(params->getFOV());
            ofRotateX(params->getROT_X());
            ofRotateY(params->getROT_Y());
            ofRotateZ(params->getROT_Z());
            float scMan = params->getSCALE();
            ofScale(scMan,scMan,scMan);
            //ofTranslate(0,0,params->getDIST());
        }
        if(params->getMode(3)==AUTO) {
            //rollCam.setFov(params->getFOV());
            
            
            ofTranslate(0,0,-1000);
            
            ofRotateY(t);
            ofRotateX(t/4);
            float sc = sin(-t/80.) + 1.3;
            ofScale(sc,sc,sc);
        }
        

        drawPointCloud();
        //drawAudio();
        
        ofDisableDepthTest();
        ofDisableAlphaBlending();
        rollCam.end();
        ofPopMatrix();
        
       //ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    }
    
    void drawAudio() {
        // Draw Audio
        ofSetLineWidth(2);
        ofSetColor(255, 50);
        ofTranslate(-2000,0,0);
        ofBeginShape();
        for(int i=0; i < params->getVolHistory()->size(); i++ ){
                if( i == 0 ) ofVertex(i*10, 400);
            
                ofVertex(sin(i)*50,  800 - params->getVolHistory()->at(i) * 400, -i*10);
                ofVertex(sin(i)*50,  800 - params->getVolHistory()->at(i) * 400, -i*10 - 10);
                
                if( i == params->getVolHistory()->size() -1 ) ofVertex(i*10, 400);
        }
        ofEndShape(false);
    }
    // cleanup
    void exit() {
        
    }
    
    void drawPointCloud() {
        int PARAM_DIST = 1000;
        
        int DIST_MIN = 100;
        int DIST_MAX = 1500;
        int w = 640;
        int h = 480;

        //depthOfField.begin();

        
        for(int i=0; i<MAX_PART; i++) {
            partix[i]->draw();
        }

    }
    
    ofxRollingCam * getRollCamRef() {
        return &rollCam;
    }
    
};
