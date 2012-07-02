/*
 *  movieCut.h
 *  videoPlayerExample
 *
 *  Created by Roy Macdonald on 6/26/12.
 *  Copyright 2012 micasa. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

class movieCut{
public:
	movieCut(){
		inPos = 0.0;
		outPos = 1.0;
		fps = 0;
	}
//--------------------------------------------------------------
	void setup(string videoPath, bool makeThmb = true, float x =0 , float y = 0){
		thumbPath = ofFilePath::getBaseName(videoPath);
		this->videoPath = videoPath;
		
		//thumbPath = path;
		//loadPos();
		if(makeThmb) makeThumb(inPos, x, y);
	}
//--------------------------------------------------------------
	//*
	void loadPos(){
		ofxXmlSettings xml;
		if( xml.loadFile(thumbPath + ".xml") ){
			inPos = xml.getValue("inPos", inPos);
			outPos = xml.getValue("outPos", outPos);
		}
	}
//--------------------------------------------------------------
	void savePos(){
		ofxXmlSettings xml;
		xml.setValue("inPos", inPos);
		xml.setValue("outPos", outPos);
		xml.saveFile(thumbPath + ".xml");
	}
	//*/
//--------------------------------------------------------------
	void draw(){
		img.draw(r.x,r.y);
	}
//--------------------------------------------------------------
	void draw(float x, float y){
		r.x = x;
		r.y = y;
		img.draw(r.x,r.y);
	}
//--------------------------------------------------------------
	void makeThumb(float p = 0.3f, float thmbX = 0, float thmbY =0){
		ofVideoPlayer tmp;
		tmp.loadMovie(videoPath);
		tmp.play();
		tmp.setPosition(p);
		fps = tmp.getTotalNumFrames()/tmp.getDuration();
		cout << "FPS: " << fps << endl;
		img.setFromPixels( tmp.getPixelsRef() );
		img.resize(120, 120.0f * (img.getHeight() / img.getWidth()) );
		img.saveImage("thumbs/"+thumbPath + ".jpg");
		r.set(thmbX, thmbY,img.getWidth(), img.getHeight());
		img.setAnchorPoint(0, 0);
	}
//--------------------------------------------------------------
	string videoPath;
	float inPos, outPos;
	string thumbPath;
	
	ofRectangle r;
	ofImage img;
	float fps;
};