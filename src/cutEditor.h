/*
 *  cutEditor.h
 *  videoPlayerExample
 *
 *  Created by Roy Macdonald on 6/28/12.
 *  Copyright 2012 micasa. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "movieCut.h"
#include "baseViewport.h"
#include "bpmManager.h"

#define TIMELINE_HEIGHT 15

class cutEditor : public baseViewport {
	//--------------------------------------------------------------		
private:
	movieCut* currentCut;
	ofRectangle playhead;
	ofRectangle timeline;
	bpmManager* bpm;
	bool bBpmLock;
	
public:
	ofVideoPlayer fullVid;
	bool bWasPaused;
	bool bMovieLoaded;
	bool bDoLoop;
	//--------------------------------------------------------------
	cutEditor(){
		setViewport();
		bBpmLock = true;
		bDoLoop = true;
		bWasPaused = false;
		bMovieLoaded = false;
		ofAddListener(ofEvents().update, this, &cutEditor::update);
		//	ofAddListener(ofEvents().draw, this, &cutEditor::draw);
	}
	//--------------------------------------------------------------
	~cutEditor(){
		ofRemoveListener(ofEvents().update, this, &cutEditor::update);
		//ofRemoveListener(ofEvents().draw, this, &cutEditor::draw);
	}
	//--------------------------------------------------------------
	//*
	void enableMouseKeys(){
		ofRegisterMouseEvents(this);
		ofRegisterKeyEvents(this);
	}	
	//--------------------------------------------------------------
	void disableMouseKeys(){
		ofUnregisterMouseEvents(this);
		ofUnregisterKeyEvents(this);
	}
	//*/
	//--------------------------------------------------------------
	void setPlayheadPos(float x =0, bool updateY = false){
		if (x>0 && x<1) {
			playhead.x = x * (myViewport.width - SCROLLWIDTH*2 -4) + myViewport.x + SCROLLWIDTH;
			if (updateY) {
				playhead.y = myViewport.height-TIMELINE_HEIGHT+ myViewport.y;				
			}
		}
	}
	
	//--------------------------------------------------------------
	void setBpmManager(bpmManager* b){
		bpm = b;
	}
	//--------------------------------------------------------------
	void setCutReference(movieCut* ref){
		currentCut = ref;
	}
	//--------------------------------------------------------------
	void update(ofEventArgs & args){
		if (bMovieLoaded) {
			fullVid.update();
			/*
			if (fullVid.getPosition() >= currentCut->outPos) {
				fullVid.setPosition(currentCut->inPos);
				fullVid.update();
			}
			//*/
			setPlayheadPos(fullVid.getPosition());
		}
	}
	//--------------------------------------------------------------
	void updateCutRef(){
		//cout << "updatedCutRef ";
		if (currentCut != NULL) {
			///			cout << "  ->currentCut != NULL";	
			if (bMovieLoaded) {
				fullVid.close();
			}
			//cout << "  " << currentCut->videoPath; 
			bMovieLoaded = fullVid.loadMovie(currentCut->videoPath);
			fullVid.setPaused(true);
			fullVid.setPosition(0.1);
			fullVid.update();
			fullVid.setPosition(currentCut->inPos);
			fullVid.update();
			if (bBpmLock) {
				currentCut->outPos =currentCut->inPos + (bpm->getDuration() / fullVid.getDuration());
			}
			//fullVid.play();
			//fullVid.setAnchorPoint(0.5, 1); // this didn't work. I don't know why. I'll take a deeper look later.
		}
		//cout << endl;
	}
	//--------------------------------------------------------------
	void setViewport(float x = 0, float y =  0, float w = ofGetWidth(), float h = 2*ofGetHeight()/3){
		baseViewport::setViewport(x, y, w, h);
		playhead.set(myViewport.x + SCROLLWIDTH, myViewport.height-TIMELINE_HEIGHT + myViewport.y, 4, TIMELINE_HEIGHT);
		timeline.set(myViewport.x + SCROLLWIDTH, myViewport.height-TIMELINE_HEIGHT + myViewport.y, myViewport.width- SCROLLWIDTH*2, TIMELINE_HEIGHT);
	}
	//--------------------------------------------------------------
	void draw(){//void draw(ofEventArgs & args){
		if (bMovieLoaded && currentCut != NULL) {
			//--------draw video. adjust size to fit viewport ---------- 
			float x, y, w=fullVid.width, h=fullVid.height;
			if(myViewport.width<=fullVid.width || myViewport.height - TIMELINE_HEIGHT<=fullVid.height){
				float viewAspect = myViewport.width/(myViewport.height - TIMELINE_HEIGHT);	
				float vidAspect = float(fullVid.width)/fullVid.height;
				if (viewAspect>vidAspect) {
					h=myViewport.height;
					w=h*vidAspect;
				}else {
					w=myViewport.width;
					h=w/vidAspect;
				}
			}
			x=(myViewport.width - w)/2;
			y=(myViewport.height - h)/2;
			fullVid.draw(x, y, w, h);
			//--------------------------------
			//------draw timeline -------------
			ofPushStyle();
			ofSetColor(85);	
			ofRect(timeline);
			ofSetColor(100);
			ofLine(timeline.x, timeline.y, timeline.x + timeline.width, timeline.y);
			//--------------------------------
			//------draw in and out markers -------------
			ofSetColor(200);
			ofRect(currentCut->inPos * (myViewport.width-SCROLLWIDTH*2) + myViewport.x + SCROLLWIDTH, myViewport.height-TIMELINE_HEIGHT+myViewport.y, 4, TIMELINE_HEIGHT);
			ofRect(currentCut->outPos * (myViewport.width-SCROLLWIDTH*2) + myViewport.x + SCROLLWIDTH, myViewport.height-TIMELINE_HEIGHT+myViewport.y, 4, TIMELINE_HEIGHT);
			//--------------------------------
			//------draw playhead--------------------------
			if (playhead.inside(ofGetMouseX(), ofGetMouseY())) {
				ofSetColor(255, 255, 0);
			}
			
			ofRect(playhead);
			ofPopStyle();
			//--------------------------------
		}
		drawBorders();
	}
	//--------------------------------------------------------------	
	void setVideoPos(float mousex){
		fullVid.setPosition(ofMap(mousex - myViewport.x - SCROLLWIDTH, 0, myViewport.width - SCROLLWIDTH*2 , 0, 0.9999, true));
		fullVid.update();
	}
	//--------------------------------------------------------------	
	void setInPos(){
		if (currentCut != NULL) {
			if (bBpmLock) {
				float tempOut = fullVid.getPosition() + bpm->getDuration() / fullVid.getDuration();
				cout << "tempOut " << tempOut << endl;
				if (tempOut <=1) {
					currentCut->outPos = tempOut;
				}else {
					return;
				}
			}
			currentCut->inPos = fullVid.getPosition();
		}
	}	
	//--------------------------------------------------------------	
	void setOutPos(){
		if (currentCut != NULL) {
			if (bBpmLock) {
				float tempIn =  fullVid.getPosition() - bpm->getDuration() / fullVid.getDuration();
				cout << "tempIn " << tempIn << endl;
				if (tempIn >= 0) {
					currentCut->inPos = tempIn;
				}else {
					return;
				}
			}
			currentCut->outPos = fullVid.getPosition();
		}
	}
	//--------------------------------------------------------------	
	void updateViewport(bool set = false){
		if (set) {
			setViewport();
		}
		setPlayheadPos(fullVid.getPosition(), true);
	}
	//--------------------------------------------------------------	
	void mouseDragged(ofMouseEventArgs & mouse){
		if(myViewport.inside(mouse.x, mouse.y)){	
			setVideoPos(mouse.x);
		}
	}
	//--------------------------------------------------------------		
	void mousePressed(ofMouseEventArgs & mouse){
		if(myViewport.inside(mouse.x, mouse.y)){	
			bWasPaused = fullVid.isPaused();
			fullVid.setPaused(true);
			setVideoPos(mouse.x);
		}
	}
	//--------------------------------------------------------------		
	void mouseReleased(ofMouseEventArgs & mouse){
		if(myViewport.inside(mouse.x, mouse.y)){
			setVideoPos(mouse.x);
			if (!bWasPaused) {
				fullVid.setPaused(false);		
				fullVid.play();
			}
		}
		
	}
	//--------------------------------------------------------------		
	void mouseMoved(ofMouseEventArgs & mouse){
	}
	//--------------------------------------------------------------		
	void keyPressed(ofKeyEventArgs & key){
		
	}
	//--------------------------------------------------------------		
	void keyReleased(ofKeyEventArgs & key){
		switch (key.key) {
			case 'i':
			case 'I':	
				setInPos();
				break;
			case 'o':
			case 'O':
				setOutPos();
				break;
			case ' ':
				fullVid.setPaused(!fullVid.isPaused());
				if (!fullVid.isPaused()) {
					fullVid.play();
				}
				break;
				
			default:
				break;
		}
	}
	
};