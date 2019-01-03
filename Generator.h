//
// Created by ramon on 12/13/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <list>

#define MAX_START 4000
#define MAX_OPERATIONAL 150
#define OPERATIONS 59

#ifndef OKPROJECT_GENERATOR_H
#define OKPROJECT_GENERATOR_H


using namespace std;

class Operation {
    int id;
    int start_time;
    int end_time;
    int operationalTime;
    bool partA;
    bool partB;
    bool maintance;
    bool empty;
public:
    Operation(int operationalTime, int id) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 100);
        this->id = id;
        this->start_time = 0;
        this->end_time = 0;
        this->partA = true;
        this->partB = false;
        this->maintance = false;
        this->empty = false;
        this->operationalTime = operationalTime;
    };

    void makePartB() {
        this->partB = true;
        this->partA = false;
        this->maintance = false;
        this->empty = false;
    };

    void makeMaintance() {
        this->setId(-1);
        this->partA = false;
        this->partB = false;
        this->empty = false;
        this->maintance = true;
    };

    void makeEmpty() {
        this->setId(-2);
        this->partA = false;
        this->partB = false;
        this->maintance = false;
        this->empty = true;
    };

    int getId() const {
        return id;
    }

    void setId(int id) {
        Operation::id = id;
    }

    bool isPartA() const {
        return partA;
    }

    void setPartA(bool partA) {
        Operation::partA = partA;
    }

    bool isPartB() const {
        return partB;
    }

    void setPartB(bool partB) {
        Operation::partB = partB;
    }

    bool isMaintance() const {
        return maintance;
    }

    void setMaintance(bool maintance) {
        Operation::maintance = maintance;
    }

    int getOperationalTime() const {
        return operationalTime;
    }


    int getStart_time() const {
        return start_time;
    }

    void setStart_time(int start_time) {
        Operation::start_time = start_time;
    }

    int getEnd_time() const {
        return end_time;
    }

    void setEnd_time(int end_time) {
        Operation::end_time = end_time;
    }

    bool isEmpty() const {
        return empty;
    }

    void setEmpty(bool empty) {
        Operation::empty = empty;
    }


};



class Generator {
    vector<Operation> tasksA;
    vector<Operation> tasksB;
    vector<Operation> maintancesA;
    vector<Operation> maintancesB;
    int numberOfTasks;
    int numberOfMaintances;
public:
    Generator(int tasks, int maintances) {
        this->numberOfTasks = tasks;
        this->numberOfMaintances = maintances;
    };

    struct myclass {
        bool operator()(Operation& i, Operation& j) { return (i.getStart_time() < j.getStart_time()); }
    } myobject;

    int getNumberOfTasks() const {
        return numberOfTasks;
    }

    int getNumberOfMaintances() const {
        return numberOfMaintances;
    }

