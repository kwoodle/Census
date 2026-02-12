cmake_minimum_required(VERSION 3.10)
project(Census)
set(CMAKE_CXX_STANDARD 11)
include(/usr/local/lib/ktrade/cmake/ktrade-targets.cmake)

# boost
find_package(Boost 1.64 CONFIG COMPONENTS system filesystem program_options date_time)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Census Census.cpp Census.h)
target_link_libraries(Census ${Boost_LIBRARIES})
target_link_libraries(Census Ktrd)

add_executable(Geoid Geoid.cpp Census.h)
target_link_libraries(Geoid ${Boost_LIBRARIES})
target_link_libraries(Geoid Ktrd)

add_executable(ElectColl ElColl.cpp ElColl.h)
target_link_libraries(ElectColl ${Boost_LIBRARIES})
target_link_libraries(ElectColl Ktrd)

add_executable(CreatePopInfile CreatePopInfile.cpp)
target_link_libraries(CreatePopInfile Ktrd)

add_executable(CreateOver18 CreateOver18.cpp)
target_link_libraries(CreateOver18 Ktrd)

add_executable(FixOverseas FixOverseas.cpp)

add_executable(Tests tests.cpp ElColl.h)
target_link_libraries(Tests Ktrd)

