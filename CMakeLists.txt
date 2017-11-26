cmake_minimum_required(VERSION 3.9)
project(log_replay)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        process_input.cpp
        test_boost.cpp
        test_boost2.cpp
        test_boost3.cpp
        test_boost_parsetime.cpp
        test_boost_parsetime2.cpp
        test_boost_ptime_duration.cpp
        test_getch.cpp
        test_getch2.cpp
        test_getch3.cpp
        test_getchar.cpp
        test_shiftkey.cpp)

include_directories(.)

add_executable(log_replay ${SOURCE_FILES})