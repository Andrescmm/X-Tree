//
//  Node.cpp
//  X-Tree Andres
//
//  Created by Andres  on 30/11/21.
//

#include "Node.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <stdio.h>

using namespace std;

node::node(){
    
    this->leaf = false;
}

node::node(vector<int> point){
    
    this->leaf = true;           //Dado que tiene dato (valores)
    this->DC.push_back(point);
    this->LB = point;
    this->UB = point;
}

node::node(node *n){
    
    this->leaf = false;
    this->children.push_back(n);
    this->LB = n->LB;
    this->UB = n->UB;
}

int node::calculateMBR(vector<int> a,vector<int> b){
    int tmp = 1;
    for(int i = 0;i < numDim;i++){
        tmp = tmp * abs(b[i] - a[i]);
    }
    return tmp;
}

int node::calculateMBR(){
    return calculateMBR(UB,LB);
}

int node::data(vector<int> &point){
    
    vector<int> _LB = this->LB;
    vector<int> _UB = this->UB;
    
    for(int i = 0; i <numDim;i++){
        
        if(_LB[i] > point[i]){
            _LB[i] = point[i];
        }
        if(_UB[i] < point[i]) {
            _UB[i] = point[i];
        }
    }
    
    return calculateMBR(_LB,_UB);
}

int node::data(node n) {
    vector<int> _LB = this->LB;
    vector<int> _UB = this->UB;
    
    for(int i = 0 ; i < numDim;i++){
        
        if(_LB[i] > n.LB[i]){
            _LB[i] = n.LB[i];
        }
        if(_UB[i] < n.LB[i]){
            _UB[i] = n.LB[i];
        }
        
        if(_LB[i] > n.UB[i]){
            _LB[i] = n.UB[i];
        }
        if(_UB[i] < n.UB[i]){
            _UB[i] = n.UB[i];
        }
    }
    return calculateMBR(_LB,UB);
}

void node::updateMBR(const vector<int> point){
    
    for(long i = 0; i < numDim;i++){
        if(this->LB[i] > point[i]){
            this->LB[i] = point[i];
        }
        if(this->UB[i] < point[i]){
            this->UB[i] = point[i];
        }
    }
}

void node::updateMBR(node *n){
    
    for(int i=0;i<numDim;i++){
        
        if(this->LB[i] > n->LB[i]){
            this->LB[i] = n->LB[i];
        }
        if(this->UB[i] < n->LB[i]){
            this->UB[i] = n->LB[i];
        }
        
        if(this->LB[i] > n->UB[i]){
            this->LB[i] = n->UB[i];
        }
        
        if(this->UB[i] < n->UB[i]){
            this->UB[i] = n->UB[i];
        }
    }
}
//LP = Lower Point
//UP = Upper Point

int node::distancePoint(vector<int>_UP,vector<int>_LP,vector<int> point){
    int distancia = 0;
    int component = 0;
    
    for(int i = 0 ; i < numDim ; i++) {
        if((point[i] >= _LP[i]) and (point[i] <= _UP[i])){
            component = 0;
        }
        else if(point[i] < _LP[i]){
            component = _LP[i] - point[i];
        }
        else{
            component = point[i] - _UP[i];
        }
        distancia += component * component;
    }
    return sqrt(distancia);
}

int node::distancePoint(vector<int> point){
    return distancePoint(this->UB,this->LB,point);
}

