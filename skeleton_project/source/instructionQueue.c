#include "instructionQueue.h"
#include <stdlib.h>

void addInstruction(Queue* queue, Instruction instruction){
    if (queue->instructions == NULL){
        //initialise the queue, since class constructors are forbidden, apparantly
        //malloc the initial memory
        queue->length = 0;
        queue->capacity = 10;
        queue->instructions = (Instruction*)malloc(queue->capacity * sizeof(Instruction));
    }

    if (queue->length >= queue->capacity){
        //alloc more memory if needed
        queue->capacity *= 2;
        queue->instructions = (Instruction*)realloc(queue->instructions, queue->capacity * sizeof(Instruction));
    }

    queue->instructions[queue->length] = instruction;
    queue->length++;
}

void clearQueue(Queue* queue){
    free(queue->instructions);
    queue->instructions = NULL;
    queue->length = 0;
    queue->capacity = 0;
}

void removeInstruction(Queue* queue, int currentFloor){
    //ok a linked list might acctually have been better in this case, but now im commited
    for (int i = 0; i < queue->length; i++){
        if (queue->instructions[i].targetFloor == currentFloor){

            //shift all instructions one step back to fill the gap
            for (int j = i; j < queue->length - 1; j++){
                queue->instructions[j] = queue->instructions[j + 1];
            }
            queue->length--;
            return;
        }
    }
}

void resetQueueLights(Output* output){
    for (int i = 0; i < 4; i++){
        output->lights.internalButtonLights[i] = false;
    }
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 2; j++){
            output->lights.buttonLights[i][j] = false;
        }
    }
}

void lightQueueStops(const Queue* queue, Output* output){

    resetQueueLights(output);

    for (int i = 0; i < queue->length; i++){
        if (queue->instructions[i].direction == STOP){
            output->lights.internalButtonLights[queue->instructions[i].targetFloor] = true;
        }
        else if (queue->instructions[i].direction == UP){
            output->lights.buttonLights[queue->instructions[i].targetFloor][0] = true;
        }
        else if (queue->instructions[i].direction == DOWN){
            output->lights.buttonLights[queue->instructions[i].targetFloor][1] = true;
        }
    }
}