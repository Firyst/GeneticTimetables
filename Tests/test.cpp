//
//#define TESTMODE
#ifdef TESTMODE

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../GeneticCore/uniquepriorityqueue.h"

#include "../GeneticCore/timetable/timetable.h"

TEST_SUITE("My priority queue") {
    TEST_CASE("Test top") {
        UniquePriorityQueue<int> myQueue;
        myQueue.push(1);
        REQUIRE(myQueue.top() == 1);
        myQueue.push(2);
        REQUIRE(myQueue.top() == 2);
        myQueue.push(0);
        REQUIRE(myQueue.top() == 2);
    }

    TEST_CASE("Test top 2") {
        UniquePriorityQueue<int> myQueue;
        myQueue.push(3);
        REQUIRE(myQueue.top() == 3);
        myQueue.push(2);
        REQUIRE(myQueue.top() == 3);
        myQueue.push(1);
        REQUIRE(myQueue.top() == 3);
    }

    TEST_CASE("Test unique") {
        UniquePriorityQueue<int> myQueue;
        REQUIRE(myQueue.size() == 0);

        REQUIRE(myQueue.push(3));
        REQUIRE(myQueue.size() == 1);

        REQUIRE(!myQueue.push(3));
        REQUIRE(myQueue.size() == 1);

        REQUIRE(myQueue.push(4));
        REQUIRE(myQueue.size() == 2);
    }

    TEST_CASE("Test pop") {
        UniquePriorityQueue<int> myQueue;
        REQUIRE(myQueue.size() == 0);

        REQUIRE(myQueue.push(3));
        REQUIRE(myQueue.push(4));
        REQUIRE(myQueue.push(5));
        REQUIRE(myQueue.size() == 3);

        myQueue.pop();
        REQUIRE(myQueue.size() == 2);

        myQueue.pop();
        REQUIRE(myQueue.size() == 1);

        myQueue.pop();
        REQUIRE(myQueue.size() == 0);

    }

    TEST_CASE("Test empty") {
        UniquePriorityQueue<int> myQueue;
        REQUIRE(myQueue.size() == 0);
        REQUIRE(myQueue.empty());

        REQUIRE_THROWS(myQueue.top());
    }

    TEST_CASE("Test sorted") {
        UniquePriorityQueue<int> myQueue;
        myQueue.push(5);
        myQueue.push(7);
        myQueue.push(11);
        myQueue.push(1);

        REQUIRE(myQueue.top() == 11);

        myQueue.pop();
        REQUIRE(myQueue.top() == 7);
    }

    TEST_CASE("Test negative") {
        UniquePriorityQueue<int> myQueue;
        myQueue.push(-5);
        myQueue.push(-7);
        myQueue.push(-11);
        myQueue.push(1);

        REQUIRE(myQueue.top() == 1);

        myQueue.pop();
        REQUIRE(myQueue.top() == -5);
    }

    TEST_CASE("Test complex") {
        UniquePriorityQueue<int> myQueue;
        myQueue.push(5);
        myQueue.push(13);
        myQueue.push(71);
        myQueue.push(1);
        myQueue.push(5);

        REQUIRE(myQueue.size() == 4);
        REQUIRE(myQueue.top() == 71);

        myQueue.push(71);

        REQUIRE(myQueue.size() == 4);
        REQUIRE(myQueue.top() == 71);

        myQueue.pop();
        REQUIRE(myQueue.top() == 13);

        myQueue.push(14);
        REQUIRE(myQueue.top() == 14);
    }

    TEST_CASE("Test timetable init") {
        UniquePriorityQueue<Timetable> myQueue;
        REQUIRE(myQueue.empty());
    }

    TEST_CASE("Test timetable add") {
        UniquePriorityQueue<Timetable> myQueue;

        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        myQueue.push(Timetable(1, 1, &testWeights));

        REQUIRE(myQueue.size() == 1);
    }

    TEST_CASE("Test timetable unique") {
        UniquePriorityQueue<Timetable> myQueue;

        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        REQUIRE(myQueue.push(Timetable(1, 1, &testWeights)));
        REQUIRE(!myQueue.push(Timetable(1, 1, &testWeights)));
        REQUIRE(myQueue.size() == 1);
    }

    TEST_CASE("Test timetable different") {
        UniquePriorityQueue<Timetable> myQueue;

        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        REQUIRE(myQueue.push(Timetable(1, 1, &testWeights)));
        REQUIRE(myQueue.push(Timetable(12, 2, &testWeights)));
        REQUIRE(myQueue.size() == 2);
    }
}

TEST_SUITE("Subject") {
    std::vector<std::vector<bool>> testSlots;
    TEST_CASE("Init") {
        REQUIRE_NOTHROW(Subject(0, "name", "teacher", testSlots));
    }

    TEST_CASE("data correct") {
        Subject sub(0, "name", "teacher", testSlots);
        REQUIRE((sub.name == "name"));
        REQUIRE(sub.teacher == "teacher");
        REQUIRE(sub.id == 0);
    }
}

TEST_SUITE("Timetable") {
    std::vector<std::vector<bool>> testSlots;
    Subject testSubject = Subject(0, "name", "teacher", testSlots);
    std::map<Subject*, int> rules = {{&testSubject, 1}};


    TEST_CASE("Init") {
        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        REQUIRE_NOTHROW(Timetable test(2, 2, &testWeights));
    }

    TEST_CASE("Init empty 2") {
        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        Timetable test(2, 2, &testWeights);
        REQUIRE(test.classes.size() == 0);
        REQUIRE(test.scoreChanged);
    }

    TEST_CASE("Randomize") {
        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        Timetable test(2, 2, &testWeights);
        test.setClassesAmount(rules);
        test.randomizeTimetable();
        REQUIRE(test.classes.size() == 1 );
    }

    TEST_CASE("Randomize 1") {
        rules[&testSubject] = 2;
        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        Timetable test(2, 2, &testWeights);
        test.setClassesAmount(rules);
        test.randomizeTimetable();
        REQUIRE(test.classes.size() == 2 );
    }

    TEST_CASE("Randomize 2") {
        rules[&testSubject] = 3;
        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        Timetable test(2, 2, &testWeights);
        test.setClassesAmount(rules);
        test.randomizeTimetable();
        REQUIRE(test.classes.size() == 3 );
    }

    TEST_CASE("Randomize 3") {
        rules[&testSubject] = 1;
        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        Timetable test(2, 2, &testWeights);
        test.setClassesAmount(rules);
        test.randomizeTimetable();
        test.randomizeTimetable();
        REQUIRE(test.classes.size() == 1 );
    }

    TEST_CASE("Randomize 5") {
        std::vector<float> testWeights = {1.0, 2.0, 3.0};
        rules[&testSubject] = 2;
        Subject testS2 = Subject(22, "", "", testSlots);
        rules[&testS2] = 1;
        Timetable test(2, 2, &testWeights);
        test.setClassesAmount(rules);
        test.randomizeTimetable();
        REQUIRE(test.classes.size() == 3 );
    }


}
#endif
