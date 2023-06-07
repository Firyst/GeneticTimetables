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

    explicit Population(long size);
};
