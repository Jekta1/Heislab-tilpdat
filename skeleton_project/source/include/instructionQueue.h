#ifndef INSTRUCTIONQUEUE_H
#define INSTRUCTIONQUEUE_H

#include "dataStructures.h"

void addInstruction(Queue* queue, Instruction instruction);
void clearQueue(Queue* queue);
void clearInstruction(Queue* queue, int currentFloor);
void removeInstruction(Queue* queue, int currentFloor);
void lightQueueStops(const Queue* queue, Output* outputs);

#endif // InstructionQueue_H