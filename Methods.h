//
// Created by ramon on 1/4/18.
//


#define MUT_PROB 100
#define MAX_SHIFT 3000
#define MAX_TASKS 50
#define MAX_SIZE 150

#ifndef OKPROJECT_METHODS_H
#define OKPROJECT_METHODS_H

#include "Generator.h"
#include "Solution.h"
#include <string.h>

struct myclass {
    bool operator()(Operation& i, Operation& j) { return (i.getStart_time() < j.getStart_time()); }
} myobject;

struct SolutionSort {
    bool operator()(Solution& i, Solution& j) { return (i.getTotalEndtime() < j.getTotalEndtime()); }
} SortSolution;


int genRandom(int x,int y){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(x, y);
    return dis(gen);
}

void replace(int position1, int id2 , vector<Operation>& var){
    Operation pom=var[position1];
    var.erase(var.begin()+position1);
    int i=0;
    for(auto item:var){
        if(item.getId()==id2) break;
        i++;
    }
    var.insert(var.begin()+i,pom);
}

void mutate(vector<Operation>& tasks){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, MUT_PROB);
    uniform_int_distribution<> dis2(1, MAX_SHIFT);
    int chance = 0;
    vector<int> toRm; //zawiera pozycje elementow do usunieca
    vector<int> toIns; // zawiera id elementow przed ktore wstawiamy;
    for(int g=0;g<tasks.size();g++){
        Operation item = tasks[g];
        chance = dis(gen);
        if(chance == MUT_PROB/2 && !item.isMaintance()){
            int swap = dis2(gen);
            //cout<<item.getId()<<";"<<swap<<endl;
            item.setStart_time(swap);
            item.setEnd_time(item.getStart_time()+item.getOperationalTime());
            tasks[g]=item;
            //mutowanie start time'u, znalezc nowe miejsce dla zadania, przesuniecie przez funkcje swapTO
            int id = 0;
            if(swap>tasks.back().getEnd_time()){
                Operation pom = tasks[g];
                tasks.erase(tasks.begin()+g);
                tasks.push_back(pom);
            }
            else {
                for (int j = 0; j < tasks.size(); j++) {
                    if (item.getStart_time() >= tasks[j].getStart_time() && item.getStart_time() <= tasks[j].getEnd_time()) {
                        id = tasks[j + 1].getId();
                        toIns.push_back(id);
                        toRm.push_back(g);
                        break;
                    }
                }
            }
        }
    }
    int k = toRm.size()-1;
    for(k;k>=0;--k){
        replace(toRm[k],toIns[k],tasks);
    }
    sort(tasks.begin(),tasks.end(),myobject);
    //Generator::reEval(tasks);
    //return tasks;
}

vector<Operation> crossover(vector<Operation> parentA, vector<Operation> parentB) {
    vector<Operation> result;
    vector<int> pom;
    vector<Operation> temp;
    int k = (int)parentA.size();
    for(int i =0 ; i<ceil((double)parentA.size()/2.0);i++){
        k = parentA.size()-i-1;
        result.push_back(parentA[i]);
        if(parentA[k].isMaintance() && k>i){
            temp.push_back(parentA[k]);
            //k--;
        }else{
            if(k>i) pom.push_back(parentA[k].getId());
            //k--;
        }
    }
    reverse(pom.begin(),pom.end());

    for(auto item:pom){
        result.push_back(Generator::searchID(item,parentB));
    }

    //ustalenie nowych start time'ow bez reewaluacji ogólnej

    for(int i =0;i<result.size();i++){
        if(result[i+1].getStart_time()<=result[i].getEnd_time() && !result[i+1].isMaintance()){
            result[i+1].setStart_time(result[i].getEnd_time()+1);
            result[i+1].setEnd_time(result[i+1].getStart_time()+result[i+1].getOperationalTime()+1);
        }
        else if(result[i+1].getStart_time()>result[i].getEnd_time() && !result[i+1].isMaintance()){
            result[i+1].setStart_time(result[i].getEnd_time()+1);
            result[i+1].setEnd_time(result[i+1].getStart_time()+result[i+1].getOperationalTime()+1);
        }
    }

    for(auto item: temp){
        result.push_back(item);
    }

    sort(result.begin(),result.end(),myobject);

    Generator::reEval(result);

    return result;

}

