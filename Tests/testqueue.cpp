//
#define TESTMODE
#ifdef TESTMODE

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../GeneticCore/uniquepriorityqueue.h"


TEST_CASE("Basic test") {
    REQUIRE(1 == 1);
}

#endif
