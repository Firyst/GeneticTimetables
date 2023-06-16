//
// Created by Philipp on 30.05.2023.
//


#include <cmath>
#include "ga.h"
#include <random>
#include <map>
#include <algorithm>


// init random generator
std::random_device rd2;
std::mt19937 rng2(rd2());
// randomizer for calculating chances
std::uniform_int_distribution<int> chanceRandomizer(0, 100);


Population::Population(long _size, int dayCount, int amount,
                       const std::vector<int>& parameters,
                       const std::vector<float>& weights):
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
        Timetable pop = Timetable(timetableLength, classCount, const_cast<std::vector<float>*>(&usedWeights));
        pop.setClassesAmount(classesAmount);
        pop.randomizeTimetable();
        // add individual to all population
        pops.push_back(std::move(pop));
    }
	randomGene = std::uniform_int_distribution<int>(0, pops[0].getClassCount() - 1);
}

void Population::crossRoulette() {
    std::vector<float> scoredVector;

    for (int popIndex{0}; popIndex<this->size; popIndex++) {
		if (pops[popIndex].scoreChanged) {
        	pops[popIndex].calculateScore();
			pops[popIndex].scoreChanged = false;

            // handle priority queue of best results
            if (pops[popIndex].currentScore > bestScoreThreshold) {
                // could be placed in top-100
                bestResults.push(pops[popIndex]);

                // check if already is a top 100
                if (bestResults.size() > 100) {
                    // remove the worst element
                    bestResults.pop();
                    // move the threshold to current minimum
                    bestScoreThreshold = bestResults.top().currentScore;
                }
            }
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
            case 2:
                smartSingleCrossover(&pops[popIndex], &pops[parentId2]);
                break;
        }
    }
	float newScore = getAverageScore();
	if (newScore - genAverageScore >= 0 && (newScore - genAverageScore) <= ((float)params.at(2) / 10000.0f)) {
		// run mutations
		for (int popIndex{0}; popIndex<this->size; popIndex++) {
			// use selected type.

			// check if pop should mutate
			if (chanceRandomizer(rng2) >= params[1]) {
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

void Population::smartSingleCrossover(Timetable* parent1, Timetable* parent2) {
    // select gene to pass from second to first
    int crossoverPoint = randomGene(rng2);
    // gene that overlaps geneId (if exists)
    int geneId2{-1};

    // generate list of all free slots
    std::vector<std::pair<int, int>> freeSlots;
    for (int day_i = 0; day_i < timetableLength; day_i++) {
        for (int order_i = 0; order_i<classCount; order_i++) {
            freeSlots.emplace_back(day_i, order_i);
        }
    }

    // check if class at that position exists in first parent
    for (int geneI{0}; geneI < parent1->classes.size(); geneI++) {
        if (geneId2 == -1 && parent1->classes[geneI].day == parent2->classes[crossoverPoint].day &&
                             parent1->classes[geneI].order == parent2->classes[crossoverPoint].order) {
            // exists
            geneId2 = geneI;
        }

        // remove free slot
        std::pair<int, int> slot{parent1->classes[geneI].day, parent1->classes[geneI].order};
        for (auto iter = freeSlots.begin(); iter != freeSlots.end(); ++iter )
        {
            if (*iter == slot)
            {
                freeSlots.erase( iter );
                break;
            }
        }

    }

    if (geneId2 != -1) {
        std::pair<int, int> selectedSlot = freeSlots[rand() % freeSlots.size()];
        // move class at crossover slot
        parent1->classes[geneId2].day = selectedSlot.first;
        parent1->classes[geneId2].order = selectedSlot.second;
    }
    // no overlap will be made
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
