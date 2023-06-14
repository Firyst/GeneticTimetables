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

	const int timetableLength;
	const int classCount;

    // selections&iterations
    void crossRoulette();

	// parameter vector
	// 0: crossoverChance
	// 1: mutationChance
	// 2: mutation threshold * 10000
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

	// pre-initialized randoms
	std::uniform_int_distribution<int> randomDay;
	std::uniform_int_distribution<int> randomOrder;
	std::uniform_int_distribution<int> randomGene;

	// crossovers
	int crossoverMode{0};
    static void defaultCrossover(Timetable* parent1, Timetable* parent2, int crossoverPoint);
    static void singleGeneCrossover(Timetable* parent1, Timetable* parent2, int crossoverPoint);

	// mutations
	/**
	 * @brief very basic random mutation
	 * Mutation that sets random gene random day and order. Fast, but not very accurate.
	 * Sometimes make things even worse.
	 * @param individual
	 */
	void randomMutation(Timetable* individual);

	/**
	 * @brief Better mutation, that does not overlap classes. Should work better, but slower.
	 * selects random spot for class. If has a class already, then swaps them,
	 * otherwise moves random selected class to new place.
	 * @param individual
	 */
	void smartMutation(Timetable* individual);

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
    Population(long size, int dayCount, int amount, std::vector<int>& parameters, std::vector<float>& weights);

	Timetable* getBestResult();

	float genAverageScore{0};
};
