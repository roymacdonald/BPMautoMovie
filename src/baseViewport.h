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

#define SCROLLWIDTH 30
#define FRAME_WIDTH 5
#define BACKGROUND_COLOR 150

class baseViewport{
public:
	ofRectangle viewport, frame;
	ofRectangle vScroll, hScroll;
	float xOffset, yOffset;
	bool bHScrollEnabled, bVScrollEnabled;
	baseViewport(){
		bHScrollEnabled=true;
		bVScrollEnabled=true;
	}
	//--------------------------------------------------------------	
	virtual void windowResized(ofResizeEventArgs &args){
		setViewport();	
		xOffset = 0;
		yOffset = 0;
	}
	//--------------------------------------------------------------
	virtual void setViewport(float x = 0, float y =  2*ofGetHeight()/3, float w = ofGetWidth(), float h = ofGetHeight()/3){
		vScroll.set(w-SCROLLWIDTH+x, y, SCROLLWIDTH, h);
		hScroll.set(x, h-SCROLLWIDTH+y, w, SCROLLWIDTH);
		frame.set(x, y, w, h);
		viewport.set(x+FRAME_WIDTH, y+FRAME_WIDTH, w-SCROLLWIDTH-(FRAME_WIDTH *2), h-SCROLLWIDTH-(FRAME_WIDTH *2));
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
		ofSetColor(BACKGROUND_COLOR-20);
		ofNoFill();
		ofLine(viewport.x, viewport.y, viewport.x + viewport.width, viewport.y);
		ofLine(viewport.x, viewport.y, viewport.x, viewport.y+viewport.height);
		ofSetColor(BACKGROUND_COLOR+20);
		ofLine(viewport.x, viewport.y + viewport.height, viewport.x + viewport.width, viewport.y + viewport.height);
		ofLine(viewport.x + viewport.width, viewport.y, viewport.x + viewport.width, viewport.y + viewport.height);
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
};