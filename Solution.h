//
// Created by ramon on 1/5/18.
//
#include "Generator.h"


#ifndef OKPROJECT_SOLUTION_H
#define OKPROJECT_SOLUTION_H


class Solution {
public:
    vector<Operation> machineA;
    vector<Operation> machineB;
    int totalEndtime;
   Solution(Generator generator){
        this->machineA = generator.CreateMachineA();
        this->machineB = generator.CreateMachineB(this->machineA);
    };

    const vector<Operation> &getMachineA() const {
        return machineA;
    }

    void setMachineA(const vector<Operation> &machineA) {
        Solution::machineA = machineA;
    }

    const vector<Operation> &getMachineB() const {
        return machineB;
    }

    void setMachineB(const vector<Operation> &machineB) {
        Solution::machineB = machineB;
    }

    int getTotalEndtime() const {
        return totalEndtime;
    }

    void setTotalEndtime(int totalEndtime) {
        Solution::totalEndtime = totalEndtime;
    }


};


#endif //OKPROJECT_SOLUTION_H
