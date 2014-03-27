//
//  ofxProcTree.cpp
//  Trae
//
//  Created by Johan Bichel Lindegaard on 27/03/14.
//
//

// Based on https://github.com/supereggbert/proctree.js

#include "ofxProcTree.h"

void ofxProcTree::Tree::calcNormals(){
    
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

void ofxProcTree::Tree::doFaces(Branch *branch){
    if(!branch) {
        branch = this->troot;
    }
    
    int segments = props->segments;
    
    if(!branch->parent){
        for(int i=0;i<verts.size();i++){
            UV[i]=ofVec2f(0,0);
        }
        ofVec3f tangent = ofVec3f(*(branch->child0->head) - *(branch->head)).getCrossed(*(branch->child1->head) - *(branch->head)).getNormalized();
        ofVec3f normal = branch->head->getNormalized();
        float angle = acos(tangent.dot(ofVec3f(-1,0,0)));
        if(ofVec3f(-1,0,0).getCrossed(tangent).dot(normal) > 0){
            angle = 2*PI-angle;
        }
        int segOffset = roundf((angle/PI/2*segments));
        for (int i = 0; i < segments; i++){
            int v1 = branch->ring0->at(i);
            int v2 = branch->root->at((i+segOffset+1)%segments);
            int v3 = branch->root->at((i+segOffset)%segments);
            int v4 = branch->ring0->at((i+1)%segments);
            vector<int> face1;
            face1.push_back(v1);
            face1.push_back(v4);
            face1.push_back(v3);
            faces.push_back(face1);
            vector<int> face2;
            face2.push_back(v4);
            face2.push_back(v2);
            face2.push_back(v3);
            faces.push_back(face2);
            UV[(i+segOffset)%segments]=ofVec2f(abs(i/segments-0.5)*2,0);
            float len = ofVec3f(verts[branch->ring0->at(i)] - verts[branch->root->at((i+segOffset)%segments)]).length()*props->vMultiplier;
            UV[branch->ring0->at(i)]=ofVec2f(abs(i/segments-0.5)*2,len);
            UV[branch->ring2->at(i)]=ofVec2f(abs(i/segments-0.5)*2,len);
        }
    
    }
    
    if(branch->child0->ring0){
        int * segOffset0 = NULL;
        int * segOffset1 = NULL;
        float match0 = 0.0;
        float match1 = 0.0;
        
        ofVec3f v1 = ofVec3f(verts[branch->ring1->at(0)] - *(branch->head));
        ofVec3f v2 = ofVec3f(verts[branch->ring2->at(0)] - *(branch->head));
        
        v1 = TreeUtils::scaleInDirection(v1, ofVec3f(*(branch->child0->head) - *(branch->head)).getNormalized(), 0);
        v2 = TreeUtils::scaleInDirection(v2, ofVec3f(*(branch->child1->head) - *(branch->head)).getNormalized(), 0);
        
        for(int i = 0; i < segments; i++){
            ofVec3f d = ofVec3f(verts[branch->child0->ring0->at(i)] - *(branch->child0->head)).getNormalized();
            float l = d.dot(v1);
            if(!segOffset0 || l>match0){
                match0 = l;
                segOffset0 = new int(segments-i);
            }
            d = ofVec3f(verts[branch->child1->ring0->at(i)] - *(branch->child1->head)).getNormalized();
            l = d.dot(v2);
            if(!segOffset1 || l>match1){
                match1 = l;
                segOffset1 = new int(segments-i);
            }
        }
        
        float UVScale = props->maxRadius/branch->radius;
        
        for(int i = 0; i < segments; i++){
            int v1 = branch->child0->ring0->at(i);
            int v2 = branch->ring1->at((i+*(segOffset0)+1)%segments);
            int v3 = branch->ring1->at((i+*(segOffset0))%segments);
            int v4 = branch->child0->ring0->at((i+1)%segments);
            
            vector<int> face1;
            face1.push_back(v1);
            face1.push_back(v4);
            face1.push_back(v3);
            faces.push_back(face1);
            
            vector<int> face2;
            face2.push_back(v4);
            face2.push_back(v2);
            face2.push_back(v3);
            faces.push_back(face2);
            
            v1 = branch->child1->ring0->at(i);
            v2 = branch->ring2->at((i+*(segOffset1)+1)%segments);
            v3 = branch->ring2->at((i+*(segOffset1))%segments);
            v4 = branch->child1->ring0->at((i+1)%segments);
            
            vector<int> face3;
            face3.push_back(v1);
            face3.push_back(v2);
            face3.push_back(v3);
            faces.push_back(face3);

            vector<int> face4;
            face4.push_back(v1);
            face4.push_back(v4);
            face4.push_back(v2);
            faces.push_back(face4);

            float len1 = ofVec3f(verts[branch->child0->ring0->at(i)] - verts[branch->ring1->at((i+*(segOffset0))%segments)] ).length()*UVScale;
            ofVec2f uv1 = UV[branch->ring1->at((i+*(segOffset0)-1)%segments)];
            
            UV[branch->child0->ring0->at(i)] = ofVec2f(uv1.x,uv1.y+len1*props->vMultiplier);
            UV[branch->child0->ring2->at(i)] = ofVec2f(uv1.x,uv1.y+len1*props->vMultiplier);
            
            float len2 = ofVec3f(verts[branch->child1->ring0->at(i)] - verts[branch->ring2->at((i+*(segOffset1))%segments)] ).length()*UVScale;
            ofVec2f uv2 = UV[branch->ring2->at((i+*(segOffset1)-1)%segments)];
            
            UV[branch->child1->ring0->at(i)] = ofVec2f(uv2.x,uv2.y+len2*props->vMultiplier);
            UV[branch->child1->ring2->at(i)] = ofVec2f(uv2.x,uv2.y+len2*props->vMultiplier);
        }
        doFaces(branch->child0);
        doFaces(branch->child1);
    } else {
        for(int i = 0;i<segments;i++){
            
            vector<int> face1;
            face1.push_back(branch->child0->end);
            face1.push_back(branch->ring1->at((i+1)%segments));
            face1.push_back(branch->ring1->at(i));
            faces.push_back(face1);
            
            vector<int> face2;
            face2.push_back(branch->child1->end);
            face2.push_back(branch->ring2->at((i+1)%segments));
            face2.push_back(branch->ring2->at(i));
            faces.push_back(face2);
            
            float len = ofVec3f(verts[branch->child0->end] - verts[branch->ring1->at(i)]).length();
            UV[branch->child0->end] = ofVec2f(abs(i/segments-1-0.5)*2,len*props->vMultiplier);

            len = ofVec3f(verts[branch->child1->end] - verts[branch->ring2->at(i)]).length();
            UV[branch->child1->end] = ofVec2f(abs(i/segments-0.5)*2,len*props->vMultiplier);

        }
    }

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

void ofxProcTree::Tree::createTwigs(ofxProcTree::Branch *branch){
    
    //TODO: CREATE TWIGS
    if (!branch) {
        branch = this->troot;
    }
    
}

void ofxProcTree::Tree::createForks(ofxProcTree::Branch *branch, float radius){
    if(!branch){
        branch = troot;
    }
    if (radius < 0) {
        radius = props->maxRadius;
    }
    branch->radius = radius;
    
    if(radius>branch->length){
        radius=branch->length;
    }
    
    int segments = props->segments;
    
    float segmentAngle = PI*2/segments;
    
    if(!branch->parent){
        // create the root of the tree
        branch->root = new vector<int>;
        ofVec3f axis(0,1,0);
        for(int i = 0; i < segments; i++){
            ofVec3f vec = ofVec3f(ofxProcTree::TreeUtils::vecAxisAngle(ofVec3f(-1,0,0), axis, -segmentAngle*i));
            branch->root->push_back(verts.size());
            verts.push_back(vec.getScaled(radius/props->radiusFalloffRate));
        }
    }

    //cross the branches to get the left
    //add the branches to get the up
    if(branch->child0){
        ofVec3f axis;
        if (branch->parent) {
            axis = ofVec3f(*(branch->head) - *(branch->parent->head)).getNormalized();
        } else {
            axis = branch->head->getNormalized();
        }
        
        ofVec3f axis1 = ofVec3f(*(branch->head) - *(branch->child0->head)).getNormalized();
        ofVec3f axis2 = ofVec3f(*(branch->head) - *(branch->child1->head)).getNormalized();
        ofVec3f tangent = axis1.getCrossed(axis2).getNormalized();
        branch->tangent = new ofVec3f(tangent);
        
        ofVec3f axis3 = tangent.getCrossed(ofVec3f(axis1.getScaled(-1) + axis2.getScaled(-1)).getNormalized()).getNormalized();
        
        ofVec3f dir = ofVec3f(axis2.x,0,axis2.z);
        ofVec3f centerloc = ofVec3f(*(branch->head) + dir.getScaled(props->maxRadius/2));
        
        branch->ring0 = new vector<int>;
        vector<int> * ring0 = branch->ring0;
        branch->ring1 = new vector<int>;
        vector<int> * ring1 = branch->ring1;
        branch->ring0 = new vector<int>;
        vector<int> * ring2 = branch->ring2;
        
        float scale = props->radiusFalloffRate;
        
        if (branch->child0->type == "trunk" || branch->type == "trunk") {
            scale = 1./props->taperRate;
        }
        
        // main segment ring
        
        int linch0 = verts.size();
        ring0->push_back(linch0);
        ring2->push_back(linch0);
        verts.push_back(ofVec3f(centerloc+tangent.getScaled(radius*scale)));
        
        int start = verts.size()-1;
        ofVec3f d1 = TreeUtils::vecAxisAngle(tangent, axis2, 1.57);
        ofVec3f d2 = tangent.getCrossed(axis).getNormalized();
        float s = 1./d1.dot(d2);
        for (int i = 1; i<segments/2; i++) {
            ofVec3f vec = TreeUtils::vecAxisAngle(tangent, axis2, segmentAngle*i);
            ring0->push_back(start+i);
            ring2->push_back(start+i);
            vec = TreeUtils::scaleInDirection(vec, d2, s);
            verts.push_back(ofVec3f(centerloc + vec.getScaled(radius*scale)));
        }
        int linch1 = verts.size();
        ring0->push_back(linch1);
        ring1->push_back(linch1);
        verts.push_back(ofVec3f(centerloc + tangent.getScaled(-radius*scale)));
        
        //HERTIL
        
    }
}

ofxProcTree::Branch::Branch(ofVec3f * _head, Branch* _parent){
    child0 = NULL;
    child1 = NULL;
    parent = NULL;
    head = NULL;
    length = 1;
    this->head = _head;
    this->parent = _parent;
}

ofVec3f ofxProcTree::Branch::mirrorBranch(ofVec3f vec, ofVec3f norm, Properties *prop){
    ofVec3f v = norm.getCrossed(vec.getCrossed(norm));
    float s = prop->branchFactor*v.dot(vec);
    return ofVec3f(vec.x-v.x*s,vec.y-v.y*s,vec.z-v.z*s);
}

void ofxProcTree::Branch::split(Properties *prop){
    // how to overload, nulling is not possible, we use negative ints.
    split(prop->levels,prop->treeSteps, prop, 1, 1);
}

void ofxProcTree::Tree::drawSkeleton(){
    troot->drawSkeleton();
}

void ofxProcTree::Branch::split(int level, int steps, Properties *prop, int l1, int l2){
    int rLevel = prop->levels-level;
    ofVec3f * po;
    if (parent) {
        po = parent->head;
    } else {
        po = new ofVec3f(0,0,0);
        type = "trunk";
    }
    ofVec3f * so = head;
    ofVec3f dir = ofVec3f(*(so) - *(po)).getNormalized();
    ofVec3f normal = dir.getCrossed(ofVec3f(dir.z, dir.x, dir.y));
    ofVec3f tangent = dir.getCrossed(normal);
    float r = prop->random(rLevel*10+l1*5+l2+prop->seed);
    float r2 = prop->random(rLevel*10+l1*5+l2+1+prop->seed);
    float clumpMax = prop->clumpMax;
    float clumpMin = prop->clumpMin;
    ofVec3f adj = normal.getScaled(r)+tangent.getScaled(1-r);
    if(r>0.5){adj = adj.getScaled(-1);}
    float clump = (clumpMax-clumpMin)*r*clumpMin;
    ofVec3f newdir = ofVec3f(adj.getScaled(1-clump)+dir.getScaled(clump)).getNormalized();
    ofVec3f newdir2 = mirrorBranch(newdir, dir, prop);
    if (r>0.5) {
        ofVec3f tmp = newdir;
        newdir = newdir2;
        newdir2 = tmp;
    }
    if(steps>0){
        float angle=steps/prop->treeSteps*2*PI*prop->twistRate;
        newdir2=ofVec3f(sin(angle), r, cos(angle)).getNormalized();
    }
    float growAmount = level*level/(prop->levels*prop->levels)*prop->growAmount;
    float dropAmount = rLevel*prop->dropAmount;
    float sweepAmount = rLevel*prop->sweepAmount;
    newdir = ofVec3f(newdir+ofVec3f(sweepAmount,dropAmount+growAmount,0)).getNormalized();
    newdir2 =ofVec3f(newdir2+ofVec3f(sweepAmount,dropAmount+growAmount,0)).getNormalized();
    ofVec3f * head0 = new ofVec3f(*(so) + newdir.getScaled(length));
    ofVec3f * head1 = new ofVec3f(*(so) + newdir2.getScaled(length));
    child0=new Branch(head0,this);
    child1=new Branch(head1,this);
    child0->length = powf(length, prop->lengthFalloffPower)*prop->lengthFalloffFactor;
    child1->length = powf(length, prop->lengthFalloffPower)*prop->lengthFalloffFactor;
    if(level > 0){
        if(steps > 0){
            child0->head = new ofVec3f(*(head) + ofVec3f((r-0.5)*2*prop->trunkKink,prop->climbRate,(r-0.5)*2*prop->trunkKink));
            child0->type = "trunk";
            child0->length=length*prop->taperRate;
            child0->split(level,steps-1,prop,l1+1,l2);
        } else {
            child0->split(level-1,0,prop,l1+1,l2);
        }
        child1->split(level-1,0,prop,l1,l2+1);
    }
}
