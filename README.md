# JobSchedulerLab

Учебный C++17 проект: in-memory Job Queue / Task Scheduler.

## v1 scope

- fixed-size ThreadPool
- delayed jobs
- future-based result retrieval
- graceful shutdown
- basic concurrency tests

## Not in v1

- cron syntax
- persistence
- distributed queue
- retries
- cancellation
- HTTP API
- priority levels

## Build

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

## Current learning target

Understand ownership, lifetime, shared state, condition_variable waiting, and shutdown semantics.
