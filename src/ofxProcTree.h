//
//  ofxProcTree.cpp
//  Trae
//
//  Created by Johan Bichel Lindegaard and Ole Kristensen on 27/03/14.
//
//

// Based on https://github.com/supereggbert/proctree.js by Paul Brunt

#include "ofMain.h"
#include <array.h>


namespace ofxProcTree {
    
    class Branch {
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
        
        Branch(ofVec3f * _head = NULL, Branch * _parent = NULL);
        ~Branch();
        
        ofVec3f mirrorBranch(ofVec3f vec, ofVec3f norm, Properties * prop);
        void split(Properties * prop);
        void split(int level, int steps, Properties * prop, int l1, int l2);

        float length = 1;
        float radius;
        string type;

        ofVec3f * head  = NULL;
        ofVec3f * tangent = NULL;

        Branch * parent = NULL;
        Branch * child0 = NULL;
        Branch * child1 = NULL;
        
        vector<int> * root;
        vector<int> * ring0;
        vector<int> * ring1;
        vector<int> * ring2;
        
        int end;        
    };
    
    
    class Tree {
    public:
        
        Branch * troot;
        Branch::Properties * props;
        
        Tree(){
            cout << "new Tree" << endl;
            props = new Branch::Properties();
            props->seed = ofRandom(0.0,10.0);
            init();
        };

        
        Tree(Branch::Properties * _properties){
            cout << "new Tree with props" << endl;
            props = _properties;
            init();
        };
        
        void init(){
            troot = new Branch(new ofVec3f(0,props->trunkLength,0));
            troot->length = props->initalBranchLength;
            troot->split(props);
        }

        ~Tree(){};
        
        void calcNormals();
        void doFaces(Branch * branch = NULL);
        void createTwigs(Branch * branch = NULL);
        void createForks(Branch * branch = NULL, float radius = -1);
        
        vector<ofVec3f> verts;
        vector<vector<int> > faces;
        vector<ofVec3f> normals;
        vector<ofVec2f> UV;
        
        ofMesh mesh;
        
        void drawSkeleton();
        
        //void flattenArray(input);
        
    };
    
    
    class TreeUtils { // We probably don't need any of this
        
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
}
