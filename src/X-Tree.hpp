//
//  X-Tree.hpp
//  X-Tree Andres
//
//  Created by Andres  on 30/11/21.
//

#ifndef X_Tree_hpp
#define X_Tree_hpp

#include "Node.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <stdio.h>

using namespace std;

// X-Tree
class Xtree{
    
public:
    
    // Variables
    node *head;
    int M;
    
    // Functions
    Xtree(int m);
    int distance(const vector<int> &a, const vector<int> &b);
    node* chooseLeaf(node *node, vector<int> elem);
    void picks(node* n,int &a,int &b);
    void insert(vector<int> point);
    void checkParent(node *node);
    void split(node** nodo);
    bool Overlap(node* region1,node* region2);
    vector<vector<int>> KNN_Search(node *root,vector<int> point,int k);
    
};


// X-Tree
Xtree::Xtree(int m){
    
    this->M = m;
    this->head = nullptr;
}


// Distance
int Xtree::distance(const vector<int> &a, const vector<int> &b){
    int distancia = 0;
    for(int i = 0;i<numDim;i++){
        
        distancia = distancia + abs((b[i]-a[i])*(b[i]-a[i]));
    }
    return sqrt(distancia);
}


// Choose leaf
node* Xtree::chooseLeaf(node *node, vector<int> point){
    if(node->leaf){
        return node;
    }
    
    // the best area
    int area = numeric_limits<int>::max();
    int tempArea = 0;
    
    for (int i = 0; i< node->children.size(); i++){
        tempArea = node->children[i]->data(point) - node->children[i]->calculateMBR();
        
        if (tempArea <= area){
            area = tempArea;
            node = node->children[i];
        }
    }
    node->updateMBR(point);
    return chooseLeaf(node,point);
}


// Select Picks
void Xtree::picks(node* n,int &a,int &b){
    a = 0;
    b = 0;
    int minLB = numeric_limits<int>::max();
    int minUB = numeric_limits<int>::max();
    int tempArea = 0;

    for(int i=0 ; i<n->DC.size() ; i++){
        tempArea = distance(n->LB,n->DC[i]);
        if(tempArea < minLB){
            minLB = tempArea;
            a = i;
        }
        
        tempArea = distance(n->UB,n->DC[i]);
        if(tempArea < minUB){
            if(i != a){
                minUB = tempArea;
                b = i;
            }
        }
    }
}


// Insert
void Xtree::insert(vector<int> point){
    
    node** nodeX = &head;
    
    if((*nodeX) == nullptr){ // If the X-Tree is empty
        *nodeX = new node(point);
        return;
    }
    else{ // when the X-Tree is not empty
        (*nodeX) = chooseLeaf((*nodeX),point);
        
        (*nodeX)->DC.push_back(point);
        (*nodeX)->updateMBR(point);
        
        if(this->M*(*nodeX)->superNode < (*nodeX)->DC.size()){
            split(nodeX);
        }
        checkParent(this->head);
        return;
    }
}

// CheckParent
void Xtree::checkParent(node *node){
    
    if(node == nullptr and node->leaf)
        return;
    
    for(int i=0;i<node->children.size();i++){
        node->children[i]->parent = node;
        checkParent(node->children[i]);
    }
}


