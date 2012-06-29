/*
 *  movieCutOrganizer.h
 *  videoPlayerExample
 *
 *  Created by Roy Macdonald on 6/26/12.
 *  Copyright 2012 micasa. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "movieCut.h"

#define SCROLLWIDTH 30
#define THUMB_MARGIN 10

class movieCutOrganizer {
public:
	
	
	vector <movieCut> thumbs;
	vector <string> videos;
	
	int selected;
	int placedIndex;
	movieCut* videoPointer;
	ofRectangle viewport;
	ofRectangle vScroll;
	bool bUpdatedMovieRef;
	bool bDragging;
	bool bVideoClicked;
//--------------------------------------------------------------

	movieCutOrganizer() {
	selected = 0;
	placedIndex = 0;
	setViewport();
	bUpdatedMovieRef = false;
	bDragging = false;
	bVideoClicked = false;
		ofAddListener(ofEvents().windowResized, this, &movieCutOrganizer::windowResized);	
	}
//--------------------------------------------------------------	
	void windowResized(ofResizeEventArgs &args){
		setViewport();	
		reSort();
	}
//--------------------------------------------------------------
	void setViewport(float x = 0, float y =  2*ofGetHeight()/3, float w = ofGetWidth(), float h = ofGetHeight()/3){
		vScroll.set(w-SCROLLWIDTH+x, y, SCROLLWIDTH, h);
		viewport.set(x, y, w-SCROLLWIDTH, h);
		
	}

//--------------------------------------------------------------
	movieCut* getMovieRef(){
		return videoPointer;
	}
//--------------------------------------------------------------
	void setMovieRef(){
		if (thumbs.size() >0) {
			videoPointer = &thumbs[selected];
			bUpdatedMovieRef =true;
		}
	}
//--------------------------------------------------------------
	void draw(){
		ofPushView();
		ofViewport(viewport);
		ofSetupScreen();
		for(int i = 0; i < thumbs.size(); i++){
			if (i == selected) {
				ofPushStyle();
				ofSetColor(255, 240, 0);
				ofRect(thumbs[i].r.x-4, thumbs[i].r.y-4, thumbs[i].r.width+8, thumbs[i].r.height+8);
				ofPopStyle();
			}
			thumbs[i].draw();
		}
		if( bDragging && thumbs.size() > 0){
			ofSetColor(255, 190, 50);
			ofRect(thumbs[placedIndex].r.x - 5, thumbs[placedIndex].r.y, 4, 80);
		}
		ofSetColor(255);
		ofPopView();
	}
//--------------------------------------------------------------
void loadMoviesFromDir(string dir = "movies/"){

	ofDirectory lister;
	lister.listDir(dir);
	for(int i = 0; i < lister.size(); i++){
		videos.push_back(lister.getPath(i));
	}
	
	
	doFirstLoad();
	randomizeThumbs();
}
//--------------------------------------------------------------
void nextVideo(){
	if( selected < thumbs.size()-1 ){
		selected ++;
		setMovieRef();
	}
}
//--------------------------------------------------------------
	void previousVideo(){
		if (selected>0) {
			selected--;
			setMovieRef();
		}	
	}
//--------------------------------------------------------------
void reSort(){
	float x = viewport.x + THUMB_MARGIN;
	float y = viewport.y + THUMB_MARGIN;
	
	for(int i = 0; i < thumbs.size(); i++){
		thumbs[i].r.x = x;
		thumbs[i].r.y = y;
		
		x += thumbs[i].r.width + THUMB_MARGIN;
		
		if( x + thumbs[i].r.width >= viewport.width + viewport.x - THUMB_MARGIN){
			x = viewport.x + THUMB_MARGIN;
			y += 100;
		}
		
	}
}
//--------------------------------------------------------------
void randomizeThumbs(){
	ofRandomize(thumbs);
	
	reSort();
}
//--------------------------------------------------------------
void saveOrder(){
	ofxXmlSettings xml;
	for(int i = 0; i < thumbs.size(); i++){
		xml.setValue("videoPath", thumbs[i].videoPath, i);
	}
	xml.saveFile("order.xml");
}
//--------------------------------------------------------------
void doFirstLoad(){
	
	float x = 10;
	float y = 10;
	
	thumbs.clear();
	for(int i = 0; i < videos.size(); i++){
		string name = videos[i].substr(7, videos[i].length()-7);
		cout << "Loading video: " << videos[i] << endl;
		string thumb = "thumbs/"+name+".jpg";
		//cout <<"thumb " << thumb <<endl;
		thumbs.push_back(movieCut());
		thumbs.back().setup(videos[i], !ofFile::doesFileExist(thumb));
	}
	setMovieRef();
}
	
//--------------------------------------------------------------
void loadOrder(){
	ofxXmlSettings xml;
	if( xml.loadFile("order.xml") ){
		
		videos.clear();
		int num = xml.getNumTags("videoPath");
		
		for(int i = 0; i < num; i++){
			videos.push_back( xml.getValue("videoPath", "null", i) );
		}
		
		doFirstLoad();
	}
}
//--------------------------------------------------------------
void reorganizeThumbs(){
	if( selected == placedIndex ) return;
	
	vector <movieCut> tmp;
	
	for(int i = 0; i < thumbs.size(); i++){
		if( i == placedIndex ){
			tmp.push_back( thumbs[selected] );
		}
		if( i != selected ){
			tmp.push_back( thumbs[i] );
		}
	}
	selected == placedIndex; //probar
	thumbs.clear();
	thumbs = tmp;
	
	reSort();
	saveOrder();	
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
		if( viewport.inside(mouse.x, mouse.y)){
			for(int i = 0; i < thumbs.size(); i++){
				if(thumbs[i].r.inside(mouse.x, mouse.y)){
					selected = i;
					bVideoClicked =true;
					setMovieRef();
					break;
				}
			}
		}
	}
//--------------------------------------------------------------
	void mouseDragged(ofMouseEventArgs & mouse){
		if( viewport.inside(mouse.x, mouse.y)){
			if (bVideoClicked) {
				bDragging =true;
				bVideoClicked =false;
			}
			for(int i = 0; i < thumbs.size(); i++){
				if( thumbs[i].r.inside(mouse.x, mouse.y) ){
					placedIndex = i;
					cout << "placedIndex " << placedIndex <<endl;
				}
			}
		}else {
			bDragging=false;
		}

	}
//--------------------------------------------------------------
	void mouseReleased(ofMouseEventArgs & mouse){
		if (bDragging){
			reorganizeThumbs();
		}
		bDragging = false;
		bVideoClicked = false;

	}
//--------------------------------------------------------------		
	void keyPressed(ofKeyEventArgs & key){
		if(key.key =='e'){
			reSort();
		}
		if( key.key == OF_KEY_RIGHT ){
			nextVideo();	
		}
		if( key.key == OF_KEY_LEFT ){
			previousVideo();	
		}
		if( key.key == 'r' ){
			randomizeThumbs();
		}
		if( key.key == 'l' ){
			loadOrder();
		}
		if( key.key == 'm' ){
			reSort();		
		}
	}
//--------------------------------------------------------------		
	void keyReleased(ofKeyEventArgs & key){

	}
};
