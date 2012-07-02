/*
 *  baseViewport.h
 *  videoPlayerExample
 *
 *  Created by Roy Macdonald on 6/29/12.
 *  Copyright 2012 micasa. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"

#define SCROLLWIDTH 20
#define FRAME_WIDTH 4
#define BACKGROUND_COLOR 150
#define BG_SHADOW_OFFSET 60
class baseViewport{
public:
	ofRectangle myViewport, frame;
	ofRectangle vScroll, hScroll;
	float xOffset, yOffset;
	bool bHScrollEnabled, bVScrollEnabled;
	baseViewport(){
		bHScrollEnabled=false;
		bVScrollEnabled=false;
		xOffset = 0;
		yOffset = 0;
	}
	//--------------------------------------------------------------	
	virtual void windowResized(ofResizeEventArgs &args){
		setViewport();	
		
	}
	//--------------------------------------------------------------
	virtual void setViewport(float x = 0, float y =  2*ofGetHeight()/3, float w = ofGetWidth(), float h = ofGetHeight()/3){
		frame.set(x, y, w, h);
		myViewport.set(x+FRAME_WIDTH, y+FRAME_WIDTH, w-(FRAME_WIDTH *2), h-(FRAME_WIDTH *2));
		vScroll.set(myViewport.width+ myViewport.x-SCROLLWIDTH, myViewport.y, SCROLLWIDTH, myViewport.height-1);
		hScroll.set(myViewport.x, myViewport.height + myViewport.y -SCROLLWIDTH, myViewport.width-1	, SCROLLWIDTH);
		
		cout << "setmyViewport " << x << ", " << y << ", " << w << ", " << h << endl;
	}
	//--------------------------------------------------------------
	void drawBorders(){
		ofPushStyle();
		ofFill();
		ofSetColor(BACKGROUND_COLOR);
		ofRect(frame.x, frame.y, frame.width, FRAME_WIDTH);
		ofRect(frame.x + frame.width, frame.y, -FRAME_WIDTH, frame.height);
		ofRect(frame.x, frame.y + frame.height, frame.width, -FRAME_WIDTH);
		ofRect(frame.x, frame.y, FRAME_WIDTH, frame.height);
		ofSetColor(BACKGROUND_COLOR-BG_SHADOW_OFFSET);
		ofNoFill();
		ofLine(myViewport.x, myViewport.y, myViewport.x + myViewport.width, myViewport.y);
		ofLine(myViewport.x, myViewport.y, myViewport.x, myViewport.y+myViewport.height);
		ofSetColor(BACKGROUND_COLOR+BG_SHADOW_OFFSET);
		ofLine(myViewport.x, myViewport.y + myViewport.height, myViewport.x + myViewport.width, myViewport.y + myViewport.height);
		ofLine(myViewport.x + myViewport.width, myViewport.y, myViewport.x + myViewport.width, myViewport.y + myViewport.height);
		ofPopStyle();
		
		ofPushStyle();
		
		if (bVScrollEnabled) {
			ofSetColor(50);
			ofRect(vScroll);
			ofRectangle bar(vScroll.x, vScroll.y + yOffset*(vScroll.height -8) , SCROLLWIDTH, 4);
			if(bar.inside(ofGetMouseX(), ofGetMouseY())){
				ofSetColor(255, 240, 0);
			}else {
				ofSetColor(200);	
			}
			ofRect(bar);
		}
		if (bHScrollEnabled) {
			ofSetColor(50);
			ofRect(hScroll);
			ofSetColor(200);
			ofRectangle bar(hScroll.x + xOffset*(hScroll.width -8), hScroll.y, 4, SCROLLWIDTH);
			if(bar.inside(ofGetMouseX(), ofGetMouseY())){
				ofSetColor(255, 240, 0);
			}else {
				ofSetColor(200);	
			}
			ofRect(bar);
		}
		
		ofPopStyle();
		
	}
	//--------------------------------------------------------------
	void doScroll(ofMouseEventArgs & mouse){
		if (bHScrollEnabled) {
			if (hScroll.inside(mouse.x, mouse.y)) {
				xOffset = (mouse.x - hScroll.y)/(hScroll.width-4);
			}
		}
		if (bVScrollEnabled) {
			if (vScroll.inside(mouse.x, mouse.y)) {
				yOffset = (mouse.y - vScroll.y)/(vScroll.height-4);
			}
		}
	}
/*
	//--------------------------------------------------------------
	void enableMouseKeys(){
		ofRegisterMouseEvents(this);
		ofRegisterKeyEvents(this);
	}	
	//--------------------------------------------------------------
	void disableMouseKeys(){
		ofUnregisterMouseEvents(this);
		ofUnregisterKeyEvents(this);
	}	
	//--------------------------------------------------------------
	void mouseMoved(ofMouseEventArgs & mouse){
	}
	//--------------------------------------------------------------
	void mousePressed(ofMouseEventArgs & mouse){
		doScroll(mouse);
	}
	//--------------------------------------------------------------
	void mouseDragged(ofMouseEventArgs & mouse){
		doScroll(mouse);
	}
	//--------------------------------------------------------------
	void mouseReleased(ofMouseEventArgs & mouse){
		doScroll(mouse);
	}
	//--------------------------------------------------------------		
	void keyPressed(ofKeyEventArgs & key){

	}
	//--------------------------------------------------------------		
	void keyReleased(ofKeyEventArgs & key){
		
	}
	//*/
};