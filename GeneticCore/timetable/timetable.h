//
// Created by Philipp on 30.05.2023.
//
#include <string>
#include <vector>
#include <set>
#include <map>
#include <random>


#ifndef GA_PLAYGROUND_TIMETABLE_H
#define GA_PLAYGROUND_TIMETABLE_H

#endif //GA_PLAYGROUND_TIMETABLE_H

struct Subject{
    Subject(int _id, std::string _name);

    int id;
    std::string name;
    // maybe something else here..
};

struct Teacher {
    std::string name;
    std::set<Subject*> subjects;  // not implemented

    explicit Teacher(std::string _name);
};

struct Class {
    int day;   // day id
    int order; // order in day
    Teacher* teacher;
    Subject* subject;

	Class(Subject* _subject);
    Class(int _day, int _order, Subject* _subject);
};


class Timetable {
    int m_length;  // count of timetable days
	std::map<Subject*, int> classesAmount;

	// scoring multipliers data
	// 0: conflicts
	// 1: time bounds
	// 2: repeats
	// 3: gaps
	// 4: week balance
	// 5: diversity
	// 6: preferred begin&end time
	const std::vector<float>* weights;


    int getGeneCount();
	bool isSlotFree(int day, int order);

public:
    std::vector<Class> classes;
	explicit Timetable(int length, const std::vector<float>* _weights);
	void setClassesAmount(std::map<Subject*, int> amount);
	int getLength() const;
	int getClassCount();
	void randomizeTimetable();  // create a individual
	std::string toString();
    void calculateScore();
    float currentScore{0};
};

