# JobSchedulerLab

[![CMake CI](https://github.com/sinaxzq/JobSchedulerTests/actions/workflows/ci.yml/badge.svg)](https://github.com/sinaxzq/JobSchedulerTests/actions/workflows/ci.yml)

A small C++ job scheduling project built around a thread pool, delayed task execution, futures, and automated tests.

## What it demonstrates

* Thread pool based task execution
* Delayed job scheduling
* `std::future` result handling
* Graceful shutdown behavior
* CMake project structure
* Automated Windows CI with GitHub Actions

## Build

### Visual Studio / Windows

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

### Single-config generators

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
