// Copyright 2020 Roger Peralta Aranibar Advanced Data Estructures
// Timer
//
//  main.cpp
//  X-Tree Andres
//
//  Created by Andres  on 30/11/21.
//

#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <sys/time.h>
#include <iomanip>
#include <memory>
#include <utility>
#include <vector>
#include "X-Tree.hpp"
#include "Node.hpp"


#define PATH1 "../dataset.txt"
#define PATH2 "../knn.txt"

using namespace std;

int m = 0;

// Timer
template <typename>
class Timer;

template <typename R, typename... T>
class Timer<R(T...)> {
public:
    typedef R (*function_type)(T...);
    function_type function;
    
    explicit Timer(function_type function, std::string process_name = "")
    : function_(function), process_name_(process_name) {}
    
    R operator()(T... args) {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();
        
        R result = function_(std::forward<T>(args)...);
        
        end = std::chrono::high_resolution_clock::now();
        int64_t duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();
        
        std::cout << std::setw(10) << process_name_ << std::setw(30)
        << "Duration: " + std::to_string(duration) + " ns\n";
        return result;
    }
    
private:
    function_type function_;
    std::string process_name_;
};



// X-Tree
Xtree *xt;


// Read Data Set
int build_data_structure(){
    string line,num = "";
    //ifstream file("/Users/andrescusirramos/Documents/CCOMP 2021-2/EDA/EDA/X-tree V01/X-tree V01/dataset.txt");
    ifstream file(PATH1);
    
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
        cout<<"     "<<i<< " Archivos Indexados"<<endl;
        cout<<endl;
        file.close();
    }
    return 0;
}


// Read KNN
vector<int> readKNN(){
    //ifstream file("/Users/andrescusirramos/Documents/CCOMP 2021-2/EDA/EDA/X-Tree Andres/X-Tree Andres/knn.txt");
    ifstream file(PATH2);
    vector<int> data;
    if (file.is_open()){
        string input;
        while(getline(file,input)){
            stringstream stream(input);
            string valor;
            while(getline(stream,valor,',')){
                data.push_back(stod(valor));
            }
        }
    }
    for(int i =0; i<data.size();i++){
        cout<<data[i];
        if(i%28==0){
            cout<<endl<<"      ";
        }
    }
    return data;
}

// KNN
std::vector<std::vector<int>>query_knn(vector<int>Point,int k){
    return xt->KNN_Search(xt->head,Point,k);
}


// Main
int main(){
    srand((unsigned) time(NULL));
    //m=1000;
    m = 100 + (rand() % 3000);
    xt= new Xtree(m);
    
    cout<<"     --------------------------------"<<endl;
    cout<<"     --------------------------------"<<endl;
    cout<<"     --------------------------------"<<endl;
    cout<<"                   X-TREE            "<<endl;
    cout<<"              ANDRES CUSIRRAMOS      "<<endl;
    cout<<"                    2021             "<<endl;
    cout<<"                  CCOMP6-1           "<<endl;
    cout<<"     --------------------------------"<<endl;
    cout<<"     --------------------------------"<<endl;
    cout<<"     --------------------------------"<<endl;
    cout<<endl;
    cout<<"     Indexando....."<<endl;
    Timer<int()> timed_built(build_data_structure, "Index");
    timed_built();
    
    cout<<endl;
    Timer<std::vector<std::vector<int>>(std::vector<int>, int)> timed_query(query_knn,"QueryKNN");
    
    vector<int> Point;
    cout<<endl;
    cout<<"     Ingrese los KNN: ";
    int knn;
    cin>>knn;
    cout<<endl;
    cout<<"     Punto a consultar: "<<endl;
    Point.clear();
    Point = readKNN();
    cout<<endl;
    cout<<"      -----------------------------"<<endl;
    cout<<"                    KNN            "<<endl;
    cout<<endl;
    
    std::vector<std::vector<int>> result = timed_query(Point,knn);
    cout<<endl;
    
    return 0;
}

