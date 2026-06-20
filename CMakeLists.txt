cmake_minimum_required(VERSION 3.16)

project(JobSchedulerLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_library(job_scheduler
    src/ThreadPool.cpp
    src/JobScheduler.cpp
)

target_include_directories(job_scheduler PUBLIC include)

target_link_libraries(job_scheduler PUBLIC Threads::Threads)

add_executable(job_scheduler_tests
    tests/main.cpp
    tests/ThreadPoolTests.cpp
    tests/JobSchedulerTests.cpp
)

target_link_libraries(job_scheduler_tests PRIVATE job_scheduler)

enable_testing()

add_test(NAME job_scheduler_tests COMMAND job_scheduler_tests)
