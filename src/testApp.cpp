#include "testApp.h"
#include "ofxXmlSettings.h"
#include "bpmManager.h"

float NUM_SECS = 1.6;
int NUM_FRAMES = 24.0*NUM_SECS;


int offset = 0;
float dur;
ofImage img;
float timer = 0;
bool bDown = false;


//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_ERROR);
	organizer.loadMoviesFromDir();
	editor.setCutReference(organizer.getMovieRef());
	editor.enableMouseKeys();
	organizer.enableMouseKeys();
	ofSetFrameRate(60);
	editor.setBpmManager(&myBpmManager);
}
//--------------------------------------------------------------
void testApp::update(){
	if (organizer.bUpdatedMovieRef) {
	editor.setCutReference(organizer.getMovieRef());	
		editor.updateCutRef();
		organizer.bUpdatedMovieRef=false;
	}
}
//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(0,0,0), ofColor(50, 50, 50), OF_GRADIENT_CIRCULAR);
	ofSetColor(255);
	organizer.draw();
	editor.draw();
	myBpmManager.draw();
	/*
	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255, 0, 0);
	ofLine(0, organizer.viewport.y, organizer.viewport.width, organizer.viewport.y);	
	ofPopStyle();
	 //*/
}
//--------------------------------------------------------------
void testApp::keyPressed  (int key){}
//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	editor.setViewport();
	organizer.setViewport();
}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}
