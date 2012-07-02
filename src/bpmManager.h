/*
 *  bpmManager.h
 *  BPMautoMovie
 *
 *  Created by Roy Macdonald on 7/1/12.
 *  Copyright 2012 micasa. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"


class bpmManager {
public:
	float bpm;
	int blacksPerBar;
	int bars;
	ofRectangle frame;
	//--------------------------------------------------------------
	bpmManager(){
		bpm = 120;
		blacksPerBar = 4;
		bars = 2;
		
		frame.set(50,50, 200, 80);
		
	}
	//--------------------------------------------------------------
	float getDuration(){
		return blacksPerBar * bars / (bpm /60);
	}
	//--------------------------------------------------------------
	void setup(){
	}
	
	void draw(float x = 0, float y = 0){
		ofPushStyle();
		ofSetColor(100, 200);
		ofFill();
		ofRect(frame);
		ofNoFill();
		ofSetColor(150, 200);
		ofRect(frame);
		ofSetColor(0);
		ofDrawBitmapString("--BPM MANAGER--", ofPoint(frame.x+30, frame.y+10));
		ofDrawBitmapString(" BPM: " + ofToString(bpm), ofPoint(frame.x+30, frame.y+30));
		ofDrawBitmapString(" BARS: " + ofToString(bars), ofPoint(frame.x+30, frame.y+50));
		ofPopStyle();
	} 
	
};

