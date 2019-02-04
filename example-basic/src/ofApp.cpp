#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    
    ofSetCircleResolution(200);
    
    ofNoFill();
    
    internal[3] = vector<int>{0, 4, 7, 6, 2};
    internal[4] = vector<int>{0, 1, 5, 8, 7, 3};
    internal[6] = vector<int>{2, 3, 7, 9, 12};
    internal[7] = vector<int>{3, 4, 8, 10, 9, 6};
    internal[8] = vector<int>{4, 5, 11, 15, 10, 7};
    internal[9] = vector<int>{7, 10, 14, 12, 6};
    internal[10] = vector<int>{7, 8, 15, 14, 9};
    internal[12] = vector<int>{2, 6, 9, 14, 13};
    internal[14] = vector<int>{13, 12, 9, 10, 15, 18, 17};
    internal[15] = vector<int>{10, 8, 11, 16, 18, 14};
    
    external[0] = ofRandom(0, 100) + 50;
    external[1] = ofRandom(0, 100) + 50;
    external[2] = ofRandom(0, 100) + 50;
    external[5] = ofRandom(0, 100) + 50;
    external[11] = ofRandom(0, 100) + 50;
    external[13] = ofRandom(0, 100) + 50;
    external[16] = ofRandom(0, 100) + 50;
    external[17] = ofRandom(0, 100) + 50;
    external[18] = ofRandom(0, 100) + 50;
    
    cp.pack(internal, external);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cp.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') {
        for (auto& x : external) {
            x.second = ofRandom(0, 100) + 50;
        }
        cp.pack(internal, external);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
