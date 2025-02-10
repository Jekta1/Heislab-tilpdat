#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include "dataStructures.h"

double getTime();
void logState(const State* state);
void logTimer(const Timer* timer);
void logQueue(const Queue* queue);
void logOutputs(const Output* output);

#endif // UTILS_H