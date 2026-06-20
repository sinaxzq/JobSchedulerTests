#pragma once

void testThreadPoolRejectsZeroWorkers();
void testThreadPoolRunsSubmittedTask();
void testThreadPoolShutdownIsIdempotent();

void testJobSchedulerCanBeCreatedAndDestroyed();
void testJobSchedulerShutdownIsIdempotent();
void testJobSchedulerRunsDelayedTask();
void testJobSchedulerRunsEarlierTaskFirst();
void testJobSchedulerRejectsScheduleAfterShutdown();