// Split Node
void Xtree::split(node** nodo){
    
    if((*nodo)->leaf){
        // Two seeds
        int sem1 = 0;
        int sem2 = 0;
        
        picks(*nodo,sem1,sem2);
        vector<int> p1 = (*nodo)->DC[sem1];
        vector<int> p2 = (*nodo)->DC[sem2];
        
        node* regionLeft = new node(p1);
        node* regionRight = new node(p2);
        
        for(int i =0;i<(*nodo)->DC.size();i++){
            if( i == sem1 || i == sem2) continue;
            
            int area1 = regionLeft->data((*nodo)->DC[i]) - regionLeft->calculateMBR();
            int area2 = regionRight->data((*nodo)->DC[i]) - regionRight->calculateMBR();
            
            if(area1 < area2){
                regionLeft->DC.push_back((*nodo)->DC[i]);
                regionLeft->updateMBR((*nodo)->DC[i]);
            }
            else{
                regionRight->DC.push_back((*nodo)->DC[i]);
                regionRight->updateMBR((*nodo)->DC[i]);
            }
        }
        
        if((*nodo) == head){
            (*nodo)->leaf = false;
            (*nodo)->DC.clear();
            regionLeft->parent = *nodo;
            regionRight->parent = *nodo;
            (*nodo)->children.push_back(regionLeft);
            (*nodo)->children.push_back(regionRight);
        }
        else{
            (*nodo)->leaf = false;
            (*nodo)->children.clear();
            node *padre = (*nodo)->parent;
            regionLeft->parent = padre;
            regionRight->parent = padre;
            
            for(int i=0;i<padre->children.size();i++){
                if(padre->children[i] == (*nodo)){
                    //cout<<"Ingreso!!!!!!!!"<<endl;
                    delete padre->children[i];
                    padre->children.erase(padre->children.begin()+i);
                    i = padre->children.size();
                }
            }
            
            padre->children.push_back(regionLeft);
            padre->children.push_back(regionRight);
            
            bool adjust_tree = true;
            while(adjust_tree and padre){
                
                if(!(padre->children.size() <= this->M*(padre->superNode))){
                    adjust_tree = false;
                }
                else{
                    padre = padre->parent;
                }
            }
            //padre->is_leaf = false;
            if(!adjust_tree){
                this->split(&padre);
            }
            else{
                return;
            }
        }
    }
    else{
        int sem1 = 0;
        int sem2 = 0;
        
        picks(*nodo,sem1,sem2);
        node* regionLeft = new node((*nodo)->children[sem1]);
        node* regionRight = new node((*nodo)->children[sem2]);
        
        for(int i=0;i<(*nodo)->children.size();i++){
            if(i==sem1 || i==sem2) continue;
            
            int area1 = regionLeft->data((*nodo)->children[i]) - regionLeft->calculateMBR();
            int area2 = regionRight->data((*nodo)->children[i]) - regionRight->calculateMBR();
            
            if(area1 < area2){
                regionRight->children.push_back((*nodo)->children[i]);
                regionRight->updateMBR((*nodo)->children[i]);
            }
            else{
                regionLeft->children.push_back((*nodo)->children[i]);
                regionLeft->updateMBR((*nodo)->children[i]);
            }
        }
        
        if(!this->Overlap(regionLeft,regionRight)){
            if(*nodo == this->head){
                (*nodo)->children.clear();
                regionLeft->parent = *nodo;
                regionRight->parent = *nodo;
                (*nodo)->children.push_back(regionLeft);
                (*nodo)->children.push_back(regionRight);
            }
            else{
                node* padre = (*nodo)->parent;
                regionLeft->parent = padre;
                regionRight->parent = padre;
                for(int i=0;i<padre->children.size();i++){
                    
                    if(padre->children[i]==(*nodo)){
                        delete padre->children[i];
                        padre->children.erase(padre->children.begin()+i);
                    }
                }
                padre->children.push_back(regionLeft);
                padre->children.push_back(regionRight);
                
                bool adjust_tree = true;
                while(adjust_tree and padre){
            
                    if(!(padre->children.size() <= this->M*(padre->superNode))){
                        adjust_tree = false;
                    }
                    else{
                        padre = padre->parent;
                    }
                }
                if(!adjust_tree){
                    this->split(&padre);
                }
                else{
                    return;
                }
            }
        }
        else{
            (*nodo)->children.push_back(regionLeft);
            (*nodo)->children.push_back(regionRight);
            
            (*nodo)->superNode++;
            node* padre = (*nodo)->parent;
            bool adjust_tree = true;
            
            while(adjust_tree and padre){
                
                if(!(padre->children.size() <= this->M*(padre->superNode))){
                    adjust_tree = false;
                }
                else{
                    padre = padre->parent;
                }
            }
            
            if(!adjust_tree){
                this->split(&padre);
            }
            else{
                return;
            }
        }
    }
}



// Overlap
bool Xtree::Overlap(node* region1,node* region2){
    for(int i=0;i<numDim;i++){
        if((region2->LB[i] < region1->UB[i]) or (region1->UB[i] > region2->LB[i])){
            return true;
        }
    }
    
    return false;
}


// KNN
vector<vector<int>> Xtree::KNN_Search(node *root,vector<int> point,int k){
    
    vector<vector<int>> consult;
    priority_queue<pair<node*,int> , vector<pair<node*,int>> , cmp> queue;
    queue.push(make_pair(root,0));
    
    int count=0;
    while(!queue.empty() and count < k){
        node*  currentNodo = queue.top().first;
        queue.pop();
        
        if(currentNodo->leaf){
            for(int i=0;i < currentNodo->DC.size() and count < k;i++){
                for(int j=0;j<currentNodo->DC[i].size();j++){
                    cout<<currentNodo->DC[i][j];
                    if(j%28==0){
                        cout<<endl<<"      ";
                    }
                }
                cout<<"\n"<<endl;
                count++;
            }
        }
        else{
            for(int i=0;i<currentNodo->children.size();i++){
              
                node *tempNodo = currentNodo->children[i];
                queue.push(make_pair(tempNodo,tempNodo->distancePoint(point)));
            }
        }
        if(currentNodo != head){
            delete currentNodo;
        }
    }
    return consult;
}
#endif /* X_Tree_hpp */
