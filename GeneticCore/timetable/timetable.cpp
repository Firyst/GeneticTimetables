#include <utility>
#include <vector>
#include <string>
#include "timetable.h"
#include <random>

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

Subject::Subject(int _id, std::string _name) {
    id = _id;
    name = std::move(_name);
}



Teacher::Teacher(std::string _name) {
    name = std::move(_name);
}


Class::Class(int _day, int _order, Subject* _subject) {
    day = _day;
    order = _order;
    teacher = nullptr;
    subject = _subject;
}

Class::Class( Subject *_subject) {
	day = 0;
	order = 0;
	teacher = nullptr;
	subject = _subject;
}

//
// TIMETABLE
//

Timetable::Timetable(int length, const std::vector<float>* _weights): weights(_weights)
{
	m_length = length;
}

void Timetable::setClassesAmount(std::map<Subject *, int> amount) {
	classesAmount = std::move(amount);
}

int Timetable::getLength() const {
	return this->m_length;
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

	for (int day_i = 0; day_i < m_length; day_i++) {
		for (int order_i = 0; order_i<7; order_i++) {
			freeSlots.push_back(std::pair<int, int>(day_i, order_i));
		}
	}

    // create distirbution

	for (auto const& [key, val] : classesAmount) {
		for (int count = 0; count < val; count++) {
			// select a random free slot
            std::uniform_int_distribution<int> uni(0, freeSlots.size() - 1);
			int randomId = uni(rng);
			std::pair<int, int> slot = freeSlots[randomId];
			classes.push_back(Class(slot.first, slot.second, key));
			freeSlots.erase(freeSlots.begin() + randomId);
		}
	}
}

std::string Timetable::toString() {
	if (classes.empty()) {
		return "Empty time table of length " + std::to_string(m_length);
	}
	std::string output;

	// very bad..
	for (int day_i=0; day_i < m_length; day_i++) {
		output += "=== Day #" + std::to_string(day_i + 1) + ": ===\n";
		for (int order_i = 0; order_i<7; order_i++) {
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
     *
     */

    float averageClassCount = (float)this->getClassCount() / (float)this->getLength();
    float score{0};

    std::vector<std::vector<Class*>> structuredClasses;

    // init vector for structuring
    for (int day_i{0}; day_i < this->getLength(); day_i++) {
        structuredClasses.push_back(std::vector<Class*>(7, nullptr));
    }

    // go through all classes and struct them
    for (int counter{0}; counter<classes.size(); counter++) {
        // check for overlaps
        if (structuredClasses[classes[counter].day][classes[counter].order] != nullptr) {
            score -= 100.0f * weights->at(0);
        }

        structuredClasses[classes[counter].day][classes[counter].order] = &classes[counter];
    }

    for (int day_i{0}; day_i < this->getLength(); day_i++) {
        // iterate through days

        int classCount{0};
        int gaps{0};

        int firstClassOrder{-1};
        int lastClassOrder{-1};

        for (int order_i{0}; order_i < 7; order_i++) {

            if (structuredClasses[day_i][order_i] != nullptr) {
                // not a gap, actually a class
                if (firstClassOrder == -1) {
                    // set up first class
                    firstClassOrder = order_i;
                }

                // mark current class as last
                lastClassOrder = order_i;
                // increment class count
                classCount++;
            } else {
                // gap
                if (firstClassOrder != -1) {
                    // midday gap
                    gaps++;
                }
            }
        }

        // final score calculations 👇
        if (classCount != 0) {
            gaps -= (7 - lastClassOrder - 1);
        }

        score -= powf(std::abs((float)classCount - averageClassCount), 2) * weights->at(4);
        score += weights->at(3) * (float)(7 - gaps);
    }

    // store current calculated score into memory
    currentScore = score;
}