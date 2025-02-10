#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include "instructionQueue.h"
#include "stateTracker.h"
#include "outputManager.h"

void controller(State* state, Queue* queue, Output* outputs);

#endif // LOGICCONTROLLER_H