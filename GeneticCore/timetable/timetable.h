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
#include <functional>

/**
 * @brief The Subject class describes subject name, teacher, id and available slots
 */
struct Subject{

    /**
     * @brief constructs the subject
     */
    Subject(int _id, std::string _name, std::string _teacher, std::vector<std::vector<bool>> slots);

    int id; ///< id
    std::string name; ///< subject shown name
    std::string teacher; ///< subject shown teacher
    std::string group;
    std::vector<std::vector<bool>> availableSlots; ///< bool table of available slots
    // maybe something else here..
};

/**
 * @brief The Class class describes a gene of timetable
 */
struct Class {
    int day; ///< day id
    int order; ///< order in day
    Subject* subject;  ///< stored subject

    /**
     *  @brief basic constuctor
     */
	Class(Subject* _subject);
    Class(int _day, int _order, Subject* _subject);
};

/**
 * @brief The Timetable class is used as individual in GA
 */
class Timetable {
    int timetableLength; ///< count of timetable days
    int classCount; ///< max classes every day
    std::map<Subject*, int> classesAmount;  ///< task

    /**
     * @brief weight parameters
     *
     * 0: conflicts
     * 1: time bounds
     * 2: repeats
     * 3: gaps
     * 4: week balance
     * 5: diversity
     */
    std::vector<float>* weights;


    /**
     * @brief getGeneCount get length of dna seq
     * @return number of genes
     */
    int getGeneCount();

    /**
     * @brief isSlotFree checks if current slot is free
     * @param day day ID
     * @param order order ID
     * @return true if is free
     */
	bool isSlotFree(int day, int order);

public:
	// this bool changes to true if some genes were changed, so the score must be calculated again
	// used to optimize unnecessary calculations
    bool scoreChanged = true;

    std::vector<Class> classes;  ///< gene dna storage
    /**
     * @brief Timetable initalize individual
     * @param length amount of days
     * @param classes amount of max classes each day
     * @param _weights weights to use
     */
    Timetable(int length, int classes, std::vector<float>* _weights);

    /**
     * @brief setClassesAmount set this for proper scoring
     * @param amount map of subjects and their requirment count
     */
	void setClassesAmount(std::map<Subject*, int> amount);

    /**
     * @brief getLength get number of days
     * @return the length
     */
	int getLength() const;

    /**
     * @brief getClassCount get max classes each day
     * @return max classes each day
     */
	int getClassCount();

    /**
     * @brief randomizeTimetable generates random timetable
     */
	void randomizeTimetable();  // create a individual

    /**
     * @brief toString visual debug function
     * @return formatted string
     */
	std::string toString();

    /**
     * @brief calculateScore calculates score with given weights and stores it to currentScore
     */
    void calculateScore();
    float currentScore{0}; ///< last counted score

    /**
     * Actually is inverted, so the std::priority queue could store best results.
     * @brief operator < compare scores of timetables. does not recalculate
     * @param other second timetable to compare
     * @return true if current timetable score is larger
     */
    bool operator<(const Timetable& other) const;
    bool operator==(const Timetable& other) const;

};


// define hashes
// source: https://stackoverflow.com/questions/13890747/hash-function-for-user-defined-class-how-to-make-friends
namespace std {
template<>
struct hash<Timetable> {
    size_t operator()(const Timetable& obj) const {
        size_t hash = 0;

        hash ^= std::hash<int>()(obj.getLength());


        for (int i{0}; i < (int)obj.classes.size(); i++) {
            hash ^= std::hash<int>()(obj.classes[i].day * (i + 1) * 31);
            hash ^= std::hash<int>()(obj.classes[i].order * (i + 1) * 31);
            hash ^= std::hash<int>()(obj.classes[i].subject->id);
        }
        return std::hash<int>()(hash);
    }
};
}

#endif //GA_PLAYGROUND_TIMETABLE_H
