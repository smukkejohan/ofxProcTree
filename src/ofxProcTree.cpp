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
    
    vector<ofVec3f> normals = mesh.getNormals();
    vector<ofVec3f> verts = mesh.getVertices();
    vector<ofIndexType> indices = mesh.getIndices();
    vector<vector<ofVec3f> > allNormals;
    const vector<ofMeshFace> faces = mesh.getUniqueFaces();

    for (int i = 0; i< verts.size(); i++){
        vector<ofVec3f> v;
        allNormals.push_back( v );
    }
    
    for (int i = 0;i<faces.size();i++) {
        ofMeshFace face = faces[i];
        
        ofVec3f norm = ((face.getVertex(1)-face.getVertex(2)).crossed(face.getVertex(1)-face.getVertex(0))).normalize();
        mesh.getI
        faceNormals[i].push_back(norm);
    }
    for (int i = 0;i<allNormals.size();i++){
        ofVec3f total(0,0,0);
        int size = allNormals[i].ge;
        
    }
    
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