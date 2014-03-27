//
//  ofxSnappyTree.cpp
//  Trae
//
//  Created by Johan Bichel Lindegaard on 27/03/14.
//
//

// Based on https://github.com/supereggbert/proctree.js

#include "ofxProcTree.h"

void ProcTree::calcNormals(){
    
    // implement using https://github.com/ofZach/ofxMeshUtils.git
    // ofxMeshUtils::calcNormals( mesh, true );
    
    /* reference implementation
     
        var normals=this.normals;
        var faces=this.faces;
        var verts=this.verts;
        var allNormals=[];
        for(var i=0;i<verts.length;i++){
            allNormals[i]=[];
        }
        for(i=0;i<faces.length;i++){
            var face=faces[i];
            var norm=normalize(cross(subVec(verts[face[1]],verts[face[2]]),subVec(verts[face[1]],verts[face[0]])));
            allNormals[face[0]].push(norm);
            allNormals[face[1]].push(norm);
            allNormals[face[2]].push(norm);
        }
        for(i=0;i<allNormals.length;i++){
            var total=[0,0,0];
            var l=allNormals[i].length;
            for(var j=0;j<l;j++){
                total=addVec(total,scaleVec(allNormals[i][j],1/l));
            }
            normals[i]=total;
        }
    };
     */
};

void ProcTree::doFaces(Branch *branch){
    if(!branch) {
        branch = new Branch(this->troot);
    }
    
    int segments = props->segments;
   
    
    /* reference implementation
     
    if(!branch) branch=this.root;
        var segments=this.properties.segments;
        var faces=this.faces;
        var verts=this.verts;
        var UV=this.UV;
        if(!branch.parent){
            for(i=0;i<verts.length;i++){
                UV[i]=[0,0];
            }
            var tangent=normalize(cross(subVec(branch.child0.head,branch.head),subVec(branch.child1.head,branch.head)));
            var normal=normalize(branch.head);
            var angle=Math.acos(dot(tangent,[-1,0,0]));
            if(dot(cross([-1,0,0],tangent),normal)>0) angle=2*Math.PI-angle;
            var segOffset=Math.round((angle/Math.PI/2*segments));
            for(var i=0;i<segments;i++){            
                var v1=branch.ring0[i];
                var v2=branch.root[(i+segOffset+1)%segments];
                var v3=branch.root[(i+segOffset)%segments];
                var v4=branch.ring0[(i+1)%segments];
                
                faces.push([v1,v4,v3]);
                faces.push([v4,v2,v3]);
                UV[(i+segOffset)%segments]=[Math.abs(i/segments-0.5)*2,0];
                var len=length(subVec(verts[branch.ring0[i]],verts[branch.root[(i+segOffset)%segments]]))*this.properties.vMultiplier;
                UV[branch.ring0[i]]=[Math.abs(i/segments-0.5)*2,len];
                UV[branch.ring2[i]]=[Math.abs(i/segments-0.5)*2,len];
            }
        }
        
        if(branch.child0.ring0){
            var segOffset0,segOffset1;
            var match0,match1;
            
            var v1=normalize(subVec(verts[branch.ring1[0]],branch.head));
            var v2=normalize(subVec(verts[branch.ring2[0]],branch.head));
            
            v1=scaleInDirection(v1,normalize(subVec(branch.child0.head,branch.head)),0);
            v2=scaleInDirection(v2,normalize(subVec(branch.child1.head,branch.head)),0);
            
            for(i=0;i<segments;i++){
                var d=normalize(subVec(verts[branch.child0.ring0[i]],branch.child0.head));
                var l=dot(d,v1);
                if(segOffset0===undefined || l>match0){
                    match0=l;
                    segOffset0=segments-i;
                }
                d=normalize(subVec(verts[branch.child1.ring0[i]],branch.child1.head));
                l=dot(d,v2);
                if(segOffset1==undefined || l>match1){
                    match1=l;
                    segOffset1=segments-i;
                }
            }
            
            var UVScale=this.properties.maxRadius/branch.radius;            

            
            for(i=0;i<segments;i++){
                v1=branch.child0.ring0[i];
                v2=branch.ring1[(i+segOffset0+1)%segments];
                v3=branch.ring1[(i+segOffset0)%segments];
                v4=branch.child0.ring0[(i+1)%segments];
                faces.push([v1,v4,v3]);
                faces.push([v4,v2,v3]);
                v1=branch.child1.ring0[i];
                v2=branch.ring2[(i+segOffset1+1)%segments];
                v3=branch.ring2[(i+segOffset1)%segments];
                v4=branch.child1.ring0[(i+1)%segments];
                faces.push([v1,v2,v3]);
                faces.push([v1,v4,v2]);
                
                var len1=length(subVec(verts[branch.child0.ring0[i]],verts[branch.ring1[(i+segOffset0)%segments]]))*UVScale;
                var uv1=UV[branch.ring1[(i+segOffset0-1)%segments]];
                
                UV[branch.child0.ring0[i]]=[uv1[0],uv1[1]+len1*this.properties.vMultiplier];
                UV[branch.child0.ring2[i]]=[uv1[0],uv1[1]+len1*this.properties.vMultiplier];
                
                var len2=length(subVec(verts[branch.child1.ring0[i]],verts[branch.ring2[(i+segOffset1)%segments]]))*UVScale;
                var uv2=UV[branch.ring2[(i+segOffset1-1)%segments]];
                
                UV[branch.child1.ring0[i]]=[uv2[0],uv2[1]+len2*this.properties.vMultiplier];
                UV[branch.child1.ring2[i]]=[uv2[0],uv2[1]+len2*this.properties.vMultiplier];
            }

            this.doFaces(branch.child0);
            this.doFaces(branch.child1);
        }else{
            for(var i=0;i<segments;i++){
                faces.push([branch.child0.end,branch.ring1[(i+1)%segments],branch.ring1[i]]);
                faces.push([branch.child1.end,branch.ring2[(i+1)%segments],branch.ring2[i]]);
                
                
                var len=length(subVec(verts[branch.child0.end],verts[branch.ring1[i]]));
                UV[branch.child0.end]=[Math.abs(i/segments-1-0.5)*2,len*this.properties.vMultiplier];
                var len=length(subVec(verts[branch.child1.end],verts[branch.ring2[i]]));
                UV[branch.child1.end]=[Math.abs(i/segments-0.5)*2,len*this.properties.vMultiplier];
            }
        }
     */
}

