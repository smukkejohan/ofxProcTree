//
//  ofxSnappyTree.cpp
//  Trae
//
//  Created by Johan Bichel Lindegaard on 27/03/14.
//
//

// Based on https://github.com/supereggbert/proctree.js

#include "ofMain.h"

class ProcTree {
public:
    ProcTree(){
        
        
    };
    ~ProcTree(){};
    
    float clumpMax              = 0.8;
    float clumpMin              = 0.5;
    float lengthFalloffFactor   = 0.85;
    float lengthFalloffPower    = 1;
    float branchFactor          = 2.0;
    float radiusFalloffRate     = 0.6;
    float climbRate             = 1.5;
    float trunkKink             = 0.00;
    float maxRadius             = 0.25;
    float treeSteps             = 2;
    float taperRate             = 0.95;
    float twistRate             = 13;
    float segments              = 6;
    float levels                = 3;
    float sweepAmount           = 0;
    float initalBranchLength    = 0.85;
    float trunkLength           = 2.5;
    float dropAmount            = 0.0;
    float growAmount            = 0.0;
    float vMultiplier           = 0.2;
    float twigScale             = 2.0;
    float seed                  = 10;
    float rseed                 =10;
    
    
    
    
};