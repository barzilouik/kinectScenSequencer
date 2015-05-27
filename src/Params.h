//
//  Params.h
//  appUtilsExampleOSX
//
//  Created by vincent roudaut on mar.5/05/2015.
//
//

#ifndef appUtilsExampleOSX_Params_h
#define appUtilsExampleOSX_Params_h

#define NB_SCENES  5

class Params {
    

public:
    Params() {
        autoMode = true;
        for(int i=0; i<NB_SCENES; i++) {
            shouldSelect[i]=true;
            sceneNb[i] = ofRandom(0,NB_SCENES);
            SequenceLasting[i] = ofRandom(100, 5000) + ofGetElapsedTimeMillis();
        }
    };
    
    vector<float>* volHistory;
    float* scaledVol;

    int mode[10];
    
    void setVolHistory(vector<float>* ref) { volHistory = ref;  }
    vector<float>*  getVolHistory()    { return volHistory; }

    void setScaledVol( float* ref ) { scaledVol = ref; }
    float getScaledVol()            { return *scaledVol; }
    
    float ROT_X;
    float ROT_Y;
    float ROT_Z;
    float SCALE;
    float DIST;
    float FOV;
    
    int far, near;
    
    // auto sequencing
    // handles the scenes
    ofxSceneManager sceneManager;
    bool autoMode = true;
    bool shouldSelect[NB_SCENES] = {true,true,true,true,true} ;
    signed long SequenceLasting[NB_SCENES];
    int sceneNb[NB_SCENES];
    
    ///////////////////////////////////////
    void setAuto(Boolean b) { autoMode = b; };
    
    void update() {
        if(autoMode) {
            int a=ofGetFrameNum();
            float alphaDeg = a % 360;
            ROT_Y = a;
             ROT_X = 0;
             ROT_Y = 0;
             SCALE = 1;
             DIST = -2000;
             FOV = 45;
            
            for(int i=0; i<NB_SCENES; i++) {
                if(shouldSelect[i]) {
                    SequenceLasting[i] = ofRandom(100, 5000) + ofGetElapsedTimeMillis();
                    shouldSelect[i] = false;
                    sceneNb[i] = ofRandom(0,NB_SCENES);
                    ofLogWarning("SequenceLasting["+ofToString(i)+"]="+ofToString(SequenceLasting[i]));
                    ofLogWarning("Sequence"+ofToString(sceneNb[i]));
                }
                
                signed long timeFinished = (SequenceLasting[i] - ofGetElapsedTimeMillis());
                if( timeFinished > 0) {
                    ofLogWarning("going to scene "+ofToString(i));
                    if(sceneManager.getCurrentSceneIndex() != sceneNb[i]) sceneManager.gotoScene(sceneNb[i], true);
                }
                else {
                    ofLogWarning("timeFinished for "+ofToString(i));
                    shouldSelect[i] = true;
                }
                
            }

        }
    }
    
    void setFar(float val ) { far=val; }
    float getFar( )          { return far; }
    void setNear(float val ) { near=val; }
    float getNear( )          { return near; }
    
    void setROT_X( float val ) { ROT_X=val; }
    float getROT_X( )          { return ROT_X; }
    
    void setROT_Y( float val ) { ROT_Y=val; }
    float getROT_Y( )          { return ROT_Y; }
    
    void setROT_Z( float val ) { ROT_Z=val; }
    float getROT_Z( )          { return ROT_Z; }
    
    void setSCALE( float val ) { SCALE=val; }
    float getSCALE( )          { return SCALE; }
    
    void setDIST( float val ) { DIST=val; }
    float getDIST( )          { return DIST; }

    void setFOV( float val ) { FOV=val; }
    float getFOV( )          { return FOV; }
    
    void setMode(int i, int val) { mode[i] = val;  }
    int getMode(int i)           { return mode[i]; }
};

#endif