//Funckja fitnessu dla danego rozwiązania zwraca całkowitą sumę jego end time'ow
int fitness(vector<Operation> A, vector<Operation> B){
    int end_sum = 0 ;
    for(int i = 0 ;i<A.size();i++){
        end_sum+=(A[i].getEnd_time()+B[i].getEnd_time());
    }
    return end_sum;
}

//TODO : zrobic usuwanie pustych, uwzglednic wszystkie wolne przestrzenie w maszynie
void removeEmptyA(vector<Operation>& A){
    for(int i = 0 ; i<A.size();i++){
        if(A[i].getEnd_time()<A[i+1].getStart_time() && !A[i+1].isMaintance()){
            A[i+1].setStart_time(A[i].getEnd_time()+1);
            A[i+1].setEnd_time(A[i+1].getStart_time()+A[i+1].getOperationalTime());
        }
    }

}

void removeEmptyB(vector<Operation>& A, vector<Operation>& B){
    for(int i = 0 ; i<A.size();i++){
        if(B[i].getEnd_time()<B[i+1].getStart_time() && !B[i+1].isMaintance()){
//            if(Generator::searchID(A[i+1].getId(),B).getEnd_time()>A[i].getEnd_time()){
//                B[i + 1].setStart_time(Generator::searchID(A[i+1].getId(),B).getEnd_time()+1);
//                B[i+1].setEnd_time(B[i+1].getStart_time()+B[i+1].getOperationalTime());
//            }
            if(Generator::searchID(B[i+1].getId(),A).getEnd_time()<B[i].getEnd_time()){
                B[i+1].setStart_time(B[i].getEnd_time()+1);
                B[i+1].setEnd_time(B[i+1].getStart_time()+B[i+1].getOperationalTime());
            }
            else{
                B[i+1].setStart_time(Generator::searchID(B[i+1].getId(),A).getStart_time()+1);
                B[i+1].setEnd_time(B[i+1].getStart_time()+B[i+1].getOperationalTime());
            }
        }
    }
}


void selection(vector<Solution>& population,Generator& generator ){
    for(int i =0; i<population.size();i++){
        //removeEmptyA(population[i].machineA);
        //removeEmptyB(population[i].machineA,population[i].machineB);
        population[i].setTotalEndtime(fitness(population[i].getMachineA(),population[i].getMachineB()));
    }

    sort(population.begin(),population.end(),SortSolution);

    if(population.size()>MAX_SIZE){
        int i = population.size();
        while(i>MAX_SIZE){
            population.pop_back();
            i--;
        }
    }
}

//metoda porownuje maszyne A i B danego rozwiazania sprawdzajac czy spelnione sa warunki:
//czesc B zadania zaczyna sie jak skonczy sie czesc A , maintancey nie moga na siebie nachodzic
//funcja odpowiada za naprawienie bledow mogacych wynikac z crossover'a
void compare(vector<Operation>& A, vector<Operation>& B){
    int error = 0;
    for(int i =0 ; i<A.size();i++){
        if(A[i].getId() == -1) continue;
        else if(Generator::searchID(i,A).getEnd_time()>=Generator::searchID(i,B).getStart_time()){
            int k;
            for(k=0;k<B.size();k++){
                if(B[k].getId() == i) break;
            }
            B[k].setStart_time(Generator::searchID(i,A).getEnd_time()+1);
            B[k].setEnd_time(B[k].getStart_time()+B[k].getOperationalTime());
            error++;
            //cout<<i<<" jest blednie umiejscowione"<<endl;
        }
    }
    if(error>0){
        //Generator::reEval(B);
    }
}

void changePos(int pos1,int pos2, vector<Operation>& vec){
    Operation pom = vec.at(pos1);
    vec.erase(vec.begin()+pos1);
    if(pos1<pos2){
        vec.insert(vec.begin()+pos2-1,pom);
    }
    else{
        vec.insert(vec.begin()+pos2,pom);
    }

}
//vector<int> changeLog(Generator::getNumberOfTasks()+Generator::getNumberOfMaintances(),0);
//changeLog[A[i+1].getId()] = change;
//






#endif //OKPROJECT_METHODS_H
