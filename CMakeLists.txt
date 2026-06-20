cmake_minimum_required(VERSION 3.16)

project(JobSchedulerLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_library(JobScheduler
    src/ThreadPool.cpp
    src/JobScheduler.cpp
)

target_include_directories(JobScheduler PUBLIC include)
target_link_libraries(JobScheduler PUBLIC Threads::Threads)

add_executable(JobSchedulerTests
    tests/main.cpp
    tests/ThreadPoolTests.cpp
    tests/JobSchedulerTests.cpp
)

target_link_libraries(JobSchedulerTests PRIVATE JobScheduler)

enable_testing()
add_test(NAME JobSchedulerTests COMMAND JobSchedulerTests)
