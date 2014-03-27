//
//  ofxSnappyTree.cpp
//  Trae
//
//  Created by Johan Bichel Lindegaard on 27/03/14.
//
//

// Based on https://github.com/supereggbert/proctree.js by Paul Brunt

#include "ofMain.h"



struct Properties {
    
    Properties();
    ~Properties();
    
    void setSeed(float rseed);
    
    float clumpMax              = 0.8;
    float clumpMin              = 0.5;
    float lengthFalloffFactor   = 0.85;
    float lengthFalloffPower    = 1;
    float branchFactor          = 2.0;
    float radiusFalloffRate     = 0.6;
    float climbRate             = 1.5;
    float trunkKink             = 0.00;
    float maxRadius             = 0.25;
    int treeSteps               = 2;
    float taperRate             = 0.95;
    float twistRate             = 13;
    int segments                = 6;
    int levels                  = 3;
    float sweepAmount           = 0;
    float initalBranchLength    = 0.85;
    float trunkLength           = 2.5;
    float dropAmount            = 0.0;
    float growAmount            = 0.0;
    float vMultiplier           = 0.2;
    float twigScale             = 2.0;
    float seed                  = 10;
    float rseed                 =10;

    float random(float a){
        return abs(cos(a+a*a));
    };
    
};


class Branch {
public:
    
    Branch * head   = NULL;
    Branch * paren  = NULL;
    Branch * child0 = NULL;
    Branch * child1 = NULL;
    
    Branch(ofVec3f _head);
    Branch(Branch * _head = NULL, Branch * _parent = NULL);
    ~Branch();
    
    int length = 1;
    
    void mirrorBranch(ofVec3f vec, ofVec3f norm, Properties * prop);
    void split(Properties * prop);
    void split(int level, int steps, Properties * prop, int l1, int l2);

    ofMesh mesh;
    
};


class ProcTree {
public:
    
    Branch * troot;
    Properties * props;
    
    ProcTree(){
        props = new Properties();
        
        props->setSeed(ofRandom(0.0,10.0));
        troot = new Branch(ofVec3f(0,props->trunkLength,0));
        
        troot->length = props->initalBranchLength;
        troot->split(props);
        
    };
    ~ProcTree(){};
    
    void calcNormals();
    void doFaces(Branch * branch);
    void createTwigs(Branch * branch);
    void createForks(Branch * branch, float radius);
    
    ofMesh mesh;
    
    //void flattenArray(input);
    
    
    
    
};


class TreeUtils { // We probably don't need any of this
    /*
    var dot=function(v1,v2){
        return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
    };
    var cross=function(v1,v2){
        return [v1[1]*v2[2]-v1[2]*v2[1],v1[2]*v2[0]-v1[0]*v2[2],v1[0]*v2[1]-v1[1]*v2[0]];
    };
    var length=function(v){
        return Math.sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    };
    var normalize=function(v){
        var l=length(v);
        return scaleVec(v,1/l);
    };
    var scaleVec=function(v,s){
        return [v[0]*s,v[1]*s,v[2]*s];
    };
    var subVec=function(v1,v2){
        return [v1[0]-v2[0],v1[1]-v2[1],v1[2]-v2[2]];
    };
    var addVec=function(v1,v2){
        return [v1[0]+v2[0],v1[1]+v2[1],v1[2]+v2[2]];
    };
    
    var vecAxisAngle=function(vec,axis,angle){
        //v cos(T) + (axis x v) * sin(T) + axis*(axis . v)(1-cos(T)
        var cosr=Math.cos(angle);
        var sinr=Math.sin(angle);
        return addVec(addVec(scaleVec(vec,cosr),scaleVec(cross(axis,vec),sinr)),scaleVec(axis,dot(axis,vec)*(1-cosr)));
    };
    
    var scaleInDirection=function(vector,direction,scale){
        var currentMag=dot(vector,direction);
        
        var change=scaleVec(direction,currentMag*scale-currentMag);
        return addVec(vector,change);
    };
    */
    
};
