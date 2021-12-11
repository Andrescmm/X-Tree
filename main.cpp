//
//  main.cpp
//  X-Tree Andres
//
//  Created by Andres  on 30/11/21.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <iomanip>
#include <memory>
#include <utility>
#include <vector>
#include "X-Tree.hpp"
#include "Node.hpp"

using namespace std;



// Read Data Set
void readDataSet(Xtree *xt){
    string line,num = "";
    ifstream file("/Users/andrescusirramos/Documents/CCOMP 2021-2/EDA/EDA/X-tree V01/X-tree V01/file3.txt");
    
    
    if(file.is_open()){
        
        string line;
        vector<int> data;
        ///cout<<"dta.size() "<<data.size()<<endl;
        int i =0;
        
        // We go throught all the lines
        while(getline(file,line)){
            
            stringstream stream(line); // Works as a file but just in memory not an actual file
            string valor;
            
            // Line by Line
            while(getline(stream,valor,',')){
                data.push_back(stoi(valor));
            }
            ///cout<<"dta.size() "<<data.size()<<endl;
            ///cout<<"Linea :"<<line<<endl;
            if(data.size()>0){
                xt->insert(data);
            }
            
            data.clear();
            i++;
        }
        cout<<" "<<i<< " Archivos Indexados"<<endl;
        file.close();
    }
    return;
}


// Read Data
vector<int> readData(string input){
    
    cout<<" Punto a consultar = "<<input<<endl;
    
    vector<int> data;
    data.clear();
    stringstream stream(input);
    string valor="";
    while(getline(stream,valor,',')){
        data.push_back(stod(valor));
    }
    
    return data;
}


// Main
int main(){
    
    Xtree *xt;
    int knn;
    int m = 1000;
    vector<int> Point;
    string input ="0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
    struct  timeval ti,tf;
    double ttime;
    xt = new Xtree(m);
 
    char optD, optK;
    cout<<" Desea Indexar el Data Set?(y/n) -> ";
    cin>>optD;
    if(optD =='y'){
        gettimeofday(&ti,NULL);
        cout<<" Indexando....."<<endl;
        readDataSet(xt);
        gettimeofday(&tf,NULL);
        ttime = (tf.tv_sec-ti.tv_sec)*1000 + (tf.tv_usec-ti.tv_usec)/1000;
        cout<<" Tiempo de Indexado: "<<ttime/1000<<"s"<<endl;
    }
    else{
        cout<<"Adios :c"<<endl;
    }
    cout<<endl;
    cout<<" Desea proceder con la busqueda los KNN?(y/n) -> ";
    cin>>optK;
    if(optK == 'y'){
        cout<<" Ingrese los KNN: ";
        cin>>knn;
        //cout<<"1"<<endl;
        //cout<<" Ingrese el punto: ";
        //cin>>input;
        cout<<"2"<<endl;
        //getline(cin,input);
        //cout<<"\n"<<endl;
        cout<<input<<endl;
        cout<<"3"<<endl;
        Point.clear();
        //cout<<"input"<<endl;
        Point = readData(input);
        cout<<"4"<<endl;
        //cout<<input<<endl;
        cout<<endl;
        cout<<" ----------------------------------------------------------------------------------------------------------- "<<endl;
        cout<<"                                              KNN"<<endl;
        cout<<endl;
        gettimeofday(&ti,NULL);
        xt->KNN_Search(xt->head,Point,knn);
        gettimeofday(&tf,NULL);
        //input = "";
        ttime = (tf.tv_sec-ti.tv_sec)*1000 + (tf.tv_usec-ti.tv_usec)/1000;
        cout<<"Tiempo de consulta: "<<ttime/1000<<"s"<<endl;
      }
    else{
        cout<<"Me hubiera gustado conocer tus vecinos :c"<<endl;
       }
        
        return 0;
    }
    
