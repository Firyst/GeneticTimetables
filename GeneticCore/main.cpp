#include <iostream>
#include <random>
#include <string>
#include "ga/ga.h"
//#include "timetable/timetable.h"
#include <map>
#include <chrono>



int main() {

    auto startTime = std::chrono::high_resolution_clock::now();

    Teacher t = Teacher("Test teacher");
    Subject s = Subject(0, "Networks");
    Subject s2 = Subject(1, "Prog");

    /*
    Class myClass = Class(&s);
    Timetable tt = Timetable(3);
         tt.setClassesAmount(timetableData);
    tt.randomizeTimetable();
    */
    std::map<Subject *, int> timetableData = {{&s,  3},
                                              {&s2, 3}};

    Population myPops = Population(1000);
    myPops.setCrossoverMode(1);
    myPops.setClassesAmount(timetableData);
    myPops.generateRandom();


    for (int p=0; p<10000; p ++) {

        myPops.evolve(1);

        std::cout << "gen: " << p << "score: " << myPops.getAverageScore() << std::endl;

    }

    // execution time
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "\nCode execution took " << duration.count() << "ms";

	return 0;
}