Branch::Branch(ofVec3f _head, Branch* _parent){
    child0 = NULL;
    child1 = NULL;
    parent = NULL;
    head = NULL;
    length = 1;
    this->head = _head;
    this->parent = _parent;
}

ofVec3f Branch::mirrorBranch(ofVec3f vec, ofVec3f norm, Properties *prop){
    ofVec3f v = vec.cross(norm);
    float s = prop->branchFactor*v.dot(vec);
    return ofVec3f(vec.x-v.x*s,vec.y-v.y*s,vec.z-v.z*s);
}

void Branch::split(Properties *prop){
    // how to overload, nulling is not possible, we use negative ints.
    split(NULL,NULL, prop, -1, -1);
}

void Branch::split(int level, int steps, Properties *prop, int l1, int l2){
    if (l1 < 0) {
        l1 = 1;
    }
    if (l2 < 0) {
        l2 = 1;
    }
    if(level < 0){
        level = prop->levels;
    }
    if(steps < 0){
        steps = prop->treeSteps;
    }
    int rLevel = prop->levels-level;
    ofVec3f * po;
    if (parent) {
        po = parent->head;
    } else {
        po = new ofVec3f(0,0,0);
        type = "trunk";
    }
    ofVec3f * so = head;
    ofVec3f dir = ofVec3f(so - po).getNormalized();
    ofVec3f normal = dir.getCrossed(ofVec3f(dir.z, dir.x, dir.y));
    ofVec3f tangent = dir.getCrossed(normal);
    float r = prop->random(rLevel*10+l1*5+l2+prop->seed);
    float r2 = prop->random(rLevel*10+l1*5+l2+1+prop->seed);
    float clumpMax = prop->clumpMax;
    float clumpMin = prop->clumpMin;
    ofVec3f adj = normal.getScaled(r)+tangent.getScaled(1-r);
    if(r>0.5){adj.scale(-1);}
    float clump = (clumpMax-clumpMin)*r*clumpMin;
    ofVec3f newdir = (adj.getScaled(1-clump)+dir.getScaled(clump)).getNormalized();
    ofVec3f newdir2 = mirrorBranch(newdir, dir, prop);
    if (r>0.5) {
        ofVec3f tmp = newdir;
        newdir = newdir2;
        newdir2 = tmp;
    }
    if(steps>0){
        float angle=steps/prop->treeSteps*2*PI*prop->twistRate;
        newdir2=ofVec3f(sin(angle), r, cos(angle));
    }
    float growAmount = level*level/(prop->levels*prop->levels)*prop->growAmount;
    float dropAmount = rLevel*prop->dropAmount;
    float sweepAmount = rLevel*prop->sweepAmount;
    newdir = (newdir+ofVec3f(sweepAmount,dropAmount+growAmount,0)).getNormalized();
    newdir2 =(newdir2+ofVec3f(sweepAmount,dropAmount+growAmount,0)).getNormalized();
    ofVec3f head0 = ofVec3f(*so) + newdir.getScaled(length);
    ofVec3f head1 = ofVec3f(*so) + newdir2.getScaled(length);
    child0=new Branch(&head0,this);
    child1=new Branch(&head1,this);
    child0->length = pow(length, prop->lengthFalloffPower)*prop->lengthFalloffFactor;
    child1->length = pow(length, prop->lengthFalloffPower)*prop->lengthFalloffFactor;
    if(level > 0){
        if(steps > 0){
            child0->head = new ofVec3f(*head + ofVec3f((r-0.5)*2*prop->trunkKink,prop->climbRate,(r-0.5)*2*prop->trunkKink));
            child0->type = "trunk";
            child0->length=length*prop->taperRate;
            child0->split(level,steps-1,prop,l1+1,l2);
        } else {
            child0->split(level-1,0,prop,l1,l2+1);
        }
        child1->split(level-1,0,prop,l1,l2+1);
    }
}