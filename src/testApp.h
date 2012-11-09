#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxSyphon.h"
#include "car.h"
#include "ofxBox2d.h"
#include "ofxMidi.h"

#define OUTPUT_W 1280
#define OUTPUT_H 960

class testApp : public ofBaseApp, public ofxMidiListener{
        
    public:
        void setup();
        void update();
        void draw();
        
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
        void saveSettings();
        void loadSettings();
    
        void resetOutsideConers();
    
    
        ofxXmlSettings XML;
        
    
        bool keys[255];
    
    ofFbo fbo;
    ofPixels pix;
    ofxSyphonClient mClient;
    
    bool bShow, bInfo;
    bool bFullScreen;
    
    ofxBox2d box2d;
    Car cars[N_CARS];
    ofPoint outSide[4];
    void drawOutside();
    bool isOut(int x, int y);
    bool isOutBounds(int x, int y);

	void newMidiMessage(ofxMidiMessage& eventArgs);
	stringstream text;
	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    

};

