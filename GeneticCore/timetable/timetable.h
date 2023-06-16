//
// Created by Philipp on 30.05.2023.
//


#ifndef GA_PLAYGROUND_TIMETABLE_H
#define GA_PLAYGROUND_TIMETABLE_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <random>


struct Subject{
    Subject(int _id, std::string _name, std::string _teacher, std::vector<std::vector<bool>> slots);

    int id;
    std::string name;
    std::string teacher;
    std::vector<std::vector<bool>> availableSlots;
    // maybe something else here..
};


struct Class {
    int day;   // day id
    int order; // order in day
    Subject* subject;

	Class(Subject* _subject);
    Class(int _day, int _order, Subject* _subject);
};


class Timetable {
    int timetableLength;  // count of timetable days
    int classCount;  // max classes every day
	std::map<Subject*, int> classesAmount;

	// scoring multipliers data
	// 0: conflicts
	// 1: time bounds
	// 2: repeats
	// 3: gaps
	// 4: week balance
	// 5: diversity
	// 6: preferred begin&end time
    std::vector<float>* weights;

    int getGeneCount();
	bool isSlotFree(int day, int order);

public:
	// this bool changes to true if some genes were changed, so the score must be calculated again
	// used to optimize unnecessary calculations
	bool scoreChanged = true;

    std::vector<Class> classes;
    Timetable(int length, int classes, std::vector<float>* _weights);
	void setClassesAmount(std::map<Subject*, int> amount);
	int getLength() const;
	int getClassCount();
	void randomizeTimetable();  // create a individual
	std::string toString();
    void calculateScore();
    float currentScore{0};

    /**
     * Actually is inverted, so the std::priority queue could store best results.
     * @brief operator < compare scores of timetables. does not recalculate
     * @param other second timetable to compare
     * @return true if current timetable score is larger
     */
    bool operator<(const Timetable& other) const;
};

#endif //GA_PLAYGROUND_TIMETABLE_H
