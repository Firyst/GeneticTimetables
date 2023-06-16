//
// Created by Philipp on 30.05.2023.
//

#ifndef GA_PLAYGROUND_GA_H
#define GA_PLAYGROUND_GA_H


#include "../timetable/timetable.h"
#include "../uniquepriorityqueue.h"
#include <vector>
#include <random>
#include <queue>

/**
 * @brief The Population class. Used to handle genetic algoritm
 */
class Population {
    std::vector<Timetable> pops; ///< all the population
    std::map<Subject*, int> classesAmount;  ///< classes task (how much of each)
    long size; ///< size of population


    float bestScoreThreshold{0}; ///< the score of last of top-100 results. Used to optimize calculations.

	const int timetableLength;
	const int classCount;

    // selections&iterations
    /**
     * Basically runs the evolution. There is potential to add another selection method.
     * @brief crossRoulette selects ones to crossover using discrete distirbution
     */
    void crossRoulette();

    /**
    * @brief parameter vector
    * 0: crossoverChance
    * 1: mutationChance
    * 2: mutation threshold * 10000
    */
	const std::vector<int> params;

	// scoring multipliers data - used for individuals

    /**
     * @brief usedWeights
     *
     * 0: conflicts
     * 1: time bounds
     * 2: repeats
     * 3: gaps
     * 4: week balance
     * 5: diversity
     */
	const std::vector<float> usedWeights;

	// pre-initialized randoms
    std::uniform_int_distribution<int> randomDay; ///< random generator for proper random days
    std::uniform_int_distribution<int> randomOrder;  ///< random generator for random orders
    std::uniform_int_distribution<int> randomGene;  ///< random generator for random genes

	// crossovers
	int crossoverMode{0};

    /**
     * @brief runs a default crossover (over a point)
     *
     * This function just takes takes some days from second parent
     *
     * @param parent1 the parent that will be edited
     * @param parent2 the parent to take genes from
     * @param crossoverPoint int in range [0; number_of_genes]
     */
    void defaultCrossover(Timetable* parent1, Timetable* parent2, int crossoverPoint);

    /**
     * This crossover just changes one gene. Seems to be most efficient.
     * @brief runs a default crossover (single gene)
     * @param parent1
     * @param parent2
     * @param crossoverPoint
     */
    void singleGeneCrossover(Timetable* parent1, Timetable* parent2, int crossoverPoint);

    /**
     * @brief does not create overlapping classes during proccess
     * @param parent1
     * @param parent2
     */
    void smartSingleCrossover(Timetable* parent1, Timetable* parent2);

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

    Population(const Population&) = delete;

    /**
     * @brief generateRandom generates random population.
     */
    void generateRandom();

    /**
     * @brief setClassesAmount sets classes rules&tasks
     * @param amount map of subjects and amount of them
     */
    void setClassesAmount(std::map<Subject*, int> amount);

    /**
     * @brief setCrossoverMode sets mode for crossover
     * @param mode 0=default, 1=single, 2=smart
     */
    void setCrossoverMode(int mode);

    /**
     * @brief evolve runs the evolution process
     * @param iterations how many iterations to run
     */
    void evolve(long iterations);

    /**
     * @brief getAverageScore calculates gen average score
     * @return float value of score
     */
    float getAverageScore();

    UniquePriorityQueue<Timetable> bestResults; ///< best result storage

	/**
	 * Population constructor
	 * @param size The size of population (number of individuals)
	 * @param parameters Vector of algorithm parameters {CrossoverChance, MutationChance}
	 * @param weights Vector of scoring weights
	 * {conflicts, timeBounds, repeats, gaps, weekBalance, diversity, beginEndTime}

	 */
    Population(long size, int dayCount, int amount, const std::vector<int>& parameters, const std::vector<float>& weights);

    /**
     * @brief getBestResult gets the best pop in current popultaion
     * @return pointer to timetable object
     */
	Timetable* getBestResult();

    float genAverageScore{0}; ///< stores the average score
};

#endif //GA_PLAYGROUND_GA_H