    void GenerateTasks() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, MAX_OPERATIONAL);
        int operationalTime = 0;
        for (int i = 0; i < numberOfTasks; i++) {
            operationalTime = dis(gen);
            Operation temp(operationalTime, i);
            tasksA.push_back(temp);
        }
        random_device rd2;
        mt19937 gen2(rd2());
        uniform_int_distribution<> dis2(1, MAX_OPERATIONAL);
        for (int i = 0; i < numberOfTasks; i++) {
            operationalTime = dis2(gen2);
            Operation temp(operationalTime, i);
            temp.makePartB();
            tasksB.push_back(temp);
        }
    };

    void GenerateMaintances() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, MAX_OPERATIONAL);
        uniform_int_distribution<> dis2(1, MAX_START - 1000);
        int operationalTime = 0;
        for(int i =0;i<numberOfMaintances;i++){
            operationalTime = dis(gen);
            Operation temp(operationalTime,-1);
            temp.setStart_time(dis2(gen));
            temp.setEnd_time(temp.getStart_time()+temp.getOperationalTime());
            temp.makeMaintance();
            maintancesA.push_back(temp);
        }
        sort(maintancesA.begin(), maintancesA.end(), myobject);
        for(int j=0;j<maintancesA.size();j++){
            if(maintancesA[j+1].getStart_time()<=maintancesA[j].getEnd_time()){
                maintancesA[j+1].setStart_time(maintancesA[j].getEnd_time()+1);
            }
        }
        for(int i =0;i<numberOfMaintances;i++) {
            operationalTime = dis(gen);
            Operation temp(operationalTime, -1);
            temp.setStart_time(dis2(gen));
            temp.setEnd_time(temp.getStart_time() + temp.getOperationalTime());
            temp.makeMaintance();
            maintancesB.push_back(temp);
        }
        sort(maintancesB.begin(), maintancesB.end(), myobject);
        for (int k = 0; k < numberOfMaintances; k++){
            if(maintancesB[k].getStart_time()<maintancesA[k].getEnd_time()){
                maintancesB[k].setStart_time(maintancesB[k].getStart_time()+(maintancesA[k].getEnd_time()-maintancesB[k].getStart_time())+1);
                maintancesB[k].setEnd_time(maintancesB[k].getStart_time() + maintancesB[k].getOperationalTime());
                while(true){
                    if(isMaintanceTime(maintancesB[k].getStart_time(),maintancesA) || isMaintanceTime(maintancesB[k].getEnd_time(),maintancesA)){
                        maintancesB[k].setStart_time(maintancesB[k].getEnd_time()+MAX_OPERATIONAL+1);
                        maintancesB[k].setEnd_time(maintancesB[k].getStart_time()+maintancesB[k].getOperationalTime());
                    }
                    else break;
                }
            }
        }
    };

    void show(vector<Operation> tasks) {
        int totalTime = 0;
        int maintances = 0;
        int maintanceTime = 0;
        int empty = 0;
        int empty_time = 0;
        int taskNumber = 0;
        int totalEndTime = 0;
        int totalEmptyTime = 0;
        vector<Operation>::iterator it;
        int i = 0;
        for (it = tasks.begin(); it < tasks.end(); it++) {
            cout << "Id : " << tasks[i].getId() << " Operational time: " << tasks[i].getOperationalTime();
            if (tasks[i].isMaintance()) {
                cout << " is a maintance nr : " << maintances << ", (start,end) : " << tasks[i].getStart_time() << ","
                     << tasks[i].getEnd_time() << ")" << endl;
                maintances++;
                maintanceTime += tasks[i].getOperationalTime();
            } else if (tasks[i].isPartA()) {
                cout << " is PartA, (start,end) : (" << tasks[i].getStart_time() << "," << tasks[i].getEnd_time() << ")"
                     << endl;
                taskNumber++;
            } else if (tasks[i].isPartB()) {
                cout << " is PartB, (start,end) : (" << tasks[i].getStart_time() << "," << tasks[i].getEnd_time() << ")"
                     << endl;
                taskNumber++;
            } else if (tasks[i].isEmpty()) {
                cout << " is empty space, (start,end) : (" << tasks[i].getStart_time() << "," << tasks[i].getEnd_time()
                     << ")" << endl;
                empty++;
                empty_time += tasks[i].getOperationalTime();
            } else cout << endl;
            totalTime += tasks[i].getOperationalTime();
            totalEndTime += tasks[i].getEnd_time();
            if(it+1!=tasks.end()) totalEmptyTime += (tasks[i+1].getStart_time() - tasks[i].getEnd_time());
            i++;
        }
        cout << "Total time amounts to : " << totalTime << " Number of tasksA: " << taskNumber << endl
             << "Number of maintances: " << maintances << " Amounting to total time of: " << maintanceTime << endl;
        cout << "Total amount of empty spaces is : " << empty << " which amounts to time total of " << totalEmptyTime
             << endl;
        cout << "Total EndTime of all operations amounts to: "<<totalEndTime<<endl;
    };

    int GetTotalOperationlTime(vector<Operation> var) {
        int sum = 0;
        int i = 0;
        vector<Operation>::iterator it;
        for (it = var.begin(); it < var.end(); it++) {
            sum += var[i].getOperationalTime();
            i++;
        }
        return sum;
    }

    static bool isCorrect(vector<Operation>& var){
        for(int i = 0 ;i<var.size();i++){
            if(var[i+1].getStart_time()<=var[i].getEnd_time()){
                return false;
            }
        }
        return true;
    }

