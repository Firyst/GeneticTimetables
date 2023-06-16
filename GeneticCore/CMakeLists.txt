cmake_minimum_required(VERSION 3.24)
project(GeneticCore)

set(CMAKE_CXX_STANDARD 20)


add_executable(GeneticCore  timetable/timetable.cpp timetable/timetable.h ga/ga.cpp ga/ga.h main.cpp)
add_executable(MyTests Tests/*.cpp)

add_test(NAME MyTests COMMAND MyTests)