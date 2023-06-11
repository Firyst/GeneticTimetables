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

    std::map<Subject *, int> timetableData = {{&s,  3},
                                              {&s2, 3}};


	std::vector<int> inputParams = {35, 20};
	std::vector<float> scoringParams = {25.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    Population myPops = Population(200, inputParams, scoringParams);
    myPops.setCrossoverMode(1);
    myPops.setClassesAmount(timetableData);
    myPops.generateRandom();


    for (int p=0; p<500; p ++) {

        myPops.evolve(10);

        std::cout << "gen: " << p * 10 << "; score: " << myPops.getAverageScore() << std::endl;

    }

    // execution time
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::cout << myPops.getBestResult()->toString() << "\n\n";

    std::cout << "\nCode execution took " << duration.count() << "ms";

	return 0;
}