//zrobione brute force'm ? do poprawy ?
    static void reEval(vector<Operation>& var){
        while(!isCorrect(var)) {
            int times = 0;
            //label1:
            for (int i = 0; i < var.size(); i++) {
                //TODO : te jedna rzecz zmienilem
//                if(var[i].getStart_time()>var[i].getEnd_time()){
//                    var[i].setEnd_time(var[i].getStart_time()+var[i].getOperationalTime());
//                }
                if (i == 0 && !var[i].isMaintance()) {
                    var[i].setStart_time(var[i - 1].getEnd_time() + 1);
                }
                var[i].setEnd_time(var[i].getStart_time() + var[i].getOperationalTime());
                if (var[i + 1].getStart_time() <= var[i].getEnd_time() && i+1<=var.size()) {
                    if (var[i + 1].isMaintance()) {
                        int k = i;
                        while (true) {
                            if (var[k + 1].getStart_time() > var[k].getEnd_time() && k+1<=var.size()) {
                                break;
                            }
                            //var[k].setStart_time(var[k + 1].getEnd_time() + 1);
                            /*if (var[k].isMaintance()) {
                                k--;
                                continue;
                            }*/
                            var[k].setStart_time(var[k+1].getEnd_time()+1);
                            //var[k].setEnd_time(var[k].getStart_time() + var[k].getOperationalTime());
                            iter_swap(var.begin() + k, var.begin() + k + 1);
                            if(k-1<=0) break;
                            k--;
                        }
                    } else
                        //if(var[i+1].getStart_time()<=var[i].getEnd_time())
                    {
                        var[i + 1].setStart_time(var[i].getEnd_time() + 1);
                        var[i + 1].setEnd_time(var[i + 1].getStart_time() + var[i + 1].getOperationalTime());
                    }
                }
            }
            /*for (int i = 0; i < var.size(); i++) {
                if (var[i].isMaintance() && !var[i + 1].isMaintance() &&
                    var[i + 1].getStart_time() <= var[i].getEnd_time() && i+1<=var.size()) {
                    var[i + 1].setStart_time(var[i].getEnd_time() + 1);
                    var[i + 1].setEnd_time(var[i + 1].getOperationalTime() + var[i + 1].getStart_time());
                }
                if (i == var.size() - 1 && times == 0) {
                    times++;
                    goto label1;
                };
            }*/
        }
    };


    vector<Operation> CreateMachineA() {
        //Create start and end times for tasksA
        vector<Operation> pom;
        vector<Operation>::iterator it;
        vector<Operation> result;
        it = tasksA.begin();
        int i = 0;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, MAX_START);
        //Ustalanie czasow rozpczecia zadan
        for (it; it != tasksA.end(); it++) {
            Operation pom2(tasksA[i].getOperationalTime(), tasksA[i].getId());
            pom2.setStart_time(dis(gen));
            pom2.setEnd_time(pom2.getStart_time() + pom2.getOperationalTime());
            pom.push_back(pom2);
            i++;
        }
        //Sortowanie w celu pozniejszeog wlozenia w maszyne, chcemy zeby
        //wczesniej zacyznajce sie zadania byly wczesniej nie ?
        sort(pom.begin(), pom.end(), myobject);

        //Wkladanie w maszyne

        int k = 0;
        int j = 0;
        i = 0;
        for (i; i < numberOfTasks + numberOfMaintances; i++) {
            if (maintancesA[k].getStart_time() < pom[j].getStart_time() && k < numberOfMaintances) {
                result.push_back(maintancesA[k]);
                k++;
            } else {
                result.push_back(pom[j]);
                j++;
            }
        }

        //Konieczna jest teraz reewaluacja start timeów żeby były zgodzne z operation timemami poprzednich zadan

        reEval(result);

        return result;

    };


    static Operation searchID(int id, vector<Operation>& tasks) {
        vector<Operation>::iterator it;
        int i = 0;
        for(it = tasks.begin();it<tasks.end();it++){
            if (tasks[i].getId() == id && id >= 0) {
                return tasks[i];
            }
            i++;
        }
    };

    bool isMaintanceTime(int time, vector<Operation>& tasks) {
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].getStart_time() < time && tasks[i].getEnd_time() > time && tasks[i].isMaintance()) {
                return true;
            }
        }
        return false;
    };

    vector<Operation> CreateMachineB(vector<Operation>& var) {
        vector<Operation> result;
        //Wygenerowanie odpowiednich star i end time'ow dla zadan na maszynie B, musza byc one odpowiednie wzgledem
        //swoich czesci na maszynie A
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0,49);
        vector<int> temp;
        for(int i =0;i<numberOfTasks;i++){
            int p = dis(gen);
            while(true){
                if(find(temp.begin(),temp.end(),p)!=temp.end()){
                    p = dis(gen);
                }
                else{
                    break;
                }
            }
            temp.push_back(p);
        }

        for(int i=0;i<numberOfTasks;i++){
            Operation pom = searchID(temp.back(),var);
            Operation pom2 = searchID(temp.back(),tasksB);
            temp.pop_back();
            pom2.setStart_time(pom.getEnd_time()+1);
            pom2.setEnd_time(pom2.getStart_time()+pom2.getOperationalTime());
            pom2.makePartB();
            result.push_back(pom2);
        }

        int prev = 0;

        for (auto item : maintancesB) {
            if(item.getStart_time()<=prev){
                item.setStart_time(prev+1);
            }
            result.push_back(item);
            prev = item.getEnd_time();
        }

        sort(result.begin(), result.end(), myobject);

        reEval(result);

        return result;
    };


    const vector<Operation> &getMaintancesA() const {
        return maintancesA;
    }

    void setMaintancesA(const vector<Operation> &maintancesA) {
        Generator::maintancesA = maintancesA;
    }

    const vector<Operation> &getMaintancesB() const {
        return maintancesB;
    }

    void setMaintancesB(const vector<Operation> &maintancesB) {
        Generator::maintancesB = maintancesB;
    }

    const vector<Operation> &getTasksA() const {
        return tasksA;
    }

    void setTasksA(const vector<Operation> &tasksA) {
        Generator::tasksA = tasksA;
    }

    const vector<Operation> &getTasksB() const {
        return tasksB;
    }

    void setTasksB(const vector<Operation> &tasksB) {
        Generator::tasksB = tasksB;
    }

};


#endif //OKPROJECT_GENERATOR_H
