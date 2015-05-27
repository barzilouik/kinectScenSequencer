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

class KinectScene1 : public ofxFadeScene {

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
		KinectScene1(ofxKinect * _localkinect, Params* p) : ofxFadeScene("Kinect1"){
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
            
            //if(params->man[0])
		}

		// called when scene is exiting, this is just a demo and this
		// implementation is not required for this class
		void updateExit() {
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateExit();
		}

		// draw
		void draw() {

            ofPushMatrix();
            cam.begin();

            ofEnableAlphaBlending();
			ofEnableDepthTest();
            
            ofTranslate(center);
            ofTranslate(0,0,params->getDIST());
            
            if(params->getMode(0)==MANU) {
                cam.setFov(params->getFOV());
                ofRotateX(params->getROT_X());
                ofRotateY(params->getROT_Y());
                ofRotateZ(params->getROT_Z());
                float scMan = params->getSCALE();
                ofScale(scMan,scMan,scMan);
                ofTranslate(0,0,params->getDIST());
            }
            if(params->getMode(0)==AUTO) {
                //            ofSetColor(255, 0, 0, 255*alpha);
                //            kinect.drawDepth(app->getRenderWidth()/2, app->getRenderHeight()/2);
                ofTranslate(0,0,-500);
                ofRotateY(t);
                float sc = sin(-t/80.) + 1.3;
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
        int step = 2;
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w; x += step) {
                int dist = localkinect->getDistanceAt(x, y);
                if(dist > near && dist < far) {
                    //mesh.addColor(kinect.getColorAt(x,y));
                    mesh.addColor(ofColor(255, (ofMap(dist, DIST_MIN, DIST_MAX, 30,255) + alpha)/2 ));
                    mesh.addVertex(localkinect->getWorldCoordinateAt(x, y));
                }
            }
        }

        glPointSize(3);
        // the projected points are 'upside down' and 'backwards'
        ofScale(1, -1, -1);
        ofTranslate(0, 0, -1000); // center the points a bit
        //mesh.drawVertices();
        mesh.drawWireframe();
    }


};
