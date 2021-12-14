//
//  Node.hpp
//  X-Tree Andres
//
//  Created by Andres  on 30/11/21.
//

#ifndef Node_hpp
#define Node_hpp
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <stdio.h>

using namespace std;

const int numDim = 784;

//Node
class node{
    
public:
    // Vectors
    vector<node*> children;
    vector<int> LB; // LowerBound
    vector<int> UB;// UpperBound
    vector<vector<int>> DC; //DataChildren/// Valores
    
    // Variables
    bool leaf;
    int superNode=1;
    node *parent;
    
    // Functions
    node();
    node(vector<int> point);
    node(node *n);
    int calculateMBR(vector<int> a,vector<int> b);
    int calculateMBR();
    int data(vector<int> &point);
    int data(node n);
    void updateMBR(const vector<int> point);
    void updateMBR(node *n);
    int distancePoint(vector<int> _upperPoint,vector<int> _lowerPoint,vector<int> point);
    int distancePoint(vector<int> point);
    
};


//cmp
class cmp{
public:
    bool operator()(pair<node*,int> a,pair<node*,int> b){
        return a.second > b.second;
    };
};

// Node
node::node(){
    
    this->leaf = false;
}

// Node Vector
node::node(vector<int> point){
    
    this->leaf = true;
    this->DC.push_back(point);
    this->LB = point;
    this->UB = point;
}

// Node node
node::node(node *n){
    
    this->leaf = false;
    this->children.push_back(n);
    this->LB = n->LB;
    this->UB = n->UB;
}


// Data Vector
int node::data(vector<int> &point){
    
    vector<int> _LB = this->LB;
    vector<int> _UB = this->UB;
    
    for(int i = 0; i <numDim;i++){
        
        if(_LB[i] < point[i]){
            _LB[i] = point[i];
        }
        if(_UB[i] > point[i]) {
            _UB[i] = point[i];
        }
    }
    
    return calculateMBR(_UB,_LB);
}


// Data Node
int node::data(node n) {
    vector<int> _LB = this->LB;
    vector<int> _UB = this->UB;
    
    for(int i = 0 ; i <numDim;i++){
        
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
    return calculateMBR(_LB,_UB);
}


// Calculate MBR Vectors
int node::calculateMBR(vector<int> a,vector<int> b){
    int tmp = 1;
    for(int i = 0;i < numDim;i++){
        tmp = tmp * abs(b[i] - a[i]);
    }
    return tmp;
}

// Calculate MBR
int node::calculateMBR(){
    return calculateMBR(UB,LB);
}


// Update MBR vector
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


// Update MBR node
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


// Distance Point
int node::distancePoint(vector<int>_UP,vector<int>_LP,vector<int> point){
    int dis = 0;
    int tmp = 0;
    
    for(int i = 0 ; i < numDim ; i++) {
        if((point[i] >= _LP[i]) and (point[i] <= _UP[i])){
            tmp = 0;
        }
        else if(point[i] < _LP[i]){
            tmp = _LP[i] - point[i];
        }
        else{
            tmp = point[i] - _UP[i];
        }
        dis += tmp * tmp;
    }
    return sqrt(dis);
}


// Distance Point
int node::distancePoint(vector<int> point){
    return distancePoint(this->UB,this->LB,point);
}


#endif /* Node_hpp */
