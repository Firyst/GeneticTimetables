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

Subject::Subject(int _id, std::string _name, std::string _teacher, std::string _group, std::vector<std::vector<bool>> slots) {
    id = _id;
    name = std::move(_name);
    teacher = std::move(_teacher);
    group = std::move(_group);
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
            for (int i = 0; i < (int)classes.size(); i++) {
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

    // float averageClassCount = (float)this->getClassCount() / (float)this->getLength();
    float score{0};

    // for each group (not stream) store all days with exams
    std::map<std::string, std::vector<int>> groupExamDays;

    std::vector<std::vector<Class*>> structuredClasses;

    // init vector for structuring
    for (int day_i{0}; day_i < this->getLength(); day_i++) {
        structuredClasses.emplace_back(classCount, nullptr);
    }

    // go through all classes and struct them
    for (int counter{0}; counter < classes.size(); counter++) {
        // check for overlaps
        auto class2 = structuredClasses[classes[counter].day][classes[counter].order];
        if (class2 != nullptr) {
            // check if same teacher
            if (class2->subject->teacher == classes[counter].subject->teacher) {
                score -= 80.0f * weights->at(0);
            }

            // check if valid group names
            if (class2->subject->group.length() == 9 && classes[counter].subject->group.length() == 9) {
                if (class2->subject->group == classes[counter].subject->group) {
                    // same group
                    score -= 80.0f * weights->at(0);
                } else if (class2->subject->group.substr(0, 8) == classes[counter].subject->group.substr(0, 8)) {
                    // same stream
                    score -= 80.0f * weights->at(0);
                }

            }
        }

        auto group = classes[counter].subject->group;

        if (group.length() == 9) {
            // check if first appearance
            if (!groupExamDays.count(group)) {
                groupExamDays[group] = std::vector<int>{};
            }
            groupExamDays.at(group).emplace_back(classes[counter].day);
        }

        structuredClasses[classes[counter].day][classes[counter].order] = &classes[counter];

        // check for time slots
        if (!classes[counter].subject->availableSlots[classes[counter].day + 2][classes[counter].order + 2]) {
            score -= 10.0f * weights->at(1);
        }
    }

    for (auto kvp : groupExamDays) {
        auto days = kvp.second;

        std::string stream = kvp.first.substr(0, 8) + "*";

        // count stream exams too
        if (groupExamDays.count(stream)) {
            days.insert(days.end(), groupExamDays[stream].begin(), groupExamDays[stream].end());
        }

        // get all days
        std::sort(days.begin(), days.end());
        for (int i{0}; i < days.size() -1; i++) {
            int diff = days[i+1] - days[i];
            if (diff == 0) {
                // two exams a day
                score -= 50.0f * weights->at(4);
            } else {
                score -= std::max(3 - diff, 0) * 8.0f * weights->at(4);  // 1 day - ok, 2 days - good, 3 days - super!
            }
        }
    }


    // store current calculated score into memory
    currentScore = score + 5000;
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
