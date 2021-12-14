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
int Xtree::distance(const vector<int> &v1, const vector<int> &v2){
    int distancia = 0;
    for(int i = 0;i<numDim;i++){
        
        distancia = distancia + abs((v2[i]-v1[i])*(v2[i]-v1[i]));
    }
    return sqrt(distancia);
}


// Select Picks
void Xtree::picks(node* nodeX,int &v1,int &v2){
    v1 = 0;
    v2 = 0;
    int minLB = numeric_limits<int>::max();
    int minUB = numeric_limits<int>::max();
    int tempArea = 0;

    for(int i=0 ; i<nodeX->DC.size() ; i++){
        tempArea = distance(nodeX->LB,nodeX->DC[i]);
        if(tempArea < minLB){
            minLB = tempArea;
            v1 = i;
        }
        
        tempArea = distance(nodeX->UB,nodeX->DC[i]);
        if(tempArea < minUB){
            if(i != v1){
                minUB = tempArea;
                v2 = i;
            }
        }
    }
}



// Choose leaf
node* Xtree::chooseLeaf(node *nodeX, vector<int> point){
    if(nodeX->leaf){
        return nodeX;
    }
    
    // the best area
    int area = numeric_limits<int>::max();
    int tempArea = 0;
    
    for (int i = 0; i< nodeX->children.size(); i++){
        tempArea = nodeX->children[i]->data(point) - nodeX->children[i]->calculateMBR();
        
        if (tempArea <= area){
            area = tempArea;
            nodeX = nodeX->children[i];
        }
    }
    nodeX->updateMBR(point);
    return chooseLeaf(nodeX,point);
}



// CheckParent
void Xtree::checkParent(node *nodeX){
    
    if(nodeX == nullptr and nodeX->leaf)
        return;
    
    for(int i=0;i<nodeX->children.size();i++){
        nodeX->children[i]->parent = nodeX;
        checkParent(nodeX->children[i]);
    }
}



// Overlap
bool Xtree::Overlap(node* r1,node* r2){
    for(int i=0;i<numDim;i++){
        if((r2->LB[i] < r1->UB[i]) or (r1->UB[i] > r2->LB[i])){
            return true;
        }
    }
    
    return false;
}


// Split Node
void Xtree::split(node** nodeX){
    
    if((*nodeX)->leaf){
        // Two seeds
        int sem1 = 0;
        int sem2 = 0;
        
        picks(*nodeX,sem1,sem2);
        vector<int> p1 = (*nodeX)->DC[sem1];
        vector<int> p2 = (*nodeX)->DC[sem2];
        
        node* rL = new node(p1);
        node* rR = new node(p2);
        
        for(int i =0;i<(*nodeX)->DC.size();i++){
            if( i == sem1 || i == sem2) continue;
            
            int area1 = rL->data((*nodeX)->DC[i]) - rL->calculateMBR();
            int area2 = rR->data((*nodeX)->DC[i]) - rR->calculateMBR();
            
            if(area1 < area2){
                rL->DC.push_back((*nodeX)->DC[i]);
                rL->updateMBR((*nodeX)->DC[i]);
            }
            else{
                rR->DC.push_back((*nodeX)->DC[i]);
                rR->updateMBR((*nodeX)->DC[i]);
            }
        }
        
        if((*nodeX) == head){
            (*nodeX)->leaf = false;
            (*nodeX)->DC.clear();
            rL->parent = *nodeX;
            rR->parent = *nodeX;
            (*nodeX)->children.push_back(rL);
            (*nodeX)->children.push_back(rR);
        }
        else{
            (*nodeX)->leaf = false;
            (*nodeX)->children.clear();
            node *padre = (*nodeX)->parent;
            rL->parent = padre;
            rR->parent = padre;
            
            for(int i=0;i<padre->children.size();i++){
                if(padre->children[i] == (*nodeX)){
                    delete padre->children[i];
                    padre->children.erase(padre->children.begin()+i);
                    i = padre->children.size();
                }
            }
            
            padre->children.push_back(rL);
            padre->children.push_back(rR);
            
            bool aTree = true;
            while(aTree and padre){
                
                if(!(padre->children.size() <= this->M*(padre->superNode))){
                    aTree = false;
                }
                else{
                    padre = padre->parent;
                }
            }
            if(!aTree){
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
        
        picks(*nodeX,sem1,sem2);
        node* rL = new node((*nodeX)->children[sem1]);
        node* rR = new node((*nodeX)->children[sem2]);
        
        for(int i=0;i<(*nodeX)->children.size();i++){
            if(i==sem1 || i==sem2) continue;
            
            int area1 = rL->data((*nodeX)->children[i]) - rL->calculateMBR();
            int area2 = rR->data((*nodeX)->children[i]) - rR->calculateMBR();
            
            if(area1 < area2){
                rR->children.push_back((*nodeX)->children[i]);
                rR->updateMBR((*nodeX)->children[i]);
            }
            else{
                rL->children.push_back((*nodeX)->children[i]);
                rL->updateMBR((*nodeX)->children[i]);
            }
        }
        
        if(!this->Overlap(rL,rR)){
            if(*nodeX == this->head){
                (*nodeX)->children.clear();
                rL->parent = *nodeX;
                rR->parent = *nodeX;
                (*nodeX)->children.push_back(rL);
                (*nodeX)->children.push_back(rR);
            }
            else{
                node* padre = (*nodeX)->parent;
                rL->parent = padre;
                rR->parent = padre;
                for(int i=0;i<padre->children.size();i++){
                    
                    if(padre->children[i]==(*nodeX)){
                        delete padre->children[i];
                        padre->children.erase(padre->children.begin()+i);
                    }
                }
                padre->children.push_back(rL);
                padre->children.push_back(rR);
                
                bool aTree = true;
                while(aTree and padre){
            
                    if(!(padre->children.size() <= this->M*(padre->superNode))){
                        aTree = false;
                    }
                    else{
                        padre = padre->parent;
                    }
                }
                if(!aTree){
                    this->split(&padre);
                }
                else{
                    return;
                }
            }
        }
        else{
            (*nodeX)->children.push_back(rL);
            (*nodeX)->children.push_back(rR);
            
            (*nodeX)->superNode++;
            node* padre = (*nodeX)->parent;
            
            bool aTree = true;
            
            while(aTree and padre){
                
                if(!(padre->children.size() <= this->M*(padre->superNode))){
                    aTree = false;
                }
                else{
                    padre = padre->parent;
                }
            }
            
            if(!aTree){
                this->split(&padre);
            }
            else{
                return;
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
