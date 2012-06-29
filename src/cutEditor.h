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

class cutEditor : public baseViewport {

public:
ofVideoPlayer fullVid;
	bool bWasPaused;
	bool bMovieLoaded;
	
//--------------------------------------------------------------
	cutEditor(){
		setViewport();
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
			playhead.x = x * (viewport.width-10) + viewport.x;
			if (updateY) {
				playhead.y = viewport.height-10 + viewport.y;				
			}
		}
	}
//--------------------------------------------------------------
	void setCutReference(movieCut* ref){
		currentCut = ref;
	}
//--------------------------------------------------------------
	void update(ofEventArgs & args){
		if (bMovieLoaded) {
			fullVid.update();
			setPlayheadPos(fullVid.getPosition());
		}
	}
//--------------------------------------------------------------
	void updateCutRef(){
		cout << "updatedCutRef ";
		if (currentCut != NULL) {
			cout << "  ->currentCut != NULL";	
			if (bMovieLoaded) {
				fullVid.close();
			}
			cout << "  " << currentCut->videoPath; 
			bMovieLoaded = fullVid.loadMovie(currentCut->videoPath);
			fullVid.setPosition(currentCut->inPos);
			fullVid.update();
			//fullVid.setAnchorPoint(0.5, 1);
		}
		cout << endl;
	}
//--------------------------------------------------------------
	void setViewport(float x = 0, float y =  0, float w = ofGetWidth(), float h = 2*ofGetHeight()/3){
		baseViewport::setViewport(x, y, w, h);
		playhead.set(viewport.x, viewport.height-10 + viewport.y, 4, 10);
	}
//--------------------------------------------------------------
	void draw(){//void draw(ofEventArgs & args){
		if (bMovieLoaded && currentCut != NULL) {
			float x, y, w=fullVid.width, h=fullVid.height;
			
			if(viewport.width<=fullVid.width || viewport.height<=fullVid.height){
				float viewAspect = viewport.width/viewport.height;	
				float vidAspect = float(fullVid.width)/fullVid.height;
				if (viewAspect>vidAspect) {
					h=viewport.height;
					w=h*vidAspect;
				}else {
					w=viewport.width;
					h=w/vidAspect;
				}
			}
			x=(viewport.width - w)/2;
			y=(viewport.height - h)/2;
			fullVid.draw(x, y, w, h);
		ofPushStyle();
		ofSetColor(200);
		ofRect(currentCut->inPos * (viewport.width-10) + viewport.x, viewport.height-10+viewport.y, 4, 10);
		ofRect(currentCut->outPos * (viewport.width-10) + viewport.x, viewport.height-10+viewport.y, 4, 10);
		if (playhead.inside(ofGetMouseX(), ofGetMouseY())) {
			ofSetColor(255, 255, 0);
		}
		ofRect(playhead);
		ofPopStyle();
		}
		drawBorders();
	}
//--------------------------------------------------------------	
	void setVideoPos(float mousex){
		fullVid.setPosition(ofMap(mousex - viewport.x, 0, viewport.width, 0, 0.9999, true));
		fullVid.update();
	}
//--------------------------------------------------------------	
	void setInPos(){
		if (currentCut != NULL) {
			currentCut->inPos = fullVid.getPosition();
		}
	}	
//--------------------------------------------------------------	
	void setOutPos(){
		if (currentCut != NULL) {
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
		if(viewport.inside(mouse.x, mouse.y)){	
			setVideoPos(mouse.x);
		}
	}
//--------------------------------------------------------------		
	void mousePressed(ofMouseEventArgs & mouse){
		if(viewport.inside(mouse.x, mouse.y)){	
			bWasPaused = fullVid.isPaused();
			fullVid.setPaused(true);
			setVideoPos(mouse.x);
			//bDown = true;		
		}
	}
//--------------------------------------------------------------		
	void mouseReleased(ofMouseEventArgs & mouse){
		if(viewport.inside(mouse.x, mouse.y)){
			setVideoPos(mouse.x);
			if (!bWasPaused) {
				fullVid.setPaused(false);		
				fullVid.play();
			}
			//timer = ofGetElapsedTimef() + NUM_SECS;		
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
			default:
				break;
		}
	}
//--------------------------------------------------------------		
private:
	movieCut* currentCut;
	ofRectangle playhead;
	
};