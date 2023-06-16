#include <utility>
#include <vector>
#include <string>
#include "timetable.h"
#include <random>
#include <set>
#include <algorithm>

// init random generator
std::random_device rd;
std::mt19937 rng(rd());


int getRandomNumber(int min, int max)
{
	// Создание генератора случайных чисел
	std::random_device rd;
	std::mt19937 rng(rd());

	// Создание распределения случайных чисел
	std::uniform_int_distribution<int> uni(min, max);

	// Генерация случайного числа
	return uni(rng);
}

/// classes for timetable realisation

Subject::Subject(int _id, std::string _name, std::string _teacher, std::vector<std::vector<bool>> slots) {
    id = _id;
    name = std::move(_name);
    teacher = std::move(_teacher);
    availableSlots = slots;
}


Class::Class(int _day, int _order, Subject* _subject) {
    day = _day;
    order = _order;
    subject = _subject;
}

Class::Class( Subject *_subject) {
	day = 0;
	order = 0;
	subject = _subject;
}

//
// TIMETABLE
//

Timetable::Timetable(int length, int classes, std::vector<float>* _weights)
{
    weights = _weights;
    timetableLength = length;
    classCount = classes;
}

void Timetable::setClassesAmount(std::map<Subject *, int> amount) {
	classesAmount = std::move(amount);
}

int Timetable::getLength() const {
	return this->timetableLength;
}

int Timetable::getClassCount() {
	int sum {0};
	for (auto const& [key, val] : classesAmount) {
		sum += val;
	}
	return sum;
}

bool Timetable::isSlotFree(int day, int order) {
	for (Class i : classes) {
		if (i.day == day && i.order == order) {
			return true;
		}
	}
	return false;
}

int Timetable::getGeneCount() {
    return (int)classes.size();
}

void Timetable::randomizeTimetable() {
	classes.clear();

	// iterate through all classes
	// generate vector of available slots

	std::vector<std::pair<int, int>> freeSlots;

	for (int day_i = 0; day_i < timetableLength; day_i++) {
		for (int order_i = 0; order_i<classCount; order_i++) {
			freeSlots.emplace_back(day_i, order_i);
		}
	}

    // create distirbution

	for (auto const& [key, val] : classesAmount) {
		for (int count = 0; count < val; count++) {
			// select a random free slot
            std::uniform_int_distribution<int> uni(0, freeSlots.size() - 1);
			int randomId = uni(rng);
			std::pair<int, int> slot = freeSlots[randomId];
			classes.emplace_back(slot.first, slot.second, key);
			freeSlots.erase(freeSlots.begin() + randomId);
		}
	}
}

std::string Timetable::toString() {
	if (classes.empty()) {
		return "Empty time table of length " + std::to_string(timetableLength);
	}
	std::string output;

	// very bad..
	for (int day_i=0; day_i < timetableLength; day_i++) {
		output += "=== Day #" + std::to_string(day_i + 1) + ": ===\n";
		for (int order_i = 0; order_i<classCount; order_i++) {
			Class* matchingClass = nullptr;

			// try to find class (bad code)
			for (int i = 0; i < classes.size(); i++) {
				if (classes.at(i).day == day_i && classes.at(i).order == order_i) {
					// has class
					matchingClass = &classes[i];
				}
			}

			output += std::to_string(order_i + 1) + ": ";
			if (matchingClass != nullptr) {
				output += matchingClass->subject->name + "\n";
			} else {
				output += "---\n";
			}
		}
	}
	return output;
}

void Timetable::calculateScore() {
    /**
     * @brief calculates score for current individual and set it as variable
     */

    float averageClassCount = (float)this->getClassCount() / (float)this->getLength();
    float score{0};

    std::vector<std::vector<Class*>> structuredClasses;

    // init vector for structuring
    for (int day_i{0}; day_i < this->getLength(); day_i++) {
        structuredClasses.emplace_back(classCount, nullptr);
    }

    // go through all classes and struct them
    for (int counter{0}; counter < classes.size(); counter++) {
        // check for overlaps
        if (structuredClasses[classes[counter].day][classes[counter].order] != nullptr) {
            score -= 100.0f * weights->at(0);
        }

        structuredClasses[classes[counter].day][classes[counter].order] = &classes[counter];

        // check for time slots
        if (!classes[counter].subject->availableSlots[classes[counter].day + 2][classes[counter].order + 2]) {
            score -= 10.0f * weights->at(1);
        }
    }

    for (int day_i{0}; day_i < this->getLength(); day_i++) {
        // iterate through days

        int dayClassCount{0};
        int gaps{0};
		int repeatedClasses{0};
		int uniqueClasses{0};
		Subject* previousClass{nullptr};
		//std::set<Subject*> allClasses;
		std::vector<Subject*> allClasses;

        int firstClassOrder{-1};
        int lastClassOrder{-1};

		if (this->currentScore > 30) {
			int debug{0};
		}

        for (int order_i{0}; order_i < this->classCount; order_i++) {

            if (structuredClasses[day_i][order_i] != nullptr) {
                // not a gap, actually a class
                if (firstClassOrder == -1) {
                    // set up first class
                    firstClassOrder = order_i;
                }

                // mark current class as last
                lastClassOrder = order_i;
                // increment class count
                dayClassCount++;

				// add to used classes
				auto it = std::find(allClasses.begin(), allClasses.end(), structuredClasses[day_i][order_i]->subject);
				if (it != allClasses.end()) {
					uniqueClasses++;
					allClasses.push_back(structuredClasses[day_i][order_i]->subject);
				}


            } else {
                // gap
                if (firstClassOrder != -1) {
                    // midday gap
                    gaps++;
                }
            }

			// check if repeats previous class
			if (structuredClasses[day_i][order_i] != nullptr) {
				if (previousClass == structuredClasses[day_i][order_i]->subject) {
					repeatedClasses++;
				}
				previousClass = structuredClasses[day_i][order_i]->subject;
			}

        }

        // final score calculations 👇
        if (dayClassCount != 0) {
            gaps -= (dayClassCount - lastClassOrder - 1);
        }

		// Repeats
		score -= weights->at(2) * (float)repeatedClasses;
		// Week balance
        score -= powf(std::abs((float)dayClassCount - averageClassCount), 2) * weights->at(4);
		// Gaps
        score += weights->at(3) * (float)(classCount - gaps);
		// Diversity (percentage of unique classes)
		if (uniqueClasses + dayClassCount != 0) {
			score += weights->at(5) * ((float)uniqueClasses / (float)dayClassCount);
		}


    }

    // store current calculated score into memory
    currentScore = score;
}

bool Timetable::operator<(const Timetable& other) const
{
    // compare scores
    return currentScore > other.currentScore;
}

bool Timetable::operator==(const Timetable& other) const {
    if (classes.size() != other.classes.size()) {
        return false;
    }
    for (int i{0}; i < (int)classes.size(); i++) {
        if (classes[i].day != other.classes[i].day || classes[i].order != other.classes[i].order || classes[i].subject != other.classes[i].subject) {
            return false;
        }
    }
    return true;
}
/*
namespace std {
size_t std::hash<Timetable>::operator()(const Timetable& obj) const {
    size_t hash = 0;

    for (int i{0}; i < (int)obj.classes.size(); i++) {
        hash ^= std::hash<int>()(obj.classes[i].day);
        hash ^= std::hash<int>()(obj.classes[i].order);
        hash ^= std::hash<int>()(obj.classes[i].subject->id);
    }
    return std::hash<int>()(hash);
}
}
*/
