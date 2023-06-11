//
// Created by Philipp on 30.05.2023.
//


#include <cmath>
#include "ga.h"
#include <random>
#include <map>


// init random generator
std::random_device rd2;
std::mt19937 rng2(rd2());
std::uniform_int_distribution<int> chanceRandomizer(0, 100);


Population::Population(long _size, std::vector<int>& parameters, std::vector<float>& weights):
	params(parameters),
	usedWeights(weights)
	{
    size = _size;
}


void Population::setClassesAmount(std::map<Subject *, int> amount) {
    classesAmount = std::move(amount);
}


void Population::generateRandom() {
    for (int p{0}; p<this->size; p++) {
        // generate new individual
        Timetable pop = Timetable(3, &usedWeights);  // <- hardcoded
        pop.setClassesAmount(classesAmount);
        pop.randomizeTimetable();
        // add individual to all population
        pops.push_back(std::move(pop));
    }
}

//
// SELECTIONS
//

void Population::crossRoulette() {
    // init random

    std::vector<float> scoredVector;

    for (int popIndex{0}; popIndex<this->size; popIndex++) {
        pops[popIndex].calculateScore();
        scoredVector.push_back(std::max(pops[popIndex].currentScore, 0.0f));
    }

    // init roulette selection
    std::discrete_distribution<> roulette(scoredVector.begin(), scoredVector.end());

    // init gene selector
    // !!! needs to be rewritten soon...
    std::uniform_int_distribution<int> geneSelector(0, pops[0].getClassCount() - 1);

    // run selection
    for (int popIndex{0}; popIndex<this->size; popIndex++) {
        // use selected type.

		// check if crossover should run
		if (chanceRandomizer(rng2) > params[0]) {
			continue;
		}

        // select second index
        int parentId2 = roulette(rng2);

        switch (crossoverMode) {
            case 0:
                // default crossover
                defaultCrossover(&pops[popIndex], &pops[parentId2], geneSelector(rng2));
                break;
            case 1:
                // single crossover
                singleGeneCrossover(&pops[popIndex], &pops[parentId2], geneSelector(rng2));
                break;
        }
    }

}

//
// CROSSOVERS
//

void Population::defaultCrossover(Timetable *parent1, Timetable *parent2, int crossoverPoint) {
    /**
     * @brief runs a default crossover (over a point)
     *
     * This function just takes takes some days from second parent
     *
     * @param parent1 the parent that will be edited
     * @param parent2 the parent to take genes from
     * @param crossoverPoint int in range [0; number_of_genes]
     */

    // pass genes

    for (int geneId{0}; geneId < crossoverPoint; geneId++) {
        parent1->classes[geneId].order = parent2->classes[geneId].order;
        parent1->classes[geneId].day = parent2->classes[geneId].day;
    }
}

void Population::singleGeneCrossover(Timetable *parent1, Timetable *parent2, int crossoverPoint) {
    parent1->classes[crossoverPoint].order = parent2->classes[crossoverPoint].order;
    parent1->classes[crossoverPoint].day = parent2->classes[crossoverPoint].day;
}

void Population::evolve(long iterations) {
    for (long i{0}; i < iterations; i++) {
        crossRoulette();
    }
}

float Population::getAverageScore() {

    double totalScore{0};

    for (int popIndex{0}; popIndex<this->size; popIndex++) {
        totalScore += pops[popIndex].currentScore;
    }

    return (totalScore / (double)this->size);
}

void Population::setCrossoverMode(int mode) {
    this->crossoverMode = mode;
}

Timetable* Population::getBestResult() {
	Timetable* result = nullptr;
	float bestScore{-99999};

	for (int popIndex{0}; popIndex<this->size; popIndex++) {
		if (pops[popIndex].currentScore > bestScore)  {
			result = &pops[popIndex];
			bestScore = result->currentScore;
		}
	}
	return result;
}