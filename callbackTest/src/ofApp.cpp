#include "ofApp.h"

/*

 Example using easing parameters.
 First 3 sliders control RGB
 We are using pEasy which is pFloat values with easing

 Presets can be loaded using 1, 2, 3, 4 keys
 and saved using COMMAND + 1, 2, 3, 4 keys

 */


//--------------------------------------------------------------
void ofApp::setup(){
	ui.setup();
	ui.createFromText("sliders.txt");
	ofSetCircleResolution(94);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(30);
	ofSetColor(255);
	float tempo = ofGetElapsedTimef();
	float x = ofNoise(tempo/5.0 * ui.pEasy["DivNoiseX"])*ofGetWindowWidth();
	float y = ofNoise(tempo/7.0 * ui.pEasy["DivNoiseY"])*ofGetWindowHeight();
	float raio = 200;
	ofSetColor(ui.pEasy["r"], ui.pEasy["g"], ui.pEasy["b"]);
	ofDrawEllipse(x,y,raio,raio);
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}
