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
	Subject s3 = Subject(2, "Linalg");

    std::map<Subject *, int> timetableData = {{&s,  8},
                                              {&s2, 10},
											  {&s3, 6}};


	std::vector<int> inputParams = {25, 5, 1};
	std::vector<float> scoringParams = {25.0f, 1.0f, 2.0f, 4.0f, 1.0f, 1.0f, 1.0f};
    Population myPops = Population(3000, 6, 7, inputParams, scoringParams);
    myPops.setCrossoverMode(1);
    myPops.setClassesAmount(timetableData);
    myPops.generateRandom();


    for (int p=0; p<1000; p ++) {

        myPops.evolve(20);

        std::cout << "gen: " << p * 20 << "; score: " << myPops.genAverageScore << std::endl;

    }

    // execution time
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::cout << myPops.getBestResult()->toString() << "\n";
	std::cout << myPops.getBestResult()->currentScore << "\n";
    std::cout << "\nCode execution took " << duration.count() << "ms";

	return 0;
}
