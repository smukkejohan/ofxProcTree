#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
 
    tree = new ofxProcTree();
    ofxProcTreeBranch::Properties * p = new ofxProcTreeBranch::Properties();
    
    p->seed = 519;
    p->segments = 14;
    p->levels = 5;
    p->vMultiplier = 1.01;
    p->twigScale = 0;
    p->initalBranchLength = 0.75;
    p->lengthFalloffFactor = 0.73;
    p->lengthFalloffPower = 0.76;
    p->clumpMax = 0.53;
    p->clumpMin = 0.419;
    p->branchFactor = 3.4;
    p->dropAmount = -0.16;
    p->growAmount = 0.619;
    p->sweepAmount = 0.01;
    p->maxRadius = 0.168;
    p->climbRate = 0.472;
    p->trunkKink = 0.06;
    p->treeSteps = 5;
    p->taperRate = 0.835;
    p->radiusFalloffRate = 0.73;
    p->twistRate = 2.29;
    p->trunkLength = 2.2;
    
    tree = new ofxProcTree(p);
    
    cam.setPosition(0, 0, -400);
    cam.setOrientation(ofVec3f(0,0,0));
    cam.lookAt(ofVec3f(0,0,0));
    
    pointLight.setPointLight();
    pointLight.setPosition(-200, 100, -200);
    pointLight.setDiffuseColor(ofFloatColor(0.8,0.7,0.9,1.0));
    
    dirLight.setDirectional();
    dirLight.setPosition(0, 0, -400);
    dirLight.lookAt(ofVec3f(0,0,0));
    
    //ofViewport(ofRectangle(-1,-1,2,2)); // Normalize viewport
    //ofSetupScreenPerspective(2,2);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.lookAt(ofVec3f(0,0,0));

    ofBackgroundGradient(ofColor(0), ofColor(40));
    ofSetColor(255);
    
    cam.begin();
    
    ofEnableDepthTest();
    ofEnableLighting();
    
    pointLight.enable();
    
    ofPushMatrix();
    ofScale(80,80,80);
    
    ofTranslate(0, -tree->mesh.getCentroid().y, 0);
    ofRotateY(ofGetElapsedTimef());
    
    
    if(!drawSkeleton) {
        tree->mesh.draw();
    } else {
        tree->drawSkeleton();
    }
    
    pointLight.disable();
    
    
    ofDisableDepthTest();
    
    ofPopMatrix();

    ofDisableLighting();
    
    
    cam.end();
    
    ofSetColor(200);
    ofDrawBitmapString("ofxProcTree example \nPress 'd' to draw skeleton \nFPS "
                       + ofToString(ofGetFrameRate()), 20, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    if(key == 'd') {
        drawSkeleton = !drawSkeleton;
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

