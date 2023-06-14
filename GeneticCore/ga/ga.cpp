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
// randomizer for calculating chances
std::uniform_int_distribution<int> chanceRandomizer(0, 100);


Population::Population(long _size, int dayCount, int amount,
					   std::vector<int>& parameters,
					   std::vector<float>& weights):
	params(parameters),
	timetableLength(dayCount),
	classCount(amount),
	usedWeights(weights){
    size = _size;
	// initialize random number generators
	randomDay = std::uniform_int_distribution<int>(0, dayCount - 1);
	randomOrder = std::uniform_int_distribution<int>(0, amount - 1);
}


void Population::setClassesAmount(std::map<Subject *, int> amount) {
    classesAmount = std::move(amount);
}


void Population::generateRandom() {
    for (int p{0}; p<this->size; p++) {
        // generate new individual
        Timetable pop = Timetable(timetableLength, classCount, &usedWeights);
        pop.setClassesAmount(classesAmount);
        pop.randomizeTimetable();
        // add individual to all population
        pops.push_back(std::move(pop));
    }
	randomGene = std::uniform_int_distribution<int>(0, pops[0].getClassCount() - 1);
}

//
// SELECTIONS
//

void Population::crossRoulette() {
    // init random

    std::vector<float> scoredVector;

    for (int popIndex{0}; popIndex<this->size; popIndex++) {
		if (pops[popIndex].scoreChanged) {
        	pops[popIndex].calculateScore();
			pops[popIndex].scoreChanged = false;
		}
        scoredVector.push_back(std::max(pops[popIndex].currentScore, 0.0f));
    }

    // init roulette selection
    std::discrete_distribution<> roulette(scoredVector.begin(), scoredVector.end());

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
                defaultCrossover(&pops[popIndex], &pops[parentId2], randomGene(rng2));
                break;
            case 1:
                // single crossover
                singleGeneCrossover(&pops[popIndex], &pops[parentId2], randomGene(rng2));
                break;
        }
    }
	float newScore = getAverageScore();
	if (newScore - genAverageScore >= 0 && (newScore - genAverageScore) <= ((float)params.at(2) / 10000.0f)) {
		// run mutations
		for (int popIndex{0}; popIndex<this->size; popIndex++) {
			// use selected type.

			// check if pop should mutate
			if (chanceRandomizer(rng2) > params[1]) {
				continue;
			}

			smartMutation(&pops[popIndex]);
		}
	}
	genAverageScore = newScore;
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

	parent1->scoreChanged = true;
}

void Population::singleGeneCrossover(Timetable *parent1, Timetable *parent2, int crossoverPoint) {
    parent1->classes[crossoverPoint].order = parent2->classes[crossoverPoint].order;
    parent1->classes[crossoverPoint].day = parent2->classes[crossoverPoint].day;

	parent1->scoreChanged = true;
}


//
// MUTATIONS
//

void Population::randomMutation(Timetable *individual) {
	individual->classes[randomGene(rng2)].day = randomDay(rng2);
	individual->classes[randomGene(rng2)].order = randomOrder(rng2);
	individual->scoreChanged = true;
}

void Population::smartMutation(Timetable* individual) {
	// select random gene that will be moved
	Class* modGene = &individual->classes[randomGene(rng2)];
	individual->scoreChanged = true;
	// generate new position
	int newDay{randomDay(rng2)};
	int newOrder{randomOrder(rng2)};

	// check if class at that position exists
	for (int geneI{0}; geneI < individual->classes.size(); geneI++) {
		if (individual->classes[geneI].day == newDay && individual->classes[geneI].order == newOrder) {
			// exists
			if (individual->classes[geneI].subject == modGene->subject) {
				// nothing to change..
				return;
			}

			// copy current class
			Class copiedClass = *modGene;
			*modGene = individual->classes[geneI];
			individual->classes[geneI] = copiedClass;
			return;
		}
	}

	// no class at this position
	modGene->day = newDay;
	modGene->order = newOrder;
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