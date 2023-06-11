//
// Created by Philipp on 30.05.2023.
//

#ifndef GA_PLAYGROUND_GA_H
#define GA_PLAYGROUND_GA_H

#endif //GA_PLAYGROUND_GA_H

#include "../timetable/timetable.h"
#include <vector>
#include <random>

float score(Timetable* timetable);

class Population {
    std::vector<Timetable> pops;
    std::map<Subject*, int> classesAmount;
    long size;

    // selections&iterations
    void crossRoulette();

	// parameter vector
	// 0: crossoverChance
	// 1: mutationChance
	const std::vector<int> params;

	// scoring multipliers data - used for individuals
	// 0: conflicts
	// 1: time bounds
	// 2: repeats
	// 3: gaps
	// 4: week balance
	// 5: diversity
	// 6: preferred begin&end time
	const std::vector<float> usedWeights;

	// crossovers
	int crossoverMode{0};
    static void defaultCrossover(Timetable* parent1, Timetable* parent2, int crossoverPoint);
    static void singleGeneCrossover(Timetable* parent1, Timetable* parent2, int crossoverPoint);

public:
    void generateRandom();
    void setClassesAmount(std::map<Subject*, int> amount);
    void setCrossoverMode(int mode);

    void generateWithScoreThr(float threshold);

    void evolve(long iterations);

    float getAverageScore();

	/**
	 * Population constructor
	 * @param size The size of population (number of individuals)
	 * @param parameters Vector of algorithm parameters {CrossoverChance, MutationChance}
	 * @param weights Vector of scoring weights
	 * {conflicts, timeBounds, repeats, gaps, weekBalance, diversity, beginEndTime}

	 */
    Population(long size, std::vector<int>& parameters, std::vector<float>& weights);

	Timetable* getBestResult();
};
