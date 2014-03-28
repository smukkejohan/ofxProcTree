//
//  ofxProcTree.cpp
//  Trae
//
//  Created by Johan Bichel Lindegaard and Ole Kristensen on 27/03/14.
//
//

// Based on https://github.com/supereggbert/proctree.js by Paul Brunt

#include "ofMain.h"
#include "ofxMeshUtils.h"

class ofxProcTreeBranch {
public:
    
    struct Properties {
        
        Properties(){};
        ~Properties(){};
        
        float clumpMax              = 0.8;
        float clumpMin              = 0.5;
        float lengthFalloffFactor   = 0.85;
        float lengthFalloffPower    = 1;
        float branchFactor          = 2.0;
        float radiusFalloffRate     = 0.6;
        float climbRate             = 1.5;
        float trunkKink             = 0.00;
        float maxRadius             = 0.25;
        int treeSteps             = 2;
        float taperRate             = 0.95;
        float twistRate             = 13;
        int segments              = 6;
        int levels                = 3;
        float sweepAmount           = 0;
        float initalBranchLength    = 0.85;
        float trunkLength           = 2.5;
        float dropAmount            = 0.0;
        float growAmount            = 0.0;
        float vMultiplier           = 0.2;
        float twigScale             = 2.0;
        float seed                  = 10;
        float rseed                 = 10;
        
        float random(){
            return this->random(rseed++);
        };
        float random(float a){
            return abs(cos(a+a*a));
        };
        
    };
    
    void drawSkeleton(){
        if(child0){
            child0->drawSkeleton();
        }
        if(child1){
            child1->drawSkeleton();
        }
        if(type == "trunk"){
            ofDrawSphere(head->x, head->y, head->z, 0.05);
        } else{
            ofDrawSphere(head->x, head->y, head->z, 0.01);
        }
        if(parent){
            ofLine(*(parent->head), *head);
        } else {
            ofDrawSphere(ofVec3f(0,0,0), 0.08);
            ofLine(ofVec3f(0,0,0), *head);
        }
    }
    
    ofxProcTreeBranch(ofVec3f * _head = NULL, ofxProcTreeBranch * _parent = NULL);
    ~ofxProcTreeBranch();
    
    ofVec3f mirrorBranch(ofVec3f vec, ofVec3f norm, Properties * prop);
    void split(Properties * prop);
    void split(int level, int steps, Properties * prop, int l1, int l2);
    
    float length = 1;
    float radius;
    string type;
    
    ofVec3f * head  = NULL;
    ofVec3f * tangent = NULL;
    
    ofxProcTreeBranch * parent = NULL;
    ofxProcTreeBranch * child0 = NULL;
    ofxProcTreeBranch * child1 = NULL;
    
    vector<int> * root;
    vector<int> * ring0;
    vector<int> * ring1;
    vector<int> * ring2;
    
    int * end;
};


class ofxProcTree {
public:
    
    ofxProcTreeBranch * troot;
    ofxProcTreeBranch::Properties * props;
    
    ofxProcTree(){
        cout << "new ofxProcTree" << endl;
        props = new ofxProcTreeBranch::Properties();
        props->seed = ofRandom(0.0,10.0);
        init();
    };
    
    
    ofxProcTree(ofxProcTreeBranch::Properties * _properties){
        cout << "new ofxProcTree with props" << endl;
        props = _properties;
        init();
    };
    
    void init(){
        troot = new ofxProcTreeBranch(new ofVec3f(0,props->trunkLength,0));
        troot->length = props->initalBranchLength;
        troot->split(props);
        createForks(troot);
        //createTwigs();
        doFaces(troot);
        //calcNormals();
        
        mesh.addVertices(verts);
        //mesh.addNormals(normals);
        mesh.addTexCoords(UV);
        for (vector< vector<int> >::iterator it = faces.begin();it != faces.end(); ++it){
            vector<int> v = *(it);
            mesh.addTriangle(v[0], v[1], v[2]);
        }
        
        mesh.mergeDuplicateVertices();
        
        ofxMeshUtils::calcNormals(mesh);
        
    }
    
    ~ofxProcTree(){};
    
    void doFaces(ofxProcTreeBranch * b);
    void createTwigs(ofxProcTreeBranch * b);
    void createForks(ofxProcTreeBranch * b, float radius = -1);
    
    vector<ofVec3f> verts;
    vector<vector<int> > faces;
    vector<ofVec3f> normals;
    vector<ofVec2f> UV;
    
    ofVboMesh mesh;
    
    void drawSkeleton();
    
    //void flattenArray(input);
    
};


class ofxProcTreeUtils {
public:
    
    static ofVec3f scaleInDirection(ofVec3f vec, ofVec3f dir, float scale){
        float currentMag = vec.dot(dir);
        ofVec3f change = dir.getScaled(currentMag*scale-currentMag);
        return ofVec3f(vec + change);
    }
    
    static ofVec3f vecAxisAngle(ofVec3f vec, ofVec3f axis, float angle){
        float cosr=cos(angle);
        float sinr=sin(angle);
        return ofVec3f((vec.getScaled(cosr) + axis.getCrossed(vec).getScaled(sinr))+ axis.getScaled(axis.dot(vec)*(1-cosr)));
    };
};
