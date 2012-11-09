#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
    
    
    fbo.allocate(OUTPUT_W, OUTPUT_H);
    
    loadSettings();
    
    for(int i = 0; i < 255; i++)
        keys[i] = false;
    
    mClient.setup();
    mClient.setApplicationName("MadMapper");
	
	box2d.init();
	box2d.setGravity(0, 0);
    box2d.setFPS(30.0);
    box2d.createBounds();
    
    cars[0].setImage("bmw.jpg");
    cars[0].setMidi(0);
    cars[0].setup(box2d.getWorld(),  OUTPUT_W/2 , OUTPUT_H/2, CAR_W/2, CAR_H/2);
    
    cars[1].setImage("ferrari.jpg");
    cars[1].setMidi(1);
    cars[1].setup(box2d.getWorld(), OUTPUT_W/2 , OUTPUT_H/2, CAR_W/2, CAR_H/2);
    
  /*  cars[2].setImage("tiza1.png");
    cars[2].setMidi(2);
    cars[2].setup(box2d.getWorld(), OUTPUT_W/2 , OUTPUT_H/2, CAR_W/2, CAR_H/2);
    
    cars[3].setImage("tiza1.png");
    cars[3].setMidi(2);
    cars[3].setup(box2d.getWorld(), OUTPUT_W/2 , OUTPUT_H/2, CAR_W/2, CAR_H/2);*/
    
	midiIn.openPort("ControllerMate");
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(true);
    
    ofSetFrameRate(30);
    bFullScreen = false;
}

//--------------------------------------------------------------
void testApp::update(){
    box2d.update();
}


//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0, 0, 0);
	ofSetHexColor(0xffffff);
    
	// Get image from MadMapper
    fbo.begin();
    mClient.draw(0, 0, OUTPUT_W, OUTPUT_H);
    drawOutside();
    fbo.end();
    fbo.readToPixels(pix);
    
    if(bShow)
        fbo.draw(0, 0);
        
    for(int i = 0; i < N_CARS; i++){
        cars[i].isOut = isOut(cars[i].getPosition().x, cars[i].getPosition().y);
        cars[i].isOutBounds = isOutBounds(cars[i].getPosition().x, cars[i].getPosition().y);
        cars[i].update();
        cars[i].draw();
    }
    
    
    if(bInfo){
        ofSetHexColor(0x00000);
        ofRect(0, 660, 1024, 760 - 660);
        
        ofSetHexColor(0xFFFF00);
        char reportStr[1024];
        
        sprintf(reportStr, "[s/l] save/load [i] info on/off [o] img on/off [MadMapper Size] %3.f %3.f Outsite [1, 2, 3, 4]", mClient.getWidth(), mClient.getHeight());
        ofDrawBitmapString(reportStr, 20, 670);
        
        ofColor c = pix.getColor(mouseX , mouseY);
        bool iO = isOut(mouseX, mouseY);
        bool iOB = isOutBounds(mouseX, mouseY);
        sprintf(reportStr, "fps:%3.0f  [mouse picker] %3d %1d %1d", ofGetFrameRate() , c.r, iO, iOB);
        ofDrawBitmapString(reportStr, 20, 685);
        
               for(int i = 0; i < N_CARS; i++){
            sprintf(reportStr, "%d CAR (%3.f,%3.f) [%4.2f]",
                    i,
                    cars[i].getPosition().x,
                    cars[i].getPosition().y,
                    cars[i].acc );
            ofDrawBitmapString(reportStr, 20, 700 + i*15);
        }
        
    }
        
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key > 0 && key <255)
        keys[key] = true;
    
	switch (key){
        case 357: //UP
            if(keys['1']) outSide[0].y -= 1;
            if(keys['2']) outSide[1].y -= 1;
            if(keys['3']) outSide[2].y -= 1;
            if(keys['4']) outSide[3].y -= 1;
        
            if(keys['a']) dAcc += 0.01;
            if(keys['d']) dDec += 0.01;
            if(keys['r']) dRot += 0.1;
            if(keys['M']) maxV += 0.1;
            if(keys['m']) minV += 0.1;
            if(keys['n']) outV += 0.1;
            break;
            
        case 359: //Down
            if(keys['1']) outSide[0].y += 1;
            if(keys['2']) outSide[1].y += 1;
            if(keys['3']) outSide[2].y += 1;
            if(keys['4']) outSide[3].y += 1;
            
            if(keys['a']) dAcc -= 0.01;
            if(keys['d']) dDec -= 0.01;
            if(keys['r']) dRot -= 0.1;
            if(keys['M']) maxV -= 0.1;
            if(keys['m']) minV -= 0.1;
            if(keys['n']) outV -= 0.1;

            break;
        case 358: //RIGHT
            if(keys['1']) outSide[0].x += 1;
            if(keys['2']) outSide[1].x += 1;
            if(keys['3']) outSide[2].x += 1;
            if(keys['4']) outSide[3].x += 1;
            break;
        case 356: //LEFT
            if(keys['1']) outSide[0].x -= 1;
            if(keys['2']) outSide[1].x -= 1;
            if(keys['3']) outSide[2].x -= 1;
            if(keys['4']) outSide[3].x -= 1;
            break;
        case 'R':
            for(int i = 0; i < N_CARS; i++)
                cars[i].reset(OUTPUT_W / 2, OUTPUT_H /2);
            break;
        case 'o':
            bShow = !bShow;
            break;
        case 's':
            saveSettings();
            break;
        case 'l':
            loadSettings();
            break;
        case 'i':
            bInfo = !bInfo;
            break;
        case 'f':
            ofToggleFullscreen();
            bFullScreen = !bFullScreen;
            if(bFullScreen) ofHideCursor();
            else ofShowCursor();
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key > 0 && key <255)
        keys[key] = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
