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
    void Select_picks__(node* n,int &a,int &b);
    void insert(vector<int> point);
    void CheckParent(node *node);
    void Split_node(node** nodo);
    bool pick_next(node* region_1,node* region_2,const vector<int>& point);
    bool Overlap(node* region1,node* region2);
    vector<vector<int>> KNN_Search(node *root,vector<int> point,int k);
    
};

Xtree::Xtree(int m){

        this->M = m;
        this->head = nullptr;
}

int Xtree::distance(const vector<int> &a, const vector<int> &b){
    int distancia = 0;
    for(int i = 0;i<numDim;i++){
        //distancia = distancia + pow(a.data[i]-b.data[i],2);
        distancia = distancia + abs((b[i]-a[i])*(b[i]-a[i]));
    }
    return sqrt(distancia);
}

node* Xtree::chooseLeaf(node *node, vector<int> point){
    if(node->leaf)
       return node;
    
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

void Xtree::Select_picks__(node* n,int &a,int &b){
    a = 0;
    b = 0;
    int min_distance_lowerBound = numeric_limits<int>::max();//
    int min_distance_upperBound = numeric_limits<int>::max();//
    int temp_area = 0.0;
    //cout<<"Point: "<<n->m_data_children.size()<<endl;
    for(int i=0 ; i<n->DC.size() ; i++){
        temp_area = distance(n->LB,n->DC[i]);
        if(temp_area < min_distance_lowerBound){
            min_distance_lowerBound = temp_area;
            a = i;
        }
        
        temp_area = distance(n->UB,n->DC[i]);
        if(temp_area < min_distance_upperBound){
            if(i != a){
                min_distance_upperBound = temp_area;
                b = i;
            }
        }
    }
}


void Xtree::insert(vector<int> point){
    
    node** nodeX = &head;
    
    if((*nodeX) == nullptr){ // If the X-Tree is empty
        *nodeX = new node(point);
        return;
    }
    else{
        (*nodeX) = chooseLeaf((*nodeX),point);
        
        (*nodeX)->DC.push_back(point);
        (*nodeX)->updateMBR(point);
        
        if(this->M*(*nodeX)->superNode < (*nodeX)->DC.size()){
            Split_node(nodeX);
        }
        CheckParent(this->head);
        return;
    }
}

// CheckParent
void Xtree::CheckParent(node *node){
    
    if(node == nullptr and node->leaf)
        return;
    
    for(int i=0;i<node->children.size();i++){
        node->children[i]->parent = node;
        CheckParent(node->children[i]);
    }
}

void Xtree::Split_node(node** nodo)
{
    if((*nodo)->leaf){
        int Seed_1 = 0;
        int Seed_2 = 0;
        
        Select_picks__(*nodo,Seed_1,Seed_2);
        vector<int> p_s_I = (*nodo)->DC[Seed_1];
        vector<int> p_s_II = (*nodo)->DC[Seed_2];
        
        node* regionLeft = new node(p_s_I);
        node* regionRight = new node(p_s_II);
        
        for(int i =0;i<(*nodo)->DC.size();i++){
            if( i == Seed_1  || i == Seed_2) continue;
            
            int area_1 = regionLeft->data((*nodo)->DC[i]) - regionLeft->calculateMBR();
            int area_2 = regionRight->data((*nodo)->DC[i]) - regionRight->calculateMBR();
            
            if(area_1 < area_2){
                regionLeft->DC.push_back((*nodo)->DC[i]);
                regionLeft->updateMBR((*nodo)->DC[i]);
            }
            else
            {
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
            (*nodo)->children.push_back(regionRight); //UPDATE REGION
        }
        else{
            (*nodo)->leaf = false; //AAD
            (*nodo)->children.clear(); //ADD
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
                this->Split_node(&padre);
            }
            else{
                 return;
            }
        }
    }
    else{
        int Seed_1 = 0;
        int Seed_2 = 0;
        
        Select_picks__(*nodo,Seed_1,Seed_2);
        node* regionLeft = new node((*nodo)->children[Seed_1]);
        node* regionRight = new node((*nodo)->children[Seed_2]);
        
        for(int i=0;i<(*nodo)->children.size();i++){
            if(i==Seed_1 || i==Seed_2) continue;
            
            int area_1 = regionLeft->data((*nodo)->children[i]) - regionLeft->calculateMBR();
            int area_2 = regionRight->data((*nodo)->children[i]) - regionRight->calculateMBR();
            
            if(area_1 < area_2){
                regionRight->children.push_back((*nodo)->children[i]);
                regionRight->updateMBR((*nodo)->children[i]);
            }
            else
            {
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
                for(int i=0;i<padre->children.size();i++)
                {
                    if(padre->children[i]==(*nodo))
                    {
                        delete padre->children[i];
                        padre->children.erase(padre->children.begin()+i);
                    }
                }
                padre->children.push_back(regionLeft);
                padre->children.push_back(regionRight);
                
                bool adjust_tree = true;
                while(adjust_tree and padre){
                    // M
                    if(!(padre->children.size() <= this->M*(padre->superNode))){
                        adjust_tree = false;
                    }
                    else{
                        padre = padre->parent;
                    }
                }
                if(!adjust_tree){
                    this->Split_node(&padre);
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
                this->Split_node(&padre);
            }
            else{
                return;
            }
        }
    }
}


//Pick Next
bool Xtree::pick_next(node* region_1,node* region_2,const vector<int>& point)
{
    int area_1 = region_1->data(point) - region_1->calculateMBR();
    int area_2 = region_2->data(point) - region_2->calculateMBR();
    
    if(area_1 < area_2){
        return false;
    }
    else{
        return true;
    }
}

// Overlap
bool Xtree::Overlap(node* region1,node* region2){
    for(int i=0;i<numDim;i++){
        if((region2->LB[i] < region1->UB[i]) or (region1->UB[i] > region2->LB[i])){
            return true; //HAY OVERLAP
        }
    }
    
    return false; //NO HAY OVERLAP
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
            cout<<"Fucking else";
            //for(auto childIndex : currentNodo->m_data)
            for(int i=0;i<currentNodo->children.size();i++){
                // Nodo *tempNodo = new Nodo(childIndex);
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

