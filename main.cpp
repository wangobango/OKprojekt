#include <iostream>
#include "Generator.h"
#include "Methods.h"
#include "fstream"
#include "Paramethers.h"

int main() {
    /*Koncepcja jest taka :
     * Rozmnazanych 10% najlepszych z 5 % najgorszych
     * Populacja rozrasta sie do max 3 krotnego rozmiaru poczatkowego
     * P mutacji wynosi = 6
     * Po rozrosnieciu sie populacji usuwane są nadmiarowe, najgorsze osobniki
     * Prawdopodbienstwo mutacji zmniejsza sie z czasem
     *
    */
    /*
    Generator generator(50,10);
    generator.GenerateTasks();
    generator.GenerateMaintances();
    vector<Operation> pom;
    vector<Operation> pom2;
    vector<Operation> pom3;
    pom = generator.CreateMachineA();
    pom2 = generator.CreateMachineB(pom);
    generator.show(pom);
    //pom3 = crossover(pom,pom);
    compare(pom,pom2);
    generator.show(pom3);*/
    //Test algorytmu:
    //1. Tworzenie populacji

    Generator generator(50,10);
    generator.GenerateTasks();
    generator.GenerateMaintances();
    vector<Operation> pom;
    vector<Operation> pom2;
    vector<Solution> population;
    int prev_size=0;
    for(int i =0;i<50;i++){
        Solution pom3(generator);
        population.push_back(pom3);
    }
    prev_size = (int)population.size();
    ofstream file;
    file.open("test.csv");
    int end_time;
    for(int i=0;i<150;i++){
        end_time = 0;
        selection(population,generator);

        //mutacja
        if(genRandom(1,100)<10){
            int j = genRandom(0,population.size()-1);
            pom = population[j].getMachineA();
            mutate(pom);

            pom2 = population[j].getMachineB();
            mutate(pom2);

            //generator.reEval(pom);
            //generator.show(pom);

            //compare(pom,pom2);
            population[j].setMachineA(pom);
            population[j].setMachineB(pom2);
        }
        //krzyzowanie
        int k;
        vector<Solution> children;
        for(k=0;k<population.size()/10;k++){
            Solution pom(generator);
            pom.setMachineA(crossover(population[k].getMachineA(),population[prev_size-k-1].getMachineA()));
            pom.setMachineB(crossover(population[k].getMachineB(),population[prev_size-k-1].getMachineB()));
            compare(pom.machineA,pom.machineB);
            children.push_back(pom);
        }
        for(auto item: children){
            population.push_back(item);
        }
        sort(population.begin(),population.end(),SortSolution);
        //if(i%10)
         //   selection(population,generator);
        for(int k=0;k<population.size();k++){
            end_time+=fitness(population[k].getMachineA(),population[k].getMachineB());
            end_time = end_time/(int)population.size();
        }
        generator.show(population[i].getMachineA());
        file<<i<<"\t"<<end_time<<"\n";
    }

    file.close();
//
//    Generator test(50,9);
//    test.GenerateTasks();
//    test.GenerateMaintances();
//
//    vector<Operation> pom = test.CreateMachineA();
//    vector<Operation> pom2 = test.CreateMachineB(pom);
//
//    test.show(pom);
//    test.show(pom2);
//
//    mutate(pom);
//
//    test.reEval(pom);
//    test.reEval(pom2);
//
//    removeEmptyA(pom);
//    removeEmptyB(pom,pom2);
//
//    test.show(pom);
//    test.show(pom2);


}