void testApp::loadSettings(){
    
    if( XML.loadFile("mySettings.xml") ){
		cout << "mySettings.xml loaded!" <<endl;
	}else{
		cout <<  "unable to load mySettings.xml check data/ folder" << endl;
	}
    
    bInfo = XML.getValue("bINFO", 0);
    bShow = XML.getValue("bSHOW", 0);
    
    dAcc = XML.getValue("CAR:dACC", 0.15);
    dDec =  XML.getValue("CAR:dDEC", 0.25);
    dRot =  XML.getValue("CAR:dROT", 5.0);
    maxV = XML.getValue("CAR:MAXV", 7.0);
    minV = XML.getValue("CAR:MINV", -1.0);
    outV = XML.getValue("CAR:OUTV", 1.0);
    
    XML.pushTag("OUTSIDE");
    for(int i = 0; i < 4; i++){
        XML.pushTag("PT", i);
        outSide[i].x = XML.getValue("X", 0);
        outSide[i].y = XML.getValue("Y", 0);
        XML.popTag();
    }
    XML.popTag();
    
    cout << "Settings loaded" << endl;
    
}

//--------------------------------------------------------------
void testApp::saveSettings(){
    XML.clear();
    XML.setValue("bINFO", bInfo);
    XML.setValue("bSHOW", bShow);
    
    
    XML.setValue("CAR:dACC", dAcc);
    XML.setValue("CAR:dDEC", dDec);
    XML.setValue("CAR:dROT", dRot);
    XML.setValue("CAR:MAXV", maxV);
    XML.setValue("CAR:MINV", minV);
    XML.setValue("CAR:OUTV", outV);

    
    
    XML.addTag("OUTSIDE");
    XML.pushTag("OUTSIDE");
    for(int i = 0; i < 4; i++){
        XML.addTag("PT");
        XML.pushTag("PT", i);
        XML.setValue("X", outSide[i].x,i);
        XML.setValue("Y", outSide[i].y,i);
        XML.popTag();
    }
    XML.popTag();
    
    
    XML.saveFile("mySettings.xml");
    cout << "Settings saved" << endl;
}

bool testApp::isOut(int x, int y){
    int xx, yy;
    int d = CAR_H /2.0;
    xx = x + d;
    yy = y + d;
    
    if(pix.getColor(x, y).r > 120) return true;
    /*
    xx = x - d;
    yy = y + d;
    if(pix.getColor(xx, yy).r > 120) return true;
    
    xx = x + d;
    yy = y - d;
    if(pix.getColor(xx, yy).r > 120) return true;
    
    xx = x - d;
    yy = y - d;
    if(pix.getColor(xx, yy).r > 120) return true;*/
    return false;
}

bool testApp::isOutBounds (int x, int y){
    
    int xx, yy;
    int d = CAR_H /2.0;
    xx = x + d;
    yy = y + d;
    
    if(pix.getColor(x, y).r == 50) return true;
    /*
    xx = x - d;
    yy = y + d;
    if(pix.getColor(xx, yy).r == 50) return true;
    
    xx = x + d;
    yy = y - d;
    if(pix.getColor(xx, yy).r == 50) return true;
    
    xx = x - d;
    yy = y - d;
    if(pix.getColor(xx, yy).r == 50) return true;*/
    return false;
}

//--------------------------------------------------------------
void testApp::drawOutside(){
    ofSetColor(50, 120, 120);
    ofFill();
    
    ofBeginShape();
    ofVertex(0,0);
    ofVertex(OUTPUT_W - 1,0);
    ofVertex(outSide[1].x,outSide[1].y);
    ofVertex(outSide[0].x,outSide[0].y);
    ofEndShape(true);

    ofBeginShape();
    ofVertex(0,0);
    ofVertex(outSide[0].x,outSide[0].y);
    ofVertex(outSide[3].x,outSide[3].y);
    ofVertex(0, OUTPUT_H - 1);
    ofEndShape(true);
    
    ofBeginShape();
    ofVertex(OUTPUT_W - 1,0);
    ofVertex(OUTPUT_W - 1, OUTPUT_H - 1);
    ofVertex(outSide[2].x,outSide[2].y);
    ofVertex(outSide[1].x,outSide[1].y);
    ofEndShape(true);
    
    ofBeginShape();
    ofVertex(OUTPUT_W - 1, OUTPUT_H - 1);
    ofVertex(0, OUTPUT_H - 1);
    ofVertex(outSide[3].x,outSide[3].y);
    ofVertex(outSide[2].x,outSide[2].y);
    ofEndShape(true);
    
    ofSetColor(255, 255, 255);
}


//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
    for(int i = 0; i < N_CARS; i++)
        cars[i].checkMidi(msg);
}

